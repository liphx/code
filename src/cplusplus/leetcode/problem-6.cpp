// https://leetcode.cn/problems/zigzag-conversion/
// 将一个给定字符串根据给定的行数，以从上往下、从左到右进行 Z 字形排列。
// 比如输入字符串为 "LEETCODEISHIRING" 行数为 3 时，排列如下：
// L   C   I   R
// E T O E S I I G
// E   D   H   N
// 之后，你的输出需要从左往右逐行读取，产生出一个新的字符串，比如："LCIRETOESIIGEDHN"。

#include "leetcode.h"

class Solution {
public:
    string convert(string s, int numRows) {
        if (s.empty() || numRows == 1) return s;
        int n = s.length();
        int ar[n];
        int t = 1, d = 1;
        for (int i = 0; i < n; ++i) {
            ar[i] = t;
            t += d;
            if (t == numRows) d = -1;
            if (t == 1) d = 1;
        }
        string ret;
        for (int i = 1; i <= numRows; ++i) {
            for (int j = 0; j < n; ++j) {
                if (ar[j] == i) ret += s[j];
            }
        }
        return ret;
    }
};

int main() {
    cout << Solution().convert("LEETCODEISHIRING", 3) << endl;  // LCIRETOESIIGEDHN
    cout << Solution().convert("LEETCODEISHIRING", 4) << endl;  // LDREOEIIECIHNTSG
}
