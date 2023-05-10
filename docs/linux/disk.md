# Disk

## 硬盘(Hard Disk Drive, HDD, 机械硬盘)

硬盘一般用作电脑上的非易失性存储器，主要由盘片，磁头，盘片转轴，电动机等组成。一个盘片上下表面均有磁性材料（一个盘片有两个盘面），二进制位01即通过不同的模式存储在上面。
每个盘面有一个磁头，磁头贴近地悬浮在盘面上，主要用来读取和改变磁盘表面磁方向。
每个盘面上有多个磁道，它们是盘面上的同心圆，多个盘片处于同一半径圆的多个磁道组成的圆柱面称为柱面。
每个磁道分为多个弧段，称为扇区(Sector)。扇区是磁盘读写的基本单位。整个磁盘的第一个扇区称为引导扇区。
因为半径不同，不同磁道上的扇区数可能不同。

扇区大小可以通过 `fdisk -l` 查看，如以下输出表示扇区大小为 512 字节

```
Units: sectors of 1 * 512 = 512 bytes
Sector size (logical/physical): 512 bytes / 512 bytes
```

磁盘容量即扇区数量乘以扇区容量

### 数据接口

硬盘的数据接口大致分为 ATA(又称IDE), SATA, SCSI, SAS. 不同接口的传输速率不同

### 电源接口

- 3.5存台式机硬盘使用电源口供电，如 SATA 硬盘使用 SATA 电源线
- 2.5存笔记本硬盘使用数据口供电，不需要额外电源口
- 移动硬盘使用 USB 接口传输与供电

## 分区表

硬盘分区是指将硬盘划分为几个逻辑部分（不方便单独对扇区进行管理）

### MBR

MBR(Master Boot Record, 主引导记录) 在磁盘的第一个扇区(512 字节)，其组成(446 字节)如下

- boot loader code (440 字节，最大446)
- 选用磁盘标志 (4 字节)
- 0x0000 (2 字节空值)

MBR 之后是4个16字节的 DPT(磁盘分区表)与2字节的结束标记(55AA)

DPT 结构如下

| 偏移量 | 字节 | 定义 |
| -- | -- | -- |
| 0x00 | 1 | 分区状态: 非活动分区(0x00), 活动分区(0x80) |
| 0x01 | 1 | 分区起始磁头号 |
| 0x02 | 2 | 分区起始扇区号(5 bits) + 分区起始柱面号(11 bits) |
| 0x04 | 1 | 文件系统标识位, 如 0x83 表示 Linux 分区 |
| 0x05 | 1 | 分区结束磁头号 |
| 0x06 | 2 | 分区结束扇区号(5 bits) + 分区结束柱面号(11 bits) |
| 0x08 | 4 | 分区起始相对扇区号 |
| 0x0c | 4 | 分区总的扇区数 |

DPT 只能记录4个分区信息，且每个分区能表示的最大扇区数为(0xFFFFFFFF, 4 字节)，最多支持4个分区，每个分区最大2T 的空间

```
2TB = 2 ^ 32(Sectors) * 512(Bytes/Sector) / 1024(Bytes/KB) / 1024(KB/MB) / 1024(MB/GB) / 1024(GB/TB)
```

文件系统标识位为 0x05 表示扩展分区，扩展分区可以用类似 GPT 的方式描述逻辑分区，因此实际上可以分出任意多个逻辑分区

`fdisk -l` 输出信息 `Disklabel type: dos` 即为 MBR 分区表

`fdisk /dev/sdx` 中命令 `o` 用于创建 MBR 分区表

### GPT

GPT(GUID Partition Table, 全局唯一标识分区表)硬盘中，分区表的信息记录在GPT头中，出于兼容性考虑，硬盘的第一个扇区仍然用作MBR，之后才是GPT头

格式如下

- LBA 0(MBR), 兼容传统 MBR 结构，仅有一个分区，标识位为 0xEE, 标识这块硬盘使用 GPT 分区表
- LBA 1(分区表)

## 分区格式