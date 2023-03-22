#include <iostream>
#include <list>
#include <stack>
using namespace std;

int main() {
    // 创建stack，默认使用deque作为底层容器
    stack<int> data1;

    // 使用 list<T> 的堆栈
    stack<int, list<int> > data2;

    // 用另一个容器来初始化stack
    list<int> data3{0, 1, 2};
    stack<int, list<int> > data4(data3);

    // 拷贝构造函数
    stack<int> data5{data1};

    // 基本操作
    data5.push(0);
    data5.push(1);
    cout << data5.top() << endl;    // 1
    cout << data5.size() << endl;   // 2
    cout << data5.empty() << endl;  // 0
    data5.pop();
    cout << data5.top() << endl;  // 0

    return 0;
}
