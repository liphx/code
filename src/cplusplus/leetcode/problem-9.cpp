// https://leetcode.cn/problems/palindrome-number/

#include "leetcode.h"

class Solution {
public:
    bool isPalindrome(int x) {
        string s = to_string(x);
        int i = 0, j = s.length() - 1;
        while (i < j) {
            if (s[i++] != s[j--]) return false;
        }
        return true;
    }
};

int main() {
    cout << Solution().isPalindrome(121) << endl;   // 1
    cout << Solution().isPalindrome(-121) << endl;  // 0
    cout << Solution().isPalindrome(10) << endl;    // 0
}
