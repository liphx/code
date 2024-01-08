#include <iostream>
#include <string>
using namespace std;

class A {
    class Base {
    public:
        virtual string str() = 0;
        virtual ~Base() {}
    };

    class Sub1 : public Base {
    public:
        string str() { return "sub1"; }
    };

    class Sub2 : public Base {
    public:
        string str() { return "sub2"; }
    };

    Base *base;

public:
    A() { base = new Sub1; }
    ~A() { delete base; }

    void print() { cout << base->str() << endl; }

    void change() {
        delete base;
        base = new Sub2;
    }
};

int main() {
    A a;
    a.print();  // sub1
    a.change();
    a.print();  // sub2
}
