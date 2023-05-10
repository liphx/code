// https://leetcode.cn/problems/print-in-order/

#include "leetcode.h"

class Foo {
public:
    void first(function<void()> printFirst) {
        // printFirst() outputs "first". Do not change or remove this line.
        printFirst();
        ++count;
    }

    void second(function<void()> printSecond) {
        // printSecond() outputs "second". Do not change or remove this line.
        while (count < 1)
            ;
        printSecond();
        ++count;
    }

    void third(function<void()> printThird) {
        // printThird() outputs "third". Do not change or remove this line.
        while (count < 2)
            ;
        printThird();
        ++count;
    }

private:
    atomic<int> count{0};
};

class Foo2 {
public:
    void first(function<void()> printFirst) {
        printFirst();
        sem.release();
    }

    void second(function<void()> printSecond) {
        sem.acquire();
        printSecond();
        sem2.release();
    }

    void third(function<void()> printThird) {
        sem2.acquire();
        printThird();
    }

private:
    binary_semaphore sem{0};
    binary_semaphore sem2{0};
};

class Foo3 {
public:
    void first(function<void()> printfirst) {
        unique_lock<mutex> lock(mtx);
        printfirst();
        ++order;
        cv.notify_all();
    }

    void second(function<void()> printsecond) {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [this]() { return order == 1; });
        printsecond();
        ++order;
        cv.notify_one();
    }

    void third(function<void()> printthird) {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [this]() { return order == 2; });
        printthird();
    }

private:
    int order = 0;
    mutex mtx;
    condition_variable cv;
};

#define Foo Foo3
int main() {
    Foo foo;
    thread t(&Foo::first, &foo, []() { cout << "first" << endl; });
    thread t2(&Foo::second, &foo, []() { cout << "second" << endl; });
    thread t3(&Foo::third, &foo, []() { cout << "third" << endl; });
    t.join();
    t2.join();
    t3.join();
}
