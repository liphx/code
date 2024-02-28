#include <arpa/inet.h>
#include <netdb.h>

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
    static const std::string rsp = "HTTP/1.1 403 Forbidden";
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
}

std::string host2ip(const std::string& host) {
    if (host.empty()) return {};
    std::regex valid_ipv4_re(R"(^((25[0-5]|(2[0-4]|1\d|[1-9]|)\d)\.?\b){4}$)");
    std::smatch ipv4_match;
    if (std::regex_match(host, ipv4_match, valid_ipv4_re)) {
        return host;
    }
    struct hostent *server = gethostbyname(host.c_str());
    if (!server) {
        LOG(ERROR) << "gethostbyname fail, host: " << host;
        return {};
    }
    char addr[128];
    inet_ntop(server->h_addrtype, server->h_addr, addr, sizeof(addr));
    return addr;
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
            if (!FLAGS_daemon) proxy_client.send(header_buffer);
            if (FLAGS_daemon) send_tunnel_ok(client_sock);

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
    CHECK(socket.bind(FLAGS_port));
    CHECK(socket.listen());
    LOG(INFO) << "start server at port " << FLAGS_port;
    server_loop(socket);
}

}  // namespace liph

int main(int argc, char *argv[]) {
    google::InitGoogleLogging(argv[0]);
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    FLAGS_logtostderr = true;
    liph::start_server();
}
