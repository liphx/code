# 动态规划

## 1. 背包问题

1.1 01 背包问题

有 N 个物品和一个承重量为 W 的背包，物品 i 的重量和价值分别是 w<sub>i</sub> 和 v<sub>i</sub>，对于物品 i，可以选择装或不装入背包，求不超过背包承重量且获得最大价值的挑选方案。

```c
int w[MAX_N];   // 每个物品的重量
int v[MAX_N];   // 每个物品的价值
int dp[MAX_W];

int solve(int N, int W)
{
    memset(dp, 0, sizeof(dp));
    for (int i = 0; i < N; i++) {
        for (int j = W; j >= w[i]; j--) {
            dp[j] = max(dp[j], dp[j - w[i]] + v[i]);
        }
    }
    return dp[W];
}
```

1.2 完全背包问题

问题同上，但每个物品选择的个数没有限制

```c

```
