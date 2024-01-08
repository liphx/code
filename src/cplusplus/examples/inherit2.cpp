#include <iostream>
using namespace std;

struct A {
    virtual void f() = 0;
};

void A::f() { cout << "A::f()" << endl; }

struct B : A {
    virtual void f() { cout << "B::f()" << endl; }
};

int main() {
    A *a = new B;
    a->f();     // B::f()
    a->A::f();  // A::f()
}
