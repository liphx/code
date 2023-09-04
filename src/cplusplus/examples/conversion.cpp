#include <iostream>

struct A {
    int x;
    A(int x2 = 0) : x(x2) { std::cout << "A(" << x << ")" << std::endl; }

    operator int() const {
        std::cout << "A::int()" << std::endl;
        return x;
    }
};

void f(int x) { std::cout << "f(): " << x << std::endl; }

void g(A a) { std::cout << "g(): " << a.x << std::endl; }

int main() {
    g(10);
    f(A(20));
}
