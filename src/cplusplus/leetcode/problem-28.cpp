// https://leetcode.cn/problems/find-the-index-of-the-first-occurrence-in-a-string/

#include "leetcode.h"

class Solution {
public:
    int strStr(string haystack, string needle) { return haystack.find(needle); }
};

int main() {
    cout << Solution().strStr("hello", "ll") << endl;  // 2
}
