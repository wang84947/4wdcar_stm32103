# 4wdcar_stm32103

亚博智能小车  raspi3b +   stm32   正点原子407  stm32 miniborad

https://www.yahboom.com/study  亚博智能小车  搭载rt-thread  just for fun and study

调试串口 ，串口1 作为finsh 串口3 作为蓝牙通讯接口 

#  智能战车 开发记录

## 硬件环境
亚博智能小车  正点原子探索者407 开发版  STM32 MINI borad
### 主要器件
- 底盘

- 电机

<!-- ![Alt text](/figures/motor.png) -->
<img src="/figures/motor.png" width="56%">

- 车轮

<!-- ![Alt text](/figures/wheel.png) -->
<img src="/figures/wheel.png" width="56%">

- 测速模块

<!-- ![Alt text](/figures/encoder.png) -->
<img src="/figures/encoder.png" width="56%">


- 电池

<!-- ![Alt text](/figures/battery.png) -->
<img src="/figures/battery.png" width="56%">

- 遥控器

<!-- ![Alt text](/figures/remote.png) -->
<img src="/figures/remote.png" width="56%">

- 控制板

<!-- ![Alt text](/figures/board.png) -->
<img src="/figures/board.png" width="56%">

### 清单

| 类别                   | 数量 |
| ---------------------- | ---- |
| 两驱三轮底盘(带电路)   | x1   |
| TT 直流减速电机        | x2   |
| 橡胶轮胎( TT 马达专用) | x4   |
| 万向轮                 | x1   |
| 光电编码器             | x2   |
| 码盘                   | x2   |
| 充电锂电池             | x2   |
| ps2 无线手柄           | x1   |
| Iot-board              | x1   |


### 软件清单
- git
- env 工具
- mdk5
- stm32CubeMX
- 串口工具

### 软件安装

#### git

获取 git 安装包: 官网下载链接: https://git-scm.com/downloads

进入后会看到如下页面

![Alt text](/figures/git_download1.png)

在 Downloads 下选择自己使用的系统点击进入，此处我们点击 Windows，会跳转到如下页面

![Alt text](/figures/git_download2.png)

在页面中选择合适的版本(64-bit or 32-bit)点击下载

下载完成后，运行安装包一路 next 即可。

#### env

首先下载 env，百度网盘: https://pan.baidu.com/s/1cg28rk#list/path=%2F&parentPath=%2Fsharelink4264488348-570001774340757。   
下载完成后解压到不含有中文字符或空格的目录。至此安装完成。

为了可以在文件夹中通过右键菜单打开 Env 控制台以方便开发 还需要进行一些操作。具体如何操作参考官网文档 [Env工具](https://www.rt-thread.org/document/site/programming-manual/env/env/)

#### mdk5

版本5.26。鉴于官网下载需要注册，较为麻烦，此处使用百度网盘下载

网盘链接: https://pan.baidu.com/s/1zUtqHq8hBdvf1z5S4ZO8Gg    
提取码: 33h8

下载完成后，运行安装包，一路 next 即可.

#### stm32CubeMX

同样鉴于官网下载需要注册，此处使用百度网盘下载

网盘链接: https://pan.baidu.com/s/1A-CTQDclfSc7qOj-yUKQ2g   
提取码: 2ftv

下载完成后，解压然后进入目录运行安装程序，之后 一路 next 即可.

#### 串口工具

该类工具较多，有多功能的 putty 等软件也有专注的串口助手工具sscom等。软件获取较为简单，此处不再介绍。

## 开发

### I. 建立工程

#### 获取 rt-thread

- 百度网盘

网盘链接: https://pan.baidu.com/s/1mgIAyWo#list/path=%2F

- git

先 clone 下来 rt-thread 仓库。      git 命令: "git clone [url]"     

然后拉取 lts-v3.1.x 分支。 git 命令: "git checkout -b [branch_name] origin/lts-v3.1.x"

![Alt text](/figures/git_clone_rtt.png)

![Alt text](/figures/git_branch_lts.png)

#### 生成独立工程

1。生成。进入到 "bsp/stm32/stm32l475-atk-pandora" 目录下，在该目录下打开 env 工具，执行命令: "scons --dist" 生成独立工程，生成后的工程在 "./dist" 目录中。(ps: 可随意拷贝该工程到任意目录下使用)

![Alt text](/figures/scons_dist.png)

![Alt text](/figures/dir_dist.png)

2。测试。在工程目录打开 env 工具，使用命令: "scons --target=mdk5" 生成 mdk5 工程。打开 mdk5 工程，编译并下载，可以观察到板载 led 闪烁 或 打开串口可发送命令交互。

### II. 外设配置

当前 bsp 驱动默认配置并不适用此项目，需要做修改。

#### bsp 驱动配置

使用 stm32CubeMX 进行配置。

- __pwm__  此处使用 TIM4：PB8，PB9 和 TIM2：PB10，PB11，配置完成后点击 GENERATE CODE 生成代码 (note: 如提示缺失 Firmware 要下载,点击取消然后点击 continue 只生成 SourceCode 即可)

![Alt text](/figures/cubemx.png)

注意事项：
- 所使用的 Timers 的 Clock Source 需要选取
- 默认开启的 TIM4 CH2 没有使用，手动关闭
- 生成代码时如提示缺失 Firware 要下载，点击No，然后在新弹框点击 Continue 只生成 source code 即可

![Alt text](/figures/cubemx_firware_miss.png)
![Alt text](/figures/cubemx_firware_miss_continue.png)

#### 修改 Kconfig

使用 stm32CubeMX 配置完成后需要对 "board/Kconfig" 做相应修改以便启用配置，修改完成后如下：

![Alt text](/figures/Kconfig.png)

### III. 应用开发

#### 1. todo

- 软件包: 使用命令 pkgs --update 更新软件包索引; 选中 rt-robots 软件包，并使能 pwm 外设; 生成mdk5工程。

    图

- 使用rt-robots软件包，确定引脚，进行小车初始化，编译下载测试

    ...

#### n. 待续

## 经验 or 注意事项 ?