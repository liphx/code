#ifndef LIPH_NET_HTTP_H_
#define LIPH_NET_HTTP_H_

#include <map>
#include <string>

#include "liph/net/tcp.h"

namespace liph::net {

class http_request {
public:
    http_request();

    std::string method() const { return method_; }

    void method(const std::string& method);

    std::string url() const { return url_; }

    void url(const std::string& url) { url_ = url; }

    std::string host() const { return host_; }

    void host(const std::string& host) { host_ = host; }

private:
    bool parse_header(std::string&);
    bool parse_request_line(const std::string&);

    void clear() {
        method_.clear();
        url_.clear();
        version_.clear();
        host_.clear();
        header_.clear();
    }

    std::string method_;
    std::string url_;
    std::string version_;
    std::string host_;
    std::map<std::string, std::string> header_;
};

class http_response {
public:
    http_response();
    void set_content(const std::string&, const std::string&);
    std::string content() const;
    void parse_header(const std::string&);
    std::map<std::string, std::string> header_;
    std::string content_;
};

class http_server {
public:
    typedef void (*http_handle)(const http_request& req, http_response& res);
    http_server(int port = 80);
    ~http_server();
    void set_handle(const std::string& url, http_handle func);
    void set_static_sever(const std::string& url, const std::string& path);
    void start();

private:
    static void handle(std::shared_ptr<socket> client);
    int port_;
    std::map<std::string, http_handle> url_map_;
};

}  // namespace liph::net

#endif  // LIPH_NET_HTTP_H_
