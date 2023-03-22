class A {
public:
    virtual ~A() = 0;
};

A::~A() {}

class B : public A {};

int main() { B b; }
