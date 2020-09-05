# 调试

```
set -x：在执行时显示参数和命令。
set +x：禁止调试。
set -v：当命令进行读取时显示输入。
set +v：禁止打印输入。
```

使用选项-x，启用shell脚本的跟踪调试功能：`$ bash -x script.sh`

使用set -x和set +x对脚本进行部分调试

```shell
#!/bin/bash
for i in {1..6}; 
do     
    set -x     
    echo $i     
    set +x 
done
```

把shebang从#!/bin/bash改成#!/bin/bash -xv，这样一来，不用任何其他选项就可以启用调试功能了

如果使用了-x -v或set -x -v，调试输出会被发送到stderr
