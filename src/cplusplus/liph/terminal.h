#ifndef LIPH_TERMINAL_H_
#define LIPH_TERMINAL_H_

#include <termios.h>

namespace liph {

inline bool echo_on(int fd) {
    struct termios tm;
    if (tcgetattr(fd, &tm) != 0) return false;
    tm.c_lflag |= ECHO;
    return tcsetattr(fd, TCSANOW, &tm) == 0;
}

inline bool echo_off(int fd) {
    struct termios tm;
    if (tcgetattr(fd, &tm) != 0) return false;
    tm.c_lflag &= ~ECHO;
    return tcsetattr(fd, TCSANOW, &tm) == 0;
}

}  // namespace liph

#endif  // LIPH_TERMINAL_H_
