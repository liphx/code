#define NDEBUG
#include "liph/lang/casts.h"

#include <cmath>

#include "gtest/gtest.h"

namespace liph {

namespace {
struct A {
    virtual ~A() = default;
};
struct B : A {};
struct C {
    virtual ~C() = default;
};
};  // namespace

TEST(casts, implicit_cast) {
    B b;
    static_cast<A *>(&b);
    implicit_cast<A *>(&b);

    void *p = 0;
    static_cast<A *>(p);
    // compile error
    // implicit_cast<A *>(p);

    A a;
    static_cast<B *>(&a);
    // no down-cast
    // implicit_cast<B *>(&a);
}

TEST(casts, down_cast) {
    B b;
    A *a = implicit_cast<A *>(&b);

    down_cast<B *>(a);
    down_cast<B&>(*a);

    void *p = 0;
    // compile error
    // down_cast<B *>(p);

    C c;
    // compile error
    // down_cast<B *>(&c);

    // nullptr
    EXPECT_TRUE(dynamic_cast<B *>(&c) == nullptr);

    A a2;
    EXPECT_TRUE(dynamic_cast<B *>(&a2) == nullptr);

    // abort in debug mode
    // down_cast<B *>(&a2);
    EXPECT_EQ(down_cast<B *>(&a2), static_cast<B *>(&a2));
}

TEST(casts, bit_cast) {
    uint32_t a = -1;
    EXPECT_TRUE(std::isnan(bit_cast<float>(a)));
}

}  // namespace liph
