#include <tuple>
#include <iostream>
using namespace std;

int main()
{
    auto t1 = make_tuple(string { "123" }, int { 10 }, bool { false });
    tuple<string, string> t2 { "123", "321" };
    auto t3 { t2 };
    cout << (t3 == t2) << endl; // 1
    get<0>(t3) = "124";
    cout << (t3 > t2) << endl;    // 1
    cout << get<int>(t1) << endl; // 10，要求t1中只有一个int

    return 0;
}
