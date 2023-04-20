# spec 换行符问题

最近把Windows上的文件上传到Linux上执行时报错

`$'\r': command not found`

于是果断加了个 `find . -name '*.sh' -exec dos2unix {} \;`

但是问题并没有解决，报错的地方似乎不在我的shell脚本里，而且这条语句是个空行，没法直接搜到出错的地方

后来发现我的spec文件里 `%prep` 部分是空的（语句被注释了），出于强迫症心理，我把 `%prep`移除了，然后发现除了`$'\r': command not found`的报错，还有这样一条

`mkdir ********\r`

看来问题出在spec文件，`dos2unix xxx.spec`，问题解决

总结，下次执行脚本遇到`\r` 问题，除了shell文件，还要注意下`spec` 文件。当然，生产环境是Linux就没这么多屁事了...
