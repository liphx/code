#include <iostream>
#include <map>
using namespace std;

struct Entry {
    string name;
    int value;
};

Entry read_entry(istream& is) {
    string s;
    int i;
    is >> s >> i;
    return {s, i};
}

int main() {
    auto [name, value] = read_entry(cin);
    cout << "{" << name << ", " << value << "}\n";

    map<string, int> data{{"abc", 1}, {"nn", 2}};
    for (const auto& [s, n] : data) {
        cout << s << ": " << n << endl;
    }

    class A {
        int x, y;
    };
    A a;
    // error: cannot decompose inaccessible member
    // auto [x, y] = a;
}
