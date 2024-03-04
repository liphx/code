#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <thread>
using namespace std;

shared_mutex mx;
int n;

int reader() {
    shared_lock lock{mx};
    return n;
}

void writer(int x) {
    unique_lock lock{mx};
    n = x;
}

int main() {
    jthread t([]() { cout << reader() << '\n'; });
    jthread t2(writer, 42);
}
