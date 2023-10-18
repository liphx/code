#ifndef LIPH_NET_SOCKET_H_
#define LIPH_NET_SOCKET_H_

#include "liph/macros.h"

#ifdef OS_UNIX

#include <sys/types.h>

#include <memory>

#include "liph/io/descriptor.h"

namespace liph::net {

class socket : public io::descriptor {
public:
    enum { IPV4 = 1, IPV6 = 2, TCP = 4, UDP = 8 };

    static const int BUFFSIZE = 1024;

public:
    socket() : descriptor(-1) {}
    socket(int type);
    socket(const socket& other);

    ~socket();

    bool set_non_block();

    bool bind(int& port);

    bool listen(int backlog);

    std::shared_ptr<socket> accept();
    bool connect(const char *ip, int port);

    bool is_tcp() const { return type_ & socket::TCP; }

    bool is_udp() const { return !is_tcp(); }

    bool is_ipv4() const { return type_ & socket::IPV4; }

    bool is_ipv6() const { return !is_ipv4(); }

    ssize_t recv(void *buf, size_t nbytes, int flags);
    ssize_t send(const void *buf, size_t nbytes, int flags);

private:
    int type_;
};

}  // namespace liph::net

#endif

#endif  // LIPH_NET_SOCKET_H_
