#ifndef LIPH_TIME_H_
#define LIPH_TIME_H_

#include <sys/time.h>

#include <cstdint>
#include <cstdio>

namespace liph {

inline int64_t gettimeofday_us() {
    struct timeval now;
    if (gettimeofday(&now, NULL) != 0) {
        perror("gettimeofday");
    }
    return now.tv_sec * 1000000L + now.tv_usec;
}

inline int64_t gettimeofday_ms() { return gettimeofday_us() / 1000L; }

inline int64_t gettimeofday_s() { return gettimeofday_us() / 1000000L; }

};  // namespace liph

#endif  // LIPH_TIME_H_
