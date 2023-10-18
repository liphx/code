#include "liph/net/http.h"

#ifdef OS_UNIX

#include <algorithm>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <vector>

#include "liph/concurrency/threadpool.h"
#include "liph/lang/singleton.h"
#include "liph/logging.h"
#include "liph/string.h"

namespace liph::net {

http_server::http_server(int port) : port_(port) {}

http_server::~http_server() {}

void http_server::set_handle(const std::string& url, http_handle func) { url_map_.insert(std::make_pair(url, func)); }

void http_server::start() {
    tcp_server svc(port_, false);
    auto server = svc.get_socket();
    auto pool = std::make_shared<ThreadPool>();
    pool->Start();
    while (true) {
        std::shared_ptr<socket> client = server->accept();
        if (!*client) continue;
        pool->AddTask(http_server::handle, client);
    }
}

void http_server::handle(std::shared_ptr<socket> client) {
    http_request req;
    std::string str;
    char buf[1024];
    while (true) {
        int size = client->read(buf, sizeof(buf));
        if (size <= 0) return;
        str += std::string(buf, size);
    }
}

http_request::http_request() {}

void http_request::method(const std::string& str) {
    std::string tmp = str;
    std::transform(str.begin(), str.end(), tmp.begin(), ::toupper);
    if (tmp == "GET")
        method_ = tmp;
    else
        throw std::runtime_error("unsupported method");
}

http_response::http_response() {}

std::string http_response::content() const { return content_; }

bool http_request::parse_request_line(const std::string& str) {
    std::vector<std::string> tokens;
    split(tokens, str, " ");
    if (tokens.size() != 3) return false;
    method_ = tokens[0];
    url_ = tokens[1];
    version_ = tokens[2];
    return true;
}

bool http_request::parse_header(std::string& ss) {
    auto pos = ss.find("\r\n\r\n");
    if (pos == std::string::npos) return false;
    std::string str = std::string(ss, 0, pos);

    std::vector<std::string> lines;
    split(lines, str, "\r\n");
    if (lines.empty() || !parse_request_line(lines[0])) return false;

    for (size_t i = 1; i < lines.size(); i++) {
        std::string& line = lines[i];
        auto pos = line.find_first_of(":");
        if (pos == std::string::npos) goto ERR;
        std::string key = std::string(line, 0, pos);
        std::string value = ltrim(std::string(line, pos + 1));

        if (key.empty() || value.empty()) goto ERR;
        header_[key] = value;
    }

    ss = std::string(ss, pos + 4);
    return true;

ERR:
    clear();
    return false;
}

void http_response::parse_header(const std::string& str) {
    std::vector<std::string> lines;
    split(lines, str, "\r\n");
    if (!lines.empty()) {
        std::string status_line = lines[0];
        auto pos = status_line.find_first_of("HTTP/");
        if (pos != std::string::npos) {
            auto pos2 = status_line.find_first_of(" ", pos);
            if (pos2 != std::string::npos) {
                header_["http_version"] = std::string(status_line, pos + 5, pos2 - pos - 5);
                auto pos3 = status_line.find_first_of(" ", pos2 + 1);
                header_["status_code"] = std::string(status_line, pos2 + 1, pos3 - pos2 - 1);
            }
        }
    }

    for (size_t i = 1; i < lines.size(); i++) {
        std::string& line = lines[i];
        auto pos = line.find_first_of(":");
        if (pos != std::string::npos) {
            std::string key = std::string(line, 0, pos);
            while (line[++pos] == ' ')
                ;
            if (pos != std::string::npos) {
                std::string value = std::string(line, pos);
                header_[key] = value;
            }
        }
    }
}

void http_response::set_content(const std::string& co, const std::string& type) {
    header_["Content-Type"] = type;
    header_["Content-Length"] = std::to_string(co.length());
    content_ = co;
}

}  // namespace liph::net

#endif
