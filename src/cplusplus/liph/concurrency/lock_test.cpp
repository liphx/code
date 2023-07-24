#include "liph/concurrency/lock.h"

#include <thread>

#include "gtest/gtest.h"

TEST(lock, spinlock) {
    liph::spinlock sl;
    auto f = [&sl](int n, int& x) {
        for (int i = 0; i < n; ++i) {
            liph::spinlock::scoped_lock lock(sl);
            ++x;
        }
    };
    int x = 0;
    std::thread t(f, 100000, std::ref(x));
    std::thread t2(f, 100000, std::ref(x));
    t.join();
    t2.join();
    EXPECT_EQ(x, 200000);
}
