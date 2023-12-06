#include <iostream>
#include <typeinfo>

auto add(int a, double b) { return a + b; }
auto add2(int a, double b) -> double { return a + b; }
auto add3(int a, double b) -> int { return a + b; }

int main() {
    std::cout << typeid(add(1, 3.1)).name() << std::endl;   // d
    std::cout << typeid(add2(1, 3.1)).name() << std::endl;  // d
    std::cout << typeid(add3(1, 3.1)).name() << std::endl;  // i
}
