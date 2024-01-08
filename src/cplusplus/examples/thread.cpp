#include <iostream>
#include <thread>

class thread_guard {
public:
    explicit thread_guard(std::thread& t) : t_(t) {}
    ~thread_guard() {
        if (t_.joinable()) {
            t_.join();
        }
    }
    thread_guard(const thread_guard&) = delete;
    thread_guard& operator=(const thread_guard&) = delete;

private:
    std::thread& t_;
};

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
    thread_guard guard(t);
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
