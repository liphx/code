#include <exception>
#include <iostream>

struct E {};
struct A : E {};
struct B : E {};

struct C {
    int *ptr;

    C() {
        try {
            ptr = new int;
            throw std::exception();
        } catch (...) {
            delete ptr;
            throw;
        }
    }

    ~C() { delete ptr; }
};

void foo(int n) try {
    if (n < 0) throw std::out_of_range("n < 0");
} catch (std::exception& e) {
    std::cout << e.what() << std::endl;
}

int main() {
    foo(0);
    foo(-1);

    try {
        throw B();
    } catch (E&) {
        std::cout << "E" << std::endl;
    } catch (A&) {
        std::cout << "A" << std::endl;
    } catch (B&) {
        std::cout << "B" << std::endl;
    }
    // E

    try {
        throw B();
    } catch (E) {
        std::cout << "E" << std::endl;
    } catch (A) {
        std::cout << "A" << std::endl;
    } catch (B) {
        std::cout << "B" << std::endl;
    }
    // E

    try {
        C();
    } catch (std::exception& e) {
        std::cout << "catch exception" << std::endl;
    }
}
