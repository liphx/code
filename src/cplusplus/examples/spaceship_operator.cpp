#include <iostream>
#include <vector>
using namespace std;

int main() {
    cout << boolalpha;

    class A {
    public:
        A(int i) : v(i) {}
        auto operator<=>(const A&) const = default;

    private:
        int v;
    };
    A a(1), a2(2);
    cout << ((a <=> a2) == 0) << endl;  // false
    cout << ((a <=> a2) < 0) << endl;   // true
    cout << ((a <=> a2) > 0) << endl;   // false
    cout << (a < a2) << endl;           // true
    cout << (a > a2) << endl;           // false
    cout << (a <= a2) << endl;          // true
    cout << (a >= a2) << endl;          // false
    cout << (a == a2) << endl;          // false
    cout << (a != a2) << endl;          // true

    //////

    class B {
    public:
        B(int i = 0, int j = 0) : x(i), y(j) {}

        auto operator<=>(const B& b) const { return x * 10 + y - b.x * 10 - b.y; }

    private:
        int x, y;
    };
    B b(1, 2), b2(2, 1);
    cout << ((b <=> b2) == 0) << endl;  // false
    cout << ((b <=> b2) < 0) << endl;   // true
    cout << ((b <=> b2) > 0) << endl;   // false
    cout << (b < b2) << endl;           // true
    cout << (b > b2) << endl;           // false
    cout << (b <= b2) << endl;          // true
    cout << (b >= b2) << endl;          // false
    // no == and !=
    // cout << (b == b2) << endl;
    // cout << (b != b2) << endl;

    //////
    vector v{1, 2, 3, 4};
    vector v2{2, 3, 4};
    cout << (v == v2) << endl;  // false
    cout << (v < v2) << endl;   // true
}
