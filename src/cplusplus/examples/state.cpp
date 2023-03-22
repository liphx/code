#include <iostream>
#include <string>
using namespace std;

/*
 * 状态模式
 * 提供一个代理类，调用实现类中相应的函数
 * 状态模式动态地改变其实现类
 */
class A {
    class Base {
    public:
        virtual string f() = 0;
        virtual ~Base() {}
    };

    class Sub1 : public Base {
    public:
        string f() { return string("sub1"); }
    };

    class Sub2 : public Base {
    public:
        string f() { return string("sub2"); }
    };

    Base *base;

public:
    A() { base = new Sub1; }

    ~A() { delete base; }

    void print() { cout << base->f() << endl; }

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

    return 0;
}
