#include <iostream>
#include <memory>

struct A {
    ~A() { std::cout << "~A()" << std::endl; }
};

struct B {
    ~B() { std::cout << "~B()" << std::endl; }
};

struct C {
    A a;
    B b;
};

int main() {
    auto pc = std::make_shared<C>();
    {
        // aliasing constructor
        std::shared_ptr<B> pb(pc, &pc->b);
        // b will not destruct here
    }
    std::cout << "here" << std::endl;
}

// output:
// here
// ~B()
// ~A()
