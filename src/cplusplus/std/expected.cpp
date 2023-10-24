#include <expected>
#include <iostream>
#include <string_view>

struct Error {
    int num;
    std::string_view msg;
    Error(int num, std::string_view msg) : num(num), msg(msg) {}
};

std::expected<int, Error> parse(int n) {
    if (n >= 0) return n;
    return std::unexpected(Error(-1, "n < 0"));
}

int main() {
    auto e = parse(42);
    if (e) std::cout << *e << std::endl;

    e = parse(-1);
    if (!e) std::cout << e.error().num << ": " << e.error().msg << std::endl;

    e = parse(-2);
    std::cout << e.value_or(42) << std::endl;
}
