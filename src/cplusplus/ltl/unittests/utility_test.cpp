#include "utility"

#include <string>

#include "gtest/gtest.h"

struct A {
    int f() & { return 1; }
    int f() && { return 2; }
};

template <class T>
int f(T&& t) {
    return ltl::forward<T>(t).f();
}

TEST(utility, forward) {
    A a;
    EXPECT_EQ(f(a), 1);
    EXPECT_EQ(f(A()), 2);
}

TEST(utility, move) {
    A a;
    EXPECT_EQ(ltl::move(a).f(), 2);
    EXPECT_EQ(ltl::move(A()).f(), 2);
}

TEST(utility, move_string) {
    std::string s("hello");
    std::string s2 = ltl::move(s);
    EXPECT_TRUE(s.empty());
    EXPECT_EQ(s2, "hello");
}

TEST(utility, swap) {
    int a = 0, b = 1;
    ltl::swap(a, b);
    EXPECT_EQ(a, 1);
    EXPECT_EQ(b, 0);
}
