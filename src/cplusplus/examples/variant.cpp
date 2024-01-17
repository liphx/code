#include <iostream>
#include <variant>

template <class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};

template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

int main() {
    {
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

        std::cout << std::boolalpha;
        std::cout << std::holds_alternative<int>(var) << std::endl;          // true
        std::cout << std::holds_alternative<std::string>(var) << std::endl;  // false
    }

    {
        struct TypeA {};
        struct TypeB {};
        struct TypeC {};

        std::variant<TypeA, TypeB, TypeC> var{TypeB{}};
        std::visit(overloaded{
                           [](TypeA& a) { std::cout << "TypeA\n"; },
                           [](TypeB& b) { std::cout << "TypeB\n"; },
                           [](TypeC& c) { std::cout << "TypeC\n"; },
                   },
                var);  // TypeB
    }
}
