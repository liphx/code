#include "liph/lang/no_destructor.h"

#include <iostream>

#include "gtest/gtest.h"

namespace liph {

int i, j;

struct A {
    A() { ++i; }
    ~A() { ++j; }
};

struct B {
    // B(int x_) : x(x_) {}
    B(int x_) : x(x_) { std::cout << "x = " << x << std::endl; }
    ~B() { x = 0; }
    int x;
};

TEST(no_destructor, no_destructor) {
    no_destructor<A>();
    EXPECT_EQ(i, 1);
    EXPECT_EQ(j, 0);

    const auto *b = no_destructor<B>(100).get();
    EXPECT_EQ(b->x, 100);
}

}  // namespace liph
