#include <algorithm>
#include <fstream>
#include <iostream>

#include "gflags/gflags.h"
#include "liph/liph.h"

DEFINE_string(ns, "liph", "");

int main(int argc, char **argv) {
    google::ParseCommandLineFlags(&argc, &argv, true);
    liph::err_exit_if(argc < 2);

    std::string str = argv[1];
    liph::err_exit_if(liph::path_exists(str));

    std::ofstream out(str);
    liph::err_exit_if(!out.is_open());

    std::transform(str.begin(), str.end(), str.begin(), [](char ch) -> char {
        if (ch == '/' || ch == '.') return '_';
        return std::toupper(ch);
    });
    str += '_';
    // liph/algorithm.h -> LIPH_ALGORITHM_H_

    out << "#ifndef " << str << std::endl;
    out << "#define " << str << std::endl;
    out << std::endl;
    out << "namespace " + FLAGS_ns + " {" << std::endl;
    out << std::endl;
    out << "}  // namespace " + FLAGS_ns << std::endl;
    out << std::endl;
    out << "#endif  // " << str << std::endl;
    out.close();
}
