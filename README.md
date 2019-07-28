# 4wdcar_stm32103

亚博智能小车  raspi3b +   stm32   正点原子407  stm32 miniborad

https://www.yahboom.com/study  亚博智能小车  搭载rt-thread  just for fun and study

调试串口 ，串口1 作为finsh 串口3 作为蓝牙通讯接口 

# 目的

实现蓝牙遥控，车辆前进、后退、左转、右转，后续以太网远程finish控制

#  智能战车 开发记录

## 硬件环境
亚博智能小车  正点原子探索者407 开发版  STM32 MINI borad

开发板图片：

小车图片：

### 主要器件
- 底盘

- 电机

- 车轮

- 测速模块


- 电池

- 控制板

### 清单

| 类别                   | 数量 |
| ---------------------- | ---- |
| 两驱4轮底盘            | x1   |
| TT 直流减速电机        | x4   |
| 橡胶轮胎( TT 马达专用) | x4   |
| 驱动板                 | x1   |
| 光电编码器             | x2   |
| 码盘                   | x2   |
| 充电锂电池包           | x1   |
| 串口蓝牙模块           | x1   |
| APP                    | x1   |


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

xshell。

## 开发

### I. 建立工程

#### 获取 rt-thread

- https://github.com/RT-Thread/rt-thread  fork 官网全部文件
- ![1564303747071](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\1564303747071.png)复制STM32 raspi2 两个文件下的所有文件
- ![1564303821494](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\1564303821494.png)保留stm32下的如图文件
#### 获取package 软件包 rtrobt

右键运行ENV,运行menuconfig 

->![1564304281961](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\1564304281961.png)

### III. 应用开发

- stm32 mini finsh 没调通，先调F4开发板的 树莓派的还不知道如何操作 先放着，
- 生成MDK 工程
- https://github.com/wang84947/4wdcar_stm32103

## 经验 or 注意事项 ?

问题一  - 车子能前进，后退不能转弯，

single_pwm_motor_set_speed(left_motor,100);
//single_pwm_motor_set_speed(right_motor,100);
这样车子不能动
//single_pwm_motor_set_speed(left_motor,100);
single_pwm_motor_set_speed(right_motor,100);
这样也不能动

最终解决：  电机控制 需要三个信号 两个IO 控制前进后退，一个PWM控制速度，我把A 轮的PWM 接到了B轮上。

问题二 -  车子跑了一段时间突然左轮能动，右轮不能动了

解决： 我直接找到轮子的相应IO 和PWM 分别插到  VCC 和GND 排除硬件问题，后LED 测量IO输出  发现均正常，

后续排查发现一公一母头的排线，公头针比较细，排插孔容易松动。幸好驱动板 树莓派的接口是排针公头 ，找对相应的控制接口 ，让开发板控制这上面的IO,用两个母头的杜邦线连接





