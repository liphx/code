#include <array>
#include <iostream>
using namespace std;

int main()
{
    // 创建100个int 型元素的array<>
    array<int, 100> data1;

    // 创建并初始化为0或者和默认元素类型等效的值
    array<double, 100> data2 {};

    // 初始化前3个数，其余的为0
    array<long, 10> data3 { 4, 5, 6 };

    // 将所有元素设成给定的值
    data1.fill(10);

    // 访问元素,at()会检查是否越界
    cout << data1[0] << endl;    // 10
    cout << data1.at(1) << endl; // 10

    // 计算元素的个数
    cout << data1.size() << endl; // 100

    // 判断是否为空
    cout << data1.empty() << endl; // 0
    array<char, 0> data4;
    cout << data4.empty() << endl; // 1

    // 基于范围的循环
    long total = 0;
    for (long x : data3)
        total += x;
    cout << total << endl; // 15

    // 返回第一个元素的引用
    data1.front()++;
    cout << data1.front() << endl; // 11

    // 返回最后一个元素的引用
    data1.back()--;
    cout << data1.back() << endl; // 9

    // data()返回指向数组的指针
    auto p = data1.data();

    // 在编译阶段检查是否越界的方法
    cout << get<99>(data1) << endl; // 9
    // cout << get<101>(data1) << endl; // error

    // 迭代器
    int sum = 0;
    auto first = data1.begin();
    auto last = data1.end();
    while (first != last)
        sum += *first++;
    cout << sum << endl; // 1000

    // 全局迭代器
    auto g_first = begin(data1);
    auto g_last = end(data1);

    // 反向迭代器
    auto rfirst = data1.rbegin();
    auto rlast = data1.rend();
    auto g_rfirst = rbegin(data1);
    auto g_rlast = rend(data1);

    // const迭代器
    auto cfirst = data1.cbegin();
    auto clast = data1.cend;

    // 比较,元素个数要相同
    array<int, 100> data5;
    data5.fill(1);
    cout << (data1 < data5) << endl; // 0

    // 赋值
    data1 = data5;
    cout << (data1 == data5) << endl; // 1

    return 0;
}
