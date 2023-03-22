// https://leetcode.cn/problems/letter-combinations-of-a-phone-number/
// 给定一个仅包含数字 2-9 的字符串，返回所有它能表示的字母组合。
// 给出数字到字母的映射如下（与电话按键相同）。注意 1 不对应任何字母。

#include "leetcode.h"

class Solution {
public:
    static map<char, string> phone;

    vector<string> letterCombinations(string digits) {
        vector<string> ans;
        int len = digits.length();
        if (len == 0) return ans;
        if (len == 1) {
            for (char i : phone[digits[0]]) {
                ans.emplace_back(string("") + i);
            }
            return ans;
        }

        // 递归
        string str = phone[digits[0]];
        vector<string> vec = letterCombinations(string(digits, 1));
        for (char i : str) {
            for (string j : vec) {
                ans.emplace_back(i + j);
            }
        }

        return ans;
    }
};

map<char, string> Solution::phone = {{'1', ""}, {'2', "abc"}, {'3', "def"}, {'4', "ghi"}, {'5', "jkl"}, {'6', "mno"},
        {'7', "pqrs"}, {'8', "tuv"}, {'9', "wxyz"}};

int main() {
    auto ans = Solution().letterCombinations("23");
    cout << ans << endl;  // [ad, ae, af, bd, be, bf, cd, ce, cf]
}
