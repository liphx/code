// 输入整数n，随后输入 n + 1 个整数表示矩阵的维度（矩阵A a×b ×B c×d 需要满足b = c）计算矩阵乘法最少次数
#include <algorithm>
#include <climits>
#include <iostream>
using namespace std;

const int N = 1000;
int n;                // n 个矩阵
int num[N + 1];       // 矩阵 i 的维度为 num[i] * num[i+i]
int m[N + 1][N + 1];  // m[i][j] 保存的是矩阵 i*...*j 最小次数

int solve() {
    cin >> n;
    for (int i = 0; i < n + 1; ++i) {
        cin >> num[i];
    }
    for (int i = 0; i < n; i++) m[i][i] = 0;
    for (int h = 2; h <= n; h++) {
        for (int i = 0; i < n - 1; i++) {
            int j = i + h - 1;
            if (i < n && j < n) {
                m[i][j] = INT_MAX;
                for (int k = i; k <= j; ++k) {
                    m[i][j] = min(m[i][j], m[i][k] + m[k + 1][j] + num[i] * num[k + 1] * num[j + 1]);
                }
            }
        }
    }
    return m[0][n - 1];
}

int main() {
    cout << solve() << endl;
    return 0;
}
