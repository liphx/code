#include <sys/time.h>  // gettimeofday

#include <cstdint>
#include <iostream>
#include <thread>

inline int64_t gettimeofday_us() {
    struct timeval now;
    (void)gettimeofday(&now, nullptr);
    return now.tv_sec * 1000000L + now.tv_usec;
}

int main() {
    int percent = 60;  // [1, 99]
    int busy = 1000;   // microseconds
    int idle = (busy * (100.0 - percent)) / percent;
    int64_t t = gettimeofday_us();
    std::cout << "percent = " << percent << ", busy = " << busy << ", idle = " << idle << std::endl;
    while (true) {
        int64_t now = gettimeofday_us();
        if (now - t > busy) {
            std::this_thread::sleep_for(std::chrono::microseconds(idle));
            t = now;  // after sleep
        }
    }
}
