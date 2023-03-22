#include "algorithm"

#include <forward_list>

#include "gtest/gtest.h"

TEST(algorithm, all_of) {
    std::vector<int> data{1, 2, 3};
    auto p = [](int x) { return x > 0; };
    EXPECT_TRUE(ltl::all_of(data.begin(), data.end(), p));
    EXPECT_TRUE(ltl::any_of(data.begin(), data.end(), p));
    EXPECT_FALSE(ltl::none_of(data.begin(), data.end(), p));
}

TEST(algorithm, for_each) {
    std::vector<int> data{1, 2, 3};
    int sum = 0;
    ltl::for_each(data.begin(), data.end(), [&sum](int x) { sum += x; });
    EXPECT_EQ(sum, 6);
}

TEST(algorithm, is_sorted) {
    // 向前迭代器
    std::forward_list<int> data{1, 3, 2};
    ASSERT_FALSE(ltl::is_sorted(data.begin(), data.end()));

    data.sort();
    ASSERT_TRUE(ltl::is_sorted(data.begin(), data.end()));
}

TEST(algorithm, is_sorted_until) {
    std::forward_list<int> data{1, 3, 2};
    EXPECT_EQ(ltl::is_sorted_until(data.begin(), data.end()), std::is_sorted_until(data.begin(), data.end()));

    std::forward_list<int> data2{1, 2, 3};
    EXPECT_EQ(ltl::is_sorted_until(data2.begin(), data2.end()), std::is_sorted_until(data2.begin(), data2.end()));

    std::forward_list<int> data3;
    EXPECT_EQ(ltl::is_sorted_until(data3.begin(), data3.end()), std::is_sorted_until(data3.begin(), data3.end()));
}

TEST(algorithm, binary_search) {
    int data[]{1, 3, 6, 6, 9, 12, 13, 15, 15, 17};
    auto iter = ltl::lower_bound(std::begin(data), std::end(data), 10);
    EXPECT_EQ(*iter, 12);

    iter = ltl::upper_bound(std::begin(data), std::end(data), 10);
    EXPECT_EQ(*iter, 12);

    EXPECT_FALSE(ltl::binary_search(std::begin(data), std::end(data), 10));

    auto pair = ltl::equal_range(std::begin(data), std::end(data), 15);
    EXPECT_EQ(ltl::distance(pair.first, pair.second), 2);
    for (auto iter = pair.first; iter != pair.second; ++iter) {
        EXPECT_EQ(*iter, 15);
    }
}
