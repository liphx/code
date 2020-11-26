# set

```shell
set [--abefhkmnptuvxBCEHPT] [-o option-name] [argument …]
set [+abefhkmnptuvxBCEHPT] [+o option-name] [argument …]
```

如果不指定参数，`set`会打印出所有变量与函数

选项

```
-e  如果命令退出状态非0，立即退出shell
-x  执行命令前先打印（先扩展变量`PS4`后打印命令）
-u  视未设置的变量和参数（特殊参数@或*除外）为错误，在标准错误写入错误信息，并退出非交互运行的shell
-o
    pipefail  只要管道中有任何一个命令返回非0，则整个管道操作被视为失败
```