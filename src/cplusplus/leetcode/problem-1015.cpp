// https://leetcode.cn/problems/smallest-integer-divisible-by-k/

#include "leetcode.h"

class Solution {
public:
    int smallestRepunitDivByK(int k) {
        int ret = 1, n = 1;
        set<int> set_n;
        while (n % k != 0) {
            if (set_n.count(n) > 0) return -1;
            set_n.insert(n);
            n = (n % k) * 10 + 1;
            ++ret;
        }
        return ret;
    }
};

int main() {
    cout << Solution().smallestRepunitDivByK(1) << endl;   // 1
    cout << Solution().smallestRepunitDivByK(2) << endl;   // -1
    cout << Solution().smallestRepunitDivByK(3) << endl;   // 3
    cout << Solution().smallestRepunitDivByK(17) << endl;  // 16
}
