# errno

```c
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
```

示例

```c
#include <stdio.h>  // perror()
#include <string.h> // strerror()
#include <errno.h>  // errno

int main()
{
    int i;
    // for (i = 0; i < 256; i++) {
    //     printf("i = %d, %s\n", i, strerror(i));
    // }

    for (i = 0; i < 256; i++) {
        char str[15];
        sprintf(str, "errno = %d", i);
        errno = i;
        perror(str);
    }

    return 0;
}
```

Linux 下输出

```
errno = 0: Success
errno = 1: Operation not permitted
errno = 2: No such file or directory
errno = 3: No such process
errno = 4: Interrupted system call
errno = 5: Input/output error
errno = 6: No such device or address
errno = 7: Argument list too long
errno = 8: Exec format error
errno = 9: Bad file descriptor
errno = 10: No child processes
errno = 11: Resource temporarily unavailable
errno = 12: Cannot allocate memory
errno = 13: Permission denied
errno = 14: Bad address
errno = 15: Block device required
errno = 16: Device or resource busy
errno = 17: File exists
errno = 18: Invalid cross-device link
errno = 19: No such device
errno = 20: Not a directory
errno = 21: Is a directory
errno = 22: Invalid argument
errno = 23: Too many open files in system
errno = 24: Too many open files
errno = 25: Inappropriate ioctl for device
errno = 26: Text file busy
errno = 27: File too large
errno = 28: No space left on device
errno = 29: Illegal seek
errno = 30: Read-only file system
errno = 31: Too many links
errno = 32: Broken pipe
errno = 33: Numerical argument out of domain
errno = 34: Numerical result out of range
errno = 35: Resource deadlock avoided
errno = 36: File name too long
errno = 37: No locks available
errno = 38: Function not implemented
errno = 39: Directory not empty
errno = 40: Too many levels of symbolic links
errno = 41: Unknown error 41
errno = 42: No message of desired type
errno = 43: Identifier removed
errno = 44: Channel number out of range
errno = 45: Level 2 not synchronized
errno = 46: Level 3 halted
errno = 47: Level 3 reset
errno = 48: Link number out of range
errno = 49: Protocol driver not attached
errno = 50: No CSI structure available
errno = 51: Level 2 halted
errno = 52: Invalid exchange
errno = 53: Invalid request descriptor
errno = 54: Exchange full
errno = 55: No anode
errno = 56: Invalid request code
errno = 57: Invalid slot
errno = 58: Unknown error 58
errno = 59: Bad font file format
errno = 60: Device not a stream
errno = 61: No data available
errno = 62: Timer expired
errno = 63: Out of streams resources
errno = 64: Machine is not on the network
errno = 65: Package not installed
errno = 66: Object is remote
errno = 67: Link has been severed
errno = 68: Advertise error
errno = 69: Srmount error
errno = 70: Communication error on send
errno = 71: Protocol error
errno = 72: Multihop attempted
errno = 73: RFS specific error
errno = 74: Bad message
errno = 75: Value too large for defined data type
errno = 76: Name not unique on network
errno = 77: File descriptor in bad state
errno = 78: Remote address changed
errno = 79: Can not access a needed shared library
errno = 80: Accessing a corrupted shared library
errno = 81: .lib section in a.out corrupted
errno = 82: Attempting to link in too many shared libraries
errno = 83: Cannot exec a shared library directly
errno = 84: Invalid or incomplete multibyte or wide character
errno = 85: Interrupted system call should be restarted
errno = 86: Streams pipe error
errno = 87: Too many users
errno = 88: Socket operation on non-socket
errno = 89: Destination address required
errno = 90: Message too long
errno = 91: Protocol wrong type for socket
errno = 92: Protocol not available
errno = 93: Protocol not supported
errno = 94: Socket type not supported
errno = 95: Operation not supported
errno = 96: Protocol family not supported
errno = 97: Address family not supported by protocol
errno = 98: Address already in use
errno = 99: Cannot assign requested address
errno = 100: Network is down
errno = 101: Network is unreachable
errno = 102: Network dropped connection on reset
errno = 103: Software caused connection abort
errno = 104: Connection reset by peer
errno = 105: No buffer space available
errno = 106: Transport endpoint is already connected
errno = 107: Transport endpoint is not connected
errno = 108: Cannot send after transport endpoint shutdown
errno = 109: Too many references: cannot splice
errno = 110: Connection timed out
errno = 111: Connection refused
errno = 112: Host is down
errno = 113: No route to host
errno = 114: Operation already in progress
errno = 115: Operation now in progress
errno = 116: Stale file handle
errno = 117: Structure needs cleaning
errno = 118: Not a XENIX named type file
errno = 119: No XENIX semaphores available
errno = 120: Is a named type file
errno = 121: Remote I/O error
errno = 122: Disk quota exceeded
errno = 123: No medium found
errno = 124: Wrong medium type
errno = 125: Operation canceled
errno = 126: Required key not available
errno = 127: Key has expired
errno = 128: Key has been revoked
errno = 129: Key was rejected by service
errno = 130: Owner died
errno = 131: State not recoverable
errno = 132: Operation not possible due to RF-kill
errno = 133: Memory page has hardware error
errno = 134: Unknown error 134
.
.
.
errno = 255: Unknown error 255
```