#include <iostream>
#include <mutex>
#include <thread>
using namespace std;

mutex m1, m2, m3;
int n1, n2, n3;

void f() {
    for (int i = 0; i < 1000; ++i) {
        scoped_lock lock{m1, m2, m3};
        ++n1;
        ++n2;
        ++n3;
    }
}

int main() {
    {
        jthread t(f);
        jthread t2(f);
    }
    cout << n1 << ' ' << n2 << ' ' << n3 << '\n';
}
