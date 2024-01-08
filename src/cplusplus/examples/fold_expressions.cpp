#include <iostream>
using namespace std;

template <class... T>
int sum(T... v) {
    // add all elements of v starting with 0
    return (v + ... + 0);
}

template <class... T>
void print(T&&...args) {
    (cout << ... << args) << '\n';
}

template <class T, class... T2>
bool oneof(T x, T2&&...args) {
    return ((x == args) || ...);
}

int main() {
    cout << sum(1) << endl;              // 1
    cout << sum(1, 2, 3, 4, 5) << endl;  // 15
    cout << sum(1, 3.14) << endl;        // 4
    cout << sum(1.9, 3.14) << endl;      // 5

    print();
    print(1);
    print(1, 2);
    print(1, 2, 3);

    cout << boolalpha;
    cout << oneof(1, 1, 2, 3) << endl;
    cout << oneof(-1, 1, 2, 3) << endl;
}
