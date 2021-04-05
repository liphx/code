#ifndef HTTP_H_
#define HTTP_H_

#include "tcp.h"
#include <string>
#include <map>

// tcp 异常类
class http_error: public std::runtime_error {
public:
    http_error(std::string msg);
};

class http_request {
public:
    http_request();
    // 获取http method
    std::string method() const;
    // 设置http method
    void method(const std::string &);
    // 获取http url
    std::string url() const;
    // 设置http url
    void url(const std::string &);

    std::string host() const;
    void host(const std::string &);
    void parse_header(const std::string &);
    std::map<std::string, std::string> header_;
private:
    std::string method_;
    std::string url_;
    std::string host_;
};

class http_response {
public:
    http_response();
    void set_content(const std::string &, const std::string &);
    std::string content() const;
    // 从字符串总解析header字段
    void parse_header(const std::string &);
    std::map<std::string, std::string> header_;
    std::string content_;
};

class http_connection: public tcp_connection {
public:
    http_connection(const char *ip, int port = 80);
    http_connection(const tcp_connection &);
    int recv_req(http_request &);
    int send_req(const http_request &);
    int recv_res(http_response &);
    int send_res(const http_response &);
};

class http_server: public tcp_server {
public:
    typedef void (*http_handle)(const http_request& req, http_response& res);
    // 构造函数，默认端口为80
    http_server(int port = 80);
    ~http_server();
    // 注册处理函数
    void handle(const std::string& url, http_handle func);
    // 开启 http server
    void start();
    http_handle find_handle_func(const http_request &);
private:
    std::map<std::string, http_handle> url_map_;
};

#endif // HTTP_H_
