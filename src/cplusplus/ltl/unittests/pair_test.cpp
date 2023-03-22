#include "utility"

#include "gtest/gtest.h"

TEST(utility, pair) {
    int n = 1;
    int a[5] = {1, 2, 3, 4, 5};

    auto p1 = ltl::make_pair(n, a[1]);
    EXPECT_EQ(p1.first, 1);
    EXPECT_EQ(p1.second, 2);

    // 从到 int 的引用和数组（退化成指针）建造 pair
    auto p2 = ltl::make_pair(std::ref(n), a);
    n = 7;
    EXPECT_EQ(p2.first, 7);
    EXPECT_EQ(*(p2.second + 2), 3);
}
