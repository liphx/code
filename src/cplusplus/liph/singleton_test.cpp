#include "singleton.h"

#include "gtest/gtest.h"

namespace liph {
namespace test {

class A {
public:
    A() : x(0) {}
    int Get() const { return x; }
    void Set(int x) { this->x = x; }

private:
    int x;
};

class B {
public:
    B(int x) : x(x) {}
    int Get() const { return x; }
    void Set(int x) { this->x = x; }

private:
    int x;
};

class C {
public:
    C() : x(0) {}
    C(int x) : x(x) {}
    int Get() const { return x; }
    void Set(int x) { this->x = x; }

private:
    int x;
};

TEST(Singleton, A) {
    A *a1 = Singleton<A>::GetInstance();
    EXPECT_EQ(a1->Get(), 0);
    a1->Set(20);

    A *a2 = Singleton<A>::GetInstance();
    EXPECT_EQ(a2->Get(), 20);
}

TEST(Singleton, B) {
    B *b1 = Singleton<B>::GetInstance(10);
    EXPECT_EQ(b1->Get(), 10);
    b1->Set(20);

    B *b2 = Singleton<B>::GetInstance(10);
    EXPECT_EQ(b2->Get(), 20);
}

TEST(Singleton, C) {
    C *c1 = Singleton<C>::GetInstance();
    EXPECT_EQ(c1->Get(), 0);
    c1->Set(20);

    C *c2 = Singleton<C>::GetInstance(10);
    EXPECT_EQ(c2->Get(), 10);
}

}  // namespace test
}  // namespace liph
