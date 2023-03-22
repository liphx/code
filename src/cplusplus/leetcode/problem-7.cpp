// https://leetcode.cn/problems/reverse-integer/
// 给出一个 32 位的有符号整数，你需要将这个整数中每位上的数字进行反转。
// 如果反转后整数超过 32 位的有符号整数的范围，就返回 0。

#include "leetcode.h"

class Solution {
public:
    int reverse(int x) {
        int f = x >= 0 ? 1 : -1;
        long long t = x;
        t = abs(t);
        long long ret = t % 10;
        t /= 10;
        while (t > 0) {
            ret *= 10;
            ret += (t % 10);
            t /= 10;
        }
        ret *= f;
        return (ret > INT_MAX || ret < INT_MIN) ? 0 : ret;
    }
};

int main() {
    cout << Solution().reverse(123) << endl;   // 321
    cout << Solution().reverse(-123) << endl;  // -321
    cout << Solution().reverse(120) << endl;   // 21
    cout << Solution().reverse(1) << endl;     // 1
}
