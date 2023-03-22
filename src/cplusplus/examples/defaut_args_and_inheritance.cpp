#include <iostream>
using namespace std;

class A {
public:
    virtual void f(int x = 100) { cout << "class A: " << x << endl; }
    virtual ~A() {}
};

class B : public A {
public:
    void f(int x = 200) { cout << "class B: " << x << endl; }
};

int main() {
    A *obj = new B;
    obj->f();

    delete obj;
}
