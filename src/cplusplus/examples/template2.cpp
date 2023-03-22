template <class T>
class A {
public:
    void f() {}
};

/* template<> class A<int> {}; */

template <class T>
class B : public A<T> {
public:
    void g() {
        /* f(); */
        this->f();
    }

    void h() {
        /* f(); */
        using A<T>::f;
        f();
    }

    void i() {
        /* f(); */
        A<T>::f();
    }
};

int main() {
    B<int> b;
    b.f();
}
