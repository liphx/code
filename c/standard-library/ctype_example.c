#include <ctype.h>
#include <stdio.h>

int main()
{
    printf("%d\t", !!islower('A'));
    printf("%d\t", !!isupper('A'));
    printf("%d\t", !!isalpha('A'));
    printf("%d\t", !!isdigit('A'));
    printf("%d\n", !!isalnum('A'));
    //=>0 1 1 0 1

    printf("%d\t", !!isblank('\t'));
    printf("%d\t", !!isblank(' '));
    printf("%d\t", !!isblank('\r'));
    printf("%d\t", !!isblank('\v'));
    printf("%d\t", !!isblank('\f'));
    printf("%d\n", !!isblank('\n'));
    //=>1 1 0 0 0 0

    printf("%d\t", !!isspace('\t'));
    printf("%d\t", !!isspace(' '));
    printf("%d\t", !!isspace('\r'));
    printf("%d\t", !!isspace('\v'));
    printf("%d\t", !!isspace('\f'));
    printf("%d\n", !!isspace('\n'));
    //=>1 1 1 1 1 1

    /* 0x00-0x1f & 0x7f */
    printf("%d\t", !!iscntrl('\x00'));
    printf("%d\t", !!iscntrl('\x1f'));
    printf("%d\n", !!iscntrl('\x7f'));
    //=>1 1 1

    /* !"#$%&'()*+,-./:;<=>?@[\]^_`{|}~ */
    printf("%d\n", !!ispunct('~'));
    //=>1

    printf("%d\t", !!isprint(' '));
    printf("%d\t", !!isgraph(' '));
    printf("%d\t", !!isprint('\t'));
    printf("%d\t", !!isgraph('\t'));
    printf("%d\t", !!isprint('A'));
    printf("%d\n", !!isgraph('A'));
    //=>1 0 0 0 1 1

    /* 0-9a-fA-F */
    printf("%d\t", !!isxdigit('a'));
    printf("%d\t", !!isxdigit('A'));
    printf("%d\t", !!isxdigit('f'));
    printf("%d\t", !!isxdigit('g'));
    printf("%d\t", !!isxdigit('0'));
    printf("%d\n", !!isxdigit('9'));
    //=>1 1 1 0 1 1

    printf("%c\t", tolower('A'));
    printf("%c\t", tolower('a'));
    printf("%c\t", tolower('0'));
    printf("%d\t", tolower('\n'));
    printf("%c\t", toupper('A'));
    printf("%c\n", toupper('a'));
    //=>a a 0 10 A A

    return 0;
}