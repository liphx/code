/*
 * goto语句只能实现函数内跳转
 * setjmp, longjmp可以在栈上跳过若干调用栈，返回到当前函数调用路径上的某一个函数中
 *
 * #include <setjmp.h>
 * int setjmp(jmp_buf env); // 若直接调用，返回0；若从longjmp返回，返回值非0
 * void longjmp(jmp_buf env, int val);
 */

#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>

static void func1(int x);
static void func2(int x);

jmp_buf env; /* env, 通常定义为全局变量 */

int main() {
    switch (setjmp(env)) {
    case 0:
        break;
    case 1:
        fprintf(stderr, "func1 error\n");
        exit(1);
    case 2:
        fprintf(stderr, "func2 error\n");
        exit(2);
    default:
        exit(-1);
    }
    func1(2);
    exit(0);
}

void func1(int x) {
    puts("func1 called");
    if (x < 0)
        longjmp(env, 1);
    else
        func2(x);
}

void func2(int x) {
    puts("func2 called");
    if (x % 2 == 0) longjmp(env, 2);
}
