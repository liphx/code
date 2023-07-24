#include <algorithm>
#include <fstream>
#include <iostream>

#include "liph/file.h"

int main(int argc, char **argv) {
    if (argc < 2) return 1;
    std::string str = argv[1];
    if (liph::path_exists(str)) return 1;
    std::ofstream out(str);
    if (!out.is_open()) return 1;

    std::transform(str.begin(), str.end(), str.begin(), [](char ch) -> char {
        if (ch == '/' || ch == '.') return '_';
        return std::toupper(ch);
    });
    str += '_';
    // liph/algorithm.h -> LIPH_ALGORITHM_H_

    out << "#ifndef " << str << std::endl;
    out << "#define " << str << std::endl;
    out << std::endl;
    out << "namespace liph {" << std::endl;
    out << std::endl;
    out << "}  // namespace liph" << std::endl;
    out << std::endl;
    out << "#endif  // " << str << std::endl;
    out.close();
}
