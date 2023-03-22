#include <fstream>
#include <iostream>
#include <string>

#include "liph/string.h"

std::string deal_oneline(const std::string& s, int space_num = 4) {
    std::string str = liph::rtrim(s);
    std::string ret;
    ret.reserve(str.length());
    int i = 0;
    for (char c : str) {
        if (c != '\t') {
            ret += c;
            i++;
        } else {
            int num = space_num - i % space_num;
            ret += std::string(num, ' ');
            i += num;
        }
    }
    return ret;
}

void deal_file(const char *pathname, std::string nl = "\n") {
    std::ifstream file(pathname);
    if (!file.is_open()) {
        return;
    }
    std::string all;
    for (std::string str; std::getline(file, str);) {
        all += deal_oneline(str) + nl;
    }
    file.close();

    std::ofstream out(pathname, std::ios::trunc);
    out << all;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {  // stdin -> stdout
        for (std::string str; std::getline(std::cin, str);) {
            std::cout << deal_oneline(str) << std::endl;
        }
    } else {  // files
        for (int i = 1; i < argc; i++) {
            deal_file(argv[i]);
        }
    }
}
