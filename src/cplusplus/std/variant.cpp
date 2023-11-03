#include <iostream>
#include <variant>

int main() {
    std::variant<int, std::string> var{"hi"};
    std::cout << var.index() << std::endl;  // 1
    var = 42;
    std::cout << var.index() << std::endl;  // 0
    try {
        int i = std::get<0>(var);
        std::cout << i << std::endl;  // 42
        std::string s = std::get<std::string>(var);
    } catch (const std::bad_variant_access& e) {
        std::cerr << e.what() << std::endl;
        // std::get: wrong index for variant
    }
}
