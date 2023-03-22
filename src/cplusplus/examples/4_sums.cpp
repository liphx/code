#include <algorithm>
#include <iostream>
using namespace std;

/*
    n个数字选4个（可重复），判断是否有和为m的可能 O(n^2logn)
*/
const int N = 1000;
int n, A[N], B[N * N], m;

// O(n^2logn)

void solve() {
    // n^2
    int k = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            B[k++] = A[i] + A[j];
        }
    }
    // n^2logn
    sort(B, B + n * n);
    // n^2logn
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int t = m - A[i] - A[j];
            if (binary_search(B, B + n * n, t)) {
                cout << "yes" << endl;
                return;
            }
        }
    }
    cout << "no" << endl;
}

int main() {
    cin >> n;
    for (int i = 0; i < n; i++) {
        cin >> A[i];
    }
    cin >> m;
    solve();
    return 0;
}
