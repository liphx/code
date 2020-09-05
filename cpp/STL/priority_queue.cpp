#include <queue>
#include <iostream>
using namespace std;

int main()
{
    // 创建优先级队列
    // 默认底层容器为vector
    // 接受第三个参数为函数对象，用来决定元素顺序
    priority_queue<int, vector<int> > data1;

    // 初始化，自动排序
    vector<int> data2 { 2, 1, 5 };
    priority_queue<int> data3 { data2.begin(), data2.end() };
    cout << data3.top() << endl;

    // 基本操作
    data3.push(10);
    cout << data3.size() << endl;
    cout << data3.top() << endl;
    data3.pop();
    cout << data3.empty() << endl;

    return 0;
}
