// https://leetcode.cn/problems/set-matrix-zeroes/

#include "leetcode.h"

class Solution {
public:
    void setZeroes(vector<vector<int>>& matrix) {
        int m = matrix.size();
        if (m == 0) return;
        int n = matrix[0].size();
        // 空间O(m + n), 记录哪些行和列需要置0
        // 空间O(1)
        int flag = INT_MIN + 1;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (matrix[i][j] == 0) {
                    // 同行同列置特殊值, bug:输入中含有特殊值
                    for (int k1 = 0; k1 < n; k1++) {
                        matrix[i][k1] = (matrix[i][k1] != 0) ? flag : 0;  // 如果是0，不修改
                    }
                    for (int k2 = 0; k2 < m; k2++) {
                        matrix[k2][j] = (matrix[k2][j] != 0) ? flag : 0;
                    }
                }
            }
        }
        // 特殊值置0
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (matrix[i][j] == flag) matrix[i][j] = 0;
            }
        }
    }
};

int main() {
    vector<vector<int>> matrix = {{1, 1, 1}, {1, 0, 1}, {1, 1, 1}};
    Solution().setZeroes(matrix);
    cout << matrix << endl;  // [[1, 0, 1], [0, 0, 0], [1, 0, 1]]
}
