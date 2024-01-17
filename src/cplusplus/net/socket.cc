#include "socket.h"

#include <cerrno>
#include <cstring>

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

socket::socket() {}

socket::socket(domain domain_, protocol protocol_) {
    int d = domain_ == domain::ipv4 ? AF_INET : domain_ == domain::ipv6 ? AF_INET6 : -1;
    int type = protocol_ == protocol::tcp ? SOCK_STREAM : protocol_ == protocol::udp ? SOCK_DGRAM : -1;
    int sock = ::socket(d, type, 0);
    if (sock == -1) {
        _error = std::strerror(errno);
    } else {
        _descriptor = std::make_unique<descriptor>(sock);
        _domain = domain_;
        _protocol = protocol_;
    }
}

socket::socket(socket&& sock) : _descriptor(std::move(sock._descriptor)), _error(std::move(sock._error)) {}

socket& socket::operator=(socket&& sock) {
    if (this != &sock) {
        _descriptor = std::move(sock._descriptor);
        _error = std::move(sock._error);
    }
    return *this;
}

socket::~socket() {}

bool socket::connect(const char *ip, int port) {
    if (!_descriptor) _error = "Connect by invalid socket";
    if (!ok()) return false;
    if (_domain == domain::ipv4) {
        struct sockaddr_in serveraddr;
        memset(&serveraddr, 0, sizeof(serveraddr));
        serveraddr.sin_family = AF_INET;
        serveraddr.sin_port = htons(port);
        serveraddr.sin_addr.s_addr = inet_addr(ip);
        if (::connect(_descriptor->fd(), (struct sockaddr *)&serveraddr, sizeof(serveraddr)) == -1) {
            _error = std::strerror(errno);
            return false;
        }
    } else if (_domain == domain::ipv6) {
        struct sockaddr_in6 serveraddr;
        serveraddr.sin6_family = AF_INET6;
        serveraddr.sin6_port = htons(port);
        inet_pton(AF_INET6, ip, &serveraddr.sin6_addr);
        if (::connect(_descriptor->fd(), (struct sockaddr *)&serveraddr, sizeof(serveraddr)) == -1) {
            _error = std::strerror(errno);
            return false;
        }
    }
    return false;
}

}  // namespace liph::net

#elif defined(_WIN32) || defined(_WIN64)

#include <winsock2.h>

#else
#error "Unsupported platform"
#endif
