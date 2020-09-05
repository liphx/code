#!/bin/bash
# 执行./build.sh > README.md 更新README

echo '# Bash学习笔记

参考

<https://www.gnu.org/software/bash/manual/>  
<https://manpages.debian.org/>  
<https://www.kernel.org/doc/man-pages/>'

dirs="syntax file builtin CLI"
for dir in $dirs;
do
    cd $dir
    title=
    file=
    case $dir in
        CLI)
        title='## 一天一个命令行'
        file='.md'
        ;;
        file)
        title='## 定制Bash'
        ;;
        syntax)
        title='## Bash语法'
        file='.md'
        ;;
        builtin)
        title="## 内置命令"
        file='.md'
        ;;
    esac

    printf "\n%s\n\n" "$title"
    ls -A | sed "s/\(.*\)${file}/[\1](${dir}\/\1${file})  /g"
    cd ..
done
