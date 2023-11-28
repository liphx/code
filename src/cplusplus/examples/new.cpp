#include <cstdio>
#include <cstdlib>

void *operator new(size_t sz) {
    puts("::operator new");
    return malloc(sz);
}

void operator delete(void *ptr) noexcept {
    puts("::operator delete");
    free(ptr);
}

struct A {
    A() { puts("A()"); }
    ~A() { puts("~A()"); }
};

struct B {
    B() { puts("B()"); }
    ~B() { puts("~B()"); }

    // static can be omitted
    void *operator new(size_t sz) {
        puts("B::operator new");
        return malloc(sz);
    }

    void operator delete(void *ptr) {
        puts("B::operator delete");
        free(ptr);
    }
};

struct C {
    C() { puts("C()"); }
    ~C() { puts("~C()"); }

    void *operator new[](size_t sz) {
        puts("C::operator new[]");
        return malloc(sz);
    }
    void operator delete[](void *ptr) {
        puts("C::operator delete[]");
        free(ptr);
    }
};

struct D {
    int i;
    D(int ii = 0) : i(ii) { printf("D(), i = %d\n", i); }
    ~D() { printf("~D(), i = %d\n", i); }

    // Placement new
    void *operator new(size_t, void *loc) {
        puts("D::operator new");
        return loc;
    }
};

int main() {
    int *pi = new int;
    delete pi;

    A *pa = new A;
    delete pa;
    // ::operator new
    // A()
    // ~A()
    // ::operator delete

    B *pb = new B;
    delete pb;
    // B::operator new
    // B()
    // ~B()
    // B::operator delete

    B *pbs = new B[1];
    delete[] pbs;
    // B()
    // ~B()

    C *pcs = new C[1];
    delete[] pcs;
    // C::operator new[]
    // C()
    // ~C()
    // C::operator delete[]

    D d1(1);
    D *pd1 = &d1;
    (void)new (pd1) D(2);
    printf("i = %d\n", d1.i);
    // D(), i = 1
    // D::operator new
    // D(), i = 2
    // i = 2
    // ~D(), i = 2 (destruct once)
}

