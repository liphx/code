# head, tail

```shell
$ head files            # 默认输出前10行
$ head -n num files     # 输出前num行，如果num为负，则输出除最后num行外的所有行
$ head -c num files     # 同上，单位是字节
$ head -v files         # 总是打印文件名
$ head -q files         # 不打印文件名
$ cat /proc/xxxx/environ | head -z -n 5 # 可以接受标准输入，-z 以 NUL 而非换行符分隔

$ tail files            # 默认输出最后10行，可用于查看日志文件
$ tail -n num files     # -n 3与 -n -3相同，输出最后3行，如果num以 + 开头，输出 num 到结尾的所有行
$ tail -c num files     # 同上，单位是字节
$ ping huawei.com > log.txt &
$ tail log.txt -f -s 2  # -f 随文件增长不断查看; -s num 在 -f 参数下每num秒查看一次，默认是1.0s
$ head -v -q -z         # 同 head
```
