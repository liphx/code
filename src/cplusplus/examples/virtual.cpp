#include <iostream>
// 不在构造与析构函数中调用虚函数

class A {
public:
    A() { f(); }
    virtual ~A() {
        std::cout << "~A()" << std::endl;
        f();
    }
    virtual void f() { std::cout << "A::f()" << std::endl; }
};

class B : public A {
public:
    void f() { std::cout << "B::f()" << std::endl; }
    ~B() { std::cout << "~B()" << std::endl; }
};

int main() {
    A *obj = new B;
    delete obj;
}
