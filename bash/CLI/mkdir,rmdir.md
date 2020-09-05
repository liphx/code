# mkdir, rmdir - 创建与删除空目录

```shell
$ mkdir a           # 在当前目录下创建空目录a
$ mkdir b/c -p      # 创建目录b，b/c，-p即在需要时创建父目录
$ mkdir -m 777 d    # 指定访问权限
$ mkdir -v -p www/{html,img}    # 打印创建的目录信息
mkdir: created directory 'www'
mkdir: created directory 'www/html'
mkdir: created directory 'www/img'
$ rmdir a           # 删除一个空目录
$ rmdir b           # 非空目录会报错
rmdir: failed to remove 'b': Directory not empty
$ rmdir b/c -p -v   # -p，删除子目录使得父目录为空时，一并删除父目录
rmdir: removing directory, 'b/c'
rmdir: removing directory, 'b'
```
