#include "poll.h"

#ifdef __linux__
#include <sys/epoll.h>
#elif __APPLE__
#include <sys/event.h>
#endif

#include <unistd.h>

#include <cerrno>
#include <cstdio>

namespace liph {

const int Poll::maxevents = 1024;

#ifdef __linux__

Poll::Poll() {
    epfd_ = epoll_create1(EPOLL_CLOEXEC);
    if (epfd_ == -1) {
        perror("create epoll fd fail");
    }
    evlist_ = new struct epoll_event[maxevents];
}

Poll::~Poll() {
    if (epfd_ != -1) {
        int ret = close(epfd_);
        if (ret == -1 && errno != EINTR) {
            perror("close epoll fd fail");
        }
        delete[] evlist_;
    }
}

bool Poll::add(int fd, void *ptr) {
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.ptr = ptr;
    if (epoll_ctl(epfd_, EPOLL_CTL_ADD, fd, &ev) == -1) {
        perror("epoll_ctl EPOLL_CTL_ADD fail");
        return false;
    }
    return true;
}

bool Poll::del(int fd) {
    if (epoll_ctl(epfd_, EPOLL_CTL_DEL, fd, nullptr) == -1) {
        perror("epoll_ctl EPOLL_CTL_DEL fail");
        return false;
    }
    return true;
}

bool Poll::wait(std::vector<void *>& ptrs) {
    int ret = epoll_wait(epfd_, evlist_, maxevents, -1);
    if (ret == -1 && errno != EINTR) {
        perror("epoll_wait fail");
        return false;
    } else if (ret == -1) {
        return false;
    }
    ptrs.resize(ret);
    for (int i = 0; i < ret; i++) {
        ptrs[i] = evlist_[i].data.ptr;
    }
    return true;
}

#elif __APPLE__

Poll::Poll() {}

Poll::~Poll() {}

bool Poll::add(int, void *) { return false; }

bool Poll::del(int) { return false; }

bool Poll::wait(std::vector<void *>&) { return false; }

#endif  // __linux__ or __APPLE__

}  // namespace liph
