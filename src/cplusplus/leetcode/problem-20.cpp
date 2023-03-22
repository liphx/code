// https://leetcode.cn/problems/valid-parentheses/
// 给定一个只包括 '('，')'，'{'，'}'，'['，']' 的字符串，判断字符串是否有效。

#include "leetcode.h"

class Solution {
public:
    bool isValid(string s) {
        stack<char> st;
        for (char ch : s) {
            switch (ch) {
            case '(':
            case '[':
            case '{':
                st.push(ch);
                break;
            case ')':
                if (st.empty() || st.top() != '(') return false;
                st.pop();
                break;
            case ']':
                if (st.empty() || st.top() != '[') return false;
                st.pop();
                break;
            case '}':
                if (st.empty() || st.top() != '{') return false;
                st.pop();
                break;
            }
        }
        return st.empty();
    }
};

int main() {
    cout << Solution().isValid("") << endl;        // 1
    cout << Solution().isValid("()") << endl;      // 1
    cout << Solution().isValid("()[]{}") << endl;  // 1
    cout << Solution().isValid("(]") << endl;      // 0
    cout << Solution().isValid("([)]") << endl;    // 0
    cout << Solution().isValid("{[]}") << endl;    // 1
}
