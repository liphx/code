#include <iostream>
#include <iterator>
#include <vector>
using namespace std;

/*
 * pattern.length() > 0
 * pattern.length() == next.size()
 */
void get_next(const string& pattern, vector<int>& next) {
    int n = pattern.length();
    next[0] = -1;
    int i = 0, j = -1;
    while (i < n) {
        while (j >= 0 && pattern[i] != pattern[j]) j = next[j];
        i++;
        j++;
        if (pattern[i] == pattern[j])
            next[i] = next[j];
        else
            next[i] = j;
    }
}

/*
 * 在 str 中查找 pattern
 * 辅助数组 next 表示失配后与模式串的哪一位比较才可能导致匹配
 * pattern.length() == next.size()
 * 返回 pattern 在 str 中出现的位置
 */
vector<int> kmp(const string& str, const string& pattern, const vector<int>& next) {
    vector<int> ans;
    int n = str.length();
    int m = pattern.length();
    int i = 0, j = 0;
    while (i < n) {
        while (j >= 0 && str[i] != pattern[j]) j = next[j];
        i++;
        j++;
        if (j == m) {
            ans.emplace_back(i - m);
            j = next[m];
        }
    }
    return ans;
}
int main() {
    string str = "aabcd";
    string pattern = "abab";
    vector<int> next(pattern.length());
    get_next(pattern, next);
    copy(next.begin(), next.end(), ostream_iterator<int>(cout, " "));

    auto ans = kmp(str, pattern, next);
    copy(ans.begin(), ans.end(), ostream_iterator<int>(cout, " "));

    return 0;
}
