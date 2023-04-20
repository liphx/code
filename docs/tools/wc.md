# wc - word count

对每个文件输出行、单词和字节统计数，如果指定的文件多于一个，则同时输出总行数。单词指以空白字符分隔的长度非零的字符序列。
如果没有指定文件，或者指定文件为“-”，则从标准输入读取

```shell
$ wc /etc/passwd /etc/group # 依次输出行数，单词数，字节数，文件名
  40   67 2303 /etc/passwd
  69   69  978 /etc/group
 109  136 3281 总用量
$ cat 1.txt
204
520
812
人生不相见，动如参与商。
sudo i love u
$ cat 1.txt | wc -l # 换行符统计数，如果文件结尾不换行结果将比行数少1
5
$ cat 1.txt | wc -w # 单词统计数
8
$ cat 1.txt | wc -c # 字节统计数
63
$ cat 1.txt | wc -m # 字符统计数
39
$ cat 1.txt | wc -L # 最大显示宽度
24
```
