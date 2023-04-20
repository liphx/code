# redis

## 简单动态字符串

sds(simple dynamic string)的定义

```c
#include "sds.h"

typedef char *sds;

struct sdshdr {
    int len; // buf 中已占用空间的长度
    int free; // buf 中剩余可用空间的长度
    char buf[]; // 数据空间
};
```

API

```c
sds sdsnewlen(const void *init, size_t initlen);    // 创建 sds
sds sdsnew(const char *init);                       // 创建 sds
sds sdsempty(void);                                 // 创建空的 sds
size_t sdslen(const sds s);                         // len 大小
sds sdsdup(const sds s);                            // 复制 sds
sds sdscpy(sds s, const char *t);                   // 复制 C字符串到 sds
void sdsfree(sds s);                                // 释放 sds
size_t sdsavail(const sds s);                       // free 大小
sds sdsgrowzero(sds s, size_t len);                 // 用空字符扩展sds 到len 长度
sds sdscat(sds s, const char *t);                   // 拼接 C字符串
sds sdscatsds(sds s, const sds t);                  // 拼接 sds
void sdsclear(sds s);                               // 清空字符串
int sdscmp(const sds s1, const sds s2);             // 比较 sds
```

一些说明

* 从`char *`(sds) 到 `sdshdr`:

```c
struct sdshdr *sh = (void*)(sds-(sizeof(struct sdshdr)));
```

`sds` 实际是 `sdshdr.buf`

* buf 分配内存, 至少是 `len + free + 1`
* 预留空间算法

```c
// 此处newlen 是len + free 的值
if (newlen < SDS_MAX_PREALLOC)
    newlen *= 2;
else
    newlen += SDS_MAX_PREALLOC;
```

* 由于 sdshdr 由 len 标识实际的长度，buf 中的值可以为任意值(包括'\0')

总结

sds 简单扩展了 C字符串，提供了自动分配内存，预留空间，常数复杂度获取长度，存储二进制的功能

```c
#include "sds.h"
#include <stdio.h>

int main() {
    sds str = sdsnew("hello");
    printf("len = %d, free = %d, str = %s\n", sdslen(str), sdsavail(str), str);
    sdsfree(str);
}
```

## 链表

链表定义与API

```c
#include "adlist.h"

typedef struct listNode {
    struct listNode *prev;
    struct listNode *next;
    void *value;
} listNode; // 结点

typedef struct listIter {
    listNode *next;
    int direction;
} listIter; // 迭代器

typedef struct list {
    listNode *head;
    listNode *tail;
    void *(*dup)(void *ptr);
    void (*free)(void *ptr);
    int (*match)(void *ptr, void *key);
    unsigned long len;
} list;

// 获取/设置各个成员都是宏
#define listLength(l) ((l)->len)
#define listFirst(l) ((l)->head)
#define listLast(l) ((l)->tail)
#define listPrevNode(n) ((n)->prev)
#define listNextNode(n) ((n)->next)
#define listNodeValue(n) ((n)->value)
#define listSetDupMethod(l,m) ((l)->dup = (m))
#define listSetFreeMethod(l,m) ((l)->free = (m))
#define listSetMatchMethod(l,m) ((l)->match = (m))
#define listGetDupMethod(l) ((l)->dup)
#define listGetFree(l) ((l)->free)
#define listGetMatchMethod(l) ((l)->match)

// 一些函数
list *listCreate(void);
void listRelease(list *list);
list *listAddNodeHead(list *list, void *value);
list *listAddNodeTail(list *list, void *value);
list *listInsertNode(list *list, listNode *old_node, void *value, int after);
void listDelNode(list *list, listNode *node);
listIter *listGetIterator(list *list, int direction);
listNode *listNext(listIter *iter);
void listReleaseIterator(listIter *iter);
list *listDup(list *orig);
listNode *listSearchKey(list *list, void *key);
listNode *listIndex(list *list, long index);
void listRewind(list *list, listIter *li);
void listRewindTail(list *list, listIter *li);
void listRotate(list *list);

#define AL_START_HEAD 0 // 从表头向表尾进行迭代
#define AL_START_TAIL 1 // 从表尾到表头进行迭代
```

总结

adlist 提供了双向链表的实现，结点中数据的类型由链表中的三个函数决定

```c
#include "adlist.h"
#include <stdio.h>

int cmp(void *a, void *b) {
    return *(int *)a == *(int *)b;
}

int main() {
    list *l = listCreate();
    int array[] = { 1, 2, 3, 4, 5 };
    for (int i = 0; i < sizeof(array) / sizeof (int); i++) {
        listAddNodeTail(l, &array[i]);
    }

    listIter *iter = listGetIterator(l, AL_START_TAIL);
    listNode *node;
    while (node = listNext(iter)) {
        printf("%d ", *(int *)node->value);
    } // 5 4 3 2 1
    puts("");

    listSetMatchMethod(l, cmp);
    int num = 2;
    node = listSearchKey(l, &num);
    if (node)
        printf("%d ", *(int *)node->value);
    puts("");

    listRelease(l);
}
```
