// https://leetcode.cn/problems/longest-palindromic-substring/
// 给定一个字符串 s，找到 s 中最长的回文子串。
// 1 <= s.length <= 1000
// s 仅由数字和英文字母组成

#include "leetcode.h"

class Solution {
public:
    // 时间复杂度O(n^2)
    string longestPalindrome(string s) {
        string ret;
        int len = 0, i = 0, n = s.length();
        while (i < n) {
            // 奇数长度
            int j = 1;
            while (i - j >= 0 && i + j < n && s[i - j] == s[i + j]) {
                j++;
            }
            if (j * 2 - 1 > len) {
                len = j * 2 - 1;
                ret = s.substr(i - j + 1, len);
            }

            // 偶数长度
            if (i + 1 < n && s[i] == s[i + 1]) {
                int j = 1;
                while (i - j >= 0 && i + j + 1 < n && s[i - j] == s[i + j + 1]) {
                    j++;
                }
                if (j * 2 > len) {
                    len = j * 2;
                    ret = s.substr(i - j + 1, len);
                }
            }

            i++;
        }
        return ret;
    }

    // Manacher算法，时间空间复杂度均为O(n)
    string longestPalindrome2(string s) {
        // 预处理，假设原字符串中无'#' '$'
        assert(s.find('#') == string::npos);
        assert(s.find('$') == string::npos);
        string str = "$#";  // 优化
        for (int i = 0; i < s.length(); ++i) str = str + s[i] + '#';
        // babad => $#b#a#b#a#d#

        // 计算回文半径数组p
        // 以s[i]为中心（无论奇偶）的回文子串在原串中的长度为p[i] - 1
        // 算法的核心为计算p数组
        int n = str.length();
        int p[n];
        // 引入辅助变量 MaxRight 表示当前已确定的回文子串能触及的最右端字符位置
        // pos 为其对称中心的位置
        int MaxRight = 0, pos = 0;
        // 记录最长子串的长度和位置
        int m = -1, mi = 0;
        for (int i = 1; i < n; ++i) {
            if (MaxRight > i)
                p[i] = min(p[2 * pos - i], MaxRight - i + 1);  // 核心代码
            else
                p[i] = 1;
            while (str[i + p[i]] == str[i - p[i]]) p[i]++;  // 开头加了'$'字符，不用考虑边界
            if (p[i] + i - 1 > MaxRight) {
                MaxRight = p[i] + i - 1;
                pos = i;
            }
            if (p[i] > (m + 1)) {
                m = p[i] - 1;
                mi = i;
            }
        }
        // 无论m奇偶都适用
        string ret;
        for (int i = mi - (m - 1); i <= mi + (m - 1); i += 2) ret += str[i];
        return ret;
    }
};

int main() {
    cout << Solution().longestPalindrome("babad") << endl;   // bab
    cout << Solution().longestPalindrome("cbbd") << endl;    // bb
    cout << Solution().longestPalindrome2("babad") << endl;  // bab
    cout << Solution().longestPalindrome2("cbbd") << endl;   // bb
}
