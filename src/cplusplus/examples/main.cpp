#include <cstdint>
#include <fstream>
#include <iostream>

int main() {
    std::cout << "hello, world." << std::endl;
    char s = -1;
    std::cout << static_cast<uint32_t>(static_cast<uint8_t>(s)) << std::endl;

    int n = 42;
    std::ofstream out{"tmp"};
    out.write((char *)&n, 4);
}
