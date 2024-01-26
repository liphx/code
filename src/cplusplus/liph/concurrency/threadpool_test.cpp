#include "liph/concurrency/threadpool.h"

#include "gtest/gtest.h"
#include "liph/concurrency/thread.h"

namespace liph {

TEST(threadpool, size) {
    threadpool pool;
    EXPECT_GT(pool.size(), 0);
}

TEST(threadpool, start) {
    threadpool pool;
    EXPECT_THROW(pool.add([] {}), std::runtime_error);  // not started
    pool.stop();
    EXPECT_THROW(pool.add([] {}), std::runtime_error);  // stopped
}

TEST(threadpool, threadpool) {
    threadpool pool;
    pool.start();
    int i = 0;
    pool.add([&i] { i = 100; });
    pool.stop();
    EXPECT_EQ(i, 100);
}

TEST(threadpool, more) {
    threadpool pool(6);
    pool.start();
    std::atomic<int> n = 0;
    for (size_t i = 0; i < pool.size() + 1; ++i)
        pool.add([&n] {
            ++n;
            sleepms(10);
        });
    pool.stop();
    EXPECT_EQ(n, 7);
}

}  // namespace liph
