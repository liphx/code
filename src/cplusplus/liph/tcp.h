#ifndef LIPH_TCP_H_
#define LIPH_TCP_H_

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "poll.h"
#include "socket.h"

namespace liph {

class tcp_connect {
public:
    tcp_connect(std::shared_ptr<Socket> socket) : socket_(socket) {}

    std::vector<uint8_t> read_buf_;
    std::vector<uint8_t> write_buf_;
    std::shared_ptr<Socket> socket_;
};

class tcp_server {
public:
    // 构造函数，server被动打开，参数port默认为0，表明动态分配端口，构造函数失败抛出runtime_error
    tcp_server(int port = 0, bool ipv6 = false);
    tcp_server(const tcp_server&) = delete;
    ~tcp_server() {}

    int get_port() const { return port_; }

    std::shared_ptr<Socket> get_socket() const { return socket_; }

    void run(std::function<void(tcp_connect&)> handle);

private:
    bool start_server();

private:
    bool ipv6_;
    int port_;
    std::shared_ptr<Socket> socket_;
    std::shared_ptr<Poll> poll_;
};

}  // namespace liph

#endif  // LIPH_TCP_H_
