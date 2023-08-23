#include "liph/liph.h"

int main() {
    auto i = liph::big_integer::rand();
    std::cout << i.string() << std::endl;
}
