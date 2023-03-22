#include <algorithm>
#include <iostream>
using namespace std;

/*
    n个数字（n >= 3），选3个数字（不可重复）的长度组成三角形，求三角形最大周长，若不能组成三角形则输出0
*/
const int N = 100;
int n, A[N];

inline bool isTriangle(int a, int b, int c) { return (a + b > c) && (a + c > b) && (b + c > a); }

// O(nlogn)
void solve() {
    sort(A, A + n, greater<int>());
    for (int i = 0; i <= n - 3; i++) {
        if (isTriangle(A[i], A[i + 1], A[i + 2])) {
            cout << A[i] + A[i + 1] + A[i + 2] << endl;
            return;
        }
    }
    cout << 0 << endl;
}

int main() {
    cin >> n;
    for (int i = 0; i < n; i++) {
        cin >> A[i];
    }
    solve();
    return 0;
}
