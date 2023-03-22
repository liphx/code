#include "array"

#include "gtest/gtest.h"

TEST(array, array) {
    // 创建100个int 型元素的array<>
    ltl::array<int, 100> data1;

    // 创建并初始化为0或者和默认元素类型等效的值
    ltl::array<double, 100> data2{};
    EXPECT_EQ(data2[0], 0);

    // 初始化前3个数，其余的为0
    ltl::array<long, 10> data3{4, 5, 6};
    EXPECT_EQ(data3[3], 0);

    // 将所有元素设成给定的值
    data1.fill(10);

    // 访问元素, at()会检查是否越界, 若越界, 抛出异常 std::out_of_range
    EXPECT_EQ(data1[0], 10);
    EXPECT_EQ(data1.at(1), 10);

    // 计算元素的个数
    EXPECT_EQ(data1.size(), 100u);

    // 判断是否为空
    EXPECT_FALSE(data1.empty());
    ltl::array<char, 0> data4;  // 长度为0 的array
    EXPECT_TRUE(data4.empty());

    // 基于范围的循环
    long total = 0;
    for (long x : data3) total += x;
    EXPECT_EQ(total, 15);

    // 返回第一个元素的引用
    data1.front()++;
    EXPECT_EQ(data1.front(), 11);

    // 返回最后一个元素的引用
    data1.back()--;
    EXPECT_EQ(data1.back(), 9);

    // 在零长 array 上调用 front() 或 back() 是未定义的

    // data()返回指向数组的指针
    auto p = data1.data();
    EXPECT_EQ(*p, 11);

    // 在编译阶段检查是否越界的方法
    EXPECT_EQ(get<99>(data1), 9);
    // get<101>(data1);  // error

    // 迭代器
    int sum = 0;
    auto first = data1.begin();
    auto last = data1.end();
    while (first != last) sum += *first++;
    EXPECT_EQ(sum, 1000);

    {
        // 全局迭代器
        auto it1 = std::begin(data1);
        auto it2 = data1.begin();
        while (it1 != std::end(data1) && it2 != data1.end()) {
            EXPECT_EQ(*it1++, *it2++);
        }
    }

    {
        // 反向迭代器
        auto it1 = std::rbegin(data1);
        auto it2 = data1.rbegin();
        while (it1 != std::rend(data1) && it2 != data1.rend()) {
            EXPECT_EQ(*it1++, *it2++);
        }
    }

    // 比较,元素个数要相同
    ltl::array<int, 100> data5;
    data5.fill(1);

    // 赋值
    data1 = data5;
    EXPECT_TRUE(data1 == data5);
}
