// https://leetcode.cn/problems/wildcard-matching/

#include "leetcode.h"

class Solution {
public:
    bool isMatch(string s, string p) {
        auto end = unique(p.begin(), p.end(), [](char a, char b) { return a == b && a == '*'; });
        p.erase(end, p.end());
        if (s.empty() && p.empty()) return true;
        if (p.empty()) return false;
        if (s.empty()) {
            return all_of(p.begin(), p.end(), [](char c) { return c == '*'; });
        }
        vector<string> list;
        split(list, p, "*");
        if (p[0] != '*' && !startswith(s, list.front())) return false;
        string_view sv = s;
        string tmp;
        for (const string& str : list) {
            size_t pos = find(sv, str);
            if (pos == -1) return false;
            tmp = string(sv.substr(pos, str.length()));
            sv = sv.substr(pos + str.length());
        }
        if (sv.empty()) return true;
        if (!p.empty() && p.back() == '*') return true;
        if (!list.empty() && endswith(tmp + string(sv), list.back()) && endswith(p, "*" + list.back())) return true;
        return false;
    }

private:
    bool startswith(const string& s, const string& t) {
        return s.size() >= t.size() &&
               equal(t.cbegin(), t.cend(), s.cbegin(), [](char a, char b) { return a == b || a == '?'; });
    }

    bool endswith(const string& s, const string& t) {
        return s.size() >= t.size() &&
               equal(t.crbegin(), t.crend(), s.crbegin(), [](char a, char b) { return a == b || a == '?'; });
    }

    size_t find(string_view s, string_view p) {
        if (none_of(p.begin(), p.end(), [](char c) { return c == '?'; })) {
            return s.find(p);
        }
        for (int i = 0; i < s.length(); i++) {
            bool found = true;
            for (int j = 0; j < p.length(); j++) {
                if (i + j >= s.length() || (p[j] != '?' && p[j] != s[i + j])) {
                    found = false;
                    break;
                }
            }
            if (found) {
                return i;
            }
        }
        return -1;
    }

    void split(vector<string>& tokens, const string& s, const string& delimiters = " ") {
        auto last_pos = s.find_first_not_of(delimiters, 0);
        auto pos = s.find_first_of(delimiters, last_pos);
        while (pos != string::npos || last_pos != string::npos) {
            tokens.emplace_back(s.substr(last_pos, pos - last_pos));
            last_pos = s.find_first_not_of(delimiters, pos);
            pos = s.find_first_of(delimiters, last_pos);
        }
    }
};

int main() {
    cout << boolalpha;
    cout << Solution().isMatch("aa", "a") << endl;                                                            // false
    cout << Solution().isMatch("aa", "*") << endl;                                                            // true
    cout << Solution().isMatch("cb", "?a") << endl;                                                           // false
    cout << Solution().isMatch("mississippi", "m??*ss*?i*pi") << endl;                                        // false
    cout << Solution().isMatch("aaabbbaabaaaaababaabaaabbabbbbbbbbaabababbabbbaaaaba", "a*******b") << endl;  // false
    cout << Solution().isMatch("aaaa", "***a") << endl;                                                       // true
    cout << Solution().isMatch("hi", "*?") << endl;                                                           // true
    cout << Solution().isMatch("aaab", "b**") << endl;                                                        // false
    cout << Solution().isMatch("abb", "**??") << endl;                                                        // true
    cout << Solution().isMatch("abaab", "*?a?") << endl;                                                      // true
    cout << Solution().isMatch(
                    "abbabaaabbabbaababbabbbbbabbbabbbabaaaaababababbbabababaabbababaabbbbbbaaaabababbbaabbbbaabbbbabab"
                    "ababbaabbaababaabbbababababbbbaaabbbbbabaaaabbababbbbaababaabbababbbbbababbbabaaaaaaaabbbbbaabaaab"
                    "abaaaabb",
                    "**aa*****ba*a*bb**aa*ab****a*aaaaaa***a*aaaa**bbabb*b*b**aaaaaaaaa*a********ba*bbb***a*ba*bb*bb**"
                    "a*b*bb")
         << endl;  // false
}
