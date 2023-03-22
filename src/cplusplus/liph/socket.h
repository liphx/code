#ifndef LIPH_SOCKET_H_
#define LIPH_SOCKET_H_

#include <sys/types.h>

#include <memory>

namespace liph {

class Socket {
public:
    enum { IPV4 = 1, IPV6 = 2, TCP = 4, UDP = 8 };

    static const int BUFFSIZE = 1024;

public:
    Socket() : fd_(-1) {}
    Socket(int type);
    Socket(const Socket& other);

    ~Socket();

    operator bool() const { return fd_ != -1; }

    int fd() const { return fd_; }

    bool set_non_block();

    bool bind(int& port);

    bool listen(int backlog);

    std::shared_ptr<Socket> accept();
    bool connect(const char *ip, int port);

    bool is_tcp() const { return type_ & Socket::TCP; }

    bool is_udp() const { return !is_tcp(); }

    bool is_ipv4() const { return type_ & Socket::IPV4; }

    bool is_ipv6() const { return !is_ipv4(); }

    ssize_t read(void *buf, size_t nbytes);
    ssize_t write(const void *buf, size_t nbytes);
    ssize_t recv(void *buf, size_t nbytes, int flags);
    ssize_t send(const void *buf, size_t nbytes, int flags);

private:
    int fd_;
    int type_;
};

}  // namespace liph

#endif  // LIPH_SOCKET_H_
