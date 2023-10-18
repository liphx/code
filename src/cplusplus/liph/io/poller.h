#ifndef LIPH_IO_POLLER_H_
#define LIPH_IO_POLLER_H_

#ifdef OS_LINUX

#include <poll.h>

namespace liph {

class poller {
public:
    enum event { none, read, write };

public:
    // nfds: fdarray element size
    // timeout = -1, wait forever; timeout = 0, do not wait; timeout > 0, wait timeout milliseconds
    // return 0 if timeout, -1 if error and number of ready file descriptors otherwise
    static int poll(struct pollfd fdarray[], nfds_t nfds, int timeout) { return ::poll(fdarray, nfds, timeout); }

private:
};

}  // namespace liph

#endif

#endif  // LIPH_IO_POLLER_H_
