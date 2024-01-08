#include <iostream>
using namespace std;

int f() {
    cout << "f()" << endl;
    return 42;
}

int main() {
    auto i = f();
    decltype(f()) j = i;
    cout << i << " " << j << endl;
}
