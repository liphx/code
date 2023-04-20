# apt

`apt`是`deb`系Linux发行版（Debian, Ubuntu, Linux Mint, Deepin等）使用的命令行软件包管理器

## 常用命令

```shell
sudo apt install build-essential python3-dev    # 安装软件，多个软件由空白符分隔
sudo apt purge vsftpd           # 清除除软件包和软件的配置文件
sudo apt remove nginx*          # 移除软件包
sudo apt autoremove             # 卸载所有自动安装且不再使用的软件包
sudo apt update                 # 更新可用软件包列表
sudo apt upgrade                # 安装/升级软件来更新系统
apt list nvidia-settings*       # 根据名称列出软件包
apt show tree                   # 显示软件包细节
apt search lua5.3               # 搜索软件包描述
sudo apt edit-sources           # 编辑软件源信息文件
sudo apt full-upgrade           # 安装已安装的软件包的候选版本，并且需要的话会移除其它的软件包
sudo apt clean                  # 完全清除本地仓库的软件包检索文件
sudo apt autoclean              # 清除本地仓库中过时软件包的软件包检索文件
```

## 软件包配置

```shell
$ grep "^deb" /etc/apt/sources.list
deb http://mirrors.huaweicloud.com/debian/ buster main contrib non-free
deb-src http://mirrors.huaweicloud.com/debian/ buster main contrib non-free #Added by software-properties
deb [arch=amd64] https://mirrors.ustc.edu.cn/docker-ce/linux/debian buster stable
```

文件 `/etc/apt/sources.list` 是包管理器apt的配置信息，目录 `/etc/apt/sources.list.d` 则提供了将配置信息保存在不同文件的机制

比如文件`/etc/apt/sources.list.d/steam.list` 保存了steam的相关软件的信息

```
deb [arch=amd64,i386] https://repo.steampowered.com/steam/ stable steam
deb-src [arch=amd64,i386] https://repo.steampowered.com/steam/ stable steam

# Uncomment these lines to try the beta version of the Steam launcher
# deb [arch=amd64,i386] https://repo.steampowered.com/steam/ beta steam
# deb-src [arch=amd64,i386] https://repo.steampowered.com/steam/ beta steam
```

## apt-get / apt-cache / aptitude

`apt-get`和`apt-cache`是最基础的基于APT的软件包管理工具，`apt`命令是一个用于软件包管理的高级命令行界面，基本上是 `apt-get`、`apt-cache`和类似命令的一个封装，被设计为针对终端用户交互的界面，它默认启用了某些适合交互式使用的选项。建议用户使用`apt`命令用于交互式的使用场景，而在 `shell` 脚本中使用 `apt-get`和`apt-cache`命令。而`aptitude`命令是最通用的基于APT的软件包管理工具，并且提供了一个全屏的交互式文本用户界面。

命令对照如下

```
apt update              aptitude update                 apt-get update
apt install             aptitude install                apt-get install
apt upgrade             aptitude safe-upgrade           apt-get upgrade
apt full-upgrade        aptitude full-upgrade           apt-get dist-upgrade
apt remove              aptitude remove                 apt-get remove
apt autoremove          N/A                             apt-get autoremove
apt purge               aptitude purge                  apt-get purge
apt clean               aptitude clean                  apt-get clean
apt autoclean           aptitude autoclean              apt-get autoclean
apt show                aptitude show                   apt-cache show
apt search              aptitude search                 apt-cache search
```
