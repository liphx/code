#include "terminal.h"

#include <termios.h>

int set_echo(int fd, int flag) {
    struct termios tm;
    if (tcgetattr(fd, &tm) != 0) return -1;

    if (flag) { /* 设为回显 */
        tm.c_lflag |= ECHO;
        if (tcsetattr(fd, TCSANOW, &tm) != 0) /* TCSANOW 使设置立即生效 */
            return -1;
    } else { /* 设为不回显*/
        tm.c_lflag &= ~ECHO;
        if (tcsetattr(fd, TCSANOW, &tm) != 0) return -1;
    }
    return 0;
}
