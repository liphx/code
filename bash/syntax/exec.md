exec命令创建全新的文件描述符

```
$ exec 3<input.txt    #使用文件描述符3打开并读取文件
$ exec 4>output.txt   #打开文件进行写入
$ exec 5>>input.txt 
```