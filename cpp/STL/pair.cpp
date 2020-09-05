#include <utility>
#include <iostream>
using namespace std;

int main()
{
    pair<int, bool> p1 { 1, 0 };
    pair<int, bool> p2 { int { 2 }, bool { false } };
    pair<int, bool> p3 = make_pair(int { 3 }, bool { 0 });
    pair<int, bool> p4 { p3 };
    cout << (p4 == p3) << endl;                   // 1
    cout << (p1 < p2) << endl;                    // 1
    cout << p3.first << " " << p3.second << endl; // 3 0

    return 0;
}
