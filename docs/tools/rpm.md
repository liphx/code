# rpm打包示例

## step 1，创建目录

```shell
BUILD_DIR=~/tmp
echo "%_topdir $BUILD_DIR/rpmbuild" > ~/.rpmmacros

rm -rf "$BUILD_DIR"/rpmbuild
mkdir -p "$BUILD_DIR"/rpmbuild
pushd "$BUILD_DIR"/rpmbuild
    mkdir -pv {BUILD,BUILDROOT,RPMS,SOURCES,SPECS,SRPMS}
popd
```

## step2，打包源代码

将源代码打包成`hello-1.0.tar.gz`并复制到 SOURCES 目录

`hello.c`

```c
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    printf("hello");
    while (*++argv)
        printf(" %s", *argv);
    printf("\n");
    exit(0);
}
```

`Makefile`

```makefile
prefix ?= /usr/local

hello:hello.c
	gcc hello.c -o hello

install:
	mkdir $(PREFIX)/bin -p
	cp hello $(PREFIX)/bin/hello
```

```shell
tar -zcf hello-1.0.tar.gz hello-1.0
cp hello-1.0.tar.gz "${BUILD_DIR}/rpmbuild/SOURCES"
```

## step3，创建 spec 文件

`hello.spec`

```
%define name            hello
%define release         1
%define version         1.0
%define buildroot       %{_topdir}/BUILDROOT
%define debug_package   %{nil}

BuildRoot:              %{buildroot}
Summary:                hello
License:                GPL
Name:                   %{name}
Version:                %{version}
Release:                %{release}
Source:                 %{name}-%{version}.tar.gz
Prefix:                 /usr/local

%description
hello rpm

%prep
%setup -q

%build
make

%install
make install PREFIX=%{buildroot}%{prefix}

%clean
rm -rf %{buildroot}

%files
%defattr(-,root,root)
/usr/local/bin/hello
```

## step4，构建RPM

```shell
cp hello.spec "${BUILD_DIR}/rpmbuild/SPECS"
rpmbuild -bb "${BUILD_DIR}/rpmbuild/SPECS/hello.spec"
```

## 查询rpm

```shell
$ rpm -qpi ~/tmp/rpmbuild/RPMS/x86_64/hello-1.0-1.x86_64.rpm
Name        : hello
Version     : 1.0
Release     : 1
Architecture: x86_64
Install Date: (not installed)
Group       : Unspecified
Size        : 8592
License     : GPL
Signature   : (none)
Source RPM  : hello-1.0-1.src.rpm
Build Date  : Tuesday, August 18, 2020 PM03:18:37 CST
Build Host  : River
Relocations : /usr/local
Summary     : hello
Description :
hello rpm
```