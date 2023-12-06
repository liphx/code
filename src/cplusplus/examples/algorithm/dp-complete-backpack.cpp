
// 求从n种重量和价值分别为wi和vi的物品中挑选出总重量不超过W的物品的挑选方案中的最大价值，其中每种物品可以挑选任意多件。

#include <cstring>
#include <iostream>
#define MAX_N 100
#define MAX_W 10000
using namespace std;

int n, W;
int w[MAX_N], v[MAX_N];

int dp[MAX_N + 1][MAX_W + 1];

void dp1() {
    // O(n * W * W)
    for (int i = 0; i < n; i++)
        for (int j = 0; j <= W; j++)
            for (int k = 0; k * w[i] <= j; k++) dp[i + 1][j] = max(dp[i + 1][j], dp[i][j - k * w[i]] + k * v[i]);
}

void dp2() {
    // O(n * W)
    for (int i = 0; i < n; i++)
        for (int j = 0; j <= W; j++)
            if (j < w[i])
                dp[i + 1][j] = dp[i][j];
            else
                dp[i + 1][j] = max(dp[i][j], dp[i + 1][j - w[i]] + v[i]);
}

int main() {
    cin >> n >> W;
    for (int i = 0; i < n; i++) cin >> w[i] >> v[i];

    dp1();
    cout << "max is " << dp[n][W] << endl;

    memset(dp, 0, sizeof(dp));
    dp2();
    cout << "max is " << dp[n][W] << endl;
    return 0;
}
