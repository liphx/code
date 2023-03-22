#include <iostream>
#include <random>
#include <string>
#include <vector>
using namespace std;

class Randomizer {
public:
    Randomizer(unsigned n) : n_(n), d_(0, n_) {}

    unsigned operator()() { return d_(e); }

private:
    int n_;
    uniform_int_distribution<unsigned> d_;

    static random_device r;
    static default_random_engine e;
};

random_device Randomizer::r;
default_random_engine Randomizer::e(Randomizer::r());

class Passwd {
public:
    static string Gen(const string& str, int len) {
        if (str.empty() || len <= 0) return "";
        Randomizer r(str.length() - 1);
        string ret;
        for (int i = 0; i < len; i++) {
            ret += str[r()];
        }
        return ret;
    }

    static string GenNumber(int len) { return Gen(chars[0], len); }

    static string GenNumberAlpha(int len) {
        static string str = GetNumberAlphaChars();
        return Gen(str, len);
    }

    static string GenAll(int len) {
        static string str = GetAllchars();
        return Gen(str, len);
    }

private:
    static const vector<string> chars;

    static string GetNumberAlphaChars() { return chars[0] + chars[1] + chars[2]; }
    static string GetAllchars() { return GetNumberAlphaChars() + chars[3]; }
};

const vector<string> Passwd::chars = {
        "0123456789", "abcdefghijklmnopqrstuvwxyz", "ABCDEFGHIJKLMNOPQRSTUVWXYZ", "!@#$%^&*()-_+=[]{};'\":,.?/<>|`~"};

int main(int argc, char **argv) {
    int n = 12;
    if (argc > 1) n = atoi(argv[1]);
    cout << Passwd::GenNumber(n) << endl;
    cout << Passwd::GenNumberAlpha(n) << endl;
    cout << Passwd::GenAll(n) << endl;
}
