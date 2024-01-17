#include <atomic>
#include <iostream>
#include <mutex>
#include <thread>
using namespace std;

atomic<bool> is_init;
mutex mx;
int n;

void init() {
    if (!is_init) {
        unique_lock lock{mx};
        if (!is_init) {
            n = 42;  // init
            is_init = true;
        }
    }
}

void user() {
    init();
    cout << n << '\n';
}

int main() {
    jthread t(user);
    jthread t2(user);
}
