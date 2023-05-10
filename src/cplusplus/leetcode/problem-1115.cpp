// https://leetcode.cn/problems/print-foobar-alternately/

#include "leetcode.h"

class FooBar {
private:
    int n;
    bool flag{true};
    mutex mtx;
    condition_variable cv;

public:
    FooBar(int n) { this->n = n; }

    void foo(function<void()> printFoo) {
        for (int i = 0; i < n; i++) {
            unique_lock<mutex> lock(mtx);
            cv.wait(lock, [this]() { return flag; });
            printFoo();
            flag = false;
            cv.notify_one();
        }
    }

    void bar(function<void()> printBar) {
        for (int i = 0; i < n; i++) {
            unique_lock<mutex> lock(mtx);
            cv.wait(lock, [this]() { return !flag; });
            printBar();
            flag = true;
            cv.notify_one();
        }
    }
};

int main() {
    int n = 3;
    FooBar obj(n);
    thread t(&FooBar::foo, &obj, []() { cout << "foo" << endl; });
    thread t2(&FooBar::bar, &obj, []() { cout << "bar" << endl; });
    t.join();
    t2.join();
}
