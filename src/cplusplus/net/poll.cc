#include "net/poll.h"

#include <map>
#include <vector>

#if defined(__linux__) || defined(__APPLE__) || defined(unix) || defined(__unix__) || defined(__unix)

#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

namespace liph::net {

struct poller {
    std::vector<struct ::pollfd> fds;
    std::map<int, std::function<void()>> callback;

    bool need_update{false};

    void update() {
        if (need_update) {
            fds.resize(callback.size());
            int i = 0;
            for (const auto& pair : callback) {
                fds[i].fd = pair.first;
                fds[i].events = POLLIN;
                fds[i].revents = 0;
                i++;
            }
            need_update = false;
        }
    }
};

poll::poll() { _poller = std::make_unique<poller>(); }
poll::~poll() {}

bool poll::add(int id, std::function<void()>&& func) {
    if (_poller->callback.find(id) != _poller->callback.end()) return false;
    _poller->callback[id] = std::move(func);
    _poller->need_update = true;
    return true;
}

bool poll::del(int id) {
    if (_poller->callback.find(id) == _poller->callback.end()) return false;
    _poller->callback.erase(id);
    _poller->need_update = true;
    return true;
}

void poll::operator()() {
    _poller->update();
    int r = ::poll(_poller->fds.data(), _poller->fds.size(), _timeout);
    if (r <= 0) return;
    for (const auto& fd : _poller->fds) {
        if (fd.revents & POLLERR || fd.revents & POLLHUP || fd.revents & POLLNVAL) {
            del(fd.fd);
            continue;
        }
        if (fd.revents & POLLIN) {
            _poller->callback[fd.fd]();
        }
    }
    return;
}

}  // namespace liph::net

#elif defined(_WIN32) || defined(_WIN64)

#include <winsock2.h>

#else
#error "Unsupported platform"
#endif
