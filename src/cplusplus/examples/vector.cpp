#include "liph/liph.h"
using namespace std;
using namespace liph;

int main() {
    // 创建vector
    vector<int> data1;
    cout << data1.size() << endl;      // 大小 0
    cout << data1.capacity() << endl;  // 容量 0

    // 增加容器的容量，若容量不小于参数则无效
    data1.reserve(20);
    cout << data1.size() << endl;      // 0
    cout << data1.capacity() << endl;  // 20

    // 初始化列表创建vector
    vector<int> data2{1, 2, 3, 4, 5};

    // 初始化元素的个数，默认值为0
    vector<int> data3(10);
    cout << data3.size() << endl;      // 10
    cout << data3.capacity() << endl;  // 10
    print(data3);                      // 0 0 0 0 0 0 0 0 0 0

    // 指定其他默认值
    vector<int> data4(5, -1);
    print(data4);  // -1 -1 -1 -1 -1

    // 改变大小
    vector<int> data5{1, 2};
    data5.resize(3);
    print(data5);  // 1 2 0
    data5.resize(5, -1);
    print(data5);  // 1 2 0 -1 -1
    data5.resize(4);
    print(data5);  // 1 2 0 -1

    // 访问元素,但不能生成新元素
    cout << data5[3] << endl;     // -1
    cout << data5.at(0) << endl;  // 1

    // 返回第一个元素的引用
    data5.front()++;
    cout << data5[0] << endl;  // 2

    // 返回最后一个元素的引用
    data5.back()--;
    cout << *(data5.end() - 1) << endl;  // -2

    // data()返回指向数组的指针
    auto p = data5.data();

    // 在末尾增加元素
    data5.push_back(100);
    cout << data5.size() << endl;  // 5
    cout << data5.back() << endl;  // 100

    // 使用emplace_back添加元素
    vector<string> data6;
    string str = "hello";
    data6.emplace_back(str, 0, 3);  // 调用string的构造函数生成对象
    cout << data6[0] << endl;       // hel

    // emplace插入一个元素
    auto iter = data6.begin();
    data6.emplace(iter, str, 1, 3);
    print(data6);  // ell hel

    // insert插入一个或多个元素
    data6.insert(data6.begin(), "00");
    data6.insert(data6.end(), {"11", "22"});
    print(data6);  // 00 ell hel 11 22

    // 删除尾部元素
    data6.pop_back();

    // 去掉容器中多余的容量
    data6.shrink_to_fit();

    // 删除一个或多个元素
    data6.erase(data6.begin());

    // 删除所有元素
    data6.clear();
    cout << data6.size() << endl;  // 0

    // 迭代器
    // 同array，插入或删除可能破坏迭代器，需要重新获取

    return 0;
}
