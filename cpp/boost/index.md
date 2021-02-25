# boost

<https://www.boost.org>

```shell
$ wget https://dl.bintray.com/boostorg/release/1.75.0/source/boost_1_75_0.tar.gz
$ tar xvf boost_1_75_0.tar.gz
$ cd boost_1_75_0/
$ ./bootstrap.sh
$ sudo ./b2 install
```

* lib dir: `/usr/local/lib`
* include dir: `/usr/local/include/boost`

首次安装后需要执行 `sudo ldconfig`