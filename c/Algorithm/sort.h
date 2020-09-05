#ifndef _SORT_H_
#define _SORT_H_

#include <stddef.h>

void bubble_sort(void *ptr, size_t count, size_t size,
            int (*comp)(const void *, const void *));

void quick_sort(void *ptr, size_t count, size_t size,
            int (*comp)(const void *, const void *));

#endif 