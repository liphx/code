// 带多态性质的基类应该声明 virtual destructor, 即如果基类带有 virtual 成员函数, 就应该带虚析构函数

class A {};

class B : public A {
public:
    ~B() { std::cout << "~B()" << std::endl; }
};

class C {
public:
    virtual ~C() {}
};

class D : public C {
public:
    ~D() { std::cout << "~D()" << std::endl; }
};

int main() {
    A *a = new B;
    delete a;

    C *c = new D;
    delete c;
}
