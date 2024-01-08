#include <iostream>
#include <regex>
#include <string>
using namespace std;

int main() {
    {
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
    }

    {
        string str = "aaab";
        string s = "(a*)b";
        smatch m;
        regex re(s);
        if (regex_match(str, m, re)) {
            printf("'%s' match regex '%s'.\n", str.c_str(), s.c_str());
        }
        cout << m.empty() << endl;  // 0
        cout << m.size() << endl;   // 2
        cout << m.str() << endl;    // aaab
        cout << m[0] << endl;       // aaab
        cout << m[1] << endl;       // aaa
    }

    {
        string str = "98120";
        regex re(R"([0-9]+)");
        smatch m;
        cout << regex_match(str, m, re) << endl;
    }

    {
        string str = "aaaa";
        regex re(R"(a+?)");  // non-greedy
        smatch m;
        regex_search(str, m, re);
        cout << m.str() << endl;  // a
    }
}
