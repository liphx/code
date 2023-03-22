// https://leetcode.cn/problems/longest-substring-without-repeating-characters/
// 给定一个字符串，请你找出其中不含有重复字符的最长子串的长度。

#include "leetcode.h"

class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        int i = 0, j = 0, ret = 0, n = s.size();
        int arr[256] = {0};
        while (i < n) {
            if (j < n && arr[s[j]] == 0) {
                arr[s[j++]] = 1;
            } else {
                while (s[i] != s[j]) {
                    arr[s[i++]] = 0;
                }
                j++;
                i++;
            }
            ret = max(ret, j - i);
        }
        return ret;
    }
};

int main() {
    string str("abcabcbb");
    int ans = Solution().lengthOfLongestSubstring(str);
    cout << ans << endl;  // 3
}
