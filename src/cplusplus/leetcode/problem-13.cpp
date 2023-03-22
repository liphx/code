// https://leetcode.cn/problems/roman-to-integer/
// 给定一个罗马数字，将其转换成整数。输入确保在 1 到 3999 的范围内。

#include "leetcode.h"

class Solution {
public:
    static map<char, int> maps;

    int romanToInt(string s) {
        int ret = 0, n = s.length();
        int pre = 0, now;
        for (int i = 0; i < n; ++i) {
            now = maps[s[i]];
            if (pre >= now || pre == 0) {
                ret += pre;
                pre = now;
            } else {
                ret += (now - pre);
                pre = 0;
            }
        }
        return ret + pre;
    }
};

map<char, int> Solution::maps = {{'I', 1}, {'V', 5}, {'X', 10}, {'L', 50}, {'C', 100}, {'D', 500}, {'M', 1000}};

int main() {
    cout << Solution().romanToInt("III") << endl;      // 3
    cout << Solution().romanToInt("IV") << endl;       // 4
    cout << Solution().romanToInt("IX") << endl;       // 9
    cout << Solution().romanToInt("LVIII") << endl;    // 58
    cout << Solution().romanToInt("MCMXCIV") << endl;  // 1994
}
