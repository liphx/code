#ifndef LIPH_NET_TCP_H_
#define LIPH_NET_TCP_H_

#include "liph/macros.h"

#ifdef OS_UNIX

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "liph/io/poll.h"
#include "liph/net/socket.h"

namespace liph::net {

class tcp_connect {
public:
    tcp_connect(std::shared_ptr<socket> socket) : socket_(socket) {}

    std::vector<uint8_t> read_buf_;
    std::vector<uint8_t> write_buf_;
    std::shared_ptr<socket> socket_;
};

class tcp_server {
public:
    // 构造函数，server被动打开，参数port默认为0，表明动态分配端口，构造函数失败抛出runtime_error
    tcp_server(int port = 0, bool ipv6 = false);
    tcp_server(const tcp_server&) = delete;
    ~tcp_server() {}

    int port() const { return port_; }

    std::shared_ptr<socket> get_socket() const { return socket_; }

    void run(std::function<void(tcp_connect&)> handle);

private:
    bool start_server();

private:
    bool ipv6_;
    int port_;
    std::shared_ptr<socket> socket_;
    std::shared_ptr<io::Poll> poll_;
};

}  // namespace liph::net

#endif

#endif  // LIPH_NET_TCP_H_
