#include <iostream>
using namespace std;

class A {
public:
    int x_;
    A(int x = 0) : x_(x) { cout << "A(" << x_ << ")" << endl; }

    operator int() const {
        cout << "A::int()" << endl;
        return x_;
    }
};

void f(int x) { cout << "f(): " << x << endl; }

void g(A a) { cout << "g(): " << a.x_ << endl; }

int main() {
    g(10);
    f(A(20));
}
