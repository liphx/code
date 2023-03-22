#include "algorithm"

#include <algorithm>
#include <vector>

#include "gtest/gtest.h"

TEST(heap, is_heap) {
    int arr[] = {40, 15, 27, 27, -32, 40, -17, 54, 41, 36, 21};
    auto len = sizeof(arr) / sizeof(int);
    EXPECT_EQ(std::is_heap(arr, arr + len), ltl::is_heap(arr, arr + len));
    std::make_heap(arr, arr + sizeof(arr) / sizeof(int));
    EXPECT_EQ(std::is_heap(arr, arr + len), ltl::is_heap(arr, arr + len));
}

TEST(heap, make_heap) {
    std::vector<int> data{40, 15, 27, 27, -32, 40, -17, 54, 41, 36, 21};
    EXPECT_FALSE(std::is_heap(data.begin(), data.end()));
    ltl::make_heap(data.begin(), data.end());
    EXPECT_TRUE(std::is_heap(data.begin(), data.end()));
}

TEST(heap, sort_heap) {
    std::vector<int> data{40, 15, 27, 27, -32, 40, -17, 54, 41, 36, 21};
    std::make_heap(data.begin(), data.end());
    ltl::sort_heap(data.begin(), data.end());
    EXPECT_TRUE(std::is_sorted(data.begin(), data.end()));
}
