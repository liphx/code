#include <future>
#include <iostream>
#include <thread>
using namespace std;
using namespace std::literals;

int main() {
    promise<int> p;

    jthread t([&]() {
        this_thread::sleep_for(1s);
        p.set_value(42);
    });

    future<int> fut = p.get_future();
    cout << fut.get() << '\n';  // wait and get

    //////

    promise<int> p2;
    jthread t3([&]() { p2.set_exception(make_exception_ptr(-1)); });

    this_thread::sleep_for(1s);
    future<int> fut2 = p2.get_future();
    try {
        fut2.get();
    } catch (int n) {
        cout << "catch " << n << '\n';
    }
}
