#pragma once

#include <cerrno>
#include <cstring>
#include <memory>
#include <optional>
#include <string>

#include "net/util.h"

namespace liph::net {

enum class domain { ipv4, ipv6 };
enum class protocol { tcp, udp };
class descriptor;

class socket {
public:
    using ssize_t = int64_t;

public:
    socket();
    socket(domain, protocol);
    socket(domain, protocol, std::unique_ptr<descriptor> descriptor);
    socket(socket&&);
    socket& operator=(socket&&);
    ~socket();

    int id() const;
    std::string address() const;
    bool close();
    bool close_read();
    bool close_write();
    bool set_non_block();
    bool set_reuse_addr();

    // dynamically allocate if port = 0
    std::optional<int> bind(int port);
    bool listen();
    bool connect(const char *ip, int port);
    std::optional<std::pair<socket, std::string>> accept();
    ssize_t send(const void *buffer, size_t size);
    ssize_t recv(void *buffer, size_t size);
    void send(std::string_view sv);
    std::string recv();

    bool ok() const { return _error.empty(); }
    std::string error() const { return _error; }
    void error(std::string str) { _error = std::move(str); }
    void perror(std::string str) { _error = std::move(str) + ": " + std::strerror(errno); }
    void perror() { _error = std::strerror(errno); }

private:
    std::unique_ptr<descriptor> _descriptor;
    std::string _error;
    domain _domain;
    protocol _protocol;
};

}  // namespace liph::net
