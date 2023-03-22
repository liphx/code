#include "deque"

#include "gtest/gtest.h"

TEST(deque, deque) {
    ltl::deque<int> data1;
    ltl::deque<int> data2(10);
    ltl::deque<int> data3{1, 2, 3, 4, 5};
    ltl::deque<int> data4{data3};

    EXPECT_EQ(data4.size(), 5u);
    EXPECT_EQ(data4[0], 1);
    EXPECT_EQ(data4.at(1), 2);
    EXPECT_EQ(data4.front(), 1);
    EXPECT_EQ(data4.back(), 5);

    data4.push_front(0);
    data4.push_back(6);
    EXPECT_EQ(data4.front(), 0);
    EXPECT_EQ(data4.back(), 6);
    data4.pop_back();
    data4.pop_front();
    EXPECT_EQ(data4.front(), 1);
    EXPECT_EQ(data4.back(), 5);

    data4.assign({10, 11, 12});
    EXPECT_EQ(data4.size(), 3u);
}
