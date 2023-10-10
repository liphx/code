#include "liph/terminal.h"

#include "gtest/gtest.h"

namespace liph {

TEST(terminal, color) {
    std::cout << rgb(255, 0, 0).fg() << rgb(255, 255, 0).bg() << 42 << "\n";
    std::cout << color_reset();
}

}  // namespace liph
