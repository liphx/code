// https://leetcode.cn/problems/generate-parentheses/
// 数字n代表生成括号的对数，请你设计一个函数，用于能够生成所有可能的并且有效的括号组合。

#include "leetcode.h"

class Solution {
public:
    vector<string> generateParenthesis(int n) {
        set<string> ret;
        if (n < 1) return vector<string>{ret.begin(), ret.end()};

        if (n == 1) {
            ret.insert("()");
            return vector<string>{ret.begin(), ret.end()};
        }

        auto ans = generateParenthesis(n - 1);
        for (const string& s : ans) {
            for (int i = 0; i < s.length(); i++) {
                if (s[i] == '(') {
                    ret.insert(string(s, 0, i + 1) + "()" + string(s, i + 1));
                }
                ret.insert(s + "()");
            }
        }
        return vector<string>{ret.begin(), ret.end()};
    }
};

int main() {
    cout << Solution().generateParenthesis(3) << endl;
    // [((())), (()()), (())(), ()(()), ()()()]
}
