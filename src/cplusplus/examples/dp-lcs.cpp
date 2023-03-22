// 求两个字符串s1s2…sn和t1t2…tm最长的公共子序列的长度。
#include <iostream>
using namespace std;

const int MAX_N = 1000;
const int MAX_M = 1000;

int n, m;
string s, t;
int dp[MAX_N + 1][MAX_M + 1];

int main() {
    cin >> n >> m;
    cin >> s >> t;
    // O(n*m)
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            if (s[i] == t[j])
                dp[i + 1][j + 1] = dp[i][j] + 1;
            else
                dp[i + 1][j + 1] = max(dp[i][j + 1], dp[i + 1][j]);

    cout << "length of LCS is " << dp[n][m] << endl;
    return 0;
}
