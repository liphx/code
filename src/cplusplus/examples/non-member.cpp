struct A {
    A(int a = 0) {}
    const A operator*(const A&) { return *this; }
};

struct B {
    B(int b = 0) {}
};

const B operator*(const B&, const B&) { return B(); }

int main() {
    A a;
    a * 2;
    // 2 * a;
    // compile error

    B b;
    b * 2;
    2 * b;
}
