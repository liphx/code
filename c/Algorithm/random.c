#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>           

// 生成[0, x)之间的随机数
int rand_int(unsigned int x)
{
    assert(x <= RAND_MAX + 1u);
    return rand() % x;
}

void swap(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}


void rand_sort(int array[], int n)
{
    srand(time(NULL));
    int i;
    for (i = 0; i < n; i++) {
        int j = rand_int(n - i);
        swap(&array[i], &array[n - 1 - j]);
    }
}

int main()
{
    int n = 10;
    int a[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    rand_sort(a, n);
    int i;
    for (i = 0; i < n; i++) {
        printf("%d ", a[i]);
    }

}