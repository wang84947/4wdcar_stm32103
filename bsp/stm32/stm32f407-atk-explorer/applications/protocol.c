/**
* @par Copyright (C): 2010-2019, Shenzhen Yahboom Tech
* @file         protcol.c
* @author       liusen
* @version      V1.0
* @date         2017.08.18
* @brief        协议解析总入口
* @details      
* @par History  见如下说明
*                 
* version:	liusen_20170818
*/

#include "protocol.h"
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
* Function       StringFind
* @author        liusen
* @date          2017.08.18    
* @brief         字符串查找
* @param[in]     pSrc:源字符串; pDst:查找的字符串; v_iStartPos:源字符串起始位置
* @param[out]    void
* @retval        void
* @par History   无
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
* Function       ProtocolCpyData
* @author        liusen
* @date          2017.08.18    
* @brief         串口数据拷贝到新的buf中防止处理过程中被新数据覆盖
* @param[in]     void
* @param[out]    void
* @retval        void
* @par History   无
*/

/**
* Function       Protocol
* @author        liusen
* @date          2017.08.18    
* @brief         协议入口
* @param[in]     void
* @param[out]    void
* @retval        void
* @par History   无
*/

int g_modeSelect = 0;  //0是默认APK上位机状态;  1:红外遥控 2:巡线模式 3:超声波避障 4: 七彩探照 5: 寻光模式 6: 红外跟踪
unsigned char g_Boolfire = 0;	    //在灭火时关闭上报，由于上报数据会使灰度与灭火IO模式冲突。

void Protocol(void)
{

  memcpy(ProtocolString, inputString, g_packnum + 1);
	memset(inputString, 0x00, sizeof(inputString));

	/*解析模式切换*/
  	//先判断是否是模式选择	
	if(StringFind((const char *)ProtocolString, (const char *)"MODE", 0) > 0 
		&& StringFind((const char *)ProtocolString, (const char *)"4WD", 0) > 0)
	{
		
	    if (ProtocolString[10] == '0') //停止模式
	    {
	    Car_Stop();
			 g_CarState = enSTOP;
			 g_modeSelect = 0;
			// BeepOnOffMode();
	    }
	    else
	    {
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
  	}
	//解析上位机发来的七彩探照灯指令并点亮相应的颜色
	//如:$4WD,CLR255,CLG0,CLB0# 七彩灯亮红色
	else if (StringFind((const char *)ProtocolString, (const char *)"CLR", 0) > 0)
	{
		int m_kp, i, ii, red, green, blue;
		char m_skp[5] = {0};
		
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
//		if (ProtocolString[5] == '1')     //鸣笛
//		{
//			whistle();
//		}

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

		newLineReceived = 0;  
		memset(ProtocolString, 0x00, sizeof(ProtocolString));  

		//根据小车状态做相应的动作
		switch (g_CarState)
		{
			case enSTOP: Car_Stop(); break;
			case enRUN: Car_Run(CarSpeedControl); break;
			case enLEFT: Car_Left(CarSpeedControl); break;
			case enRIGHT: Car_Right(CarSpeedControl); break;
			case enBACK: Car_Back(CarSpeedControl); break;
			case enTLEFT: Car_SpinLeft(CarSpeedControl, CarSpeedControl); break;
			case enTRIGHT: Car_SpinRight(CarSpeedControl, CarSpeedControl); break;
			default: Car_Stop(); break;
		}
	}
	

}

