# docker

## 命令

查看 Docker 信息

```shell
docker info
```

从 Docker 镜像仓库获取镜像

```shell
docker pull [选项] [Docker Registry 地址[:端口号]/]仓库名[:标签]
```

列出镜像

```shell
docker image ls
```

查看镜像、容器、数据卷所占用的空间

```shell
docker system df
```

删除本地镜像

```shell
docker image rm [选项] <镜像1> [<镜像2> ...]
```

其中，<镜像> 可以是 镜像短 ID、镜像长 ID、镜像名 或者 镜像摘要。

启动容器

```shell
docker run [选项] 镜像 [指令] [参数...]
docker exec [选项] 容器 [指令] [参数...]
```

选项：  
`-i` 交互模式，让容器的标准输入保持打开  
`-t` 让Docker分配一个伪终端（pseudo-tty）并绑定到容器的标准输入上  
`-d` 容器在后台运行，打印容器id

```shell
docker run -t -i ubuntu:18.04 /bin/bash # 启动一个 bash 终端，允许用户进行交互
docker exec -it my_container /bin/bash
```

终止容器

```shell
docker container ls # 列出容器
docker ps -a # 列出容器
docker container stop container # 终止容器
docker container start container # 重新启动处于终止状态的容器
docker container restart container # 将一个运行态的容器终止，然后再重新启动它
docker container rm container # 删除处于终止状态的容器
```
