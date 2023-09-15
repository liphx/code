#include "liph/liph.h"

int main() {
    PRINT(liph::mkdir("a"));
    PRINT(liph::mkdir("b/c"));
    PRINT(liph::mkdirp("d"));
    PRINT(liph::mkdirp("e/f"));
}
