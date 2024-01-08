#include <iostream>

struct A {
    A(int i) { std::cout << "A:" << i << std::endl; }
};

class B {
public:
    // B() {}
    B(int i) : a(i) {}
    B(int i, int) { a = i; }

private:
    A a = 0;
};

int main() {
    B b(1);
    B b2(2, 0);
}
