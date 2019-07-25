/**
* @par Copyright (C): 2010-2019, Shenzhen Yahboom Tech
* @file         app_motor.h
* @author       liusen
* @version      V1.0
* @date         2015.01.03
* @brief        小车运动控制函数
* @details      
* @par History  见如下说明
*                 
* version:	liusen_20170717
*/

#ifndef __APP_MOTOR_H__
#define __APP_MOTOR_H__


//#include "bsp_motor.h"



void Car_Run(int Speed);
void Car_Back(int Speed);
void Car_Left(int Speed);
void Car_Right(int Speed);
void Car_Stop(void);
void Car_SpinStop(void);
void Car_SpinLeft(int LeftSpeed, int RightSpeed);
void Car_SpinRight(int LeftSpeed, int RightSpeed);




#endif


