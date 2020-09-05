# pipeline

```shell
cmd1 | cmd2     # cmd1的标准输出成为cmd2的标准输入
cmd1 |& cmd2    # cmd1的标准输出、标准错误成为cmd2的标准输入，类似于cmd1 2>&1 | cmd2
```

未开启`pipefail`选项下，管道的退出状态是最后一条命令的退出状态
