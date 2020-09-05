#include <queue>
#include <list>
#include <iostream>
using namespace std;

int main()
{
    queue<int> data1;
    queue<int> data2 { data1 };
    queue<int, list<int> > data3;

    data1.push(0);
    data1.push(1);
    data1.push(2);
    cout << data1.front() << endl; // 0
    cout << data1.back() << endl;  // 2
    cout << data1.size() << endl;  // 3
    cout << data1.empty() << endl; // 0
    data1.pop();
    cout << data1.front() << endl; // 1
    cout << data1.back() << endl;  // 2

    return 0;
}
