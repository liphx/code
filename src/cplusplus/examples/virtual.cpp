// 不在构造与析构函数中调用虚函数

class A {
public:
    A() { f(); }
    virtual ~A() { f(); }
    virtual void f() { std::cout << "A::f()" << std::endl; }
};

class B : public A {
public:
    void f() { std::cout << "B::f()" << std::endl; }
};

int main() {
    A *obj = new B;
    delete obj;
}
