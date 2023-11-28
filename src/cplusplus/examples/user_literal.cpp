#include <iostream>

int operator""_k(unsigned long long n) { return n * 1000; }

int main() {
    std::cout << 1_k << std::endl;
    std::cout << 2_k - 121 << std::endl;
}
