```shell
$ cat 0.c
void f() {}
$ cat 1.c
extern void f();
void g() { f(); }
$ gcc -c 0.c 1.c
$ ar -r lib0.a 0.o
ar: 正在创建 lib0.a
$ ar -r lib1.a 1.o
ar: 正在创建 lib1.a
$ cat 2.c
extern void g();
int main() { g(); }
$ gcc 2.c lib0.a lib1.a                                 # error
lib1.a(1.o)：在函数‘g’中：
1.c:(.text+0xa)：对‘f’未定义的引用
collect2: error: ld returned 1 exit status
$ gcc 2.c lib1.a lib0.a                                 # ok
$ gcc 2.c lib0.a lib1.a lib0.a                          # ok
$ gcc 2.c -Xlinker "-(" lib0.a lib1.a -Xlinker "-)"     # ok
```
