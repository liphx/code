// https://leetcode.cn/problems/check-if-numbers-are-ascending-in-a-sentence/

#include "leetcode.h"

class Solution {
public:
    bool areNumbersAscending(string s) {
        int pre = 0, cur;
        string str;
        stringstream ss(s);
        while (ss >> str) {
            if (!str.empty() && isdigit(str[0])) {
                cur = stoi(str);
                if (cur <= pre) return false;
                pre = cur;
            }
        }
        return true;
    }
};

int main() {
    cout << boolalpha;
    cout << Solution().areNumbersAscending("1 box has 3 blue 4 red 6 green and 12 yellow marbles") << endl;  // true
    cout << Solution().areNumbersAscending("hello world 5 x 5") << endl;                                     // false
}
