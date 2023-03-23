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

TEST(singleton, A) {
    A *a1 = singleton<A>::instance();
    EXPECT_EQ(a1->Get(), 0);
    a1->Set(20);

    A *a2 = singleton<A>::instance();
    EXPECT_EQ(a2->Get(), 20);
}

TEST(singleton, B) {
    B *b1 = singleton<B>::instance(10);
    EXPECT_EQ(b1->Get(), 10);
    b1->Set(20);

    B *b2 = singleton<B>::instance(10);
    EXPECT_EQ(b2->Get(), 20);
}

TEST(singleton, C) {
    C *c1 = singleton<C>::instance();
    EXPECT_EQ(c1->Get(), 0);
    c1->Set(20);

    C *c2 = singleton<C>::instance(10);
    EXPECT_EQ(c2->Get(), 10);
}

}  // namespace test
}  // namespace liph
