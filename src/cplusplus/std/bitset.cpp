#include <bitset>
#include <iostream>

int main() {
    std::cout << sizeof(std::bitset<1>) << " ";
    std::cout << sizeof(std::bitset<63>) << " ";
    std::cout << sizeof(std::bitset<64>) << " ";
    std::cout << sizeof(std::bitset<65>) << std::endl;
    // 8 8 8 16

    std::bitset<20> bs(65535);
    std::cout << bs << std::endl;
    // 00001111111111111111
    unsigned long l = bs.to_ulong();
    std::cout << l << std::endl;
    // 65535

    std::bitset<6> bs2("1001");
    std::cout << bs2 << " " << bs2.to_ulong() << std::endl;
    // 001001 9

    std::bitset<6> bs3("110");
    std::cout << (bs2 & bs3) << std::endl;  // 000000
    std::cout << (bs2 | bs3) << std::endl;  // 001111
    std::cout << (bs2 ^ bs3) << std::endl;  // 001111

    std::cout << bs3.set(0).set(5) << std::endl;  // 100111
    std::cout << bs3.reset() << std::endl;        // 000000
    std::cout << bs3.flip() << std::endl;         // 111111
}
