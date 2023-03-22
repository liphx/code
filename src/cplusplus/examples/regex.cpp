/*
# 正则表达式库

`#include <regex> // since c++11`

## class

### `std::basic_regex`

特化

| 类型 | 定义 |
| -- | -- |
| `regex`   | `basic_regex<char>`       |
| `wregex`  | `basic_regex<wchar_t>`    |

构造函数

- `regex()`  默认构造函数。构造将不匹配内容的空正则表达式
- `regex(const char* s, flag_type f)` 从空终止字符串 s 构造正则表达式
- `regex(const string& str, flag_type f)` 从 string str 构造正则表达式


`flag_type` f, 用于指引转译字符序列为正则表达式的标志, 默认为 `regex_constants::ECMAScript`

### `std::match_results`

特化

| 类型 | 定义 |
| -- | -- |
| `cmatch`  | `match_results<const char*>`                  |
| `smatch`  | `match_results<std::string::const_iterator>`  |

## 算法

### `std::regex_match`

确定正则表达式 e 是否匹配整个目标字符序列

- bool regex\_match(BidirIt first, BidirIt last, match\_results& m, const regex& e, regex\_constants::match\_flag\_type
flags = regex\_constants::match\_default);
- bool regex\_match(const char* str, m, e, flags);
- bool regex_match(const string& str, m, e, flags);
- bool regex_match(first, last, e, flags);
- bool regex_match(s, e, flags);
- bool regex_match(str, e, flags);

### `std::regex_search`

确定正则表达式 e 和目标字符序列中的某个子序列间是否有匹配

- bool regex_search(first, last, m, e, flags);
- bool regex_search(const char* str, m, e, flags);
- bool regex\_search(const string& str, m, e, flags);
- bool regex\_search(first, last, e, flags);
- bool regex\_search(s, e, flags);
- bool regex\_search(str, e, flags);

### `std::regex_replace`

regex\_replace 用正则表达式进行字符序列的替换

- OutputIt regex\_replace(OutputIt out, BidirIt first, BidirIt last, const regex& re, const string& fmt, flags);
- OutputIt regex\_replace(out, first, last, re, const char *fmt, flags);
- string regex_replace(const string& str, re, fmt, flags);
- string regex_replace(const char* s, re, fmt, flags);

## 迭代器

### `std::regex_iterator`

特化

| 类型 | 定义 |
| -- | -- |
| `cregex_iterator` | `regex_iterator<const char*>` |
| `sregex_iterator` | `regex_iterator<std::string::const_iterator>` |

*/

#include <iostream>
#include <regex>
#include <string>
using namespace std;

int main() {
    string str("hello, world.");
    string s("\\w+");
    regex re(s);
    smatch m;
    if (regex_search(str, m, re)) {
        printf("'%s' contains regex '%s'.\n", str.c_str(), s.c_str());
    }
    cout << m.empty() << endl;  // 0
    cout << m.size() << endl;   // 1
    cout << m.str() << endl;    // hello

    auto iter = sregex_iterator(str.begin(), str.end(), re);
    for (; iter != sregex_iterator(); iter++) {
        string match = iter->str();
        cout << "regex matchs: " << match << endl;
    }

    str = "aaab";
    s = "(a*)b";
    m = smatch();
    re = regex(s);
    if (regex_match(str, m, re)) {
        printf("'%s' match regex '%s'.\n", str.c_str(), s.c_str());
    }
    cout << m.empty() << endl;  // 0
    cout << m.size() << endl;   // 2
    cout << m.str() << endl;    // aaab
    cout << m[0] << endl;       // aaab
    cout << m[1] << endl;       // aaa
}
