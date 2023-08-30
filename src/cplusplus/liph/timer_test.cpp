#include "liph/timer.h"

#include <iostream>

#include "gtest/gtest.h"
#include "liph/time.h"

struct A {
    A() {}
    A(const A&) { std::cout << "copy constructor" << std::endl; }
    void func() { std::cout << "here" << std::endl; }
};

TEST(timer, DISABLED_copy) {
    A a;
    liph::timer timer;
    timer.add(
            1, [](A a) { a.func(); }, a);
    timer.start();
    std::this_thread::sleep_for(std::chrono::seconds(3));
}

TEST(timer, DISABLED_timer) {
    liph::timer timer;
    timer.add(3, []() { std::cout << liph::time_format() << " hello" << std::endl; });
    timer.start();
    timer.add(6, []() { std::cout << liph::time_format() << " world" << std::endl; });

    std::this_thread::sleep_for(std::chrono::seconds(15));
    timer.stop();
}
