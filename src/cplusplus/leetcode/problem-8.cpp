// https://leetcode.cn/problems/string-to-integer-atoi/

#include "leetcode.h"

class Solution {
public:
    int myAtoi(string s) {
        if (s.empty()) return 0;
        int i = 0, n = s.length();
        while (s[i] == ' ') ++i;
        int f = 1;
        if (s[i] == '-') {
            ++i;
            f = -1;
        } else if (s[i] == '+') {
            ++i;
        }
        long long ret = 0;
        while (i < n && s[i] >= '0' && s[i] <= '9') {
            ret = ret * 10 + (s[i] - '0');
            if (ret > INT_MAX && f == 1) return INT_MAX;
            if (ret > INT_MAX && f == -1) return INT_MIN;
            ++i;
        }
        return f * ret;
    }
};

int main() {
    cout << Solution().myAtoi("") << endl;                 // 0
    cout << Solution().myAtoi("42") << endl;               // 42
    cout << Solution().myAtoi("   -42") << endl;           // -42
    cout << Solution().myAtoi("4193 with words") << endl;  // 4193
    cout << Solution().myAtoi("words and 987") << endl;    // 0
    cout << Solution().myAtoi("-91283472332") << endl;     // -2147483648
}
