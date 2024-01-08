#include <iostream>
using namespace std;

void example() {
    struct A {
        int x;
        A(int a) : x(a) {}
    };

    struct B {
        int y;
        B(int b) : y(b) {}
    };

    struct C : A, B {
        int z;
        C(int a, int b, int c) : A(a), B(b), z(c) {}
    };

    cout << sizeof(A) << " " << sizeof(B) << " " << sizeof(C) << endl;  // 4 4 12
    C c(1, 2, 3);
    cout << c.x << " " << c.y << " " << c.z << endl;  // 1 2 3

    A *a = &c;
    B *b = &c;
    cout << a << " " << b << " " << &c << endl;

    C *c1 = static_cast<C *>(a);
    C *c2 = static_cast<C *>(b);
    cout << c1 << " " << c2 << endl;
}

void example2() {
    struct A {
        int x;
        A(int a) : x(a) {}
    };

    struct B1 : A {
        int y1;
        B1(int a, int b1) : A(a), y1(b1) {}
    };

    struct B2 : A {
        int y2;
        B2(int a, int b2) : A(a), y2(b2) {}
    };

    struct C : B1, B2 {
        int z;
        C(int a, int b1, int b2, int c) : B1(a, b1), B2(a, b2), z(c) {}
    };

    cout << sizeof(A) << " " << sizeof(B1) << " " << sizeof(B2) << " " << sizeof(C) << " " << endl;
    // 4 8 8 20

    C c(1, 2, 3, 4);
    // c.x;
    // error: 'x' is ambiguous

    cout << c.y1 << " " << c.y2 << " " << c.z << endl;  // 2 3 4

    B1 *b1 = &c;
    B2 *b2 = &c;
    cout << b1 << " " << b2 << endl;
    cout << b1->x << " " << b2->x << endl;  // 1 1

    // A *a = &c;
    // 'A' is ambiguous

    A *a1 = b1, *a2 = b2;
    cout << a1->x << " " << a2->x << endl;  // 1 1
}

void example3() {
    struct A {
        int x;
        A(int a) : x(a) {}
    };

    struct B1 : virtual public A {
        int y1;
        B1(int a, int b1) : A(a), y1(b1) {}
    };

    struct B2 : virtual public A {
        int y2;
        B2(int a, int b2) : A(a), y2(b2) {}
    };

    struct C : public B1, public B2 {
        int z;
        C(int a, int b1, int b2, int c) : A(a), B1(0, b1), B2(0, b2), z(c) {}
    };

    cout << sizeof(A) << " " << sizeof(B1) << " " << sizeof(B2) << " " << sizeof(C) << " " << endl;
    // 4 16 16 40

    C c(1, 2, 3, 4);
    cout << c.x << " " << c.y1 << " " << c.y2 << " " << c.z << endl;  // 1 2 3 4

    B1 *b1 = &c;
    B2 *b2 = &c;
    cout << b1 << " " << b2 << endl;
    cout << b1->x << " " << b2->x << endl;  // 1 1

    A *a = &c;
    cout << a << endl;

    A *a1 = b1, *a2 = b2;
    cout << a1->x << " " << a2->x << endl;  // 1 1
}

void example4() {
    struct A {};

    struct B1 : virtual A {
        void f() { cout << "B1::f()" << endl; }
    };

    struct B2 : virtual A {
        void f() { cout << "B2::f()" << endl; }
    };

    struct C : B1, B2 {};

    struct D : B1, B2 {
        using B1::f;
    };

    C c;
    // c.f();
    // 'f' is ambiguous

    D d;
    d.f();  // B1::f()
}

int main() {
    std::cout << "======" << std::endl;
    example();
    std::cout << "======" << std::endl;
    example2();
    std::cout << "======" << std::endl;
    example3();
    std::cout << "======" << std::endl;
    example4();
    std::cout << "======" << std::endl;
}
