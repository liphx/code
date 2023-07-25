#include "liph/concurrency/thread.h"

#include "gtest/gtest.h"

void f() {
    std::thread t([]() {});
    liph::thread_guard g(t);
    throw 0;
}

void g() {
    std::thread t([]() {});
    throw 0;
    t.join();
}

TEST(thread, thread_guard) {
    try {
        f();
        EXPECT_THROW(g(), int);
    } catch (...) {
    }
}

TEST(thread, scoped_thread) {
    {
        std::thread t;
        EXPECT_THROW({ liph::scoped_thread(std::move(t)); }, std::logic_error);
    }

    int n = 0;
    {
        liph::scoped_thread t(std::thread([](int& n) { n++; }, std::ref(n)));
    }
    EXPECT_EQ(n, 1);
}
