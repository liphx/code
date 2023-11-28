#include <iostream>
#include <tuple>

auto foo() { return std::make_tuple(42, 3.14, "hello"); }

int main() {
    {
        // C++17 structured binding
        auto [i, d, s] = foo();
        std::cout << i << ", " << d << ", " << s << std::endl;
    }
    {
        auto [_1, d, _2] = foo();
        std::cout << d << std::endl;
    }
    {
        int i;
        double d;
        const char *s;
        std::tie(i, d, s) = foo();
        std::cout << i << ", " << d << ", " << s << std::endl;
    }
    {
        double d;
        std::tie(std::ignore, d, std::ignore) = foo();
        std::cout << d << std::endl;
    }
}
