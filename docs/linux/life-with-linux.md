# Linux 是一种生活态度

## X Window System

## X server & X Client

## X Display Manager

* gdm3 - GNOME Display Manager
* sddm - Simple Desktop Display Manager
* kdm - KDE Display manager
* lightdm - a display manager

```shell
$ cat /etc/X11/default-display-manager
/usr/sbin/gdm3
$ sudo dpkg-reconfigure lightdm
```

## Desktop Environment

* gnome
* kdm
* xfce
* lxde

## 使用Linux的一些原则

+ 能不用root身份执行的命令尽量不用root
+ 不要执行任何不熟悉的命令
+ 文件命名推荐只使用使用字符的一个子集 `a-z, A-Z, 0-9, ., -, _`
+ 尊重其他用户的隐私（无论你是否以root身份登录）

## 免密登录ssh

把本地的`ssh公钥`，比如文件`~/.ssh/id_rsa.pub`，追加写入服务器的`~/.ssh/authorized_keys`

## 免密sudo

```shell
sudo echo 'your_name ALL=(ALL:ALL) NOPASSWD:ALL' >> /etc/sudoers
```

## 文件/etc/motd

`motd`是`message of the day`的缩写，当我们用`ssh`登录服务器的时候就会显示这个文件中的内容

## 工具

文本对比

diff colordiff

## 限制通过ssh 登录 root

1. 编辑 `/etc/ssh/sshd_config`
2. 设置 `PermitRootLogin no`
3. `service ssh restart`
