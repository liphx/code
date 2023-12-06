#include <stdio.h>

int main() {
    int ch, state = 0;  // 定义正常状态为0
    while ((ch = getchar()) != EOF) {
        /* 为使代码清晰，不嵌套使用switch */
        if (state == 0) {
            switch (ch) {
            case '/':  // 状态0到状态1，可能出现注释
                state = 1;
                break;
            case '\'':  // 状态0到状态2，进入字符常量''
                putchar(ch);
                state = 2;
                break;
            case '\"':  // 状态0到状态3，进入字符串常量""
                putchar(ch);
                state = 3;
                break;
            default:
                putchar(ch);
                break;
            }
        } else if (state == 1) {
            switch (ch) {
            case '*':  // 进入多行注释状态/*，到状态4
                state = 4;
                break;
            case '/':  // 进入单行注释状态//，到状态5
                state = 5;
                break;
            default:  // 没有遇到*或/，可能为头文件的路径分隔符或除法运算，打印/并恢复到状态0
                state = 0;
                putchar('/');
                break;
            }
        } else if (state == 2) {
            putchar(ch);
            switch (ch) {
            case '\\':  // 字符常量中遇到转义，到状态6
                state = 6;
                break;
            case '\'':  // 字符常量状态结束
                state = 0;
                break;
            default:
                break;
            }
        } else if (state == 3) {
            putchar(ch);
            switch (ch) {
            case '\\':  // 字符串常量中遇到转义，到状态7
                state = 7;
                break;
            case '\"':  // 字符串常量状态结束
                state = 0;
                break;
            default:
                break;
            }
        } else if (state == 4) {
            switch (ch) {
            case '*':  // 多行注释状态可能要结束，到状态8
                state = 8;
                break;
            default:
                break;
            }
        } else if (state == 5) {
            switch (ch) {
            case '\\':  // 单行注释状态遇到\，可能进入折行注释，进入状态9
                state = 9;
                break;
            case '\n':  // 单行注释状态结束，恢复到状态0
                putchar('\n');
                state = 0;
                break;
            default:
                break;
            }
        } else if (state == 6) {
            // 转义状态结束，恢复到字符常量状态
            putchar(ch);
            state = 2;
        } else if (state == 7) {
            // 转义状态结束，恢复到字符串常量状态
            putchar(ch);
            state = 3;
        } else if (state == 8) {
            switch (ch) {
            case '/':  // 多行注释状态结束，恢复到状态0
                state = 0;
                break;
            case '*':  // 保持状态8
                break;
            default:  //*后面不是*或/回到多行注释状态
                state = 4;
                break;
            }
        } else if (state == 9) {
            switch (ch) {
            case '\\':  // 可能进入折行注释，保持状态9
                break;
            default:  // 进入折行注释，恢复到状态5
                state = 5;
                break;
            }
        }
    }
}
