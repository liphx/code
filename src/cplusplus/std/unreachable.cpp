#include <iostream>
#include <utility>

int f(int n) {
    switch (n) {
    case 0:
        [[fallthrough]];
    case 1:
        return n;
    default:
        std::unreachable();
    }
}

int main() {
    std::cout << f(0) << std::endl;
    std::cout << f(1) << std::endl;
    std::cout << f(2) << std::endl;  // ub
}
