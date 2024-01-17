#include <array>
#include <iostream>

struct A {
    virtual void f() = 0;
};

struct B : A {
    B(int n) : i(n) {}
    int i;
    void f() override { std::cout << i << '\n'; }
};

int main() {
    std::array<int, 0> arr;
    std::cout << arr.size() << '\n';
    std::cout << arr.empty() << '\n';

    std::cout << "sizeof(A)=" << sizeof(A) << '\n';
    std::cout << "sizeof(B)=" << sizeof(B) << '\n';
    B b[4]{B(0), B(1), B(2), B(3)};
    A *p = b;
    // bug
    p[2].f();  // 1
}
