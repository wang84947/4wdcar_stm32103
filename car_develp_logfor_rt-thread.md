# 智能战车 开发记录

**_项目仓库_**

GitHub ：https://github.com/wang84947/4wdcar_stm32103

## 项目背景

- 做车起因

   Just for fun and study

- 起始点(个人相关技术能力或类似项目经历等)

  熟悉掌握 STM32 相关开发及基本操作：

  主要会keil 编译吧，debug调试吧，然后就是ST官网的stm32cube;

  env 构建代码https://www.rt-thread.org/document/site/tutorial/env-video/

  主要会配置界面                 menuconfig

  ​            软件包更新命令    pkgs --update

  ​			KEIL 工程生成命令 scons =mdk5

  就好了。

## 项目规划

实现蓝牙遥控，车辆前进、后退、左转、右转

### 计划/安排

购买硬件，

熟悉硬件，

github 上建立项目

软件开发

## 项目过程

### 项目环境准备

#### 小车硬件

- 材料清单（包含小车各‘组件’ 和 控制板等）

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
  | 蓝牙APP                | x1   |

  亚博智能小车  正点原子探索者407 开发版  STM32 MINI borad

  开发板图片：

  ![board](https://www.rt-thread.org/document/site/tutorial/quick-start/stm32f407-atk-explorer/figures/board.jpg)

  小车图片：

  ![1565092038441](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\1565092038441.png)

  ### 主要器件

  - 底盘+电机

  - ![1565092162410](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\1565092162410.png)

    

  - 车轮

  - ![1565092407808](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\1565092407808.png)

  - 测速模块

  - ![1565092562633](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\1565092562633.png)

  - 电池
  - ![1565100105597](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\1565100105597.png)
  - 控制板
  - ![1565100231907](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\1565100231907.png)

  ### 

  

  ### 蓝牙模块

  ![1565697267813](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\1565697267813.png)

- ### APP

- ![1565697309952](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\1565697309952.png)

- 组装过程 （经验 or 注意事项）

  组装的时候注意保存铜柱螺丝，

  四轮注意组装尽量平直，不然小车直行的时候没法走直线，会有所偏移。

#### 软件环境

- ### 软件清单

  - git
  - env 工具
  - mdk5
  - stm32CubeMX
  - 串口工具

  ### 软件安装

  #### git

  获取 git 安装包: 官网下载链接: https://git-scm.com/downloads

  在页面中选择合适的版本(64-bit or 32-bit)点击下载

  下载完成后，运行安装包一路 next 即可。

  具体可参考

  https://www.rt-thread.org/document/site/application-note/setup/git/an0015-rtthread-setup-git/#git_1

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
- ![1564303747071](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\1564303747071.png)
  - 复制STM32 raspi2 两个文件下的所有文件
- ![1564303821494](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\1564303821494.png)
  - 保留stm32下的如图文件
  
  #### 获取package 软件包 rt-robot
  
  右键运行ENV,运行menuconfig 

  ->![1564304281961](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\1564304281961.png)

  ### III. 应用开发

  - stm32 mini finsh 没调通，先调F4开发板的 树莓派的还不知道如何操作 先放着，
- 生成MDK 工程
  - https://github.com/wang84947/4wdcar_stm32103

  ## PWM调试驱动小车
  
  ###### 1介绍小车驱动原理

  ![1565101147351](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\1565101147351.png)

  我的驱动板是这样控制的 2个IO ，一个pwm 控制一个电机   

  ###### 2pwm组件 

  调用rt-thread 的PWM应用接口，需要开启PWM功能![1565101348784](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\1565101348784.png)

  

  

  ![1565101406945](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\1565101406945.png)

  ![1565101610043](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\1565101610043.png)

  

  ###### 3robot package APP 应用

  提供驱动小车输出pwm 和io的一系列应用

  chassis_update  

  ![1565101751207](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\1565101751207.png)

  

  ![1565101881226](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\1565101881226.png)

  chassis_update  调用 wheel_update 调用 motor_run,  motor_run  前又调用

   whl->rpm = encoder_measure_rpm(whl->w_encoder); 计算编码器  算出现在小车的轮速，后传入controller_update 进行PID算法算出理论输出pwm

  ```
controller_update(whl->w_controller, whl->rpm);
  ```

  ## 蓝牙模块串口接收APP命令
  
  ###### 1串口接收搭建

  直接上代码吧

  /*

  - Copyright (c) 2006-2018, RT-Thread Development Team
  *
  - SPDX-License-Identifier: Apache-2.0
  *
  - Change Logs:
  - Date           Author       Notes
  - 2018-08-15     misonyo      first implementation.
    */
    /*
  - 程序清单：这是一个 串口 设备使用例程
  - 例程导出了 uart_sample 命令到控制终端
  - 命令调用格式：uart_sample uart2
  - 命令解释：命令第二个参数是要使用的串口设备名称，为空则使用默认的串口设备
  - 程序功能：通过串口输出字符串"hello RT-Thread!"，然后错位输出输入的字符
    */
  
  #include <rtthread.h>
  #include "drv_usart.h"
  extern void Protocol();
#define SAMPLE_UART_NAME       "uart6"      /* 串口设备名称 */
  
  /* 用于接收消息的信号量 */
  static struct rt_semaphore rx_sem;
  static rt_device_t serial;

  /* 接收数据回调函数 */
  static rt_err_t uart_input(rt_device_t dev, rt_size_t size)
  {
    /* 串口接收到数据后产生中断，调用此回调函数，然后发送接收信号量 */
      rt_sem_release(&rx_sem);
  
  ```
  return RT_EOK;
  ```

  }
  
  unsigned char newLineReceived = 0;
unsigned char inputString[80] = {0};  
  unsigned int g_num = 0;
unsigned char  startBit = 0;
  int g_packnum = 0;
  static void serial_thread_entry(void *parameter)
  {
      char ch;
  unsigned char  uartvalue = 0;
      while (1)
      {
          /* 从串口读取一个字节的数据，没有读取到则等待接收信号量 */
          while (rt_device_read(serial, -1, &ch, 1) != 1)
          {
              /* 阻塞等待接收信号量，等到信号量后再次读取数据 */
              rt_sem_take(&rx_sem, RT_WAITING_FOREVER);
          }
          /* 读取到的数据通过串口错位输出 */
  				uartvalue=ch;
  			if(uartvalue == '$')
  	    {
  	      startBit = 1;
  		    g_num = 0;
  	    }
  	    if(startBit == 1)
  	    {
  	       	inputString[g_num] = uartvalue;     
  	    }  
  	    if (startBit == 1 && uartvalue == '#') 
  	    {	    	
  			newLineReceived = 1; 
  			startBit = 0;
  			g_packnum = g_num;	
  			//rt_kprintf("receive  %s \n",inputString);
  				Protocol();
  	    }
  			g_num++;
  			if(g_num >= 80)
  			{
  			g_num = 0;
  			startBit = 0;
  			newLineReceived	= 0;
  		  }	
  
  ​							
  ​	//rt_kprintf("receive a char %c \n",ch);
  ​	  //  ch = ch + 1;
​	   // rt_device_write(serial, 0, &ch, 1);
  ​	}
  }
  
  static int uart_sample(int argc, char *argv[])
  {
      rt_err_t ret = RT_EOK;
    char uart_name[RT_NAME_MAX];
      char str[] = "hello RT-Thread!\r\n";
  
  ```
  if (argc == 2)
  {
    rt_strncpy(uart_name, argv[1], RT_NAME_MAX);
  }
  else
  {
      rt_strncpy(uart_name, SAMPLE_UART_NAME, RT_NAME_MAX);
  }
  
  /* 查找串口设备 */
  serial = rt_device_find(uart_name);
  if (!serial)
  {
      rt_kprintf("find %s failed!\n", uart_name);
      return RT_ERROR;
  }
  	{
  		struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT;
  		config.baud_rate = BAUD_RATE_9600;
  		config.data_bits = DATA_BITS_8;
  		config.stop_bits = STOP_BITS_1;
  		config.parity = PARITY_NONE;
  		/* 打开设备后才可修改串口配置参数 */
  		rt_device_control(serial, RT_DEVICE_CTRL_CONFIG, &config);
  	}	
  /* 初始化信号量 */
  rt_sem_init(&rx_sem, "rx_sem", 0, RT_IPC_FLAG_FIFO);
  /* 以中断接收及轮询发送方式打开串口设备 */
  rt_device_open(serial, RT_DEVICE_FLAG_INT_RX);
  /* 设置接收回调函数 */
  rt_device_set_rx_indicate(serial, uart_input);
  /* 发送字符串 */
  rt_device_write(serial, 0, str, (sizeof(str) - 1));
  
  /* 创建 serial 线程 */
  rt_thread_t thread = rt_thread_create("serial", serial_thread_entry, RT_NULL, 1024, 25, 10);
  /* 创建成功则启动线程 */
  if (thread != RT_NULL)
  {
      rt_thread_startup(thread);
  }
  else
  {
      ret = RT_ERROR;
  }
  ```
  
  /*
  	rt_pin_mode(GET_PIN(B, 4), PIN_MODE_OUTPUT);
  	rt_pin_mode(GET_PIN(B, 5), PIN_MODE_OUTPUT);
	rt_pin_mode(GET_PIN(B, 8), PIN_MODE_OUTPUT);
  	rt_pin_mode(GET_PIN(B, 9), PIN_MODE_OUTPUT);
  */
      return ret;
  }
  /* 导出到 msh 命令列表中 */
  INIT_APP_EXPORT(uart_sample);
  //MSH_CMD_EXPORT(uart_sample, uart device sample);
  
  改造了下官方的uart_sample
  
  ![1565102325058](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\1565102325058.png)

  2 协议移植

  主要移植了 亚博官方的字符串报文解析，保留遥控部分 其余屏蔽掉

  /**

  - @par Copyright (C): 2010-2019, Shenzhen Yahboom Tech
- @file         protcol.c
  - @author       liusen
- @version      V1.0
  - @date         2017.08.18
  - @brief        协议解析总入口
  - @details      
  - @par History  见如下说明
  - 
  - version:	liusen_20170818
    */
  
  #include "protocol.h"
  #include "app_car.h"
  //#include "usart.h"
//#include "app_buzzer.h"
  //#include "bsp_servo.h"
  //#include "bsp_colorful.h"
  //#include "app_bluetooth.h"
  //#include "bsp_fire.h"
  #include <stdlib.h>
  #include <string.h>
  
  extern unsigned char newLineReceived ;
  extern unsigned char inputString[80] ;  
  extern unsigned int g_num;
extern unsigned char  startBit ;
  extern int g_packnum ;
  
  unsigned char ProtocolString[80] = {0};
  int CarSpeedControl = 2400;
  
/*小车运行状态枚举*/
  
  enum {

    enSTOP = 0,
  enRUN,
    enBACK,
  enLEFT,
    enRIGHT,
    enTLEFT,
    enTRIGHT
  
  }enCarState;
  
  #define 	run_car     '1'//按键前
#define 	back_car    '2'//按键后
  #define 	left_car    '3'//按键左
#define 	right_car   '4'//按键右
  #define 	stop_car    '0'//按键停
  
  int g_CarState = enSTOP; //  1前2后3左4右0停止
  
  /**

  - Function       StringFind
- @author        liusen
  - @date          2017.08.18    
- @brief         字符串查找
  - @param[in]     pSrc:源字符串; pDst:查找的字符串; v_iStartPos:源字符串起始位置
  - @param[out]    void
  - @retval        void
  - @par History   无
    */
  
  int StringFind(const char *pSrc, const char *pDst, int v_iStartPos)  
  {  
      int i, j;  
    for (i = v_iStartPos; pSrc[i]!='\0'; i++)  
      {  
          if(pSrc[i]!=pDst[0])  
              continue;         
          j = 0;  
          while(pDst[j] !='\0' && pSrc[i+j]!='\0')  
          {  
              j++;  
              if(pDst[j]!=pSrc[i+j])  
              break;  
          }  
          if(pDst[j]=='\0')  
              return i;  
      }  
      return -1;  
  } 
  
  /**
  
  - Function       ProtocolCpyData
- @author        liusen
  - @date          2017.08.18    
- @brief         串口数据拷贝到新的buf中防止处理过程中被新数据覆盖
  - @param[in]     void
  - @param[out]    void
  - @retval        void
  - @par History   无
    */
  
  /**
  
  - Function       Protocol
- @author        liusen
  - @date          2017.08.18    
- @brief         协议入口
  - @param[in]     void
  - @param[out]    void
  - @retval        void
  - @par History   无
    */
  
  int g_modeSelect = 0;  //0是默认APK上位机状态;  1:红外遥控 2:巡线模式 3:超声波避障 4: 七彩探照 5: 寻光模式 6: 红外跟踪
  unsigned char g_Boolfire = 0;	    //在灭火时关闭上报，由于上报数据会使灰度与灭火IO模式冲突。
  
void Protocol(void)
  {
  
  memcpy(ProtocolString, inputString, g_packnum + 1);
  	memset(inputString, 0x00, sizeof(inputString));
  
```
  /*解析模式切换*/
  ```

    	//先判断是否是模式选择	
  	if(StringFind((const char *)ProtocolString, (const char *)"MODE", 0) > 0 
  		&& StringFind((const char *)ProtocolString, (const char *)"4WD", 0) > 0)
	{
  		
  
  ```
      if (ProtocolString[10] == '0') //停止模式
      {
    Car_Stop();
  		 g_CarState = enSTOP;
  		 g_modeSelect = 0;
  		// BeepOnOffMode();
      }
      else
      {
  ```
  
  //						switch (ProtocolString[9])
  //					{
  //						case '0': g_modeSelect = 0; ModeBEEP(0); break;
//						case '1': g_modeSelect = 1; ModeBEEP(1); break;
  //						case '2': g_modeSelect = 2; ModeBEEP(2); break;
  //						case '3': g_modeSelect = 3; ModeBEEP(3); break;
  //						case '4': g_modeSelect = 4; ModeBEEP(4); break;
  //						case '5': g_modeSelect = 5; ModeBEEP(5); break;
  //						case '6': g_modeSelect = 6; ModeBEEP(6); break;
  //						default: g_modeSelect = 0; break;
  //					}
  //			  BeepOnOffMode();
  	    }
  	    newLineReceived = 0;  
  		  memset(ProtocolString, 0x00, sizeof(ProtocolString));
  	    return;
    	}
  
  /*
  	//解析上位机发来的舵机云台的控制指令并执行舵机旋转
  	//如:$4WD,PTZ180# 舵机转动到180度
	if (StringFind((const char *)ProtocolString, (const char *)"PTZ", 0) > 0)
  	{
  		int m_kp, i, ii;
  
  ```
  	i = StringFind((const char *)ProtocolString, (const char *)"PTZ", 0); //寻找以PTZ开头,#结束中间的字符
  	ii = StringFind((const char *)ProtocolString, (const char *)"#", i);
	if (ii > i)
  	{
  		char m_skp[5] = {0};
  		memcpy(m_skp, ProtocolString + i + 3, ii - i -3);
  		
  		m_kp = atoi(m_skp);        //将找到的字符串变成整型
  
  		Angle_J1 = (180 - m_kp);//转动到指定角度m_kp
  
  		newLineReceived = 0;  
  		memset(ProtocolString, 0x00, sizeof(ProtocolString));
  		return;
  	}
  ```
  
    	}
  	//解析上位机发来的七彩探照灯指令并点亮相应的颜色
  	//如:$4WD,CLR255,CLG0,CLB0# 七彩灯亮红色
	else if (StringFind((const char *)ProtocolString, (const char *)"CLR", 0) > 0)
  	{
  		int m_kp, i, ii, red, green, blue;
  		char m_skp[5] = {0};
  		
  
  ```
  	i = StringFind((const char *)ProtocolString, (const char *)"CLR", 0);
  	ii = StringFind((const char *)ProtocolString, (const char *)",", i);
	if (ii > i)
  	{			
  		memcpy(m_skp, ProtocolString + i + 3, ii - i -3);
  		m_kp = atoi(m_skp);
  		red =   m_kp;
  	}
  	i = StringFind((const char *)ProtocolString, (const char *)"CLG", 0);
  	ii = StringFind((const char *)ProtocolString, (const char *)",", i);
  	if (ii > i)
  	{
  		memcpy(m_skp, ProtocolString + i + 3, ii - i -3);
  		m_kp = atoi(m_skp);
  		green =   m_kp;
  	}
  	i = StringFind((const char *)ProtocolString, (const char *)"CLB", 0);
  	ii = StringFind((const char *)ProtocolString, (const char *)"#", i);
  	if (ii > i)
  	{
  		memcpy(m_skp, ProtocolString + i + 3, ii - i -3);
  		m_kp = atoi(m_skp);
  		blue =  m_kp;
  		bsp_Colorful_RGB_PWM(red, green, blue);//点亮相应颜色的灯
  		newLineReceived = 0;  
  		memset(ProtocolString, 0x00, sizeof(ProtocolString));
  		return;
  	}
  }
  
  */
  //解析上位机发来的通用协议指令,并执行相应的动作
  //如:$1,0,0,0,0,0,0,0,0,0#    小车前进
  if (StringFind((const char *)ProtocolString, (const char *)"4WD", 0) == -1)
  {
  	//小车原地左旋右旋判断
  	if (ProtocolString[3] == '1')      //小车原地左旋
  	{
  		g_CarState = enTLEFT;
  	}
  	else if (ProtocolString[3] == '2') //小车原地右旋
  	{
  		g_CarState = enTRIGHT;
  	}
  	else
  	{
  		g_CarState = enSTOP;
  	}
  
  	//小车鸣笛判断
  ```
  
  //		if (ProtocolString[5] == '1')     //鸣笛
  //		{
  //			whistle();
//		}
  
  ```
  	//小车加减速判断
  	if (ProtocolString[7] == '1')    //加速，每次加600
	{
  		CarSpeedControl += 600;
  		if (CarSpeedControl > 7200)
  		{
  			CarSpeedControl = 7200;
  		}
  	}
  	if (ProtocolString[7] == '2')	//减速，每次减600
  	{
  		CarSpeedControl -= 600;
  		if (CarSpeedControl < 3600)
  		{
  			CarSpeedControl = 3600;
  		}
  	}
  ```
  
  #if 0
  		//舵机左旋右旋判断
  		if (ProtocolString[9] == '1') //舵机旋转到180度
		{
  			Angle_J1 = 180;
  		}
  		if (ProtocolString[9] == '2') //舵机旋转到0度
  		{
  			Angle_J1 = 0;
  		}
  
  ```
  	//点灯判断
  	if (ProtocolString[13] == '1')//七彩灯亮白色
	{
  		bsp_Colorful_RGB_PWM(255, 255, 255);
  	}
  	else if (ProtocolString[13] == '2')//七彩灯亮红色
  	{
  		bsp_Colorful_RGB_PWM(255, 0, 0);
  	}
  	else if (ProtocolString[13] == '3')//七彩灯亮绿灯
  	{
  		bsp_Colorful_RGB_PWM(0, 255, 0);
  	}
  	else if (ProtocolString[13] == '4') //七彩灯亮蓝灯
  	{
  		bsp_Colorful_RGB_PWM(0, 0, 255);
  	}
  	else if (ProtocolString[13] == '5') //七彩灯亮青色
  	{
  		bsp_Colorful_RGB_PWM(0, 255, 255);
  	}
  	else if (ProtocolString[13] == '6') //七彩灯亮品红
  	{
  		bsp_Colorful_RGB_PWM(255, 0, 255);
  	}
  	else if (ProtocolString[13] == '7') //七彩灯亮黄色
  	{
  		bsp_Colorful_RGB_PWM(255, 255, 0);
  	}
  	else if (ProtocolString[13] == '8') //七彩灯灭
  	{
  		bsp_Colorful_RGB_PWM(0, 0, 0);
  	}
  
  	//灭火判断
  	if (ProtocolString[15] == '1')  //灭火
  	{
  		//digitalWrite(OutfirePin, !digitalRead(OutfirePin) );
  		Fire_Control(ON);
  		g_Boolfire = 1;
  	}
  	else if (ProtocolString[15] == '0')  //灭火
  	{
  		//digitalWrite(OutfirePin, !digitalRead(OutfirePin) );
  		Fire_Control(OFF);
  		g_Boolfire = 0;
  	}
  
  	//舵机归为判断
  	if (ProtocolString[17] == '1') //舵机旋转到90度
  	{
  		Angle_J1 = 90;
  	}
  ```
  
  #endif
  		//小车的前进,后退,左转,右转,停止动作
  		if (g_CarState != enTLEFT && g_CarState != enTRIGHT)
		{
  			switch (ProtocolString[1])
  			{
  				case run_car:   g_CarState = enRUN;  break;
  				case back_car:  g_CarState = enBACK;  break;
  				case left_car:  g_CarState = enLEFT;  break;
  				case right_car: g_CarState = enRIGHT;  break;
  				case stop_car:  g_CarState = enSTOP;  break;
  				default: g_CarState = enSTOP; break;
  			}
  		}
  
  ```
  	newLineReceived = 0;  
  	memset(ProtocolString, 0x00, sizeof(ProtocolString));  

  	//根据小车状态做相应的动作
  	switch (g_CarState)
  	{
  ```
  
  //			case enSTOP: Car_Stop(); break;
  //			case enRUN: Car_Run(CarSpeedControl); break;
  //			case enLEFT: Car_Left(CarSpeedControl); break;
//			case enRIGHT: Car_Right(CarSpeedControl); break;
  //			case enBACK: Car_Back(CarSpeedControl); break;
  //			case enTLEFT: Car_SpinLeft(CarSpeedControl, CarSpeedControl); break;
  //			case enTRIGHT: Car_SpinRight(CarSpeedControl, CarSpeedControl); break;
  //			default: Car_Stop(); break;
  		}
  	switch (g_CarState)
  		{
  			case enSTOP: car_stop(0,0); break;
  			case enRUN: car_forward(0,0); break;
  			case enLEFT: car_turnleft(0,0); break;
  			case enRIGHT: car_turnright(0,0); break;
  			case enBACK: car_backward(0,0); break;
  			case enTLEFT: Car_SpinLeft(0,0); break;
  			case enTRIGHT: Car_SpinRight(0,0); break;
  			default: car_stop(0,0); break;
  		}
  	}
  	
  
  }
  
  

  ## PID 调试

  ###### PID 原理

  ![1565102670405](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\1565102670405.png)

  

  

  

  ![1565102743887](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\1565102743887.png)

  ![1565102762479](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\1565102762479.png)

  

  代码和公式相对应p_error 对应KP增量，i_error对应积分增量，d_error 对应微分增量

  调试过程

  下图的波形是用那个VCAN工具画的

  ![1565102958520](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\1565102958520.png)明）



## 项目总结

##### 问题一  - 车子能前进，后退不能转弯

single_pwm_motor_set_speed(left_motor,100);
//single_pwm_motor_set_speed(right_motor,100);
这样车子不能动
//single_pwm_motor_set_speed(left_motor,100);
single_pwm_motor_set_speed(right_motor,100);
这样也不能动

**最终解决**：  电机控制 需要三个信号 两个IO 控制前进后退，一个PWM控制速度，我把A 轮的PWM 接到了B轮上。

##### 问题二 -  车子跑了一段时间突然左轮能动，右轮不能动了

**解决**： 我直接找到轮子的相应IO 和PWM 分别插到  VCC 和GND 排除硬件问题，后LED 测量IO输出  发现均正常，

后续排查发现一公一母头的排线，公头针比较细，排插孔容易松动。幸好驱动板 树莓派的接口是排针公头 ，找对相应的控制接口 ，让开发板控制这上面的IO,用两个母头的杜邦线连接。

##### PID 调试。

调试过程比较复杂，刚开始轮速总是突然加的很快，又突然停了。

多次尝试得出以下经验：

稳定编码器输出，采样周期过快，编码器的采样输出不稳定，输出都不稳定PID 的输出就更不稳定了

所以刻意调高了编码器采样，示波器打印出来的值还比较问题，然后PID 在根据这个稳定的值做调试，

调试过程发现，应该先调积分，它是不断做差值累计，KP和kd设置为0 ，你会发现输出是一条稳定上升的直线，

后跳KP，稍微加点KP值，输出会超调目标设定值， KD最后调不过值不用很大，网上说是超前调整。





