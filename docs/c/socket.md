# 网络IPC: socket

## 套接字描述符

```c
#include <sys/socket.h>

int socket(int domain, int type, int protocol);
// 若成功返回socket 描述符，若失败返回-1
```

参数 domain

| 域 | 描述 |
| -- | -- |
| `AF_INET` | IPv4 因特网域 |
| `AF_INET6`| IPv6 因特网域 |
| `AF_UNIX` | UNIX域 |
| `AF_UNSPE`| 未指定 |

参数 type

| 类型 | 描述 |
| -- | -- |
| `SOCK_DGRAM`  | 固定长度的、无连接的、不可靠的报文传递 |
| `SOCK_RAW`    | IP协议的数据报接口 |
| `SOCK_SEQPACKET` | 固定长度的、有序的、可靠的、面向连接的报文传递 |
| `SOCK_STREAM` | 有序的、可靠的、双向的、面向连接的字节流 |

参数 protocol

0 表示为给定的域和套接字类型选择默认协议，`AF_INET` 下，`SOCK_STREAM` 默认是TCP，`SOCK_DGRAM` 默认是UDP

| 协议 | 描述 |
| --   | --  |
| `IPPROTO_IP`  | IPv4 网际协议 |
| `IPPROTO_IPV6`| IPv6 网际协议 |
| `IPPROTO_ICMP`| 因特网控制报文协议 |
| `IPPROTO_RAW` | 原始IP数据包协议 |
| `IPPROTO_TCP` | 传输控制协议 |
| `IPPROTO_UDP` | 用户数据报协议 |

```c
#include <sys/socket.h>

int shutdown(int sockfd, int how);
// 成功返回0，否则返回-1
```

| how | 描述|
| --  | --  |
| `SHUT_RD` | 关闭读端 |
| `SHUT_WR` | 关闭写端 |
| `SHUT_RDW`| 关闭读写 |

shutdown 与 close 的区别：只有最后一个活动引用关闭时，close 才释放网络端点。

## 寻址

目标通信进程：网络地址，端口号

### 字节序

TCP/IP 协议栈使用大端序

```c
#include <arpa/inet.h>

uint32_t htonl(uint32_t hostint32);
// 返回以网络字节序表示的32位整数
uint16_t htons(uint16_t hostint16);
// 返回以网络字节序表示的16位整数
uint32_t ntohl(uint32_t netint32);
// 返回以主机字节序表示的32位整数
uint16_t ntohs(uint16_t netint16);
// 返回以主机字节序表示的16位整数
```

确认主机序是大端还是小端

```c
# define IS_SMALL_ENDIAN() ((union {uint16_t u16; uint8_t c;}){.u16 = 1}.c)
```

### 地址格式

地址格式与通信域有关，为使不同的格式地址能传入套接字函数，会将其强制转换为通用的地址结构 sockaddr

```c
struct sockaddr {
    sa_family_t sa_family;  /* address family */
    char sa_data[];         /* variable-length address */
    ...
};
```

`AF_INET` 套接字地址用 `sockaddr_in` 表示

```c
#include <netinet/in.h>

struct in_addr {
    in_addr_t       s_addr;        /* IPv4 address */
};

struct sockaddr_in {
    sa_family_t    sin_family;    /* address family */
    in_port_t      sin_port;      /* port number */
    struct in_addr sin_addr;      /* IPv4 address */
};

// in_addr_t => uint32_t
// in_port_t => uint16_t
```

地址格式转换

```c
#include <arpa/inet.h>
const char *inet_ntop(int domain, const void *restrict addr,
    char *restrict str, socklen_t size);
// 若成功，返回地址字符串指针，否则为NULL

int inet_pton(int domain, const char *restrict str,
    void *restrict addr);
// 若成功返回1；格式无效返回0；出错返回-1
```

```c
#include <arpa/inet.h>
#include <stdio.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
    printf("INET_ADDRSTRLEN = %d\n", INET_ADDRSTRLEN);

    uint32_t addr;
    char str[INET_ADDRSTRLEN] = "0.0.0.1";
    int ret = inet_pton(AF_INET, str, &addr);
    printf("ret = %d, addr = %d\n", ret, ntohl(addr));

    uint32_t addr2 = htonl(1);
    char str2[INET_ADDRSTRLEN] = { 0 };
    inet_ntop(AF_INET, &addr2, str2, sizeof(str2));
    printf("ip is %s\n", str2);
}
```

```
INET_ADDRSTRLEN = 16
ret = 1, addr = 1
ip is 0.0.0.1
```

### 地址查询

```c
#include <netdb.h>
struct hostent *gethostent(void);
// 若成功返回指针，否则返回NULL
void sethostent(int stayopen);
void endhostent(void);

struct hostent {
    char   *h_name;       /* name of host */
    char  **h_aliases;    /* pointer to alternate host name array */
    int     h_addrtype;   /* address type */
    int     h_length;     /* length in bytes of address */
    char  **h_addr_list;  /* pointer to array of network addresses */
    ...
};

struct netent *getnetbyaddr(uint32_t net, int type);
struct netent *getnetbyname(const char *name);
struct netent *getnetent(void);
// 若成功返回指针，否则返回NULL
void setnetent(int stayopen);
void endnetent(void);

struct netent {
    char     *n_name;      /* network name */
    char    **n_aliases;   /* alternate network name array pointer */
    int       n_addrtype;  /* address type */
    uint32_t  n_net;       /* network number */
    ...
};

struct protoent *getprotobyname(const char *name);
struct protoent *getprotobynumber(int proto);
struct protoent *getprotoent(void);
// 若成功返回指针，否则返回NULL
void setprotoent(int stayopen);
void endprotoent(void);

struct protoent {
    char   *p_name;     /* protocol name */
    char  **p_aliases;  /* pointer to alternate protocol name array */
    int     p_proto;    /* protocol number */
    ...
};

struct servent *getservbyname(const char *name, const char *proto);
struct servent *getservbyport(int port, const char *proto);
struct servent *getservent(void);
// 若成功返回指针，否则返回NULL
void setservent(int stayopen);
void endservent(void);

struct servent {
    char   *s_name;     /* service name */
    char  **s_aliases;  /* pointer to alternate service name array */
    int     s_port;     /* port number */
    char   *s_proto;    /* name of protocol */
    ...
};

#include <sys/socket.h>
#include <netdb.h>
int getaddrinfo(const char *restrict host, const char *restrict service,
    const struct addrinfo *restrict hint, struct addrinfo **restrict res);
// 若成功返回，否则返回非0错误码
void freeaddrinfo(struct addrinfo *ai);

struct addrinfo {
    int ai_flags;               /* customize behavior */
    int ai_family;              /* address family */
    int ai_socktype;            /* socket type */
    int ai_protocol;            /* protocol */
    socklen_t ai_addrlen;       /* length in bytes of address */
    struct sockaddr *ai_addr;   /* address */
    char *ai_canonname;         /* canonical name of host */
    struct addrinfo  *ai_next;  /* next in list */
    ...
};

const char *gai_strerror(interror);

int getnameinfo(const struct sockaddr *restrict addr,
    socklen_t alen, char *restrict host, socklen_t hostlen,
    char *restrict service, socklen_t servlen, int flags);
// 若成功返回，否则返回非0错误码
```

### 将套接字与地址关联

```c
#include <sys/socket.h>
int bind(int sockfd, const struct sockaddr *addr, socklen_tlen);
// 若成功返回0；否则返回-1
```

* 服务器使用 bind 函数关联地址和套接字
* 在服务器上，地址必须有效，不能是其他机器的地址
* 地址必须和创建套接字时的地址族所支持的格式相匹配
* root 才能使用小于1024 的端口
* 英特网域IP 如果为 `INADDR_ANY`(定义在`<netinet/in.h>`)表示可以被绑定到所有的系统网络接口上
* 如果调用`connect` 或 `listen`而没有将地址绑定到套接字上，系统会选一个地址绑定到套接字上

```c
#include <sys/socket.h>
int getsockname(int sockfd, struct sockaddr *restrict addr,
    socklen_t *restrict alenp);

int getpeername(int sockfd, struct sockaddr *restrict addr,
    socklen_t *restrict alenp);

// 若成功返回0；否则返回-1
```

## 建立连接

对于面向连接的网络服务(`SOCK_STREAM` 和 `SOCK_SEQPACKET`)，交换数据前需要先建立连接

```c
#include <sys/socket.h>

int connect(int sockfd, const struct sockaddr *addr, socklen_t len);
int listen(int sockfd, int backlog);

// 若成功返回0；否则返回-1
```

如果是无连接的网络服务(`SOCK_DGRAM`)，调用`connect`, 会设置传送报文的目标地址，传送报文时不再需要提供地址，但仅能接收来自指定地址的报文

服务器调用`listen` 表示愿意接受连接请求，参数backlog 提示系统该进程所要入队的未完成连接请求数量

服务器调用`listen` 后就可以使用`accept` 获得连接请求并建立连接，返回的套接字连接到调用connect 函数的客户端

```c
#include <sys/socket.h>
int accept(int sockfd, struct sockaddr *restrict addr, socklen_t *restrict len);
// 若成功返回socket，否则返回-1
```

如果没有请求在连接，accept 会阻塞直到请求到来。如果sockfd 处于非阻塞状态，返回-1

## 数据传输

除了read, write 函数，还可以用以下函数进行数据传输

```c
#include <sys/socket.h>

ssize_t send(int sockfd, const void *buf, size_t nbytes, int flags);
ssize_t sendto(int sockfd, const void *buf, size_t nbytes, int flags, const struct sockaddr *destaddr, socklen_t destlen);
ssize_t sendmsg(int sockfd, const struct msghdr *msg, int flags);
// 若成功返回发送的字节数，若失败返回-1

ssize_t recv(int sockfd, void *buf, size_t nbytes, int flags);
ssize_t recvfrom(int sockfd, void *restrict buf, size_t len, int flags, struct sockaddr *restrict addr, socklen_t *restrict addrlen);
ssize_t recvmsg(int sockfd, struct msghdr *msg, int flags);
// 返回数据的字节长度；若无可用数据或对等方已按序结束，返回0；出错返回-1

struct msghdr {
    void *msg_name;             /* optional address */
    socklen_t msg_namelen;      /* address size in bytes */
    struct iovec *msg_iov;      /* array of I/O buffers */
    int msg_iovlen;             /* number of elements in array */
    void *msg_control;          /* ancillary data */
    socklen_t msg_controllen;   /* number of ancillary bytes */
    int msg_flags;              /* flags for received message */
    ...
};
```

`sendto, recvfrom` 可用于无连接的套接字，对于面向连接的套接字，地址是被忽略的

send 函数的参数 flag

| flag | 描述 |
|--  | --  |
| `MSG_CONFIRM` | 提供链路层反馈以保持地址映射有效 |
| `MSG_DONTROUT`| 勿将数据包路由出本地网络 |
| `MSG_DONTWAIT`| 允许非阻塞操作 |
| `MSG_EOF  `   | 发送数据后关闭套接字的发送端 |
| `MSG_EOR  `   | 如果协议支持，标记记录结束 |
| `MSG_MORE `   | 延迟发送数据包允许写更多数据 |
| `MSG_NOSIGNAL`| 在写无连接的套接字时不产生SIGPIPE 信号 |
| `MSG_OOB  `   | 如果协议支持，发送带外数据 |

recv 函数的参数 flag

| flag | 描述 |
|--  | --  |
| `MSG_CMSG_CLOEXEC`| 为UNIX域套接字上接收的文件描述符设置执行时关闭标志 |
| `MSG_DONTWAIT `   | 启用非阻塞操作 |
| `MSG_ERRQUEUE `   | 接收错误信息作为辅助数据 |
| `MSG_OOB      `   | 如果协议支持，发送带外数据 |
| `MSG_PEEK     `   | 返回数据包内容而不真正取走数据包 |
| `MSG_TRUNC    `   | 即使数据包被截断也返回数据包的实际长度 |
| `MSG_WAITALL  `   | 等待直到所有的数据可用(仅`SOCK_STREAM`) |
