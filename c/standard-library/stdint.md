# stdint

## 类型

提供定宽整数类型, `typedef`

宽度**恰好**为8, 16, 32, 64位的有/无符号整型

* int8_t
* int16_t
* int32_t
* int64_t
* uint8_t
* uint16_t
* uint32_t
* uint64_t

宽度**至少**有8, 16, 32, 64位的**最快**的有/无符号整型型

* int_fast8_t
* int_fast16_t
* int_fast32_t
* int_fast64_t
* uint_fast8_t
* uint_fast16_t
* uint_fast32_t
* uint_fast64_t

宽度**至少**有8, 16, 32, 64位的**最小**的有/无符号整型型

* int_least8_t
* int_least16_t
* int_least32_t
* int_least64_t
* uint_least8_t
* uint_least16_t
* uint_least32_t
* uint_least64_t

最大宽度的有/无符号整数类型

* intmax_t
* uintmax_t

足以保有指针的有/无符号整数类型 

* intptr_t
* uintptr_t

## 常量宏

上面各类型的最小/最大值分别为对应的 `_MIN`, `_MAX` 值

如 `INT8_MIN, INTMAX_MIN, UINT32_MAX, UINTPTR_MAX`

## 函数宏

展开成拥有以其参数指定的值和类型分别为 `(u)int_least8/16/32/64_t, (u)intmax_t` 类型的整数常量表达式

* INT8_C
* INT16_C
* INT32_C
* INT64_C
* INTMAX_C
* UINT8_C
* UINT16_C
* UINT32_C
* UINT64_C
* UINTMAX_C

例如, `UINT64_C(0x123)` 可以展开成 0x123ULL 或 0x123UL
