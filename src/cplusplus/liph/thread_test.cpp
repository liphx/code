#include "liph/thread.h"

#include "gtest/gtest.h"

void f() {
    std::thread t([]() {});
    liph::thread_guard g(t);
    throw 0;
}

TEST(thread, thread_guard) {
    try {
        f();
    } catch (...) {
    }
}
