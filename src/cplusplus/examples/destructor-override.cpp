struct A {
    virtual ~A() {}
};

struct B : A {
    ~B() override {}
};

int main() {}
