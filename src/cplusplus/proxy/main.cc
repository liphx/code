#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>

#include <future>
#include <regex>

#include "gflags/gflags.h"
#include "glog/logging.h"
#include "glog/stl_logging.h"
#include "liph/concurrency/thread.h"
#include "liph/encoding/base64.h"
#include "liph/string.h"
#include "net/socket.h"

DEFINE_bool(daemon, false, "");
DEFINE_int32(port, 0, "");
DEFINE_string(next_proxy, "", "");
DEFINE_int32(next_proxy_port, 0, "");
DEFINE_string(username, "", "");
DEFINE_string(password, "", "");

namespace liph {

bool authentication(const std::string& msg) {
    static const std::string prefix = "Basic ";
    if (!startswith(msg, prefix)) return false;
    auto str = base64::decode(msg.substr(prefix.size()));
    auto tokens = split(str, ":");
    if (tokens.size() != 2) return false;
    return tokens[0] == FLAGS_username && tokens[1] == FLAGS_password;
}

void send_forbidden(net::socket& socket) {
    static const std::string rsp = "HTTP/1.1 403 Forbidden\r\n\r\n";
    socket.send(rsp);
}

void send_need_auth(net::socket& socket) {
    static const std::string rsp =
            "HTTP/1.1 407 Proxy Authentication Required\r\n"
            "Proxy-Authenticate: Basic\r\n\r\n";
    socket.send(rsp);
}

void send_tunnel_ok(net::socket& socket) {
    static const std::string rsp = "HTTP/1.1 200 Connection Established\r\n\r\n";
    socket.send(rsp);
}

void forward_data(net::socket& src, net::socket& dst) {
    char buffer[16 * 1024];
    int n;
    while ((n = src.recv(buffer, sizeof(buffer), 0)) > 0) {
        for (int i = 0; i < n; ++i) buffer[i] ^= 42;
        dst.send(buffer, n, 0);
    }
    src.close_read();
    dst.close_write();
}

std::string host2ip(const std::string& host) {
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;  // IPv4 or IPv6
    struct addrinfo *result = nullptr;
    int r = getaddrinfo(host.c_str(), nullptr, &hints, &result);
    if (r != 0) {
        LOG(ERROR) << "host: " << host;
        LOG(ERROR) << "getaddrinfo fail: " << gai_strerror(r);
        return {};
    }
    std::unique_ptr<struct addrinfo, decltype([](struct addrinfo *ptr) { if (ptr) freeaddrinfo(ptr); })> res(result);
    if (res) {  // do not need ai_next
        void *addr = nullptr;
        switch (res->ai_family) {
        case AF_INET:
            addr = &(reinterpret_cast<struct sockaddr_in *>(res->ai_addr)->sin_addr);
            break;
        case AF_INET6:
            addr = &(reinterpret_cast<struct sockaddr_in6 *>(res->ai_addr)->sin6_addr);
            break;
        default:
            return {};
        }
        if (addr) {
            char buffer[128];
            inet_ntop(res->ai_family, addr, buffer, sizeof(buffer));
            return buffer;
        }
        return {};
    }
    return {};
}

std::optional<std::pair<std::string, int>> parse_host(std::string str) {
    if (str.empty()) return {};
    auto pos = str.find(':');
    if (pos == std::string::npos) {
        return std::make_pair(str, 80);
    }
    auto port = to_int(str.substr(pos + 1));
    if (port) return std::make_pair(str.substr(0, pos), *port);
    return {};
}

struct Buffer {
    Buffer(net::socket& socket) : socket_(socket) {}

    std::optional<std::string> readline() {
        std::string line;
        char ch;
        while (true) {
            auto n = socket_.recv(&ch, 1, 0);
            if (n == 0 || (n < 0 && errno != EINTR)) {
                return {};
            }
            if (FLAGS_daemon) {
                ch ^= 42;
            }
            if (ch == '\n') break;
            line += ch;
        }
        if (!line.empty() && line.back() == '\r') line = line.substr(0, line.size() - 1);
        return line;
    }

private:
    net::socket& socket_;
};

void server_loop(net::socket& socket) {
    while (true) {
        auto client = socket.accept();
        if (!client) {
            LOG(ERROR) << "accept error: " << socket.error();
            continue;
        }
        auto& [client_sock, client_addr] = *client;
        LOG(INFO) << "client addr: " << client_addr;
        std::packaged_task task([](net::socket client_sock) {
            Buffer buffer(client_sock);
            std::string header_buffer;
            auto start_line = buffer.readline();
            if (!start_line) {
                LOG(ERROR) << "no start line, bad request";
                return;
            }
            header_buffer += *start_line;
            header_buffer += "\r\n";
            std::map<std::string, std::string> header;
            while (true) {
                auto line = buffer.readline();
                if (!line) {
                    LOG(ERROR) << "no blank line, bad request";
                    return;
                }
                header_buffer += *line;
                header_buffer += "\r\n";
                if (line->empty()) break;  // blank line, http request header end
                auto pos = line->find_first_of(':');
                if (pos == std::string::npos) {
                    LOG(ERROR) << "no colon, bad request";
                    return;
                }
                std::string key = std::string(trim(line->substr(0, pos)));
                std::string value = std::string(trim(line->substr(pos + 1)));
                header[key] = value;
            }
            // LOG(INFO) << "header: " << header;
            auto auth = header.find("Proxy-Authorization");
            if (auth == header.end()) {
                send_need_auth(client_sock);
                return;
            }
            if (!authentication(auth->second)) {
                send_forbidden(client_sock);
                return;
            }

            net::socket proxy_client{net::domain::ipv4, net::protocol::tcp};
            if (!FLAGS_next_proxy.empty()) {
                if (!proxy_client.connect(FLAGS_next_proxy, FLAGS_next_proxy_port)) {
                    LOG(ERROR) << "connect fail: " << proxy_client.error();
                    return;
                }
            } else {
                auto host = header.find("Host");
                if (host == header.end()) {
                    LOG(ERROR) << "no host, bad request";
                    return;
                }
                LOG(INFO) << "Host: " << *host;
                auto host_port = parse_host(host->second);
                if (!host_port) {
                    LOG(ERROR) << "host/port error, bad request";
                    return;
                }
                auto remoteip = host2ip(host_port->first);
                LOG(INFO) << host_port->first << " -> " << remoteip;
                if (remoteip.empty()) {
                    LOG(ERROR) << "get ip from host fail";
                    return;
                }
                if (!proxy_client.connect(remoteip, host_port->second)) {
                    LOG(ERROR) << "connect fail: " << proxy_client.error();
                    return;
                }
            }

            // LOG(INFO) << header_buffer;
            if (!FLAGS_daemon) {
                for (auto& ch : header_buffer) ch ^= 42;
                proxy_client.send(header_buffer);
            } else
                send_tunnel_ok(client_sock);

            auto fut = std::async(std::launch::async, forward_data, std::ref(client_sock), std::ref(proxy_client));
            auto fut2 = std::async(std::launch::async, forward_data, std::ref(proxy_client), std::ref(client_sock));
            fut.wait();
            fut2.wait();
        });
        std::thread(std::move(task), std::move(client_sock)).detach();
    }
}

void start_server() {
    net::socket socket{net::domain::ipv4, net::protocol::tcp};
    CHECK(socket.ok()) << socket.error();
    CHECK(socket.set_reuse_addr());
    auto port = socket.bind(FLAGS_port);
    CHECK(port);
    CHECK(socket.listen());
    LOG(INFO) << "start server at port " << *port << ", pid: " << getpid();
    server_loop(socket);
}

}  // namespace liph

int main(int argc, char *argv[]) {
    google::InitGoogleLogging(argv[0]);
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    FLAGS_logtostderr = true;

    signal(SIGPIPE, SIG_IGN);
    liph::start_server();
}
