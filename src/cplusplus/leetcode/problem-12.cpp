// https://leetcode.cn/problems/integer-to-roman/
// 给定一个整数，将其转为罗马数字。输入确保在 1 到 3999 的范围内。

#include "leetcode.h"

class Solution {
public:
    static const vector<pair<int, string>> maps;

    string intToRoman(int num) {
        string ret;
        int n = maps.size();
        while (n-- > 0) {
            while (num >= maps[n].first) {
                ret += maps[n].second;
                num -= maps[n].first;
            }
        }

        return ret;
    }
};

const vector<pair<int, string>> Solution::maps = {{1, "I"}, {4, "IV"}, {5, "V"}, {9, "IX"}, {10, "X"}, {40, "XL"},
        {50, "L"}, {90, "XC"}, {100, "C"}, {400, "CD"}, {500, "D"}, {900, "CM"}, {1000, "M"}};

int main() {
    cout << Solution().intToRoman(3) << endl;     // III
    cout << Solution().intToRoman(4) << endl;     // IV
    cout << Solution().intToRoman(9) << endl;     // IX
    cout << Solution().intToRoman(58) << endl;    // LVIII
    cout << Solution().intToRoman(1994) << endl;  // MCMXCIV
}
