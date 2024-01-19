#include "net/server.h"

#include "net/poll.h"

namespace liph::net {

tcp_server::tcp_server(int port, bool ipv6) : _port(port) {
    if (ipv6) {
        _socket = socket(domain::ipv6, protocol::tcp);
    } else {
        _socket = socket(domain::ipv4, protocol::tcp);
    }
    _socket.set_reuse_addr();
    auto p = _socket.bind(_port);
    if (p) {
        _port = *p;
        _socket.listen();
    }
    if (!_socket.ok()) throw std::runtime_error("start tcp server fail: " + _socket.error());
}

void tcp_server::run() {
    _socket.set_non_block();
    poll poller;
    poller.add(_socket.id(), [&]() {
        auto sock = _socket.accept();
        if (!sock) return;
        auto client = std::move(sock->first);
        auto addr = sock->second;
        client.set_non_block();
        auto conn = std::make_shared<tcp_connect>(client, addr);
        if (_open_handler) {
            _open_handler(conn);
        }
        int id = client.id();
        poller.add(id, make_copyable_function<void()>([this, id, addr, &poller, client = std::move(client)] mutable {
            std::string message = client.recv();
            auto conn = std::make_shared<tcp_connect>(client, addr);
            if (!client.ok()) {
                client.close();
                if (_close_handler) {
                    _close_handler(conn);
                }
                poller.del(id);
                return;
            }
            if (_message_handler) {
                _message_handler(conn, std::move(message));
            }
        }));
    });

    while (true) poller();
}

}  // namespace liph::net
