/* 有n个无区别的物品，将他们划分成不超过m组，有多少种分法；例如：n = 4，m = 3，有四种划分方式{ {1,1,2}, {2,2}, {4}, */
/*  * {1,3}} */
/* 定义dp[i][j]：划分j的i种方法 */
/* dp递推式：dp[i][j] = dp[i][j-i] + dp[i-1][j] */
/* 例如，6的3划分等价于6的2划分加上把6划分成3组的分法，而把6划分成3组等价于(6-3)的3划分 */

#include <iostream>
using namespace std;

const int N = 10000;
int n, m;
int dp[N][N];

void solve() {
    dp[0][0] = 1;
    for (int i = 1; i <= m; i++) {
        for (int j = 0; j <= n; j++) {
            if (j - i >= 0) {
                dp[i][j] = dp[i - 1][j] + dp[i][j - i];
            } else {
                dp[i][j] = dp[i - 1][j];
            }
        }
    }
    cout << dp[m][n] << endl;
}
int main() {
    cin >> n >> m;
    solve();
    return 0;
}
