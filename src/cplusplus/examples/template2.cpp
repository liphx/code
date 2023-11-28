#include <iostream>
using namespace std;

template <class T>
struct A {
    void foo() { cout << "A<T>::foo" << endl; }
};

// template <>
// struct A<int> {};

template <class T>
struct B : A<T> {
    void bar() {
        // foo();
        this->foo();
    }

    void bar2() {
        // foo();
        using A<T>::foo;
        foo();
    }

    void bar3() {
        // foo();
        A<T>::foo();
    }
};

int main() {
    B<int> b;
    b.foo();
}
