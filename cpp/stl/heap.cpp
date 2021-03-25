#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

void print(vector<int> &data)
{
    for (auto i : data)
        cout << i << " ";
    cout << endl;
}

int main()
{
    vector<int> data { 14, 5, 13, 26, 4, 18, 47, 26 };

    // 创建大顶堆
    make_heap(begin(data), end(data));
    print(data); // 47 26 18 26 4 14 13 5

    // 创建小顶堆
    make_heap(begin(data), end(data), greater<>());
    print(data); // 4 5 13 26 26 14 18 47

    // 添加
    data.push_back(50);
    push_heap(begin(data), end(data), greater<>());
    print(data); // 4 5 13 26 26 14 18 47 50

    // 删除
    pop_heap(begin(data), end(data), greater<>());
    data.pop_back();
    print(data); // 5 26 13 26 50 14 18 47

    // 判断是否依旧为堆
    cout << is_heap(begin(data), end(data)) << endl;              // 0
    cout << is_heap(begin(data), end(data), greater<>()) << endl; // 1

    // 排序
    if (is_heap(begin(data), end(data), greater<>()))
        sort_heap(begin(data), end(data), greater<>());
    print(data); // 50 47 26 26 18 14 13 5

    return 0;
}
