#include <iostream>

struct A {
    int *& ref;
    A(int *& i) : ref(i) {}
    ~A() { std::cout << *ref << std::endl; }
};

int main(int argc, char **) {
    int i = 9;
    int *p = &i;
    A a(p);
    if (argc == 1) {
        std::cout << "1" << std::endl;
        return 0;
    }

    std::cout << "2" << std::endl;
    return 0;
}
