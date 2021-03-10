#include "tcp.h"
using namespace tcp;
#include <string>
#include <memory>
#include <cstring>
#include <cerrno>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>

tcp_error::tcp_error(std::string msg): std::runtime_error(msg)
{
}

tcp_server::tcp_server(int port): port_(port)
{
    sockfd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd_ == -1)
        throw tcp_error(std::string("create socket fail: ") + strerror(errno));

    struct sockaddr_in name;
    memset(&name, 0, sizeof(name));
    name.sin_family = AF_INET;
    name.sin_port = htons(port_);
    name.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(sockfd_, (struct sockaddr *)&name, sizeof(name)) == -1) {
        close(sockfd_);
        throw tcp_error(std::string("bind fail: ") + strerror(errno));
    }
        
    
    if (port_ == 0) { // 是否动态分配端口 
        socklen_t namelen = sizeof(name);
        if (getsockname(sockfd_, (struct sockaddr *)&name, &namelen) == -1) {
            close(sockfd_);
            throw tcp_error(std::string("getsockname fail: ") + strerror(errno));
        }
            
        port_ = ntohs(name.sin_port);
    }

    if (listen(sockfd_, 16) < 0) {
        close(sockfd_);
        throw tcp_error(std::string("listen fail: ") + strerror(errno));
    }
}

tcp_server::~tcp_server()
{
    close(sockfd_);
}

struct tcp_info {
    int fd;
    int type; // type = 0: tcp server
    tcp_connection *data;
};

static inline void epoll_add(int epfd, tcp_info *ptr)
{
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.ptr = ptr;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, ptr->fd, &ev) == -1) {
        throw tcp_error("epoll_ctl error");
    }
}

void tcp_server::start(void (*handle)(const tcp_connection&))
{
    int epfd = epoll_create1(EPOLL_CLOEXEC);
    if (epfd == -1) {
        throw tcp_error(std::string("epoll_create1 fail: ") + strerror(errno));
    }

    tcp_info data = {
        .fd = sockfd_,
        .type = 0,
        .data = nullptr
    };
    epoll_add(epfd, &data);
    const int maxevents = 16;
    struct epoll_event evlist[maxevents];
    int fds;
    for (;;) {
        fds = epoll_wait(epfd, evlist, maxevents, -1);
        if (fds == -1) {
            if (errno == EINTR) {
                continue;
            } else {
                throw tcp_error("epoll_wait error");
            }
        }
        for (int i = 0; i < fds; i++) {
            tcp_info *ptr = (tcp_info *)evlist[i].data.ptr;
            if (ptr->type == 0) { // accept
                tcp_connection *pclient = new tcp_connection(sockfd_);
                if (pclient != nullptr) {
                    tcp_info data = {
                        .fd = pclient->get_sockfd(),
                        .type = 1,
                        .data = pclient
                    };
                    epoll_add(epfd, &data);
                }
            } else {
                std::unique_ptr<tcp_connection> client(ptr->data);
                if (handle != nullptr)
                    handle(*client);
            }
        }
    }
}

int tcp_server::get_sockfd() const
{
    return sockfd_;
}

int tcp_server::get_port() const
{
    return port_;
}

tcp_connection::tcp_connection(int sockfd)
{
    struct sockaddr_in client_name;
    socklen_t client_name_len = sizeof(client_name);
    sockfd_ = ::accept(sockfd, (struct sockaddr *)&client_name, &client_name_len);
    if (sockfd_ == -1) {
        throw tcp_error(std::string("accept fail: ") + strerror(errno));
    }
}

tcp_connection::tcp_connection(const char *ip, int port)
{
    sockfd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd_ < 0)
        throw tcp_error(std::string("create socket fail: ") + strerror(errno));

    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(port);
    serveraddr.sin_addr.s_addr = inet_addr(ip);

    if (connect(sockfd_, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) < 0) {
        close(sockfd_);
        throw tcp_error(std::string("connect fail: ") + strerror(errno));
    }
}

tcp_connection::~tcp_connection()
{
    close(sockfd_);
}

int tcp_connection::get_sockfd() const
{
    return sockfd_;
}

ssize_t tcp_connection::read(void *buf, size_t nbytes) const
{
    return ::read(sockfd_, buf, nbytes);
}

ssize_t tcp_connection::write(const void *buf, size_t nbytes) const
{
    return ::write(sockfd_, buf, nbytes);
}

ssize_t tcp_connection::readn(void *buf, size_t nbytes) const
{
    size_t left = nbytes;
    ssize_t size;
    while (left > 0) {
        size = read(buf, left);
        if (size < 0) {
            if (left == nbytes)
                return -1;
            break;
        } else if (size == 0) {
            break;
        } else {
            left -= size;
            buf = (char *)buf + size; 
        }
    }
    return nbytes - left;
}

ssize_t tcp_connection::writen(const void *buf, size_t nbytes) const
{
    size_t left = nbytes;
    ssize_t size;
    while (left > 0) {
        size = write(buf, left);
        if (size < 0) { // write error
            if (left == nbytes)
                return -1; // 写了0字节
            else 
                break; // 返回已写字节数
        } else if (size == 0) {
            break;
        } else {
            left -= size;
            buf = (char *)buf + size;
        }
    }
    return nbytes - left;
}

ssize_t tcp_connection::recv(void *buf, size_t nbytes, int flags) const
{
    return ::recv(sockfd_, buf, nbytes, flags);
}

ssize_t tcp_connection::send(const void *buf, size_t nbytes, int flags) const
{
    return ::send(sockfd_, buf, nbytes, flags);
}
