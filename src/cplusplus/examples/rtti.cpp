#include <iostream>
#include <typeinfo>

struct A {
    virtual ~A() { std::cout << "~A" << std::endl; }
    virtual void f() { std::cout << "A::f" << std::endl; };
};

struct B : A {
    ~B() { std::cout << "~B" << std::endl; }
    void f() { std::cout << "B::f" << std::endl; };
};

int main() {
    A *a = new A;
    a->f();  // A::f
    A *b = new B;
    b->f();  // B::f

    B *c = dynamic_cast<B *>(b);
    std::cout << c << std::endl;
    B *d = dynamic_cast<B *>(a);
    std::cout << d << std::endl;  // 0

    std::cout << typeid(a).name() << std::endl;   // P1A
    std::cout << typeid(b).name() << std::endl;   // P1A
    std::cout << typeid(*a).name() << std::endl;  // 1A
    std::cout << typeid(*b).name() << std::endl;  // 1B

    delete a;  // ~A
    delete b;
    // ~B
    // ~A
}
