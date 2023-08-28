#include <iostream>
#include <string_view>

#include "liph/liph.h"
struct A {
    A(bool b) { std::cout << 1 << std::endl; }
    A(std::string_view s) { std::cout << 2 << std::endl; }
};

int main(int argc, char **argv) {
    std::cout << liph::big_integer::rand().string() << std::endl;
    // std::cout << liph::big_integer::rand(1024).string() << std::endl;

    liph::json j("hello");
    std::cout << j.to_string() << std::endl;
    A("hello");
    std::cout << liph::format("{}, {}, {}", sizeof(long), sizeof(long long), sizeof(int64_t)) << std::endl;

    std::string str;
    liph::read_file(argv[1], str);
    auto json = liph::json::parse(str);
    std::cout << json.to_string() << std::endl;
}
