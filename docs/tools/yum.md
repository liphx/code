# yum

```
deplist        List a package's dependencies
info           Display details about a package or group of packages
install        Install a package or packages on your system
    --downloadonly --downloaddir=<dir>

```

```
rpm2cpio xxx.rpm | cpio -div
rpm -ql xxx | tar -T /dev/stdin -cf /dev/stdout | tar -xf /dev/stdin
rpm -qf /usr/include/tbb
yum provides xxx
```

