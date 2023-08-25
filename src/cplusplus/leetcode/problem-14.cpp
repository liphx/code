// https://leetcode.cn/problems/longest-common-prefix/
// 编写一个函数来查找字符串数组中的最长公共前缀。如果不存在公共前缀，返回空字符串 ""。

#include "leetcode.h"

class Solution {
public:
    string longestCommonPrefix(vector<string>& strs) {
        string ret;
        size_t i = 0;
        char c = 0;
        while (true) {
            for (size_t j = 0; j < strs.size(); j++) {
                if (i >= strs[j].size()) {
                    return ret;
                }
                if (j == 0) {
                    c = strs[0][i];
                } else if (c != strs[j][i]) {
                    return ret;
                }
            }
            ret += c;
            i++;
        }
    }
};

int main() {
    vector<string> strs1{"flower", "flow", "flight"}, strs2{"dog", "racecar", "car"};
    cout << Solution().longestCommonPrefix(strs1) << endl;  // fl
    cout << Solution().longestCommonPrefix(strs2) << endl;  // ""
}
