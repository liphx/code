#include "liph/string_view.h"

#include <iostream>

#include "gtest/gtest.h"

namespace liph {
namespace test {

TEST(string_view, string_view) {
    constexpr string_view unicode[]{"▀▄─", "▄▀─", "▀─▄", "▄─▀"};

    for (int y{}, p{}; y != 6; ++y, p = ((p + 1) % 4)) {
        for (int x{}; x != 16; ++x) std::cout << unicode[p];
        std::cout << '\n';
    }
}

}  // namespace test
}  // namespace liph
