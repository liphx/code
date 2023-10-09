#ifndef LIPH_TERMINAL_H_
#define LIPH_TERMINAL_H_

#include <string>

namespace liph {

bool stdin_echo_on();
bool stdin_echo_off();
bool stdin_buffering_on();
bool stdin_buffering_off();
void clear_screen();
void clear_scrollback_buffer();

std::string color256fg(unsigned char);
std::string color256bg(unsigned char);
std::string color_reset();

extern std::string color256name[256];

}  // namespace liph

#endif  // LIPH_TERMINAL_H_
