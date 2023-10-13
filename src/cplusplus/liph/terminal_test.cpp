#include "liph/terminal.h"

#include "gtest/gtest.h"

namespace liph {

TEST(terminal, color) { std::cout << rgb(255, 0, 0).fg() << rgb(255, 255, 0).bg() << 42 << color_reset() << std::endl; }

TEST(terminal, text_style) {
    std::cout << text_bold() << "hello" << color_reset() << std::endl;
    std::cout << text_faint() << "hello" << color_reset() << std::endl;
    std::cout << text_italic() << "hello" << color_reset() << std::endl;
    std::cout << text_underline() << "hello" << color_reset() << std::endl;
    std::cout << text_blink() << "hello" << color_reset() << std::endl;
    std::cout << text_rapid_blink() << "hello" << color_reset() << std::endl;
    std::cout << text_reverse() << "hello" << color_reset() << std::endl;
    std::cout << text_conceal() << "hello" << color_reset() << std::endl;
    std::cout << text_strikethrough() << "hello" << color_reset() << std::endl;
}

}  // namespace liph
