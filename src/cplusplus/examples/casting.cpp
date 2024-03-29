#include <iostream>
using namespace std;

namespace tmp {

struct A {
    int a;
};

struct B : public A {
    virtual void f() {}
};

void f() {
    {
        int x = int(1.5);
        int y = (int)1.5;
        int z = static_cast<int>(1.5);
        cout << x << endl;
        cout << y << endl;
        cout << z << endl;
    }
    {
        // pa == pb ?
        B b;
        B *pb = &b;
        A *pa = pb;
        cout << "pa = " << pa << endl;
        cout << "pb = " << pb << endl;
    }
}

}  // namespace tmp

struct A {
    A() : a(0) {}
    /* A(const A& a) = delete; */
    int a;
    virtual void f() { a = 1; }
};

struct B : public A {
public:
    virtual void f() {
        // call A(const A&)
        static_cast<A>(*this).f();
    }
};

struct C : public A {
public:
    virtual void f() { A::f(); }
};

int main() {
    B b;
    b.f();
    cout << b.a << endl;

    C c;
    c.f();
    cout << c.a << endl;
}
