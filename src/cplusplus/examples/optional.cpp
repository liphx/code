#include <iostream>
#include <optional>
#include <string>

std::optional<int> to_int(const std::string& s) {
    try {
        return std::stoi(s);
    } catch (...) {
        return std::nullopt;
    }
}

int main() {
    for (auto s : {"42", "  077", "hello", "0x33"}) {
        std::optional<int> i = to_int(s);
        if (i)
            std::cout << *i << std::endl;
        else
            std::cout << "null" << std::endl;
    }
}
