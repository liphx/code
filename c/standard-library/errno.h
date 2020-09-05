#define errno /* implement */
// errno预处理宏展开成一个int类型的可修改左值
// 该值具有线程局域 from c11
// 一些标准库函数通过写入正整数到 errno 指定错误，通常，会将 errno 的值设置为错误码之一
// 错误码作为以字母 E 后随大写字母或数字开始的宏常量
// errno 的值在程序启动时为 ​0​ ，而且无论是否出现错误，都允许库函数将正整数写入 errno ，但不会将 ​0​ 存储于 errno

#define EDOM    /* int 类型常量表达式，指明数学参数在定义域外 */
#define EILSEQ  /* int 类型常量表达式，指明非法字节序列 */
#define ERANGE  /* int 类型常量表达式，指明结果过大 */

/* 具体的系统定义了很多额外的错误常量，并以E 开头 */