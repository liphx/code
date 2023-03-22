#include "memory"

#include <vector>

#include "gtest/gtest.h"

TEST(allocator, constructor) {
    ltl::allocator<int> alloc;
    ltl::allocator<int> alloc2(alloc);
    ltl::allocator<long> alloc3(alloc);
    EXPECT_TRUE(alloc2 == alloc3);

    ltl::allocator<int> alloc4;
    alloc4 = alloc;
    alloc4 = alloc3;
    EXPECT_TRUE(alloc3 == alloc4);
}

TEST(allocator, allocate) {
    ltl::allocator<int> alloc;
    int *p = alloc.allocate(1);
    *p = 100;
    alloc.deallocate(p, 1);
}

TEST(allocator, stl) {
    std::vector<int, ltl::allocator<int>> data;
    for (int i = 0; i < 5; i++) {
        data.emplace_back(i);
    }
    for (int i = 0; i < 5; i++) {
        EXPECT_EQ(data[i], i);
    }
}
