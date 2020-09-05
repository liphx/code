#ifdef NDEBUG
#define assert(condition) ((void)0)
#else
#define assert(condition) /* 若condition 等于0，在标准错误输出诊断信息并调用abort() */
#endif


#define static_assert _Static_assert /* from c11 */
/* _Static_assert(constant-expression, string-literal); */
/* 对于_Static_assert，若常量表达式等于0，则产生编译错误，显示错误信息 */
/* static_assert不受 NDEBUG影响，且发生在编译期 */