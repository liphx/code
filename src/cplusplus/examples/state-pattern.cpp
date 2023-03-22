#include "liph/liph.h"

class A {
    class Base {
    public:
        virtual std::string str() = 0;
        virtual ~Base() {}
    };

    class Sub1 : public Base {
    public:
        std::string str() { return "sub1"; }
    };

    class Sub2 : public Base {
    public:
        std::string str() { return "sub2"; }
    };

    Base *base;

public:
    A() { base = new Sub1; }
    ~A() { delete base; }

    void print() {
        if (base) {
            liph::print(base->str());
        }
    }

    void change() {
        delete base;
        base = new Sub2;
    }
};

int main() {
    A a;
    a.print();
    a.change();
    a.print();

    return 0;
}
