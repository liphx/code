#include <iostream>

#include "liph/numeric/random.h"

int main(int argc, char **argv) {
    int n = 16;  // default length
    if (argc > 1) n = atoi(argv[1]);
    std::cout << liph::random::gen_str(n, "0123456789") << std::endl;
    std::cout << liph::random::gen_str(n,
                         "0123456789"
                         "abcdefghijklmnopqrstuvwxyz"
                         "ABCDEFGHIJKLMNOPQRSTUVWXYZ")
              << std::endl;
    std::cout << liph::random::gen_str(n,
                         "0123456789"
                         "abcdefghijklmnopqrstuvwxyz"
                         "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                         "!@#$%^&*()-_+=[]{};'\":,.?/<>|`~")
              << std::endl;
}
