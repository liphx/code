#include <iostream>
#include <syncstream>
#include <thread>
using namespace std;

void foo() {
    for (int i = 0; i < 1000; ++i) {
        cout << i << " foo " << this_thread::get_id() << endl;
    }
}

void bar() {
    for (int i = 0; i < 1000; ++i) {
        osyncstream oss(cout);
        oss << i << " bar " << this_thread::get_id() << endl;
    }
}

int main() {
    {
        thread t(foo);
        thread t2(foo);
        t.join();
        t2.join();
    }
    cout << "======" << endl;
    {
        thread t(bar);
        thread t2(bar);
        t.join();
        t2.join();
    }
}
