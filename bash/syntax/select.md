# select

`select name [in words …]; do commands; done`

如果省略`in words`，则会指定`in $@`

创建菜单供用户选择

```shell
echo "select your linux distribution"
linux="Debian Centos Ubuntu Arch"
select os in $linux:
do 
    echo "your select is ${REPLY}, $os is the best!"
    break;
done
```

```shell
$ bash select.sh
select your linux distribution
1) Debian
2) Centos
3) Ubuntu
4) Arch:
#? 1
your select is 1, Debian is the best!
```
