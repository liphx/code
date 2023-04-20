```shell
$ ifconfig
enp0s3: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500
        inet 10.0.2.15  netmask 255.255.255.0  broadcast 10.0.2.255
        inet6 fe80::a00:27ff:fe94:e8c9  prefixlen 64  scopeid 0x20<link>
        ether 08:00:27:94:e8:c9  txqueuelen 1000  (Ethernet)
        RX packets 4776  bytes 2291302 (2.2 MB)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 3367  bytes 478232 (478.2 KB)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0

lo: flags=73<UP,LOOPBACK,RUNNING>  mtu 65536
        inet 127.0.0.1  netmask 255.0.0.0
        inet6 ::1  prefixlen 128  scopeid 0x10<host>
        loop  txqueuelen 1000  (Local Loopback)
        RX packets 206  bytes 17133 (17.1 KB)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 206  bytes 17133 (17.1 KB)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0

$ ifconfig enp0s3
enp0s3: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500
        inet 10.0.2.15  netmask 255.255.255.0  broadcast 10.0.2.255
        inet6 fe80::a00:27ff:fe94:e8c9  prefixlen 64  scopeid 0x20<link>
        ether 08:00:27:94:e8:c9  txqueuelen 1000  (Ethernet)
        RX packets 4805  bytes 2293404 (2.2 MB)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 3385  bytes 481348 (481.3 KB)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0
```