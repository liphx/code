#include <deque>
#include <iostream>
using namespace std;

int main()
{
    deque<int> data1;
    deque<int> data2(10);
    deque<int> data3 { 1, 2, 3, 4, 5 };
    deque<int> data4 { data3 };

    cout << data4.size() << endl;  // 5
    cout << data4[0] << endl;      // 1
    cout << data4.at(1) << endl;   // 2
    cout << data4.front() << endl; // 1
    cout << data4.back() << endl;  // 5

    data4.push_front(0);
    data4.push_back(6);
    cout << data4.front() << " " << data4.back() << endl; // 0 6
    data4.pop_back();
    data4.pop_front();
    cout << data4.front() << " " << data4.back() << endl; // 1 5

    data4.assign({ 10, 11, 12 });
    for (auto i : data4)
        cout << i << " ";
    cout << data4.size() << endl; // 10 11 12 3

    return 0;
}
