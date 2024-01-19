#pragma once

#include <functional>

#include "net/socket.h"

namespace liph::net {

class poller;

class poll {
public:
    poll();
    ~poll();
    bool add(int, std::function<void()>&&);
    bool del(int);

    void operator()();

    int timeout() const { return _timeout; }
    void timeout(int t) { _timeout = t; }

private:
    std::unique_ptr<poller> _poller;
    // timeout = -1, wait forever; timeout = 0, do not wait; timeout > 0, wait timeout milliseconds
    int _timeout{-1};
};

}  // namespace liph::net
