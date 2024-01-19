#include "net/socket.h"

#if defined(__linux__) || defined(__APPLE__) || defined(unix) || defined(__unix__) || defined(__unix)

#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

namespace liph::net {

class descriptor {
public:
    descriptor(int fd) : descriptor_(fd) {}
    int fd() const { return descriptor_; }

private:
    int descriptor_;
};

int socket::id() const {
    if (!_descriptor) return -1;
    return _descriptor->fd();
}

std::string socket::address() const {
    if (!_descriptor) return {};
    if (_domain == domain::ipv4) {
        struct sockaddr_in name;
        socklen_t namelen = sizeof(name);
        if (::getsockname(_descriptor->fd(), reinterpret_cast<struct sockaddr *>(&name), &namelen) == 0) {
            return addr2ip(name.sin_addr.s_addr) + ":" + std::to_string(ntohs(name.sin_port));
        }
    } else if (_domain == domain::ipv6) {
        struct sockaddr_in6 name;
        socklen_t namelen = sizeof(name);
        if (::getsockname(_descriptor->fd(), reinterpret_cast<struct sockaddr *>(&name), &namelen) == 0)
            return std::to_string(ntohs(name.sin6_port));
    }
    return {};
}

socket::socket() {}

socket::socket(domain domain_, protocol protocol_) {
    int d = domain_ == domain::ipv4 ? AF_INET : domain_ == domain::ipv6 ? AF_INET6 : -1;
    int type = protocol_ == protocol::tcp ? SOCK_STREAM : protocol_ == protocol::udp ? SOCK_DGRAM : -1;
    int sock = ::socket(d, type, 0);
    if (sock == -1) {
        perror();
    } else {
        _descriptor = std::make_unique<descriptor>(sock);
        _domain = domain_;
        _protocol = protocol_;
    }
}

socket::socket(domain domain_, protocol protocol_, std::unique_ptr<descriptor> descriptor)
        : _descriptor(std::move(descriptor)), _domain(domain_), _protocol(protocol_) {}

socket::socket(socket&& sock)
        : _descriptor(std::move(sock._descriptor)),
          _error(std::move(sock._error)),
          _domain(sock._domain),
          _protocol(sock._protocol) {}

socket& socket::operator=(socket&& sock) {
    if (this != &sock) {
        _descriptor = std::move(sock._descriptor);
        _error = std::move(sock._error);
        _domain = sock._domain;
        _protocol = sock._protocol;
    }
    return *this;
}

socket::~socket() {
    if (_descriptor) {
        ::close(_descriptor->fd());
    }
}

bool socket::connect(const char *ip, int port) {
    if (!_descriptor) error("Invalid socket");
    if (!ok()) return false;
    if (_domain == domain::ipv4) {
        struct sockaddr_in serveraddr;
        memset(&serveraddr, 0, sizeof(serveraddr));
        serveraddr.sin_family = AF_INET;
        serveraddr.sin_port = htons(port);
        serveraddr.sin_addr.s_addr = inet_addr(ip);
        if (::connect(_descriptor->fd(), (struct sockaddr *)&serveraddr, sizeof(serveraddr)) == -1) {
            perror();
            return false;
        }
    } else if (_domain == domain::ipv6) {
        struct sockaddr_in6 serveraddr;
        serveraddr.sin6_family = AF_INET6;
        serveraddr.sin6_port = htons(port);
        inet_pton(AF_INET6, ip, &serveraddr.sin6_addr);
        if (::connect(_descriptor->fd(), (struct sockaddr *)&serveraddr, sizeof(serveraddr)) == -1) {
            perror();
            return false;
        }
    }
    return true;
}

std::optional<int> socket::bind(int port) {
    if (!_descriptor) error("Invalid socket");
    if (_protocol == protocol::udp) error("Unsupported protocol");
    if (!ok()) return {};
    if (_domain == domain::ipv4) {
        struct sockaddr_in name;
        memset(&name, 0, sizeof(name));
        name.sin_family = AF_INET;
        name.sin_port = htons(port);
        name.sin_addr.s_addr = htonl(INADDR_ANY);
        if (::bind(_descriptor->fd(), reinterpret_cast<struct sockaddr *>(&name), sizeof(name)) == -1) {
            perror();
            return {};
        }
    } else if (_domain == domain::ipv6) {
        struct sockaddr_in6 name;
        memset(&name, 0, sizeof(name));
        name.sin6_family = AF_INET6;
        name.sin6_port = htons(port);
        name.sin6_addr = IN6ADDR_ANY_INIT;
        if (::bind(_descriptor->fd(), reinterpret_cast<struct sockaddr *>(&name), sizeof(name)) == -1) {
            perror();
            return {};
        }
    } else {
        error("Unsupported protocol");
        return {};
    }
    if (port == 0) {
        if (_domain == domain::ipv4) {
            struct sockaddr_in name;
            socklen_t namelen = sizeof(name);
            if (::getsockname(_descriptor->fd(), reinterpret_cast<struct sockaddr *>(&name), &namelen) == 0)
                return ntohs(name.sin_port);
        } else if (_domain == domain::ipv6) {
            struct sockaddr_in6 name;
            socklen_t namelen = sizeof(name);
            if (::getsockname(_descriptor->fd(), reinterpret_cast<struct sockaddr *>(&name), &namelen) == 0)
                return ntohs(name.sin6_port);
        }
        return {};
    }
    return port;
}

bool socket::listen() {
    if (!_descriptor) error("Invalid socket");
    if (_protocol == protocol::udp) error("Unsupported protocol");
    if (!ok()) return {};

    if (::listen(_descriptor->fd(), 16) == -1) {
        perror("listen fail");
        return false;
    }
    return true;
}

std::optional<std::pair<socket, std::string>> socket::accept() {
    if (!_descriptor) error("Invalid socket");
    if (_protocol == protocol::udp) error("Unsupported protocol");
    if (!ok()) return {};
    if (_domain == domain::ipv4) {
        struct sockaddr_in client_name;
        socklen_t client_name_len = sizeof(client_name);
        int sockfd = ::accept(_descriptor->fd(), reinterpret_cast<struct sockaddr *>(&client_name), &client_name_len);
        if (sockfd == -1) {
            perror("accept fail");
            return {};
        }
        std::string address = addr2ip(client_name.sin_addr.s_addr) + ":" + std::to_string(htons(client_name.sin_port));
        return std::make_pair(socket{domain::ipv4, protocol::tcp, std::make_unique<descriptor>(sockfd)}, address);
    } else if (_domain == domain::ipv6) {
        struct sockaddr_in6 client_name;
        socklen_t client_name_len = sizeof(client_name);
        int sockfd = ::accept(_descriptor->fd(), reinterpret_cast<struct sockaddr *>(&client_name), &client_name_len);
        if (sockfd == -1) {
            perror("accept fail");
            return {};
        }
        return std::make_pair(socket{domain::ipv6, protocol::tcp, std::make_unique<descriptor>(sockfd)}, std::string{});
    }
    return {};
}

socket::ssize_t socket::send(const void *buffer, size_t size) { return ::write(_descriptor->fd(), buffer, size); }
socket::ssize_t socket::recv(void *buffer, size_t size) { return ::read(_descriptor->fd(), buffer, size); }

void socket::send(std::string_view sv) {
    auto n = sv.size();
    while (n > 0) {
        auto r = send(sv.end() - n, n);
        if (r > 0 || errno == EAGAIN) {
            n -= r;
        } else {
            perror("send fail");
            return;
        }
    }
}

std::string socket::recv() {
    std::string message;
    constexpr size_t size = 1024;
    char buffer[size];
    while (true) {
        auto n = recv(buffer, size);
        if (n > 0) {
            message.append(buffer, n);
        } else if (n == 0 || (n < 0 && errno == EAGAIN)) {
            break;
        } else {
            perror("recv fail");
            return {};
        }
    }
    return message;
}

bool socket::close() {
    if (!_descriptor) {
        error("Invalid socket");
        return false;
    }
    int r = shutdown(_descriptor->fd(), SHUT_RDWR);
    if (r != 0) {
        perror();
        return false;
    }
    return true;
}

bool socket::close_read() {
    if (!_descriptor) {
        error("Invalid socket");
        return false;
    }
    int r = shutdown(_descriptor->fd(), SHUT_RD);
    if (r != 0) {
        perror();
        return false;
    }
    return true;
}

bool socket::close_write() {
    if (!_descriptor) {
        error("Invalid socket");
        return false;
    }
    int r = shutdown(_descriptor->fd(), SHUT_WR);
    if (r != 0) {
        perror();
        return false;
    }
    return true;
}

bool socket::set_non_block() {
    if (!_descriptor) {
        error("Invalid socket");
        return false;
    }
    int r = fcntl(_descriptor->fd(), F_GETFL);
    if (r == -1) {
        perror("fcntl F_GETFL fail");
        return false;
    }
    r = fcntl(_descriptor->fd(), F_SETFL, r | O_NONBLOCK);
    if (r == -1) perror("fcntl F_SETFL O_NONBLOCK fail");
    return r != -1;
}

bool socket::set_reuse_addr() {
    if (!_descriptor) {
        error("Invalid socket");
        return false;
    }
    const int enable = 1;
    if (::setsockopt(_descriptor->fd(), SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) != 0) {
        perror("setsockopt fail");
        return false;
    }
    return true;
}

}  // namespace liph::net

#elif defined(_WIN32) || defined(_WIN64)

#include <winsock2.h>

#else
#error "Unsupported platform"
#endif
