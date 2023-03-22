// https://leetcode.cn/problems/first-letter-to-appear-twice/

#include "leetcode.h"

class Solution {
public:
    char repeatedCharacter(string s) {
        int counts[26]{0};
        for (char c : s) {
            if (counts[c - 'a'] == 1) return c;
            ++counts[c - 'a'];
        }
        return 'a';
    }
};

int main() {
    cout << Solution().repeatedCharacter("abccbaacz") << endl;  // c
}
