// https://leetcode.cn/problems/divide-two-integers/

#include "leetcode.h"

class Solution {
public:
    // n < 0, return x * 2 ^ n
    int exp(int x, int n) {
        unsigned int ret = x;
        for (int i = 0; i < n; i++) {
            ret <<= 1;
            if ((int)ret >= 0) {
                return INT_MIN;
            }
        }
        return ret;
    }

    int divide(int dividend, int divisor) {
        int sign = (dividend ^ divisor) < 0 ? -1 : 1;
        if (dividend > 0) dividend = -dividend;
        if (divisor > 0) divisor = -divisor;

        int ans = 0;
        int half = INT_MIN >> 1;
        if (dividend <= divisor && divisor < half) {
            return sign;
        }
        int tmp = 0;
        while (dividend <= divisor) {
            if (exp(divisor, tmp + 1) <= dividend) {
                ans -= 1 << tmp;
                dividend -= exp(divisor, tmp);
                tmp = 0;
            } else {
                tmp++;
            }
        }

        if (ans == INT_MIN && sign == 1) return INT_MAX;
        return sign == -1 ? ans : -ans;
    }
};

int main() {
    cout << Solution().divide(-7, 2) << endl;            // -3
    cout << Solution().divide(7, 2) << endl;             // 3
    cout << Solution().divide(79, 1) << endl;            // 79
    cout << Solution().divide(-2147483648, -1) << endl;  // 2147483647
    cout << Solution().divide(2147483647, 3) << endl;    // 715827882
}
