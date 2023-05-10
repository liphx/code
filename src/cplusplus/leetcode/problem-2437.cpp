// https://leetcode.cn/problems/number-of-valid-clock-times/

#include "leetcode.h"

class Solution {
public:
    int countTime(string time) {
        int ret = 1;
        if (time[0] == '?' && time[1] == '?')
            ret = 24;
        else if (time[0] == '?')
            ret = time[1] < '4' ? 3 : 2;
        else if (time[1] == '?')
            ret = time[0] < '2' ? 10 : 4;
        if (time[3] == '?') ret *= 6;
        if (time[4] == '?') ret *= 10;
        return ret;
    }
};

int main() {
    cout << Solution().countTime("?5:00") << endl;  // 2
    cout << Solution().countTime("0?:0?") << endl;  // 100
    cout << Solution().countTime("??:??") << endl;  // 1440
}
