# getchar()与putchar()复制文件

```c
/* copy.c */
#include <stdio.h>

int main()
{
    int c;
    while((c = getchar()) != EOF)
        putchar(c);
    return 0;
}
```

Linux下，通过重定向，这段代码可以复制任何格式的文件（文本文件与普通二进制文件）

```shell
gcc copy.c
./a.out < file1 > file2
```

Windows下，也可以用来复制文本文件，但文本中单独的\r或\n均会被替代为\r\n

当复制普通二进制文件时，由于ASCII字符26（0x1a）为替代字符（SUB），被用于替代识别为无效、错误或不能在指定设备上表示的字符；相当于标准计算机键盘同时按下Ctrl + Z键，getchar()会返回-1，程序终止。
