#include "http.h"
#include <algorithm>
#include <vector>
#include <functional>
using namespace tcp;
using namespace http;

#include <iostream>
template<class... Args>
void print(const Args&... args)
{
    // std::initializer_list<int> { ([](auto i){ std::cout << i << ' '; }(args), 0)... };
    // std::cout << std::endl;
}

http_error::http_error(std::string msg): std::runtime_error(msg)
{
}

// ------------------- http_server --------------------------------

http_server::http_server(int port): tcp_server(port)
{
}

http_server::~http_server()
{
}

void http_server::handle(const std::string& url, http_handle func)
{
    url_map_.insert(std::make_pair(url, func));
}

void http_server::start()
{
    std::function<void(const tcp_connection &)> handle = [this](const tcp_connection& client){
        print("start");
        http_connection conn(client);
        http_request req;
        conn.recv_req(req);
        http_response res;
        http_handle func = find_handle_func(req);
        if (func != nullptr) {
            func(req, res);
            if (res.content_ != "")
                conn.send_res(res);
        }
    };

    tcp_server::start(handle);
}

http_server::http_handle http_server::find_handle_func(const http_request& req)
{
    if (req.header_.find("http_url") == req.header_.end())
        return nullptr;
    std::string url = req.header_.at("http_url");
    if (url_map_.find(url) == url_map_.end())
        return nullptr;
    return url_map_.at(url);
}

// ------------------- http_request --------------------------------

http_request::http_request()
{

}

std::string http_request::method() const
{
    return method_;
}

std::string http_request::url() const
{
    return url_;
}

std::string http_request::host() const
{
    return host_;
}

void http_request::method(const std::string& str)
{
    std::string tmp = str;
    std::transform(str.begin(), str.end(), tmp.begin(), ::toupper);
    if (tmp == "GET")
        method_ = tmp;
    else
        throw http_error("unsupported method");
}

void http_request::url(const std::string& str)
{
    url_ = str;
}

void http_request::host(const std::string& str)
{
    host_ = str;
}

// ------------------- http_response --------------------------------

http_response::http_response()
{

}

std::string http_response::content() const
{
    return content_;
}

static void split(std::vector<std::string>& lines, const std::string& str, const std::string& delimiter)
{
    auto pos1 = str.find_first_not_of(delimiter, 0);
    auto pos2 = str.find_first_of(delimiter, pos1);
    while (pos2 != std::string::npos || pos1 != std::string::npos) {
        lines.emplace_back(str.substr(pos1, pos2 - pos1));
        pos1 = str.find_first_not_of(delimiter, pos2);
        pos2 = str.find_first_of(delimiter, pos1);
    }
}

void http_request::parse_header(const std::string& str)
{
    std::vector<std::string> lines;
    split(lines, str, "\r\n");
    // 第一行，方法行
    if (!lines.empty()) {
        std::string method_line = lines[0];
        std::vector<std::string> tokens;
        split(tokens, method_line, " ");
        if (tokens.size() == 3) {
            if (tokens[0] != "GET")
               return;
            header_["http_method"] = tokens[0];
            method_ = tokens[0];
            header_["http_url"] = tokens[1];
            url_ = tokens[1];
            header_["http_version"] = tokens[2];
            print(tokens[0], tokens[1], tokens[2]);
        }
    }

    for (int i = 1; i < lines.size(); i++) {
        std::string& line = lines[i];
        // std::cout << "origin line: " << line << std::endl;
        auto pos = line.find_first_of(":");
        if (pos != std::string::npos) {
            std::string key = std::string(line, 0, pos);
            while (line[++pos] == ' ');
            if (pos != std::string::npos) {
                std::string value = std::string(line, pos);
                header_[key] = value;
            }
        }
    }
}

void http_response::parse_header(const std::string& str)
{
    std::vector<std::string> lines;
    split(lines, str, "\r\n");
    // std::cout << lines.size() << std::endl;
    // 第一行 状态行
    if (!lines.empty()) {
        std::string status_line = lines[0];
        // std::cout << "origin line: " << status_line << std::endl;
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

    for (int i = 1; i < lines.size(); i++) {
        std::string& line = lines[i];
        // std::cout << "origin line: " << line << std::endl;
        auto pos = line.find_first_of(":");
        if (pos != std::string::npos) {
            std::string key = std::string(line, 0, pos);
            while (line[++pos] == ' ');
            if (pos != std::string::npos) {
                std::string value = std::string(line, pos);
                header_[key] = value;
            }
        }
    }

    // for (auto i: header_) {
    //     std::cout << i.first << ": " << i.second << std::endl;
    // }

}

void http_response::set_content(const std::string& co, const std::string& type)
{
    header_["Content-Type"] = type;
    header_["Content-Length"] = std::to_string(co.length());
    content_ = co;
    for (auto i: header_) {
        print(i.first, i.second);
    }
}

// ------------------- http_connection --------------------------------

http_connection::http_connection(const char *ip, int port): tcp_connection(ip, port)
{

}

http_connection::http_connection(const tcp_connection &conn): tcp_connection(conn)
{

}

int http_connection::recv_req(http_request& req)
{
    // read header
    std::string str;
    int prev = 0;
    char ch;
    while (prev < 4) {
        if (read(&ch, 1) != 1) {
            return -1;
        }
        str += ch;
        if (ch == '\r' && (prev == 0 || prev == 2))
            prev++;
        else if (ch == '\n' && (prev == 1 || prev == 3))
            prev++;
        else
            prev = 0;
    }
    print(str);
    req.parse_header(str);
    return 0;
}

int http_connection::send_req(const http_request& req)
{
    std::string msg = req.method() + " " + req.url() + " " + "HTTP/1.1\r\n";
    msg += "Host: " + req.host() + "\r\n\r\n";
    if (writen(msg.data(), msg.length()) == msg.length())
        return 0;
    return -1; // error
}

int http_connection::recv_res(http_response& res)
{
    // read header
    std::string str;
    int prev = 0;
    char ch;
    while (prev < 4) {
        if (read(&ch, 1) != 1) {
            return -1;
        }
        str += ch;
        if (ch == '\r' && (prev == 0 || prev == 2))
            prev++;
        else if (ch == '\n' && (prev == 1 || prev == 3))
            prev++;
        else
            prev = 0;
    }

    // std::cout << "before parse: " << str << std::endl; 
    // std::cout << "after parse: " << std::endl; 
    // parse header
    res.parse_header(str);

    if (res.header_.find("Transfer-Encoding") != res.header_.end()) {
        int chunk_size = 0;
        for (;;) {
            // read chunk size
            std::string str;
            int prev = 0;
            char ch;
            while (prev < 2) {
                if (read(&ch, 1) != 1) {
                    return -1;
                }
                str += ch;
                if (ch == '\r' && prev == 0)
                    prev++;
                else if (ch == '\n' && prev == 1)
                    prev++;
                else
                    prev = 0;
            };
            str = std::string(str, 0, str.length() - 2);
            if (str.empty()) {
                break;
            }
            // std::cout << "------------" << str.length() << " " << str << std::endl;
            try {
                chunk_size = std::stoi(str, nullptr, 16);
            } catch (std::exception &) {
                chunk_size = 0;
            }
            if (chunk_size <= 0)
                break;
            char buf[BUFSIZ];
            int left = chunk_size;
            while (left > 0) {
                int size = read(buf, std::min(BUFSIZ, left));
                res.content_ += std::string(buf, size);
                left -= size;
            }
        }

    } else if (res.header_.find("Content-Length") != res.header_.end()) {
        int length = 0;
        try {
            length = std::stoi(res.header_["Content-Length"]);
        } catch (std::exception &) {
            return -1;
        }
        char buf[BUFSIZ];
        int left = length;
        while (left > 0) {
            int size = read(buf, std::min(BUFSIZ, left));
            res.content_ += std::string(buf, size);
            left -= size;
        }

    }

    return 0;
}

int http_connection::send_res(const http_response& res)
{
    try {
        std::string msg =   "HTTP/1.1 200 OK\r\n"
                            "Content-Type: " + res.header_.at("Content-Type") + "\r\n"
                            "Content-Length: " + res.header_.at("Content-Length") + "\r\n"
                            "\r\n";
        
        if (writen(msg.data(), msg.length()) != msg.length())
            return -1;
        if (writen(res.content_.data(), res.content_.length()) != res.content_.length())
            return -1;
    } catch (std::exception&) {
        return -1;
    }
    return 0;
}
