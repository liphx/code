# Go

## 安装

```shell
$ wget https://golang.org/dl/go1.15.7.linux-amd64.tar.gz
$ sudo tar -C /usr/local -xzf go1.15.7.linux-amd64.tar.gz
$ echo 'export PATH=$PATH:/usr/local/go/bin' >> ~/.profile
$ source ~/.profile
$ go version
go version go1.15.7 linux/amd64
```

## hello, world

```go
package main

import "fmt"

func main() {
    fmt.Println("Hello, World!")
}
```

```shell
$ go run .
Hello, World!
```

## go commands

`go <command> [arguments]`

| command   | args  | 解释 |
|:--        |:--    |:--|
| run       | package | 编译并运行程序 |
| build     | package | 编译 |
| get       | package | 下载安装包及其依赖 |
| env       |         | 输出go 环境变量 |

一些网站

* <https://go.dev/>
* <https://go-zh.org/>

## 包

### 包声明

package _name_

main 包定义的是可执行程序而非库

### 导入包

以下两种方式等价

```go
import "fmt"
import "os"
```

```go
import (
    "fmt"
    "os"
)
```

### 导入包的重命名

通过重命名可用导入名称相同的包，也可以用来简化包名

```go
import (
    "crypto/rand"
    mrand "math/rand"
)
```

### 匿名导入

导入包而不使用会产生编译错误`imported and not used`, 将包重命名为 `_` 可用匿名导入这个包，它会计算包级变量的初始化表达式和导入包的`init` 函数

### 下载包

## 语法

### 常量

`const a = 1`

### 变量

`var name type = expression`

* name 和 expression 可用省略一个
* 若省略expression, 为这个类型的零值

```go
var i, j, k int // int, int, int
var b, f, s = true, 2.3, "four" // bool, float64, string
var f, err = os.Open(name) // os.Open returns a file and an error
```

函数内部的变量声明可用用如下简化方式

`name := expression`

```go
freq := rand.Float64() * 3.0
t := 0.0
i, j := 0, 1
f, err := os.Open(name)
```

简短变量声明语句中必须至少要声明一个新的变量, 以下用法是正确的

```go
a := 1
a, b := 2, 2
```

而下面用法则错误

```go
a := 1
a := 2
```

### 指针

```go
x := 1
p := &x         // p, of type *int, points to x
fmt.Println(*p) // "1"
*p = 2          // equivalent to x = 2
fmt.Println(x)  // "2"
```

可以返回局部变量的地址

```go
func f() *int {
    a := 1
    return &a
}
```

new

`pa := new(int)`

### 赋值

```go
x = 1
x, y = y, x
f, err = os.Open("foo.txt")
_, ok = x.(T)
```

### 类型定义

`type name underlying-type`

## 数据类型

```go
var x int           // 0
var x float64       // 0
var x complex128    // (0+0i)
var x bool          // false
var x string        // ""
var x [3]int        // [0 0 0]
var x []int         // [] Slice


var x map[string]int // map[]
var y = make(map[string]int)

type X struct {
    a int
    b bool
}
var x X             // {0 false}
```


## 函数

```go
func name(parameter-list) (result-list) {
    body
}
```

## 测试

`vim add_test.go`

```go
package test_add

import "testing"

func Add(a, b int) int {
	return a - b
}

func TestAdd(t *testing.T) {
	if (Add(0, 0) != 0) {
		t.Errorf("error")
	}

	if (Add(1, 1) != 2) {
		t.Errorf("error")
	}
}
```

```shell
$ go test add_test.go
--- FAIL: TestAdd (0.00s)
    add_test.go:15: error
FAIL
FAIL    command-line-arguments  0.002s
FAIL
```
