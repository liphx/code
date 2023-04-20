/*
 *
 * 对于下面这些函数，参数c是一个能表示成unsigned char或等于EOF的int，否则是未定义的行为
 * 函数的行为受本地化影响
 * `is*` 函数检查参数是否为相应类型，若是则返回非零值(true)，否则返回0
 *   int islower(int c);     // 检查是否为小写字母
 *   int isupper(int c);     // 检查是否为大写字母
 *   int isalpha(int c);     // 检查是否为字母
 *   int isdigit(int c);     // 检查是否为数字
 *   int isalnum(int c);     // 检查是否为字母或数字
 *   int isblank(int c);     // 检查是否为空格字符 from c99
 *   int isspace(int c);     // 检查是否为空白字符
 *   int iscntrl(int c);     // 检查是否为控制字符
 *   int ispunct(int c);     // 检查是否为标点
 *   int isgraph(int c);     // 检查是否为可显示的字符
 *   int isprint(int c);     // 检查是否为可打印字符
 *   int isxdigit(int c);    // 检查是否为十六进制的字符
 *   int tolower(int c);     // 将字符转换成小写
 *   int toupper(int c);     // 将字符转换成大写
 */

#include <ctype.h>
#include <stdio.h>

#define print(b) puts(b ? "true" : "false")

int main() {
    print(islower('A'));  // false
    print(isupper('A'));  // true
    print(isalpha('A'));  // true
    print(isdigit('A'));  // false
    print(isalnum('A'));  // true

    print(isblank('\t'));  // true
    print(isblank(' '));   // true
    print(isblank('\r'));  // false
    print(isblank('\v'));  // false
    print(isblank('\f'));  // false
    print(isblank('\n'));  // false

    print(isspace('\t'));  // true
    print(isspace(' '));   // true
    print(isspace('\r'));  // true
    print(isspace('\v'));  // true
    print(isspace('\f'));  // true
    print(isspace('\n'));  // true

    /* 0x00-0x1f & 0x7f */
    print(iscntrl('\x00'));  // true
    print(iscntrl('\x1f'));  // true
    print(iscntrl('\x7f'));  // true

    /* !"#$%&'()*+,-./:;<=>?@[\]^_`{|}~ */
    print(ispunct('~'));  // true

    print(isprint(' '));   // true
    print(isgraph(' '));   // false
    print(isprint('\t'));  // false
    print(isgraph('\t'));  // false
    print(isprint('A'));   // true
    print(isgraph('A'));   // true

    /* 0-9a-fA-F */
    print(isxdigit('a'));  // true
    print(isxdigit('A'));  // true
    print(isxdigit('f'));  // true
    print(isxdigit('g'));  // false
    print(isxdigit('0'));  // true
    print(isxdigit('9'));  // true

    printf("%c\t", tolower('A'));
    printf("%c\t", tolower('a'));
    printf("%c\t", tolower('0'));
    printf("%d\t", tolower('\n'));
    printf("%c\t", toupper('A'));
    printf("%c\n", toupper('a'));
    // a a 0 10 A A
}
