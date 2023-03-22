#include <iostream>
using namespace std;

/*
 * 代理模式
 * 提供一个代理类，代码与代理类打交道，而做实际工作的类隐藏在代理类背后
 * 当调用代理类中的一个函数时，代理类仅转而去调用实现类中相应的函数
 * 代理模式只有一个实现类，代理模式控制对其实现类的访问
 */
class ProxyBase {
public:
    virtual void f() = 0;
    virtual void g() = 0;
    virtual void h() = 0;
    virtual ~ProxyBase() {}
};

class Implementation : public ProxyBase {
public:
    void f() { cout << "Implementation.f()" << endl; }
    void g() { cout << "Implementation.g()" << endl; }
    void h() { cout << "Implementation.h()" << endl; }
};

class Proxy : public ProxyBase {
    ProxyBase *implementation;

public:
    Proxy() { implementation = new Implementation; }

    ~Proxy() { delete implementation; }

    void f() { implementation->f(); }
    void g() { implementation->g(); }
    void h() { implementation->h(); }
};

int main() {
    Proxy p;
    p.f();  // Implementation.f()
    p.g();  // Implementation.g()
    p.h();  // Implementation.h()

    return 0;
}
