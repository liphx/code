#include <iostream>

struct A {
    virtual ~A() {}
};
struct B : A {};
struct C : A {};

int main() {
    B b;
    A *ap = &b;

    C *cp = dynamic_cast<C *>(ap);
    std::cout << cp << std::endl;

    try {
        C& cr = dynamic_cast<C&>(*ap);
    } catch (std::bad_cast& e) {
        std::cout << e.what() << std::endl;
    }
}
