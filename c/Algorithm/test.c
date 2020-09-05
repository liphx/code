#include <stdio.h>
#include <stdlib.h>
#include "sort.h"

int comp_int(int *a, int *b)
{
    if (*a > *b) return 1;
    if (*a == *b) return 0;
    return -1;
}

typedef int (*func_comp)(const void *, const void *);

int main()
{
    int array[8] = { 4, 1, 7, 7, 3, -1, 2, 5 };
    int i;
    for (i = 0; i < 8; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");

    quick_sort(array, 8, sizeof (int), (func_comp)comp_int);

    for (i = 0; i < 8; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");

    return 0;
}