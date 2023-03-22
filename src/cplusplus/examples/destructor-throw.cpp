#include <iostream>

struct A {
    ~A() {
        std::cout << "~A()" << std::endl;
        throw 0;
    }
};

int main() {
    try {
        A();
        std::cout << "after ~A()" << std::endl;
    } catch (...) {
        std::cout << "catch" << std::endl;
    }
}
