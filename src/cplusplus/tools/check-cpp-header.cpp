#include <algorithm>

#include "liph/liph.h"
using namespace liph;

bool check_one_file(const std::string& pathname) {
    std::string str;
    if (!read_file(pathname, str)) return false;

    std::string guard = pathname;
    std::transform(guard.begin(), guard.end(), guard.begin(), [](char ch) -> char {
        if (ch == '/' || ch == '.') return '_';
        return std::toupper(ch);
    });
    guard += '_';
    // liph/algorithm.h -> LIPH_ALGORITHM_H_

    std::vector<std::string> ss;
    split(ss, str, "\n");
    if (ss.size() < 3) return false;
    if (ss[0] != "#ifndef " + guard) return false;
    if (ss[1] != "#define " + guard) return false;
    if (ss.back() != "#endif  // " + guard) return false;
    return true;
}

int main(int argc, char **argv) {
    err_exit_if(argc < 2);
    auto entries = list_files(argv[1]);
    int count = 0;
    for (const auto& entry : entries) {
        if (endswith(entry, ".h")) {
            /* print("checking", entry); */
            /* print(entry, check_one_file(entry) ? "correct." : "incorrect!!"); */
            if (!check_one_file(entry)) {
                print(entry, "incorrect!!");
                err_exit();
            }
            ++count;
        }
    }
    print("Check cpp header:", count, "PASSED");
}
