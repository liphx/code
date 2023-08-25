#include <exception>
#include <iostream>

class E {};
class A : public E {};
class B : public E {};

class C {
public:
    int *p;
    C() {
        try {
            p = new int;
            throw std::exception();
        } catch (...) {
            delete p;
            throw;
        }
    }
    ~C() { delete p; }
};

void f(int n) try {
    if (n < 0) throw std::out_of_range("n < 0");
} catch (std::exception& e) {
    std::cout << e.what() << std::endl;
}

int main() {
#if 0
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

    f(0);
    f(-1);
#endif
}
