# netstat - 显示网络连接，路由表，接口状态，伪装连接，网络链路信息和组播成员组

```
netstat   [address_family_options]   [--tcp|-t]   [--udp|-u]   [--raw|-w]   [--listening|-l]   [--all|-a]   [--numeric|-n]  [--numeric-
hosts][--numeric-ports][--numeric-ports]  [--symbolic|-N]  [--extend|-e[--extend|-e]]   [--timers|-o]   [--program|-p]   [--verbose|-v]
[--continuous|-c] [delay]

netstat  {--route|-r}  [address_family_options]  [--extend|-e[--extend|-e]]  [--verbose|-v] [--numeric|-n] [--numeric-hosts][--numeric-
ports][--numeric-ports] [--continuous|-c] [delay]

netstat {--interfaces|-i}  [iface]  [--all|-a]  [--extend|-e[--extend|-e]]  [--verbose|-v]  [--program|-p]  [--numeric|-n]  [--numeric-
hosts][--numeric-ports][--numeric-ports] [--continuous|-c] [delay]

netstat {--groups|-g} [--numeric|-n] [--numeric-hosts][--numeric-ports][--numeric-ports] [--continuous|-c] [delay]

netstat {--masquerade|-M} [--extend|-e] [--numeric|-n] [--numeric-hosts][--numeric-ports][--numeric-ports] [--continuous|-c] [delay]

netstat {--statistics|-s} [--tcp|-t] [--udp|-u] [--raw|-w] [delay]

netstat {--version|-V}

netstat {--help|-h}

address_family_options:

[--protocol={inet,unix,ipx,ax25,netrom,ddp}[,...]]  [--unix|-x] [--inet|--ip] [--ax25] [--ipx] [--netrom] [--ddp]
```

```shell
$ netstat | head
Active Internet connections (w/o servers)
Proto Recv-Q Send-Q Local Address           Foreign Address         State      
tcp        0      0 river:32820             39.106.63.54:ssh        ESTABLISHED
tcp        1      0 river:51122             203.208.50.33:https     CLOSE_WAIT 
tcp        1      0 river:33854             39.106.63.54:http       CLOSE_WAIT 
tcp        1      0 river:53916             23.54.32.13.in-ad:https CLOSE_WAIT 
tcp        1      0 river:37684             203.208.50.65:https     CLOSE_WAIT 
tcp        0      0 river:60212             53.250.102.104.in-:http ESTABLISHED
Active UNIX domain sockets (w/o servers)
Proto RefCnt Flags       Type       State         I-Node   Path
$ netstat | grep ssh
tcp        0      0 river:32820             39.106.63.54:ssh        ESTABLISHED
$ netstat -r
Kernel IP routing table
Destination     Gateway         Genmask         Flags   MSS Window  irtt Iface
default         192.168.1.1     0.0.0.0         UG        0 0          0 eno1
192.168.1.0     0.0.0.0         255.255.255.0   U         0 0          0 eno1

```

选项

```
(none)
    无选项时, netstat 显示打开的套接字.  如果不指定任何地址族，那么打印出所有已配置地址族的有效套接字。

--route , -r
    显示内核路由表。

--groups , -g
    显示IPv4 和 IPv6的IGMP组播组成员关系信息。

--interface=iface , -i
    显示所有网络接口列表或者是指定的 iface 。

--masquerade , -M
    显示一份所有经伪装的会话列表。

--statistics , -s
    显示每种协议的统计信息。

--verbose , -v
    详细模式运行。特别是打印一些关于未配置地址族的有用信息。

--numeric , -n
    显示数字形式地址而不是去解析主机、端口或用户名。

--numeric-hosts
    显示数字形式的主机但是不影响端口或用户名的解析。

--numeric-ports
    显示数字端口号，但是不影响主机或用户名的解析。

--numeric-users
    显示数字的用户ID，但是不影响主机和端口名的解析。

--protocol=family , -A
    指定要显示哪些连接的地址族(也许在底层协议中可以更好地描述)。  family  以逗号分隔的地址族列表，比如  inet,  unix,  ipx, ax25, netrom, 和
    ddp。 这样和使用 --inet, --unix (-x), --ipx, --ax25, --netrom, 和 --ddp 选项效果相同。

    地址族 inet 包括raw, udp 和tcp 协议套接字。

-c, --continuous
    将使 netstat 不断地每秒输出所选的信息。

-e, --extend
    显示附加信息。使用这个选项两次来获得所有细节。

-o, --timers
    包含与网络定时器有关的信息。

-p, --program
    显示套接字所属进程的PID和名称。

-l, --listening
    只显示正在侦听的套接字(这是默认的选项)

-a, --all
    显示所有正在或不在侦听的套接字。加上 --interfaces 选项将显示没有标记的接口。

-F
    显示FIB中的路由信息。(这是默认的选项)
-C
    显示路由缓冲中的路由信息。

delay
    netstat将循环输出统计信息，每隔 delay 秒。

输出 OUTPUT
活动的Internet网络连接 (TCP, UDP, raw)
Proto
    套接字使用的协议。

Recv-Q
    连接此套接字的用户程序未拷贝的字节数。

Send-Q
    远程主机未确认的字节数。

Local Address
    套接字的本地地址(本地主机名)和端口号。除非给定-n --numeric (-n)  选项，否则套接字地址按标准主机名(FQDN)进行解析，而端口号则转换到相应的
    服务名。

Foreign Address
    套接字的远程地址(远程主机名)和端口号。 Analogous to "Local Address."

State
    套接字的状态。因为在RAW协议中没有状态，而且UDP也不用状态信息，所以此行留空。通常它为以下几个值之一：

    ESTABLISHED
           套接字有一个有效连接。

    SYN_SENT
           套接字尝试建立一个连接。

    SYN_RECV
           从网络上收到一个连接请求。

    FIN_WAIT1
           套接字已关闭，连接正在断开。

    FIN_WAIT2
           连接已关闭，套接字等待远程方中止。

    TIME_WAIT
           在关闭之后，套接字等待处理仍然在网络中的分组

    CLOSED 套接字未用。

    CLOSE_WAIT
           远程方已关闭，等待套接字关闭。

    LAST_ACK
           远程方中止，套接字已关闭。等待确认。

    LISTEN 套接字监听进来的连接。如果不设置 --listening (-l) 或者 --all (-a) 选项，将不显示出来这些连接。

    CLOSING
           套接字都已关闭，而还未把所有数据发出。

    UNKNOWN
           套接字状态未知。

User
    套接字属主的名称或UID。

PID/Program name
    以斜线分隔的处理套接字程序的PID及进程名。  --program 使此栏目被显示。你需要 superuser 权限来查看不是你拥有的套接字的信息。对IPX套接字还
    无法获得此信息。

活动的UNIX域套接字
Proto
    套接字所用的协议(通常是unix)。

RefCnt
    使用数量(也就是通过此套接字连接的进程数)。

Flags
    显示的标志为SO_ACCEPTON(显示为 ACC), SO_WAITDATA (W) 或 SO_NOSPACE (N)。 如果相应的进程等待一个连接请求，那么SO_ACCECPTON用于未连接的套
    接字。其它标志通常并不重要

Type
    套接字使用的一些类型：

    SOCK_DGRAM
           此套接字用于数据报(无连接)模式。

    SOCK_STREAM
           流模式(连接)套接字

    SOCK_RAW
           此套接字用于RAW模式。

    SOCK_RDM
           一种服务可靠性传递信息。

    SOCK_SEQPACKET
           连续分组套接字。

    SOCK_PACKET
           RAW接口使用套接字。

    UNKNOWN
           将来谁知道它的话将告诉我们，就填在这里 :-)

State
    此字段包含以下关键字之一：

    FREE   套接字未分配。

    LISTENING
           套接字正在监听一个连接请求。除非设置 --listening (-l) 或者 --all (-a) 选项，否则不显示。

    CONNECTING
           套接字正要建立连接。

    CONNECTED
           套接字已连接。

    DISCONNECTING
           套接字已断开。

    (empty)
           套接字未连。

    UNKNOWN
           ！不应当出现这种状态的。

PID/Program name
    处理此套接字的程序进程名和PID。上面关于活动的Internet连接的部分有更详细的信息。

Path
    当相应进程连入套接字时显示路径名。

活动的IPX套接字
    (this needs to be done by somebody who knows it)

Active NET/ROM sockets
    (this needs to be done by somebody who knows it)

Active AX.25 sockets
    (this needs to be done by somebody who knows it)

注意 NOTES
    从linux 2.2内核开始 netstat -i 不再显示别名接口的统计信息。要获得每个别名接口的计数器，则需要用 ipchains(8) 命令。

文件 FILES
    /etc/services -- 服务解释文件

    /proc -- proc文件系统的挂载点。proc文件系统通过下列文件给出了内核状态信息。

    /proc/net/dev -- 设备信息

    /proc/net/raw -- RAW套接字信息

    /proc/net/tcp -- TCP套接字信息

    /proc/net/udp -- UDP套接字信息

    /proc/net/igmp -- IGMP组播信息

    /proc/net/unix -- Unix域套接字信息

    /proc/net/ipx -- IPX套接字信息

    /proc/net/ax25 -- AX25套接字信息

    /proc/net/appletalk -- DDP(appletalk)套接字信息

    /proc/net/nr -- NET/ROM套接字信息

    /proc/net/route -- IP路由信息

    /proc/net/ax25_route -- AX25路由信息

    /proc/net/ipx_route -- IPX路由信息

    /proc/net/nr_nodes -- NET/ROM节点列表

    /proc/net/nr_neigh -- NET/ROM邻站

    /proc/net/ip_masquerade -- 伪装连接

    /proc/net/snmp -- 统计
```

