# du df

## du - disk usage 

`du [OPTION]... [FILE]...` 显示磁盘空间显示情况

du参数如果是目录会递归地显示子目录使用的磁盘空间

```shell
$ du file.txt -h
88K	file.txt
$ du $HOME -s -h
174G	/home/liph
```

GNU 选项
```
-a, --all
       显示对所有文件的统计，而不只是包含子目录。

-b, --bytes
       输出以字节为单位的大小，替代缺省时1024字节的计数单位。

--block-size=size
       输出以块为单位的大小，块的大小为 size 字节。( file- utils-4.0 的新选项)

-c, --total
       在处理完所有参数后给出所有这些参数的总计。这个选项被 用给出指定的一组文件或目录使用的空间的总和。

-D, --dereference-args
       引用命令行参数的符号连接。但不影响其他的符号连接。  这对找出象  /usr/tmp  这样的目录的磁盘使用量有用，  /usr/tmp  等通常是符号连接。  译住：例如在 /var/tmp 下建立一个目录test, 而/usr/tmp 是指向
       /var/tmp 的符号连接。du /usr/tmp 返回一项 /usr/tmp , 而 du - D /usr/tmp 返回两项 /usr/tmp，/usr/tmp/test。

--exclude=pattern
       在递归时，忽略与指定模式相匹配的文件或子目录。模式 可以是任何 Bourne shell 的文件 glob 模式。( file- utils-4.0 的新选项)

-h, --human-readable
       为每个数附加一个表示大小单位的字母，象用M表示二进制 的兆字节。

-H, --si
       与 -h 参数起同样的作用，只是使用法定的 SI 单位( 用 1000的幂而不是 1024 的幂，这样 M 代表的就是1000000 而不是 1048576)。(fileutils-4.0 的新选项)

-k, --kilobytes
       输出以1024字节为计数单位的大小。

-l, --count-links
       统计所有文件的大小，包括已经被统计过的(作为一个硬连接)。

-L, --dereference
       引用符号连接(不是显示连接点本身而是连接指向的文件或 目录所使用的磁盘空间)。

-m, --megabytes
       输出以兆字节的块为计数单位的大小(就是 1,048,576 字节)。

--max-depth=n
       只输出命令行参数的小于等于第 n 层的目录的总计。 --max-depth=0的作用同于-s选项。(fileutils-4.0的新选项)

-s, --summarize
       对每个参数只显示总和。

-S, --separate-dirs
       单独报告每一个目录的大小，不包括子目录的大小。

-x, --one-file-system
       忽略与被处理的参数不在同一个文件系统的目录。

-X file, --exclude-from=file
       除了从指定的文件中得到模式之外与 --exclude 一样。 模式以行的形式列出。如果指定的文件是'-',那么从标准输 入中读出模式。(fileutils-4.0 的新选项) GNU 标准选项

--help 在标准输出上输出帮助信息后正常退出。

--version
       在标准输出上输出版本信息后正常退出。

--     终结选项列表

```

## df - disk free

`df [OPTION]... [FILE]...` 显示磁盘可用空间信息

df 命令也可以使用目录作为参数。在这种情况下,会输出该目录所在分区的可用磁盘空间情况
```shell
$ df -h
文件系统        容量  已用  可用 已用% 挂载点
udev             16G     0   16G    0% /dev
tmpfs           3.2G  9.7M  3.2G    1% /run
/dev/sda2       143G  7.8G  128G    6% /
tmpfs            16G   62M   16G    1% /dev/shm
tmpfs           5.0M  4.0K  5.0M    1% /run/lock
tmpfs            16G     0   16G    0% /sys/fs/cgroup
/dev/sdb1       458G  174G  261G   41% /home
/dev/sda1       115M   32M   83M   28% /boot/efi
tmpfs           3.2G   16K  3.2G    1% /run/user/116
tmpfs           3.2G   44K  3.2G    1% /run/user/1000
/dev/sdc1       3.7T  3.5T  151G   96% /media/liph/data
$ df /home -h
文件系统        容量  已用  可用 已用% 挂载点
/dev/sdb1       458G  174G  261G   41% /home
```

GNU 选项

```
-a, --all
       列出包括BLOCK为0的文件系统

--block-size=SIZE use SIZE-byte blocks
       指定块的大小

-h,--huma-readable"
       用常见的格式显示出大小(例如:1K 234M 2G)

-H,--si"
       同上,但是这里的1k等于1000字节而不是1024字节

-i, --inodes
       用信息索引点代替块表示使用状况

-k, --kilobytes
       指定块大小等于1024字节来显示使用状况

-l, --local
       只显示本地文件系统使用状况

-m, --megabytes
       以指定块大小等于1048576字节(1M)来显示使用状况

--no-sync
       在取得使用信息前禁止调用同步 (default)

-P, --portability
       使用POSIX格式输出

--sync 在取得使用信息前调用同步

-t, --type=TYPE
       只显示指定类型(TYPE)的文件系统

-T, --print-type
       输出每个文件系统的类型

-x, --exclude-type=TYPE
       只显示指定类型(TYPE)之外的文件系统.

-v (忽略)

--     输出该命令的帮助信息并退出

--version
       输出版本信息并退出
```
