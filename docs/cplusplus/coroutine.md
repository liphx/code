# 协程

## 1. 从函数调用栈说起

```cpp
int foo(int a, int b, int c) {
    int sum = a + b + c;
    return sum;
}

int main() {
    int x = foo(100, 200, 300);
    return 0;
}
```

`https://godbolt.org/` x86-64 gcc 12.2, opts: `-m32`

```
foo(int, int, int):
        push    ebp
        mov     ebp, esp
        sub     esp, 16
        mov     edx, DWORD PTR [ebp+8]
        mov     eax, DWORD PTR [ebp+12]
        add     edx, eax
        mov     eax, DWORD PTR [ebp+16]
        add     eax, edx
        mov     DWORD PTR [ebp-4], eax
        mov     eax, DWORD PTR [ebp-4]
        leave
        ret
main:
        push    ebp
        mov     ebp, esp
        sub     esp, 16
        push    300
        push    200
        push    100
        call    foo(int, int, int)
        add     esp, 12
        mov     DWORD PTR [ebp-4], eax
        mov     eax, 0
        leave
        ret
```

栈帧 stack frame, x86栈增长方向为高位向低位增长

```
| ......                 | Stack Pointer, esp
| Callee saved registers |
| temporary storage      |
| Callee saved registers |
| Local #2               | ebp - 8
| Local #1               | ebp - 4
| Caller's EBP           | ebp
| Return Address         |
| Argument #1            | ebp + 8
| Argument #2            | ebp + 12
| Argument #3            | ebp + 16
| Caller saved registers |
```

## 2. 有栈协程

协程是能暂停执行以在之后恢复的函数。(yield, resume)，实现协程关键在于保存与恢复上下文。


## 参考

- https://redirect.cs.umbc.edu/~chang/cs313.s02/stack.shtml
