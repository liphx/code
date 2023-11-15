#include "liph/liph.h"
using namespace liph;

int main() {
    print(demangle<int>());
    print(demangle<std::string>());
    print(__cplusplus);

    std::string str = "abc000abc--";
    std::cout << replace(str, "abc", "=") << std::endl;
    std::cout << replace(str, "abc", "===") << std::endl;
    std::cout << replace(str, "abc", "====") << std::endl;
}
