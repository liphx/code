class A {};
class B : public A {};
class C : private A {};
class D : protected A {};

void f(const A&) {}

void f() {
    f(A());
    f(B());
    /* f(C()); */
    /* f(D()); */
}

class AA {
public:
    virtual void f() {}
    virtual void f(int) {}
};

class BB : public AA {
public:
    using AA::f;
    virtual void f() {}
};

int main() {
    BB b;
    b.f();
    b.f(1);
}
