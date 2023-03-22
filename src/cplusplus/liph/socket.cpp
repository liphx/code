#include "socket.h"

#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cerrno>
#include <cstdio>
#include <cstring>

namespace liph {

/*
 * 通过IPV4/IPV6/TCP/UDP的按位或创建socket
 * 如果同时设置IPV4, IPV6， 则IPV4生效
 * 如果同时设置TCP, UDP， 则TCP生效
 * 若失败，fd_为-1，可通过operator bool()检测
 */
Socket::Socket(int type) : type_(type) {
    int domain = is_ipv4() ? AF_INET : AF_INET6;
    int sock = is_tcp() ? SOCK_STREAM : SOCK_DGRAM;
    int protocol = 0;
    fd_ = socket(domain, sock, protocol);
    if (fd_ == -1) {
        perror("create socket fail");
    }
}

Socket::Socket(const Socket& other) {
    if (other) {
        fd_ = dup(other.fd_);
        if (fd_ == -1) {
            perror("dup socket fd fail");
        }
        type_ = other.type_;
    } else {
        fd_ = -1;
    }
}

/*
 * 通过dup复制一个描述符，析构时可以直接调用close
 */
Socket::~Socket() {
    if (fd_ != -1) {
        int ret = close(fd_);
        if (ret == -1 && errno != EINTR) {
            perror("close socket fd fail");
        }
    }
}

bool Socket::set_non_block() {
    int ret = fcntl(fd_, F_GETFL);
    if (ret == -1) {
        perror("fcntl F_GETFL fail");
        return false;
    }
    ret = fcntl(fd_, F_SETFL, ret | O_NONBLOCK);
    if (ret == -1) {
        perror("fcntl F_SETFL O_NONBLOCK fail");
    }
    return ret != -1;
}

bool Socket::bind(int& port) {
    if (is_udp()) {
        return false;
    }

    if (is_ipv4()) {
        struct sockaddr_in name;
        memset(&name, 0, sizeof(name));
        name.sin_family = AF_INET;
        name.sin_port = htons(port);
        name.sin_addr.s_addr = htonl(INADDR_ANY);
        if (::bind(fd_, (struct sockaddr *)&name, sizeof(name)) == -1) {
            perror("bind fail");
            return false;
        }
        if (port == 0) {  // 是否动态分配端口
            socklen_t namelen = sizeof(name);
            if (getsockname(fd_, (struct sockaddr *)&name, &namelen) == -1) {
                perror("getsockname fail");
                return false;
            }
            port = ntohs(name.sin_port);
        }
    } else {  // ipv6
        return false;
    }

    return true;
}

bool Socket::listen(int backlog) {
    if (is_udp()) {
        return false;
    }

    if (::listen(fd_, backlog) == -1) {
        perror("listen fail");
        return false;
    }

    return true;
}

std::shared_ptr<Socket> Socket::accept() {
    auto ret = std::make_shared<Socket>();
    if (is_ipv4()) {
        struct sockaddr_in client_name;
        socklen_t client_name_len = sizeof(client_name);
        int sockfd = ::accept(fd_, (struct sockaddr *)&client_name, &client_name_len);
        if (sockfd == -1) {
            perror("accept fail");
        } else {
            ret->fd_ = sockfd;
        }
    } else {
        // ipv6
    }

    ret->type_ = type_;
    return ret;
}

bool Socket::connect(const char *ip, int port) {
    if (is_ipv4()) {
        struct sockaddr_in serveraddr;
        memset(&serveraddr, 0, sizeof(serveraddr));
        serveraddr.sin_family = AF_INET;
        serveraddr.sin_port = htons(port);
        serveraddr.sin_addr.s_addr = inet_addr(ip);

        if (::connect(fd_, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) == -1) {
            perror("connect fail");
            return false;
        }
    } else {
        // ipv6
    }

    return true;
}

ssize_t Socket::read(void *buf, size_t nbytes) { return ::read(fd_, buf, nbytes); }

ssize_t Socket::write(const void *buf, size_t nbytes) { return ::write(fd_, buf, nbytes); }

ssize_t Socket::recv(void *buf, size_t nbytes, int flags) { return ::recv(fd_, buf, nbytes, flags); }

ssize_t Socket::send(const void *buf, size_t nbytes, int flags) { return ::send(fd_, buf, nbytes, flags); }

}  // namespace liph
