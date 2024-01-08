#include <iostream>

int main() {
    while (char c = std::cin.get() != 'q') std::cout << (int)c << std::endl;
    // != has a higher priority than =, so 'c' is unprintable

    // while ((char c = std::cin.get()) != 'q') std::cout << c << std::endl;
    // compile error

    char c;
    while ((c = std::cin.get()) != 'q') std::cout << c;
}
