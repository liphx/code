#include "liph/algorithm.h"

#include <iostream>

#include "gtest/gtest.h"

namespace liph {

TEST(algorithm, erase) {
    std::vector v{1, 2};
    std::set<size_t> idx{};
    erase(v, idx);
    EXPECT_EQ(v, (std::vector{1, 2}));
}

TEST(algorithm, erase_2) {
    std::vector<int> v;
    std::set<size_t> idx{0, 2};
    erase(v, idx);
    EXPECT_EQ(v, (std::vector<int>{}));
}

TEST(algorithm, erase_3) {
    std::vector v{0, 2, 4, 6, 8, 10, 12};
    std::set<size_t> idx{0, 1, 5};
    erase(v, idx);
    EXPECT_EQ(v, (std::vector{4, 6, 8, 12}));
}

TEST(algorithm, erase_4) {
    std::vector v{0, 2, 4};
    std::set<size_t> idx{0, 1, 5};
    erase(v, idx);
    EXPECT_EQ(v, (std::vector{4}));
}

TEST(algorithm, topk) {
    const std::vector<int> data{1, 3, 2, 3};
    auto ans = topk(data, 2);
    ASSERT_EQ(ans.size(), 2U);
    EXPECT_EQ(ans[0], 3);
    EXPECT_EQ(ans[1], 3);

    ans = topk(data, 0);
    EXPECT_TRUE(ans.empty());
}

TEST(algorithm, find_kth) {
    const std::vector<int> data{1, 3, 2, 3};
    EXPECT_EQ(find_kth(data, 1), 3);
    EXPECT_EQ(find_kth(data, 4), 1);
}

TEST(algorithm, binary_search) {
    int arr[]{-9, 0, 1, 7};
    EXPECT_EQ(binary_search(arr, 4, -9), 0);
    EXPECT_EQ(binary_search(arr, 4, 0), 1);
    EXPECT_EQ(binary_search(arr, 4, 1), 2);
    EXPECT_EQ(binary_search(arr, 4, 7), 3);
    EXPECT_EQ(binary_search(arr, 4, 8), -1);
    EXPECT_EQ(binary_search(arr, 1, -9), 0);
    EXPECT_EQ(binary_search(arr, 1, 0), -1);
    EXPECT_EQ(binary_search(arr, 3, 0), 1);
}

}  // namespace liph
