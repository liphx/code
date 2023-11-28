// 求从n个重量和价值分别为wi和vi的物品中挑选出总重量不超过W的物品的挑选方案中的最大价值。
#include <cstring>
#include <iostream>
using namespace std;

const int MAX_N = 100;
const int MAX_W = 10000;
int n, W;
int w[MAX_N], v[MAX_N];
int dp[MAX_N + 1][MAX_W + 1];

// 对于背包中的每样物品都尝试选或不选 O(2^n)
int rec(int i, int j) {
    int res;
    if (i == n)
        res = 0;
    else if (j < w[i])
        res = rec(i + 1, j);
    else
        res = max(rec(i + 1, j), rec(i + 1, j - w[i]) + v[i]);
    return res;
}

// 利用记忆化数组 O(n*W)
int rec2(int i, int j) {
    if (dp[i][j] >= 0) return dp[i][j];
    int res;
    if (i == n)
        res = 0;
    else if (j < w[i])
        res = rec2(i + 1, j);
    else
        res = max(rec2(i + 1, j), rec2(i + 1, j - w[i]) + v[i]);
    return dp[i][j] = res;
}

// 通过递推式直接将dp数组计算出来
void rec3() {
    for (int i = n - 1; i >= 0; i--)
        for (int j = 0; j <= W; j++)
            if (j < w[i])
                dp[i][j] = dp[i + 1][j];
            else
                dp[i][j] = max(dp[i + 1][j], dp[i + 1][j - w[i]] + v[i]);
}

int main() {
    cin >> n >> W;
    for (int i = 0; i < n; i++) cin >> w[i] >> v[i];

    cout << "max is " << rec(0, W) << endl;

    memset(dp, -1, sizeof(dp));
    cout << "max is " << rec2(0, W) << endl;

    memset(dp, 0, sizeof(dp));  // 如果改变了全局变量dp的值，则需要重新设置
    rec3();
    cout << "max is " << dp[0][W] << endl;

    return 0;
}
