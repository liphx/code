#include <iostream>
#include <string_view>
using namespace std;
using namespace std::literals::string_view_literals;

string_view get() { return "view"; }
string_view get(const string& str) { return str; }
// string_view get(string str) { return str; }

int main() {
    {
        auto s = get();
        cout << s << endl;
        cout << (void *)s.data() << " " << s.size() << endl;
    }

    {
        string str = "hello";
        auto s = get(str);
        cout << (void *)str.data() << endl;
        cout << s << endl;
        cout << (void *)s.data() << " " << s.size() << endl;
    }

    {
        string str{"hello"};
        string_view sv{&str[2], 2};
        cout << sv << endl;  // ll
    }

    {
        // string_view_literals: the length is computed at compile time
        string_view sv = "hello"sv;
        cout << sv << endl;  // hello
    }

    {
        string s = "hello";
        string_view sv = s;
        // sv[0] = 'H';
        // compile error: assignment of read-only location
    }
}
