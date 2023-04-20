# ECMAScript

## 标识符

以 _ $ 或 字母开头，后面的字符可以是 _ $ 字母和数字。字母包括 Unicode 中的字母字符。

## 注释

```
// 单行注释
/* 多行
注释 */
```

## 严格模式

在脚本开头或单个函数开头加上

```js
"use strict";
```

## 语句

分号可选，不加分号则由解释器来确定语句的结尾

## 保留字与关键字

```
break       do          in          typeof      case        else
instanceof  var         catch       export      new         void
class       extends     return      while       const       finally
super       with        continue    for         switch      yield
debugger    function    this        default     if          throw
delete      import      try         enum        implements  package
public      interface   protected   static      let         private
await
```

## 变量

声明变量: var, const, let

### var 声明提升

```js
console.log(age);
var age = 26;
```

等价于

```js
var age;
console.log(age);
age = 26;
```

输出 undefined

## 数据类型

typeof 操作符返回以下字符串之一

- "undefined"
- "boolean"
- "string"
- "number"
- "object"
- "function"
- "symbol"

```js
console.log(typeof a)           //=> undefined
console.log(typeof undefined)   //=> undefined
console.log(typeof 3.14)        //=> number
console.log(typeof "")          //=> string
console.log(typeof typeof "")   //=> string
console.log(typeof true)        //=> boolean
console.log(typeof null)        //=> object
console.log(typeof {})          //=> object
console.log(typeof [])          //=> object
console.log(typeof console.log) //=> function
```

## Boolean 类型

Boolean() 转型函数将以下值转为 false

```js
Boolean(false)
Boolean("")
Boolean(0)
Boolean(NaN)
Boolean(null)
Boolean(undefined)
```

其它值转为 true

## Number 类型

```js
console.log(Number.MAX_VALUE)   //=> 1.7976931348623157e+308
console.log(Number.MIN_VALUE)   //=> 5e-324
console.log(Number.POSITIVE_INFINITY)   //=> Infinity
console.log(Number.NEGATIVE_INFINITY)   //=> -Infinity
console.log(isFinite(Number.MAX_VALUE * 2)); //=> false
console.log(0/0);   // NaN
console.log(1/0);   // Infinity
console.log(-1/0);  // -Infinity
console.log(-1/-0); // Infinity
console.log(NaN == NaN);    // false
console.log(isNaN(NaN));    // true
console.log(isNaN("10"));   // false，可以转换为数值10
console.log(isNaN("blue")); // true，不可以转换为数值
console.log(isNaN(true));   // false，可以转换为数值1
```

转型函数 Number(), parseInt(), parseFloat()

Number() 函数基于如下规则执行转换

- 布尔值，true转换为1，false转换为0
- 数值，直接返回
- null，返回0
- undefined，返回NaN
- 字符串，返回对应的值或NaN
- 对象，调用valueOf()，并按上述规则转换；如果转换结果是NaN，调用toString()，再按字符串的规则转换

parseInt(), parseFloat() 转换字符串为数值

```js
console.log(parseInt("123abc"))     // 123
console.log(parseInt(""))           // NaN
console.log(parseInt("0xa"))        // 10
console.log(parseInt("22.5"))       // 22
console.log(parseInt("0xAF", 16))   // 175, 指定底数
console.log(parseInt("AF", 16))     // 175, 指定底数, 16进制可忽略前导0x
console.log(parseInt("AF"))         // NaN
console.log(parseInt("10", 2))      // 2
console.log(parseFloat("22.34.5"))  // 22.34
console.log(parseFloat("3.125e7"))   // 31250000
```

## String 类型

字符串可以以双引号("), 单引号('), 反引号(`) 包裹

String() 转型函数

- 如果值有toString()方法，则调用该方法（不传参数）并返回结果
- 如果值是null，返回"null"
- 如果值是undefined，返回"undefined"

