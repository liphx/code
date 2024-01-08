#include <climits>
#include <iostream>
#include <limits>

#define print(s) std::cout << #s " = " << s << std::endl

int main() {
    print(std::numeric_limits<char>::digits);
    print(std::numeric_limits<unsigned char>::digits);
    print(std::numeric_limits<signed char>::digits);
    print(CHAR_BIT);
    // use std::numeric_limits<unsigned char>::digits or CHAR_BIT to get the number of bits in a byte

    print(std::numeric_limits<int>::digits);
    print(std::numeric_limits<unsigned int>::digits);
}
