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
        /* g(); */
    } catch (...) {
    }
}
