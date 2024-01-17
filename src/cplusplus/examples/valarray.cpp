#include <iostream>
#include <valarray>
using namespace std;

int main() {
    valarray<double> v{1.0, 3.14, -5.2};
    v *= 2;
    for (auto x : v) cout << x << ' ';
    cout << '\n';
    // 2 6.28 -10.4

    v = abs(v);
    for (auto x : v) cout << x << ' ';
    cout << '\n';
    // 2 6.28 10.4
}
