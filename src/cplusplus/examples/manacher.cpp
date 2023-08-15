#include <cassert>
#include <iostream>
#include <string>
using namespace std;

string manacher(const string& s) {
    string ret;

    // 预处理，假设原字符串中无'#' '$'
    assert(s.find('#') == string::npos);
    assert(s.find('$') == string::npos);
    string str = "$#";  // 优化
    for (int i = 0; i < s.length(); ++i) str = str + s[i] + '#';

    // 计算回文半径数组p
    // 以s[i]为中心（无论奇偶）的回文子串在原串中的长度为p[i] - 1
    // 算法的核心为计算p数组
    int n = str.length();
    int p[n];
    // 引入辅助变量 MaxRight 表示当前已确定的回文子串能触及的最右端字符位置
    // pos 为其对称中心的位置
    int MaxRight = 0, pos = 0;
    int m = -1, mi = 0;  // 记录最长子串的长度和位置
    for (int i = 1; i < n; ++i) {
        if (MaxRight > i)
            p[i] = min(p[2 * pos - i], MaxRight - i + 1);  // 核心算法
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
    for (int i = mi - (m - 1); i <= mi + (m - 1); i += 2) ret += str[i];

    return ret;
}

int main() {
    cout << manacher("babad") << endl;  //=>bab
    cout << manacher("cbbd") << endl;   //=>bb

    return 0;
}
