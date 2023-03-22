#include "list"

#include "gtest/gtest.h"

TEST(list, constructor) {
    ltl::list<int> data1;
    EXPECT_TRUE(data1.empty());

    ltl::list<int> data2(10);
    EXPECT_EQ(data2.size(), 10u);

    ltl::list<int> data3(10, 1);
    ltl::list<int> data4{1, 2, 3, 4, 5};
    ltl::list<int> data5{data4};
}

TEST(list, iterator) {
    int arr[]{1, 2, 3, 4, 5};
    ltl::list<int> data{std::begin(arr), std::end(arr)};
    int i = 0;
    for (auto it = data.begin(); it != data.end(); ++it) {
        EXPECT_EQ(*it, arr[i++]);
    }
}

TEST(list, element) {
    ltl::list<int> data{1, 2, 3, 4, 5};
    EXPECT_EQ(data.size(), 5u);
    EXPECT_EQ(*++data.begin(), 2);
    EXPECT_EQ(*--data.end(), 5);
    data.push_front(0);
    EXPECT_EQ(data.front(), 0);
    data.push_back(6);
    EXPECT_EQ(data.back(), 6);
    data.emplace_front(-1);
    EXPECT_EQ(data.front(), -1);
    data.emplace_back(7);
    EXPECT_EQ(data.back(), 7);
    data.insert(data.end(), 8);
    EXPECT_EQ(data.back(), 8);
    data.insert(data.begin(), {-3, -2});
    EXPECT_EQ(data.front(), -3);
    // -3 -2 -1 0 1 2 3 4 5 6 7 8

    data.clear();
    EXPECT_TRUE(data.empty());

    ltl::list<int> data2{0, 0, 0, 5, 1, 2, 5, 5, 0};
    auto it = data2.erase(--data2.end());
    EXPECT_EQ(it, data2.end());
    EXPECT_EQ(data2.size(), 8u);
    EXPECT_EQ(data2.back(), 5);

    auto n = data2.remove(0);
    EXPECT_EQ(n, 3u);
    EXPECT_EQ(data2.size(), 5u);
    EXPECT_EQ(*data2.begin(), 5);
    data2.remove_if([](int n) { return n % 2 == 0; });
    EXPECT_EQ(data2.size(), 4u);
    data2.unique();
    EXPECT_EQ(data2.size(), 3u);
}

TEST(list, operations) {
    ltl::list<int> l{1, 1, 2, 5, 4, 4, 3};
    l.sort(std::greater<int>());
    EXPECT_EQ(l.back(), 1);
    l.sort();
    EXPECT_EQ(l.back(), 5);
    ltl::list<int> l2{0, 6};
    l.merge(l2);  // 0 1 1 2 3 4 4 5 6
    EXPECT_EQ(l.size(), 9u);
    EXPECT_EQ(l2.size(), 0u);
    EXPECT_EQ(l.front(), 0);
    l.unique();  // 0 1 2 3 4 5 6
    EXPECT_EQ(l.size(), 7u);
    l.reverse();  // 6 5 4 3 2 1 0
    EXPECT_EQ(l.front(), 6);
    l.remove_if([](int x) { return x > 3; });  // 3 2 1 0
    EXPECT_EQ(l.size(), 4u);
    EXPECT_EQ(l.front(), 3);
    l.remove(0);  // 3 2 1
    EXPECT_EQ(l.back(), 1);
}
