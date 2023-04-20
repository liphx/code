#ifndef TERMINAL_H_
#define TERMINAL_H_

#define SET_ECHO_ON  1
#define SET_ECHO_OFF 0

/* 设置终端是否回显，成功返回0，否则返回-1 */
int set_echo(int fd, int flag);

#endif  // TERMINAL_H_
