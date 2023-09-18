#include "liph/liph.h"
#include "thirdparty/tinyxml2-9.0.0/tinyxml2.h"

int main() {
    PRINT(liph::mkdir("a"));
    PRINT(liph::mkdir("b/c"));
    PRINT(liph::mkdirp("d"));
    PRINT(liph::mkdirp("e/f"));

    tinyxml2::XMLDocument doc;
}
