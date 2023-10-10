#include "liph/liph.h"
using namespace liph;

int main() {
    stdin_echo_off();
    stdin_buffering_off();
    char ch;
    while (std::cin.get(ch)) {
        if (ch == 4) {
            break;
        }
        std::cout << static_cast<int>(ch) << std::endl;
    }
    std::cout << color_reset();
}
