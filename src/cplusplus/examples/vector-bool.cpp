#include "liph/liph.h"

int main() {
    std::vector<bool> vb(10, true);
    liph::print(vb);
    vb.push_back(false);
    liph::print(vb);
    vb.flip();
    liph::print(vb);
    vb[3] = 1;
    liph::print(vb);
}
