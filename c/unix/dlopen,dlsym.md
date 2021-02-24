# dlopen, dlsym

`hello.c`

```c
#include <stdio.h>

void hello()
{
    puts("hello called");
}
```

`main.c`

```c
#include <stdio.h>
#include <dlfcn.h>

typedef void (*pf)();

int main()
{
    void *handle = dlopen("./hello.so", RTLD_NOW | RTLD_GLOBAL);
    pf start = dlsym(handle, "hello");
    start();
}
```

```shell
$ gcc -fPIC -shared hello.c -o hello.so
$ gcc main.c -ldl
$ ./a.out 
hello called
```
