# file 

`file [OPTION...] [FILE...]` 确定文件类型

```shell
$ file /etc/passwd /home /bin/bash /dev/sda
/etc/passwd: ASCII text
/home:       directory
/bin/bash:   ELF 64-bit LSB pie executable, x86-64, version 1 (SYSV), dynamically linked, interpreter /lib64/ld-linux-x86-64.so.2, for GNU/Linux 3.2.0, BuildID[sha1]=ffe165dc81a64aea2b05beda07aeda8ad71f1e7c, stripped
/dev/sda:    block special (8/0)
```

选项

```
-b      不输出文件名 (简要模式).

-c      检查时打印输出幻数文件的解析结果.常与 -m 一起使用，用来在安装幻数文件之前调试它.

-f 命名文件
        从在参数表前的 命名文件 中读出将要检查的文件名(每行一个文件).要有 命名文件 ，或者至少有一个文件名参数; 如果要检查标准输入, 使用``-''作为文件参数.

-m list 指定包含幻数的文件列表.可以是单个文件，也可以是 用冒号分开的多个文件.

-n      每检查完一个文件就强制刷新标准输出. 仅在检查一组文件时才有效.  一般在将文件类型输出到管道时才采用此选项.

-v      打印程序版本并退出.

-z      试图查看压缩文件内部信息.

-L      (在支持符号链接的系统上)选项显示符号链接文件的原文件, 就像 ls(1) 命令的like-named 选项.

-s      通常,  file 只是试图去检查在文件列表中那些 stat(2) 报告为正常文件的文件的类型.由于读特殊文件将可能导致 不可知后果，所以这样可以防止发生问题.使用 -s 选项时 file 命令也将去读文件列表中的块特殊文
        件和字符特殊文件.  一般用于从原始磁盘分区中获得文件系统类型，此文件为块 特殊文件. 这个选项也导致 file 命令忽略 stat(2) 报告的文件大小，因为在有些系统中原始磁盘分区的大小报告为0.
```

文件 /usr/share/magic 为默认的幻数列表; 环境变量 MAGIC 用于设置默认的幻数文件.
