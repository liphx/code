#include "liph/lang/no_destructor.h"

#include "gtest/gtest.h"

int i, j;

struct A {
    A() { ++i; }
    ~A() { ++j; }
};

TEST(no_destructor, no_destructor) {
    liph::no_destructor<A>();
    EXPECT_EQ(i, 1);
    EXPECT_EQ(j, 0);
}
