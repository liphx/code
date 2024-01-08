#include <fstream>
#include <iostream>
#include <regex>

#include "liph/format.h"

int main(int argc, char **argv) {
    int ret = 1;
    if (argc != 3) {
        std::cerr << liph::format("Usage: {} [pattern] [file]", argv[0]) << std::endl;
        return 1;
    }
    std::regex re(argv[1]);
    std::ifstream stream(argv[2]);
    if (!stream.is_open()) {
        std::cerr << liph::format("Open file fail: {}", argv[2]) << std::endl;
        return 1;
    }
    std::string line;
    while (std::getline(stream, line)) {
        std::smatch match;
        if (std::regex_search(line, match, re)) {
            ret = 0;
            std::cout << line << std::endl;
        }
    }
    return ret;
}
