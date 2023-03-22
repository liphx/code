#define ABCD 2
#include <stdio.h>

#define ABCD 2
// #define ABCD 1  // Wmacro-redefined

int main() {
#ifdef ABCD
    printf("1: yes\n");
#else
    printf("1: no\n");
#endif

#ifndef ABCD
    printf("2: no1\n");
#elif ABCD == 2
    printf("2: yes\n");
#else
    printf("2: no2\n");
#endif

#if !defined(DCBA) && (ABCD < 2 * 4 - 3)
    printf("3: yes\n");
#endif
}
