#include "liph/liph.h"
using namespace liph;

int main() {
    print(demangle<int>());
    print(demangle<std::string>());
    print(__cplusplus);
}
