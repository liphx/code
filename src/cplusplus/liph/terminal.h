#ifndef LIPH_TERMINAL_H_
#define LIPH_TERMINAL_H_

#include <cstdint>
#include <string>

#include "liph/format.h"

namespace liph {

bool stdin_echo_on();
bool stdin_echo_off();
bool stdin_buffering_on();
bool stdin_buffering_off();
void clear_screen();
void clear_scrollback_buffer();
void reset_screen();

enum class color256 : uint8_t {
    Black = 0,
    Maroon = 1,
    Green = 2,
    Olive = 3,
    Navy = 4,
    Purple = 5,
    Teal = 6,
    Silver = 7,
    Grey = 8,
    Red = 9,
    Lime = 10,
    Yellow = 11,
    Blue = 12,
    Fuchsia = 13,
    Aqua = 14,
    White = 15,
    NavyBlue = 17,
    DarkBlue = 18,
    DarkGreen = 22,
    DarkCyan = 36,
    LightSeaGreen = 37,
    DarkTurquoise = 44,
    MediumSpringGreen = 49,
    DarkRed = 52,
    BlueViolet = 57,
    SteelBlue = 67,
    CornflowerBlue = 69,
    CadetBlue = 72,
    MediumTurquoise = 80,
    DarkMagenta = 90,
    DarkViolet = 92,
    LightSlateGrey = 103,
    MediumPurple = 104,
    LightSlateBlue = 105,
    DarkSeaGreen = 108,
    LightGreen = 119,
    MediumVioletRed = 126,
    IndianRed = 131,
    MediumOrchid = 134,
    DarkGoldenrod = 136,
    RosyBrown = 138,
    DarkKhaki = 143,
    LightSteelBlue = 147,
    GreenYellow = 154,
    Orchid = 170,
    Violet = 177,
    Tan = 180,
    HotPink = 205,
    DarkOrange = 208,
    LightCoral = 210,
    SandyBrown = 215
};

std::string color256fg(uint8_t);
std::string color256bg(uint8_t);
inline std::string color256fg(color256 c) { return color256fg(static_cast<uint8_t>(c)); }
inline std::string color256bg(color256 c) { return color256bg(static_cast<uint8_t>(c)); }
std::string color_reset();
// https://en.wikipedia.org/wiki/ANSI_escape_code#Terminal_output_sequences
inline std::string text_bold() { return "\033[01m"; }
inline std::string text_faint() { return "\033[02m"; }
inline std::string text_italic() { return "\033[03m"; }
inline std::string text_underline() { return "\033[04m"; }
inline std::string text_blink() { return "\033[05m"; }
inline std::string text_rapid_blink() { return "\033[06m"; }
inline std::string text_reverse() { return "\033[07m"; }
inline std::string text_conceal() { return "\033[08m"; }
inline std::string text_strikethrough() { return "\033[09m"; }

struct rgb {
    uint8_t r, g, b;
    rgb() : rgb(0, 0, 0) {}
    rgb(uint8_t r_, uint8_t g_, uint8_t b_) : r(r_), g(g_), b(b_) {}

    std::string fg() const {
        return format("\033[38;2;{};{};{}m", static_cast<int>(r), static_cast<int>(g), static_cast<int>(b));
    }

    std::string bg() const {
        return format("\033[48;2;{};{};{}m", static_cast<int>(r), static_cast<int>(g), static_cast<int>(b));
    }
};

}  // namespace liph

#endif  // LIPH_TERMINAL_H_
