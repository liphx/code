// 递归生成全排列

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

vector<string> ans;

void premutations(string str, int k, int m);

vector<string> solve(string str) {
    ans.clear();
    if (str == "") {
        return ans;
    }
    premutations(str, 0, str.size() - 1);
    return ans;
}

void premutations(string str, int k, int m) {
    if (k == m)
        ans.push_back(str);
    else {
        for (int i = k; i <= m; ++i) {
            swap(str[k], str[i]);
            premutations(str, k + 1, m);
            swap(str[k], str[i]);
        }
    }
}

// 另外，借助stl算法next_premutation也可以帮我们建立字符串的全排列
// 如果字符串有重复的字符，而且要求结果不重复，那么直接调用next_premutation就可以得到不重复的结果

vector<string> stlfun(string str) {
    vector<string> ret;
    if (str == "") {
        return ret;
    }
    sort(str.begin(), str.end());
    ret.push_back(str);
    while (next_permutation(str.begin(), str.end())) {
        ret.push_back(str);
    }
    return ret;
}

int main() {
    string s("abcd");
    vector<string> str = solve(s);
    for (string x : str) {
        cout << x << endl;
    }
    return 0;
}
