#include <iostream>
#include <string>
using namespace std;

string deal_oneline(string str, int space_num = 4)
{
    string ret;
    int len = str.length();
    int index = 0;
    for (char c: str) {
        if (c != '\t') {
            ret += c;
            index += 1;
        } else {
            int num = space_num - index % space_num;
            ret += string(num, ' ');
            index += num;
        }
    }
    return ret;
}

int main(int argc, char *argv[])
{
    string str;
    while (getline(cin, str)) {
        cout << deal_oneline(str) << endl;
    }
}
