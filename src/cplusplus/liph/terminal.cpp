#include "liph/terminal.h"

#include "liph/macros.h"

#ifdef OS_UNIX
#include <termios.h>
#include <unistd.h>
#endif

#include <cstdio>
#include <cstdlib>

namespace liph {

#ifdef OS_UNIX
bool stdin_echo_on() {
    struct termios tm;
    if (tcgetattr(STDIN_FILENO, &tm) != 0) return false;
    tm.c_lflag |= ECHO;
    return tcsetattr(STDIN_FILENO, TCSANOW, &tm) == 0;
}

bool stdin_echo_off() {
    struct termios tm;
    if (tcgetattr(STDIN_FILENO, &tm) != 0) return false;
    tm.c_lflag &= ~ECHO;
    return tcsetattr(STDIN_FILENO, TCSANOW, &tm) == 0;
}

bool stdin_buffering_on() { return false; }

bool stdin_buffering_off() {
    struct termios tm;
    if (tcgetattr(STDIN_FILENO, &tm) != 0) return false;
    tm.c_lflag &= ~ICANON;
    tm.c_cc[VMIN] = 1;
    tm.c_cc[VTIME] = 0;
    return tcsetattr(STDIN_FILENO, TCSANOW, &tm) == 0;
}

void clear_screen() { printf("\033[2J"); }
void clear_scrollback_buffer() { printf("\033[3J"); }
void reset_screen() { system("reset"); }

#endif

std::string color256fg(unsigned char n) { return "\033[38;5;" + std::to_string(static_cast<int>(n)) + "m"; }
std::string color256bg(unsigned char n) { return "\033[48;5;" + std::to_string(static_cast<int>(n)) + "m"; }
std::string color_reset() { return "\033[0m"; }

}  // namespace liph
