# chattr, lsattr

```shell
$ sudo chattr +i file1
$ sudo rm file1 -f
rm: cannot remove 'file1': Operation not permitted
$ lsattr file1
----i----------- file1
$ sudo chattr -i file1
$ rm file1 -vf
removed 'file1'
```
