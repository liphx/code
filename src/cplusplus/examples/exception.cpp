// 别让异常逃离析构函数

class A {
public:
    ~A() throw(...) {
        std::cout << "~A() begin" << std::endl;
        throw std::exception();
        std::cout << "~A() end" << std::endl;
    }
};

class B {
public:
    void f() { throw std::exception(); };
    ~B() throw(...) {
        std::cout << "~B() begin" << std::endl;
        throw std::exception();
        std::cout << "~B() end" << std::endl;
    }
};

int main() {
    A *a = new A;
    try {
        delete a;
    } catch (std::exception&) {
    }

    A *arr = new A[2];
    try {
        delete[] arr;
    } catch (std::exception&) {
    }

    try {
        B b;
        b.f();
    } catch (std::exception&) {
    }
}
