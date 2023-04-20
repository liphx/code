# TCP/UDP 基本概念

## TCP状态

建立连接:三路握手 (three-way handshake), 连接终止: 四次挥手

初始状态 CLOSED

服务器被动打开: socket, bind, listen

```c
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket failed");
        exit(1);
    }

    int port = 10000;
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind error");
        exit(1);
    }

    if (listen(sockfd, 16) < 0) {
        perror("listen error");
        exit(1);
    }

    for (;;) ;
}
```

此时server 状态: LISTEN

可以netstat -an 查看

```shell
$ netstat -an | grep 10000
tcp        0      0 0.0.0.0:10000           0.0.0.0:*               LISTEN
```

客户端调用connect, 若三路握手完成, 状态为 ESTABLISHED

若未完成,比如服务器listen 队列设置的比较小**且未accept**, 此时状态为 SYN_SENT, connect 阻塞,一段时间后返回失败, errno 设为 Connection timed out

```c
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket failed");
        exit(1);
    }

    int port = 10000;
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("connect failed");
        exit(1);
    }

    for (;;) ;
}
```

此时把linsten 第二个参数设为1, 再运行

```shell
$ ./server &
$ ./client &
$ netstat -anp | grep 10000
tcp        1      0 0.0.0.0:10000           0.0.0.0:*               LISTEN      11788/./server
tcp        0      0 127.0.0.1:52234         127.0.0.1:10000         ESTABLISHED 11840/./client
tcp        0      0 127.0.0.1:10000         127.0.0.1:52234         ESTABLISHED -
$ ./client &
$ netstat -anp | grep 10000
tcp        2      0 0.0.0.0:10000           0.0.0.0:*               LISTEN      11788/./server
tcp        0      0 127.0.0.1:52234         127.0.0.1:10000         ESTABLISHED 11840/./client
tcp        0      0 127.0.0.1:52242         127.0.0.1:10000         ESTABLISHED 11853/./client
tcp        0      0 127.0.0.1:10000         127.0.0.1:52242         ESTABLISHED -
tcp        0      0 127.0.0.1:10000         127.0.0.1:52234         ESTABLISHED -
$ ./client &
$ netstat -anp | grep 10000
tcp        2      0 0.0.0.0:10000           0.0.0.0:*               LISTEN      11788/./server
tcp        0      0 127.0.0.1:52234         127.0.0.1:10000         ESTABLISHED 11840/./client
tcp        0      0 127.0.0.1:52242         127.0.0.1:10000         ESTABLISHED 11853/./client
tcp        0      0 127.0.0.1:10000         127.0.0.1:52242         ESTABLISHED -
tcp        0      0 127.0.0.1:10000         127.0.0.1:52234         ESTABLISHED -
tcp        0      1 127.0.0.1:52244         127.0.0.1:10000         SYN_SENT    11862/./client
$ ... # 一段时间后
connect failed: Connection timed out

[4]+  退出 1                ./client
```

listen函数 backlog参数指定未完成连接队列的最大长度

SYN_SENT 状态下,客户端已发送SYN, 此时还没有接收SYN,ACK,发送ACK, 若后者完成则进入ESTABLISHED

在server 代码中加入accept, 可以建立更多的连接

```c
for (;;) {
    struct sockaddr_in client_addr;
    socklen_t len = sizeof(client_addr);
    int client_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &len);
    if (client_sockfd < 0) {
        continue;
    }
}
```

建立连接后就可以收发消息了,如果此时客户端关闭套接字(调用close 或退出进程),tcp 状态如下

```
tcp        0      0 0.0.0.0:10000           0.0.0.0:*               LISTEN      27948/./server
tcp        1      0 127.0.0.1:10000         127.0.0.1:54264         CLOSE_WAIT  27948/./server
tcp        0      0 127.0.0.1:54264         127.0.0.1:10000         FIN_WAIT2   -
```

可以看到多出了两个状态: FIN_WAIT2, CLOSE_WAIT

其中, 客户端调用close 后, 发送FIN, 达到FIN_WAIT1(上面没有这个状态), 接收到服务端的ACK 之后即达到FIN_WAIT2 状态,
FIN_WAIT2 状态下,收到服务器的FIN并发送ACK后即进入TIME_WAIT 状态,经历 2MSL 时间后回到CLOSED 状态

对于服务端, 在ESTABLISHED 状态下收到FIN 进入CLOSE_WAIT, 在调用close(服务端)后, 则发送FIN 进入LAST_ACK 状态,
收到客户端的ACK 回到CLOSED 状态

上面的程序中, 服务器只在循环里面accept, 没有`close(client_sockfd)`, 因此还维持在CLOSE_WAIT 状态

```c
tcp        0      0 0.0.0.0:10000           0.0.0.0:*               LISTEN      27948/./server
tcp        1      0 127.0.0.1:10000         127.0.0.1:54264         CLOSE_WAIT  27948/./server
```

这里的一个问题是客户端是如何从FIN_WAIT2 到 CLOSED (先存疑?)

现在,服务器改为如下代码, 客户端close 后,服务器也close socketfd, 由于这里我仅测试一个连接,因此没有使用多线程或epoll 来多路复用

```c
for (;;) {
    struct sockaddr_in client_addr;
    socklen_t len = sizeof(client_addr);
    int client_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &len);
    if (client_sockfd < 0) {
        continue;
    }
    for (;;) {
        char buf[1024];
        if (read(client_sockfd, buf, sizeof(buf)) == 0)
            break;
    }
    close(client_sockfd);
}
```

客户端退出后状态如下

```
tcp        0      0 0.0.0.0:10000           0.0.0.0:*               LISTEN      30209/./server
tcp        0      0 127.0.0.1:54660         127.0.0.1:10000         TIME_WAIT   -
```

由于服务端close 了, 客户端收到了FIN并发送ACK, 进入TIME_WAIT 状态, 在上一个例子中,如果直接kill server 进程, 状态也会从FIN_WAIT2
到 TIME_WAIT

到现在为止,我们看到了CLOSED, LISTEN, ESTABLISHED, SYN_SENT, FIN_WAIT2, CLOSE_WAIT, TIME_WAIT 状态

如期所述，FIN_WAIT1 是客户端调用close 发送FIN 后的状态，如果收到服务端ACK 到达FIN_WAIT2; 如果这时先收到服务器调用close 发送的FIN,
发送ACK后进入状态 CLOSING, 再收到ACK 进入TIME_WAIT, 这里的区别在于服务端close 的时机

另一种情况是，服务器的ACK 和 FIN 一并发送时，客户端发送ACK 后直接从FIN_WAIT1 到 TIME_WAIT

tcp 定义的11种状态中，还有一个 SYN_RCVD, UNP 中用如下图表示状态转换

