# stdlib

`#include <stdlib.h>`

## `rand, srand`

宏 `RAND_MAX` 展开成等于函数 rand() 最大返回值的整数常量表达式。此值为实现定义。标准保证此值至少为 32767

```c
int rand();                     // 返回0与RAND_MAX间的随机整数值（包含 0 与 RAND_MAX）
void srand(unsigned seed);      // 以值 seed 播种 rand() 所用的随机数生成器。
```

若在任何对 `srand()` 的调用前使用 `rand()` ，则 `rand()` 表现如同它以 `srand(1)` 播种。每次以 `srand()` 播种 `rand()` 时，它必须产生相同的值数列。

## `malloc, calloc, realloc, free`

```c
void* malloc(size_t size);
void* calloc(size_t num, size_t size);
void *realloc(void *ptr, size_t new_size);
void free(void* ptr);
```

`malloc(0)`: implementation-defined, may not be NULL

```c
void *p = malloc(0);
printf("%p\n", p);

// free(): avoid memory leak
free(p);
```
