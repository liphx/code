#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

/*
 * n 个物品 v[i], 质量为 v[i][0], 价值为 v[i][1]
 * 背包最大质量为 W
 * 每个物品可以挑选或者不挑选，求背包承载物品的最大价值
 * v.size() > 0; v[i].size() = 2; v[i][j] > 0; W > 0
 */
int solve(int W, const std::vector<std::vector<int>>& v) {
    int n = v.size();
    int dp[n + 1][W + 1];
    memset(dp, 0, sizeof(int) * (n + 1) * (W + 1));
    for (int i = n - 1; i >= 0; i--) {
        for (int j = 0; j <= W; j++) {
            if (j < v[i][0])
                dp[i][j] = dp[i + 1][j];
            else
                dp[i][j] = std::max(dp[i + 1][j], dp[i + 1][j - v[i][0]] + v[i][1]);
        }
    }
    return dp[0][W];
}

int main() {
    int W = 5;
    std::vector<std::vector<int>> v{{2, 3}, {1, 2}, {3, 4}, {2, 2}};
    std::cout << solve(W, v) << std::endl;  // 7
}
