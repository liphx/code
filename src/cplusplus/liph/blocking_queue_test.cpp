#include "liph/blocking_queue.h"

#include <thread>

#include "gtest/gtest.h"
#include "liph/print.h"

TEST(blocking_queue, threads) {
    liph::blocking_queue<int> q;
    int n = 100000;

    std::thread t([&q, n]() {
        for (int i = 0; i < n; ++i) {
            q.push(i);
        }
    });
    std::thread t2([&q, n]() {
        int count = 0;
        int loop = 0;
        while (count < n) {
            int i;
            if (q.pop(i)) {
                EXPECT_EQ(i, count);
                ++count;
            }
            ++loop;
        }
        P(count, loop);
    });

    t.join();
    t2.join();
}
