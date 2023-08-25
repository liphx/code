#ifndef LIPH_IO_POLL_H_
#define LIPH_IO_POLL_H_

#include <poll.h>

#include <cstdint>
#include <map>
#include <vector>

#include "liph/io/descriptor.h"

struct epoll_event;

namespace liph::io {

enum class event_type { read, write, read_write };

struct poll_event {
    int fd;
    event_type type;
    void *ptr;
};

class poll_base {
public:
    poll_base() {}
    virtual ~poll_base() {}
    virtual bool add(descriptor fd, event_type type, void *ptr) = 0;
    virtual bool del(descriptor fd) = 0;
    // -1 for error, 0 for timeout, positive for nums of ready descriptor
    virtual int wait(std::vector<poll_event>& events, int64_t timeout_us) = 0;
};

class poll : public poll_base {
public:
    poll() {}
    ~poll() {}

    bool add(descriptor fd, event_type type, void *ptr) override {
        if (fdmap_.find(fd) != fdmap_.end()) {
            return false;
        }
        fdmap_[fd.fd()] = std::make_pair(type, ptr);
        update_ = true;
        return true;
    }

    bool del(descriptor fd) override {
        if (fdmap_.find(fd) == fdmap_.end()) {
            return false;
        }
        fdmap_.erase(fd.fd());
        update_ = true;
        return true;
    }

    int wait(std::vector<poll_event>& events, int64_t timeout_us) override {
        update();
        int r = ::poll(fdset_.data(), fdset_.size(), timeout_us < 0 ? -1 : timeout_us / 1000);
        if (r <= 0) {
            return r;
        }
        events.resize(r);
        int i = 0;
        for (auto it = fdset_.begin(); i < r && it != fdset_.end(); ++it) {
            if (it->revents & POLLIN && it->revents & POLLOUT) {
                events[i].fd = it->fd;
                events[i].type = event_type::read_write;
                events[i].ptr = fdmap_[it->fd].second;
                i++;
            }
        }
        if (i != r) {
            events.clear();
            return false;
        }
        return r;
    }

private:
    void update() {
        if (update_) {
            fdset_.resize(fdmap_.size());
            int i = 0;
            for (const auto& pair : fdmap_) {
                fdset_[i].fd = pair.first;
                switch (pair.second.first) {
                case event_type::read:
                    fdset_[i].events = POLLIN;
                    break;
                case event_type::write:
                    fdset_[i].events = POLLOUT;
                    break;
                default:
                    fdset_[i].events = POLLIN | POLLOUT;
                }
                fdset_[i].revents = 0;
                i++;
            }
            update_ = false;
        }
    }

private:
    std::map<int, std::pair<event_type, void *>> fdmap_;
    std::vector<struct ::pollfd> fdset_;
    bool update_{false};
};

class Poll {
public:
    Poll();

    ~Poll();

    operator bool() const { return epfd_ != -1; }

    bool add(int fd, void *ptr);

    bool del(int fd);

    bool wait(std::vector<void *>& ptrs);

    static const int maxevents;

    // private:
    int epfd_;
    struct epoll_event *evlist_;
};

}  // namespace liph::io

#endif  // LIPH_IO_POLL_H_
