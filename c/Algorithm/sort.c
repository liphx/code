#include "sort.h"
#include <string.h>

static void swap(void *ptr1, void *ptr2, size_t size)
{
    char tmp[size];
    memcpy(tmp, ptr1, size);
    memcpy(ptr1, ptr2, size);
    memcpy(ptr2, tmp, size);
}

/* 冒泡排序 */
void bubble_sort(void *ptr, size_t count, size_t size,
            int (*comp)(const void *, const void *))
{
    size_t i, j;
    for (i = 0; i < count; i++) {
        for (j = 0; j < count; j++) {
            if (comp(ptr + i * size, ptr + j * size) < 0) {
                swap(ptr + i * size, ptr + j * size, size);
            }
        }
    }
}

/* 一趟快速排序 */
static int _partition(void *ptr, int low, int high, size_t size,
            int (*comp)(const void *, const void *))
{
    char pivotkey[size];
    memcpy(pivotkey, ptr + low * size, size);
    while (low < high) {
        while (low < high && comp(ptr + high * size, pivotkey) >= 0) --high;
        memcpy(ptr + low * size, ptr + high * size, size);
        while (low < high && comp(ptr + low * size, pivotkey) <= 0) ++low;
        memcpy(ptr + high * size, ptr + low * size, size);
    }
    memcpy(ptr + low * size, pivotkey, size);
    return low;
}

/* 快速排序，分治思想 */
static void _quick_sort(void *ptr, int low, int high, size_t size,
            int (*comp)(const void *, const void *))
{
    if (low < high) {
        size_t pivotkey = _partition(ptr, low, high, size, comp);
        _quick_sort(ptr, low, pivotkey - 1, size, comp);
        _quick_sort(ptr, pivotkey + 1, high, size, comp);
    }
}

/* 快速排序 */
void quick_sort(void *ptr, size_t count, size_t size,
            int (*comp)(const void *, const void *))
{
    _quick_sort(ptr, 0, count - 1, size, comp);
}

