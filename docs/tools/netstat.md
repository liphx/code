```shell
$ netstat
Command 'netstat' not found, but can be installed with:
sudo apt install net-tools
```

## netstat -i

```shell
$ netstat -i
Kernel Interface table
Iface      MTU    RX-OK RX-ERR RX-DRP RX-OVR    TX-OK TX-ERR TX-DRP TX-OVR Flg
enp0s3    1500     4450      0      0 0          3164      0      0      0 BMRU
lo       65536      202      0      0 0           202      0      0      0 LRU
```

获取网络接口

- lo(loopback) 环回接口
- enp0s3
- eth0

## netstat -r

```shell
$ netstat -rn
Kernel IP routing table
Destination     Gateway         Genmask         Flags   MSS Window  irtt Iface
0.0.0.0         10.0.2.2        0.0.0.0         UG        0 0          0 enp0s3
10.0.2.0        0.0.0.0         255.255.255.0   U         0 0          0 enp0s3
10.0.2.2        0.0.0.0         255.255.255.255 UH        0 0          0 enp0s3
192.168.1.1     10.0.2.2        255.255.255.255 UGH       0 0          0 enp0s3
```

-r 展示路由表

-n 输出数值地址，不反向解析成名字

