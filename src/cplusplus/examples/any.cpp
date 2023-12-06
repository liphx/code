#include <any>
#include <iostream>

int main() {
    std::cout << std::boolalpha;

    std::any a;
    std::cout << a.type().name() << std::endl;
    // v

    a = 1;
    std::cout << a.type().name() << ": " << std::any_cast<int>(a) << std::endl;
    // i: 1
    a = 3.14;
    std::cout << a.type().name() << ": " << std::any_cast<double>(a) << std::endl;
    // d: 3.14
    a = true;
    std::cout << a.type().name() << ": " << std::any_cast<bool>(a) << std::endl;
    // b: true

    try {
        a = 1;
        std::cout << std::any_cast<float>(a) << std::endl;
    } catch (const std::bad_any_cast& e) {
        std::cout << e.what() << std::endl;
    }
    // bad any_cast

    a = 1;
    if (a.has_value()) {
        std::cout << a.type().name() << std::endl;
    }
    // i

    a.reset();
    if (!a.has_value()) {
        std::cout << "no value" << std::endl;
    }
    // no value

    a = 1;
    int *i = std::any_cast<int>(&a);
    std::cout << *i << std::endl;
    // 1
}
