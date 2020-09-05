#include <stdio.h>  // perror()
#include <string.h> // strerror()
#include <errno.h>  // errno

int main()
{
    int i;
    for (i = 0; i < 256; i++) {
        printf("i = %d, %s\n", i, strerror(i));
    }

    for (i = 0; i < 256; i++) {
        char str[15];
        sprintf(str, "errno = %d", i);
        errno = i;
        perror(str);
    }

    return 0;
}