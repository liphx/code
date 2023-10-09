#include "liph/liph.h"
using namespace liph;

int main() {
    for (int i = 0; i < 256; ++i) {
        std::cout << color256bg(i) << i << color_reset() << std::endl;
    }
}
