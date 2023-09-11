#include "liph/liph.h"

int main(int argc, char **argv) {
    std::vector<std::string> out;
    std::string str;
    if (argc == 1) {  // stdin
        while (std::getline(std::cin, str)) {
            out.emplace_back(str);
        }
    } else {
        for (int i = 1; i < argc; ++i) {
            std::ifstream in(argv[i]);
            while (std::getline(in, str)) {
                out.emplace_back(str);
            }
        }
    }
    std::reverse(out.begin(), out.end());
    std::for_each(out.begin(), out.end(), [](const std::string& str) { std::cout << str << std::endl; });
}
