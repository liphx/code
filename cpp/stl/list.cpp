#include <list>
#include <iostream>
using namespace std;

int main()
{
    // 创建list
    list<int> data1;
    list<int> data2(10);
    list<int> data3(10, 1);
    list<int> data4 { 1, 2, 3, 4, 5 };
    list<int> data5 { data4 };

    // 访问list，list的迭代器为双向迭代器
    cout << data5.size() << endl;     // 5
    cout << *++data5.begin() << endl; // 2
    cout << *--data5.end() << endl;   // 5

    // 添加元素
    data5.push_front(0);
    data5.push_back(6);
    data5.emplace_front(-1);
    data5.emplace_back(7);
    data5.insert(data5.end(), 8);
    data5.insert(data5.begin(), { -3, -2 });
    auto first = data5.begin();
    auto last = data5.end();
    while (first != last)
        cout << *first++ << " ";
    cout << endl; // -3 -2 -1 0 1 2 3 4 5 6 7 8

    // 删除元素
    data5.clear();
    cout << data5.size() << endl; // 0
    list<int> data6 { 0, 0, 0, 5, 1, 2, 5, 5, 0 };
    data6.erase(--data6.end());
    cout << *--data6.end() << endl;                    // 5
    data6.remove(0);                                   // 移除所有0
    cout << data6.size() << endl;                      // 5
    cout << *data6.begin() << endl;                    // 5
    data6.remove_if([](int n) { return n % 2 == 0; }); // 移除偶数
    cout << data6.size() << endl;                      // 4
    data6.unique();                                    // 移除连续重复的数
    cout << data6.size() << endl;                      // 3

    // 排序
    list<int> data7 { 2, 2, 1, 0, 5, 3 };
    data7.sort();                   // 从小到大
    cout << *data7.begin() << endl; // 0
    data7.sort([](int a, int b) { return a > b ? a : b; });
    cout << *data7.begin() << endl; // 5

    // 合并
    list<int> data8 { 1, 4, 57, -8, 63 };
    list<int> data9 { 87, 0, -8, 49 };
    data8.sort();
    data9.sort();
    data8.merge(data9);
    cout << data8.size() << " " << data9.size() << endl; // 9 0
    list<int> data10(5, 1);
    data8.splice(data8.begin(), data10, data10.begin(), data10.end());
    cout << data8.size() << " " << data10.size() << endl; // 14 0

    return 0;
}
