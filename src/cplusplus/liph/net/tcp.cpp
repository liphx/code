#include "liph/net/tcp.h"

#include <cerrno>
#include <cstring>
#include <memory>
#include <stdexcept>
#include <string>

namespace liph::net {

bool tcp_server::start_server() {
    if (ipv6_) {
        socket_ = std::make_shared<socket>(socket::TCP | socket::IPV6);
    } else {
        socket_ = std::make_shared<socket>(socket::TCP | socket::IPV4);
    }

    if (!*socket_) {
        return false;
    }

    /* if (!socket_->set_non_block()) { */
    /*     return false; */
    /* } */

    if (!socket_->bind(port_)) {
        return false;
    }

    if (!socket_->listen(16)) {
        return false;
    }

    return true;
}

tcp_server::tcp_server(int port, bool ipv6) : ipv6_(ipv6), port_(port) {
    if (!start_server()) {
        throw std::runtime_error("start tcp server fail");
    }
}

void tcp_server::run(std::function<void(tcp_connect&)> handle) {
    poll_.reset(new io::Poll());
    if (!poll_) {
        throw std::runtime_error("create poll fail");
    }
    poll_->add(socket_->fd(), nullptr);
    std::vector<void *> ptrs;
    while (true) {
        if (!poll_->wait(ptrs)) {
            continue;
        }
        for (void *ptr : ptrs) {
            if (ptr == nullptr) {  // listen
                std::shared_ptr<socket> client = socket_->accept();
                if (!client || !client->set_non_block()) {
                    continue;
                }
                auto conn = new tcp_connect(client);
                poll_->add(client->fd(), conn);
            } else {  // client
                tcp_connect *conn = (tcp_connect *)ptr;
                char buf[socket::BUFFSIZE];
                while (true) {
                    int size = conn->socket_->read(buf, socket::BUFFSIZE);
                    if (size < 0 && errno == EAGAIN) {  // 数据读完
                        break;
                    } else if (size <= 0) {  // 断开连接 or 读出错
                        poll_->del(conn->socket_->fd());
                        break;
                    }
                    conn->read_buf_.insert(conn->read_buf_.end(), buf, buf + size);
                }
                if (handle) {
                    handle(*conn);
                    if (!conn->write_buf_.empty())
                        conn->socket_->write(conn->write_buf_.data(), conn->write_buf_.size());
                }
                delete conn;
            }
        }
    }
}

}  // namespace liph
