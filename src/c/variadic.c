/*
 * void va_start(va_list ap, parmN);
 * T va_arg(va_list ap, T);
 * void va_copy(va_list dest, va_list src); // from c99
 * void va_end(va_list ap);
 */

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#define PRINT_MAX_SIZE 1024

/* str为分隔符，返回值为输出的长度，注意打印的字符串长度最多为 PRINT_MAX_SIZE -1 */
int print(const char *fmt, const char *str, ...) {
    char buf[PRINT_MAX_SIZE] = {0};
    va_list ap;
    va_start(ap, str);
    while (*fmt) {
        /* 最后一项不打印分隔符，ISO C 规定多余的 str 参数被丢弃 */
        if (*fmt == 'd') {
            int i = va_arg(ap, int);
            snprintf(buf + strlen(buf), PRINT_MAX_SIZE - strlen(buf), *(fmt + 1) ? "%d%s" : "%d", i, str);
        } else if (*fmt == 'c') {
            /* 字符字面量为 int类型，且char 在 ... 中被提升为 int 类型 */
            int c = va_arg(ap, int);
            snprintf(buf + strlen(buf), PRINT_MAX_SIZE - strlen(buf), *(fmt + 1) ? "%c%s" : "%c", c, str);
        } else if (*fmt == 'f') {
            double d = va_arg(ap, double);
            snprintf(buf + strlen(buf), PRINT_MAX_SIZE - strlen(buf), *(fmt + 1) ? "%f%s" : "%f", d, str);
        } else if (*fmt == 's') {
            char *s = va_arg(ap, char *);
            snprintf(buf + strlen(buf), PRINT_MAX_SIZE - strlen(buf), *(fmt + 1) ? "%s%s" : "%s", s, str);
        } else {
            va_end(ap);
            return -1;
        }
        fmt++;
    }
    va_end(ap);
    return printf("%s", buf);
}

int main() {
    int count;
    count = print("fdscs", " ", 0.99, 42, "hello,", '\b', "world.\n");
    printf("count = %d\n", count);
    count = printf("%f %d %s %c %s", 0.99, 42, "hello,", '\b', "world.\n");
    printf("count = %zu\n", strlen("0.990000 42 hello, world."));

    return 0;
}
