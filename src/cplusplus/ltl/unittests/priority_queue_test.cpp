#include "queue"

#include <functional>
#include <vector>

#include "gtest/gtest.h"

TEST(priority_queue, priority_queue) {
    // 创建优先级队列
    // 默认底层容器为vector
    // 接受第三个参数为函数对象，用来决定元素顺序
    ltl::priority_queue<int, std::vector<int>> q1;

    // 初始化，自动排序
    std::vector<int> data{2, 1, 5};
    ltl::priority_queue<int> q2(data.begin(), data.end());
    EXPECT_EQ(q2.top(), 5);

    // 基本操作
    q2.push(10);
    EXPECT_EQ(q2.size(), 4u);
    EXPECT_EQ(q2.top(), 10);

    q2.pop();
    EXPECT_EQ(q2.size(), 3u);
    EXPECT_EQ(q2.top(), 5);

    ASSERT_FALSE(q2.empty());
    q2.pop();
    q2.pop();
    q2.pop();
    ASSERT_TRUE(q2.empty());
}

TEST(priority_queue, greater) {
    std::vector<int> data{2, 1, 5};
    ltl::priority_queue<int, std::vector<int>, std::greater<int>> q(data.begin(), data.end());
    EXPECT_EQ(q.top(), 1);
}
