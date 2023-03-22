#ifndef LIPH_POLL_H_
#define LIPH_POLL_H_

#include <vector>

struct epoll_event;

namespace liph {

class Poll {
public:
    Poll();

    ~Poll();

    operator bool() const { return epfd_ != -1; }

    bool add(int fd, void *ptr);

    bool del(int fd);

    bool wait(std::vector<void *>& ptrs);

    static const int maxevents;

private:
    int epfd_;
    struct epoll_event *evlist_;
};

}  // namespace liph

#endif  // LIPH_POLL_H_
