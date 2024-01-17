#include <future>
#include <iostream>
#include <thread>
using namespace std;
using namespace std::literals;

int f(int n) {
    this_thread::sleep_for(100ms);
    return n + 1;
}

int main() {
    packaged_task task(f);
    auto fut = task.get_future();
    jthread t(move(task), 100);
    cout << fut.get() << '\n';

    packaged_task task2(f);
    auto fut2 = task2.get_future();
    task2(200);
    cout << fut2.get() << '\n';
}
