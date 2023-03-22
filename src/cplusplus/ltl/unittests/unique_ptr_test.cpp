#include <iostream>

#include "gtest/gtest.h"
#include "memory"

namespace unique_ptr_test {

int count;

class A {
public:
    int x;
    A(int x = 0) : x(x) {}
    ~A() { count += x; }
};

TEST(unique_ptr, common) {
    { ltl::unique_ptr<A> pa(new A(1)); }
    EXPECT_EQ(count, 1);

    {
        ltl::unique_ptr<A> pa(new A(1));
        pa.reset(new A(2));
        EXPECT_EQ(count, 2);
        pa = nullptr;
        EXPECT_EQ(count, 4);

        {
            ltl::unique_ptr<A> pa2(new A(3));
            // pa = pa2; // 不能复制 unique_ptr
            pa = ltl::move(pa2);
        }
        // pa2 离开作用域，但 A(3) 未析构
        EXPECT_EQ(count, 4);

        A *p;
        {
            ltl::unique_ptr<A> pa3 = move(pa);  // 现在 A(3) 所有权在 pa3
            p = pa3.release();  // 释放被管理对象的所有权，返回指向被管理对象的指针
            EXPECT_EQ(count, 4);
            EXPECT_EQ(pa3.get(), nullptr);  // pa3.get() 现在是 nullptr
        }                                   // pa3 离开作用域，但不再管理 A(3)
        EXPECT_EQ(p->x, 3);

        ltl::unique_ptr<A> pa4(p);  // 再次管理 A(3)
        pa4.swap(pa);               // pa4 与 pa(nullptr) 交换
        EXPECT_EQ(pa4.get(), nullptr);
        EXPECT_EQ(pa->x, 3);

        auto pa5 = ltl::make_unique<A>(4);  // 优先使用 make_unique 构造 unique_ptr
        EXPECT_EQ(pa5->x, 4);
    }
    EXPECT_EQ(count, 11);
}

}  // namespace unique_ptr_test
