#include "liph/algorithm.h"

#include <iostream>

#include "gtest/gtest.h"

TEST(algorithm, topk) {
    const std::vector<int> data{1, 3, 2, 3};
    auto ans = liph::topk(data, 2);
    ASSERT_EQ(ans.size(), 2U);
    EXPECT_EQ(ans[0], 3);
    EXPECT_EQ(ans[1], 3);

    ans = liph::topk(data, 0);
    EXPECT_TRUE(ans.empty());
}

TEST(algorithm, find_kth) {
    const std::vector<int> data{1, 3, 2, 3};
    EXPECT_EQ(liph::find_kth(data, 1), 3);
    EXPECT_EQ(liph::find_kth(data, 4), 1);
}
