# docker

## docker pull

```
docker pull [选项] [Docker Registry 地址[:端口号]/]仓库名[:标签]
docker pull ubuntu:18.04
```

## docker run

```
docker run [选项] 镜像 [指令] [参数...]
docker run -it --rm ubuntu:18.04 bash

选项
`-i` 交互模式，让容器的标准输入保持打开
`-t` 让Docker分配一个伪终端（pseudo-tty）并绑定到容器的标准输入上
`-d` 容器在后台运行，打印容器id
`--rm` 容器退出后随之将其删除
```

## docker exec

```
docker exec [选项] 容器 [指令] [参数...]
```

## docker image

```
docker image ls
docker image prune
docker image rm
```

## docker system

```
docker system df # 查看镜像、容器、数据卷所占用的空间
```

## docker info

```
docker info
```

## docker container

```
docker container ls
docker container stop container # 终止容器
docker container start container # 重新启动处于终止状态的容器
docker container restart container # 将一个运行态的容器终止，然后再重新启动它
docker container rm container
```

## docker ps

```
docker ps -a
```

## docker login url/docker logout url

## Dockerfile

https://docs.docker.com/engine/reference/builder/
