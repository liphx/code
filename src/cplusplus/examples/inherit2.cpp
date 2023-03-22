class A {
public:
    virtual void f() = 0;
};

void A::f() { cout << "A::f()" << endl; }

class B : public A {
public:
    virtual void f() { cout << "B::f()" << endl; }
};

int main() {
    A *a = new B;
    a->f();
    a->A::f();
}
