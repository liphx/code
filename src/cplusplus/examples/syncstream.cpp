#include <iostream>
#include <syncstream>
#include <thread>
#include <vector>
using namespace std;

void foo() { cout << "foo " << this_thread::get_id() << endl; }

void bar() {
    osyncstream oss(cout);
    oss << "bar " << this_thread::get_id() << endl;
}

int main() {
    {
        vector<thread> threads(100);
        for (auto& t : threads) t = thread(foo);
        for (auto& t : threads) t.join();
    }
    cout << "======" << endl;
    {
        vector<thread> threads(100);
        for (auto& t : threads) t = thread(bar);
        for (auto& t : threads) t.join();
    }
}
