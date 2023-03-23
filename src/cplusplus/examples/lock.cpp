#include <iostream>
#include <mutex>
#include <thread>
using namespace std;

class A {
public:
    A(int i) : i_(i) {}
    int i_;
    mutex m_;
};

void Swap(A& a1, A& a2, size_t times) {
    for (size_t i = 0; i < times; i++) {
        lock_guard<mutex> lock(a1.m_);
        lock_guard<mutex> lock2(a2.m_);
        std::swap(a1.i_, a2.i_);
    }
}

void Swap2(A& a1, A& a2, size_t times) {
    for (size_t i = 0; i < times; i++) {
        lock_guard<mutex> lock(*min(&a1.m_, &a2.m_));
        lock_guard<mutex> lock2(*max(&a1.m_, &a2.m_));
        std::swap(a1.i_, a2.i_);
    }
}

int main() {
    A a(1), a2(2);
    thread t(Swap2, ref(a), ref(a2), 100000);
    thread t2(Swap2, ref(a2), ref(a), 100000);
    t.join();
    t2.join();

    cout << a.i_ << " " << a2.i_ << endl;
}
