#include <forward_list>
#include <iterator>
#include <iostream>
using namespace std;

int main()
{
    forward_list<int> data { 1, 2, 3 };
    cout << distance(data.begin(), data.end()) << endl; // 没有size()函数来获取大小
    data.push_front(0);                                 // 没有push_back()
    data.insert_after(data.begin(), 0);
    data.pop_front();
    cout << data.front() << endl;
    // sort, merge, remove, remove_if, unique 同list

    return 0;
}
