#include <cstdio>
#include <cstdlib>

void *operator new(size_t sz) {
    puts("::operator new");
    return malloc(sz);
}

void operator delete(void *m) noexcept {
    puts("::operator delete");
    free(m);
}

class A {
public:
    A() { puts("A()"); }
    ~A() { puts("~A()"); }
};

// 为一个类重载new和delete时，尽管不必显示地使用static，但实际上仍是在创建static成员函数
// 如果一个类重载了 operator new() 和 operator delete()
// 但要创建这个类的一个数组对象时，全局new将会被调用
class B {
public:
    B() { puts("B()"); }
    ~B() { puts("~B()"); }
    void *operator new(size_t sz) {
        puts("B::operator new");
        return malloc(sz);
    }
    void operator delete(void *m) {
        puts("B::operator delete");
        free(m);
    }
};

// 可以通过重载 operator new[], operator delete[] 来控制对象数组的内存分配
class C {
public:
    C() { puts("C()"); }
    ~C() { puts("~C()"); }
    void *operator new[](size_t sz) {
        puts("C::operator new[]");
        return malloc(sz);
    }
    void operator delete[](void *m) {
        puts("C::operator delete[]");
        free(m);
    }
};

// 定位new
class D {
public:
    int i;
    D(int ii = 0) : i(ii) { printf("D(), i = %d\n", i); }
    ~D() { printf("~D(), i = %d\n", i); }
    void *operator new(size_t, void *loc) {
        puts("D::operator new");
        return loc;
    }
};

int main() {
    int *pi = new int;  // ::operator new
    delete pi;

    A *pa = new A;  // ::operator new
    delete pa;

    B *pb = new B;  // B::operator new
    delete pb;

    B *pbs = new B[1];  // ::operator new
    delete[] pbs;

    C *pcs = new C[1];  // C::operator new[]
    delete[] pcs;

    D d1(1);
    D *pd1 = &d1;
    (void)new (pd1) D(2);
    printf("i = %d\n", d1.i);
    // 只析构了一次
}
