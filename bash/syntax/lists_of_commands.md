# Lists of Commands

```shell
cmd1 && cmd2    # 当且仅当cmd1退出状态为0才执行cmd2
cmd1 || cmd2    # 当且仅当cmd1退出状态不为0才执行cmd2
cmd1 ; cmd2     # 按顺序执行cmd1，cmd2
cmd &           # 后台异步执行cmd
```

优先级 && 和 || 相同，优于优先级相同的 ; 和 & 
