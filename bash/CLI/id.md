# id - print real and effective user and group IDs

`id [选项]... [用户]` 显示真实和有效用户ID及组ID

显示指定用户的用户和用户组信息，在没有给定用户信息的情况下显示当前用户的信息。

```shell
$ id 
uid=1000(liph) gid=1000(liph) 组=1000(liph),24(cdrom),25(floppy),29(audio),30(dip),44(video),46(plugdev),109(netdev),112(bluetooth),116(lpadmin),117(scanner)
$ id -u
1000
$ id -u --name
liph
$ id root
uid=0(root) gid=0(root) 组=0(root)
```

选项

```
-a     忽略，仅用于与其它版本兼容

-Z, --context
       只显示当前进程的安全上下文

-g, --group
       只显示有效组 ID

-G, --groups
       显示所有组 ID

-n, --name
       显示名称而非编号，与 -ugG 共同使用

-r, --real
       显示真实 ID 而非有效 ID，与 -ugG 共同使用

-u, --user
       只显示有效用户 ID

-z, --zero
       使用 NUL 字符分隔不同项，而不用空格；

       在默认格式情况下不允许使用

--help 显示此帮助信息并退出

--version
       显示版本信息并退出

如果没有指定任何选项，将会显示一些有用的身份信息。
```
