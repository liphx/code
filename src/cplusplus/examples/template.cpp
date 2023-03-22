class A {
public:
    A(int a) {}
};

const A operator*(const A& lhs, const A& rhs) { return lhs; }

/////////

template <class T>
class B {
public:
    B(T b) {}
};

template <class T>
const B<T> operator*(const B<T>& lhs, const B<T>& rhs) {
    return lhs;
}

/////////

template <class T>
class C {
public:
    C(T c) {}

    friend const C operator*(const C& lhs, const C& rhs);
    // 等同下面的声明
    // friend const C<T> operator*(const C<T>& lhs, const C<T>& rhs);
};

template <class T>
const C<T> operator*(const C<T>& lhs, const C<T>& rhs) {
    return lhs;
}

/////////

template <class T>
class D {
public:
    D(T d) {}

    friend const D operator*(const D& lhs, const D& rhs) { return lhs; }
};

int main() {
    A a(0);
    a * 2;
    2 * a;

    B<int> b(0);
    // compile error
    // b * 2;
    // 2 * b;

    C<int> c(0);
    // link error
    // c * 2;
    // 2 * c;

    D<int> d(0);
    d * 2;
    2 * d;
}
