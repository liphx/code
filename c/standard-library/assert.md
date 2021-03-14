# assert

1. assert

宏 assert 的定义依赖于标准库不定义的另一个宏 `NDEBUG`

```c
#ifdef NDEBUG
#define assert(condition) ((void)0)
#else
#define assert(condition) /* 若condition 等于0，在标准错误输出诊断信息并调用abort() */
#endif
```

2. static_assert

static_assert不受 NDEBUG影响，且发生在编译期

```c
#define static_assert _Static_assert /* from c11 */
/* _Static_assert(constant-expression, string-literal); */
/* 对于_Static_assert，若常量表达式等于0，则产生编译错误，显示错误信息 */
```

示例

```c
// #define NDEBUG
// 若定义NDEBUG，则assert不起作用
// 需在#define NDEBUG后 #include <assert.h>才能让assert失效
/* 
    #include <assert.h>
    #define NDEBUG
    #include <assert.h>
    仍然让assert 失效
 */
#include <assert.h>

int main()
{
    int i = 1;
    // assert(i < 0); /* 去掉注释则断言失败，程序中止 */

    static_assert(sizeof(void *) == 8, "指针长度是8字节大小"); 
    /* 若sizeof(void *)不为8则产生编译错误 */

    return 0;
}
```
