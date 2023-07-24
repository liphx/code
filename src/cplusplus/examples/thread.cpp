#include <iostream>
#include <thread>

#include "liph/liph.h"

struct A {
    A() { std::cout << "t: " << std::this_thread::get_id() << ", A: " << this << std::endl; }
    A(const A& other) {
        std::cout << "t: " << std::this_thread::get_id() << ", A(" << &other << "): " << this << std::endl;
    }
    ~A() { std::cout << "t: " << std::this_thread::get_id() << ", ~A: " << this << std::endl; }
    void operator()() { std::cout << "t: " << std::this_thread::get_id() << ", " << this << std::endl; }
};

#if 0
int main() {
    A a;
    a();
    std::thread t(a);  // copy a
    t.join();

    // std::thread t(A()); it is a function declaration
    // use std::thread t((A()));
    // or std::thread t{A()};
}
#endif

void f() {
    std::thread t([]() {});
    try {
        throw 0;
    } catch (...) {
        t.join();
        throw;
    }
    t.join();
}

void g() {
    std::thread t([]() {});
    liph::thread_guard guard(t);
    throw 0;
}

int main() {
    try {
        f();
    } catch (...) {
    }

    try {
        g();
    } catch (...) {
    }
}
