#include "latch"

#include <cstdio>
#include <thread>

#include "gtest/gtest.h"

TEST(latch, latch) {
    int n = 8;
    std::vector<std::thread> threads(n);
    ltl::latch done(n);
    for (int i = 0; i < n; i++) {
        threads[i] = std::thread([&done, i]() {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            printf("thread %d done\n", i);
            done.count_down();
        });
    }
    done.wait();
    puts("all done");
    for (int i = 0; i < n; i++) {
        threads[i].join();
    }
}
