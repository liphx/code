#include <iostream>

enum class E : int {
    zero = 0,
    one = 1,
};

int main() {
    E e = E::zero;
    // int i = e;
    int i = static_cast<int>(e);
    std::cout << i << std::endl;
}
