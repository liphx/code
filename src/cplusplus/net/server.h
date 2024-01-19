#pragma once

#include <functional>

#include "net/socket.h"

namespace liph::net {

class tcp_connect {
public:
    tcp_connect(socket& sock, std::string addr) : _socket(sock), _peeraddr(addr) {}
    void send(const std::string& str) { _socket.send(str); }
    std::string localaddr() { return _socket.address(); }
    std::string peeraddr() { return _peeraddr; }

private:
    socket& _socket;
    std::string _peeraddr;
};

class tcp_server {
public:
    tcp_server(int port = 0, bool ipv6 = false);

    int port() const { return _port; }

    void on_open(std::function<void(std::shared_ptr<tcp_connect>)> handler) { _open_handler = handler; }
    void on_close(std::function<void(std::shared_ptr<tcp_connect>)> handler) { _close_handler = handler; }
    void on_message(std::function<void(std::shared_ptr<tcp_connect>, std::string)> handler) {
        _message_handler = handler;
    }
    void run();

private:
    socket _socket;
    int _port;
    std::function<void(std::shared_ptr<tcp_connect>)> _open_handler;
    std::function<void(std::shared_ptr<tcp_connect>)> _close_handler;
    std::function<void(std::shared_ptr<tcp_connect>, std::string)> _message_handler;
};

class tcp6_server {};

}  // namespace liph::net
