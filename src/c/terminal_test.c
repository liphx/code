#include "terminal.h"

#include "common.h"

int main() {
    char buf[1024];
    scanf("%s", buf);

    puts("set echo off");
    if (set_echo(STDIN_FILENO, SET_ECHO_OFF) != 0) err_exit("set echo fail", 1);
    scanf("%s", buf);

    puts("set echo on");
    if (set_echo(STDIN_FILENO, SET_ECHO_ON) != 0) err_exit("set echo fail", 1);
    scanf("%s", buf);
}
