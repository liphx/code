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

    static_assert(sizeof(void *) == 8, "指针长度是8字节大小"); /* 若sizeof(void *)不为8则产生编译错误 */

    return 0;
}