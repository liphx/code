#include "liph/concurrency/blocking_queue.h"

#include <thread>

#include "gtest/gtest.h"

struct A {
    A(int) {}
};

struct B {
    B() = default;
    B(const B&) = default;
    B& operator=(const B&) = default;

    B(const B&&) = delete;
    B& operator=(const B&&) = delete;
};

TEST(blocking_queue, pop) {
    liph::blocking_queue<A> q;
    q.push(A(0));
    A a(0);  // must construct `struct A` before pop
    EXPECT_TRUE(q.pop(a));
    EXPECT_TRUE(q.empty());

    liph::blocking_queue<B> qb;
    qb.push(B());
    // can not `move`
    // B b;
    // qb.pop(b);
}

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
        int fail = 0;
        int loop = 0;
        while (count < n) {
            int i;
            if (q.pop(i)) {
                EXPECT_EQ(i, count);
                ++count;
            } else {
                ++fail;
            }
            ++loop;
        }
        EXPECT_EQ(count + fail, loop);
    });

    t.join();
    t2.join();
}
