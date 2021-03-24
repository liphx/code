#include "allocator.hpp"
#include <gtest/gtest.h>
#include <vector>
#include <iostream>

TEST(allocator, constructor)
{
    liph::allocator<int> alloc;
    liph::allocator<int> alloc2(alloc);
    liph::allocator<long> alloc3(alloc);
    EXPECT_TRUE(alloc2 == alloc3);

    liph::allocator<int> alloc4;
    alloc4 = alloc;
    alloc4 = alloc3;
}

TEST(allocator, allocate)
{
    liph::allocator<int> alloc;
    int *p = alloc.allocate(1);
    *p = 100;
    alloc.deallocate(p, 1);
}

TEST(allocator, stl)
{
    std::vector<int, liph::allocator<int>> data;
    for (int i = 0; i < 5; i++) {
        data.emplace_back(i);
    }
    for (int i = 0; i < 5; i++) {
        EXPECT_EQ(data[i], i);
    }
}
