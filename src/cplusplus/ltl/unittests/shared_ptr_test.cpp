#include <memory>

#include "gtest/gtest.h"
#define ltl std

namespace shared_ptr_test {

int X = -1;

class A {
public:
    int x;
    A(int x = 0) : x(x) {}
    ~A() { X = x; }
};

int func(ltl::shared_ptr<A> pa) { return pa.use_count(); }

int func_ref(ltl::shared_ptr<A>& pa) { return pa.use_count(); }

TEST(shared_ptr, shared_ptr) {
    { ltl::shared_ptr<A> pa(new A); }
    EXPECT_EQ(X, 0);

    ltl::shared_ptr<A> pa(new A(1));
    EXPECT_EQ(pa.use_count(), 1);
    {
        ltl::shared_ptr<A> pa2(pa);
        EXPECT_EQ(pa2.use_count(), 2);
    }
    EXPECT_EQ(pa.use_count(), 1);
    EXPECT_EQ(func(pa), 2);      //  by value
    EXPECT_EQ(func_ref(pa), 1);  //  by ref

    ltl::shared_ptr<A> pa2(pa);
    pa2 = nullptr;
    EXPECT_EQ(pa.use_count(), 1);
    pa.reset(new A(2));
    EXPECT_EQ(X, 1);  // ~A(1)
    pa = nullptr;
    EXPECT_EQ(X, 2);  // ~A(2)

    {
        auto pa3 = ltl::make_shared<A>(3);
        EXPECT_EQ(pa3->x, 3);
    }
    EXPECT_EQ(X, 3);
}

}  // namespace shared_ptr_test
