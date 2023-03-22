#include <iostream>

struct B {
    B(int i) { std::cout << "B(" << i << ")" << std::endl; }
};

class A {
public:
    // Wreorder
    A(int i) : second(i), first(second) {}
    B first;
    int second;
};

int main() { A(100); }
