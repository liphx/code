#ifndef TCP_H_
#define TCP_H_

#include <stdexcept>
#include <string>
#include <functional>
#include <vector>

// tcp 异常类
class tcp_error: public std::runtime_error {
public:
    tcp_error(std::string msg);
};

// tcp 连接类
class tcp_connection {
public:
    // 构造函数，服务器建立连接，参数为服务器端socket，失败则抛出异常
    tcp_connection(int sockfd);
    // 构造函数，客户端主动打开，参数为服务器ip与端口，失败则抛出异常
    tcp_connection(const char *ip, int port);
    virtual ~tcp_connection();
    // 返回套接字描述符
    int get_sockfd() const;
    ssize_t read(void *buf, size_t nbytes) const; 
    ssize_t write(const void *buf, size_t nbytes) const;
    // 读n个字节，返回已读字节数
    ssize_t readn(void *buf, size_t nbytes) const;
    // 写n个字节，返回已写字节数
    ssize_t writen(const void *buf, size_t nbytes) const;
    ssize_t recv(void *buf, size_t nbytes, int flags) const;
    ssize_t send(const void *buf, size_t nbytes, int flags) const;

    std::vector<uint8_t> read_buf_;
    std::vector<uint8_t> write_buf_;
private:
    int sockfd_;
};

class tcp_server {
public:
    // 构造函数，tcp_server被动打开，参数port默认为0，表明动态分配端口，构造函数失败抛出tcp_error
    tcp_server(int port = 0);
    tcp_server(const tcp_server&) = delete;
    virtual ~tcp_server();
    // 启动服务，对每个tcp连接以handle处理
    void start(std::function<void(tcp_connection&)> handle);
    // 返回监听套接字描述符
    int get_sockfd() const;
    // 返回监听端口号
    int get_port() const;
private:
    int sockfd_;
    int port_;
};

#endif // TCP_H_
