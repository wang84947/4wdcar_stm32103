/**
* @par Copyright (C): 2010-2019, Shenzhen Yahboom Tech
* @file         app_motor.c
* @author       liusen
* @version      V1.0
* @date         2015.01.03
* @brief        小车运动控制函数
* @details      
* @par History  见如下说明
*                 
* version:	liusen_20170717
*/
#include "app_motor.h"
#include <rtthread.h>
#include <rtdevice.h>
#include <drv_gpio.h>



#define Motor_RCC		 RCC_APB2Periph_GPIOB
#define Motor_Port		 GPIOB
#define Left_MotoA_Pin 	 GPIO_Pin_9
#define Left_MotoB_Pin 	 GPIO_Pin_8

#define Right_MotoA_Pin	 GPIO_Pin_4
#define Right_MotoB_Pin  GPIO_Pin_5


/*#define SetBits(Motor_Port, Left_MotoA_Pin)    rt_pin_write(GET_PIN(B, 9), PIN_HIGH) 
#define ResetBits(Motor_Port, Left_MotoA_Pin)  rt_pin_write(GET_PIN(B, 9), PIN_LOW)
#define SetBits(Motor_Port, Left_MotoB_Pin)    rt_pin_write(GET_PIN(B, 8), PIN_HIGH)
#define ResetBits(Motor_Port, Left_MotoB_Pin)  rt_pin_write(GET_PIN(B, 8), PIN_LOW)
#define SetBits(Motor_Port, Right_MotoA_Pin)   rt_pin_write(GET_PIN(B, 4), PIN_HIGH)
#define ResetBits(Motor_Port, Right_MotoA_Pin) rt_pin_write(GET_PIN(B, 4), PIN_LOW)
#define SetBits(Motor_Port, Right_MotoB_Pin)   rt_pin_write(GET_PIN(B, 5), PIN_HIGH)
#define ResetBits(Motor_Port, Right_MotoB_Pin) rt_pin_write(GET_PIN(B, 5), PIN_LOW)*/

#define  LeftMotor_Go()			{rt_pin_write(GET_PIN(B, 9), PIN_HIGH); rt_pin_write(GET_PIN(B, 8), PIN_LOW);}
#define  LeftMotor_Back()		{ rt_pin_write(GET_PIN(B, 9), PIN_LOW); rt_pin_write(GET_PIN(B, 8), PIN_HIGH);}
#define  LeftMotor_Stop()		{ rt_pin_write(GET_PIN(B, 9), PIN_LOW); rt_pin_write(GET_PIN(B, 8), PIN_LOW);}

#define  RightMotor_Go()		{rt_pin_write(GET_PIN(B, 4), PIN_HIGH); rt_pin_write(GET_PIN(B, 5), PIN_LOW);}
#define  RightMotor_Back()		{ rt_pin_write(GET_PIN(B, 4), PIN_LOW); rt_pin_write(GET_PIN(B, 5), PIN_HIGH);}
#define  RightMotor_Stop()		{ rt_pin_write(GET_PIN(B, 4), PIN_LOW); rt_pin_write(GET_PIN(B, 5), PIN_LOW);}

#define  LeftMotorPWM(Speed)		 rt_pwm_set((struct rt_device_pwm *)rt_device_find("PWM_DEV_NAME"), 1, 7200, Speed);
#define  RightMotorPWM(Speed)		 rt_pwm_set((struct rt_device_pwm *)rt_device_find("PWM_DEV_NAME"), 2, 7200, Speed);		



/**
* Function       Car_Run
* @author        liusen
* @date          2017.07.17    
* @brief         小车前进
* @param[in]     Speed  （0~7200） 速度范围
* @param[out]    void
* @retval        void
* @par History   无
*/

void Car_Run(int Speed)
{
	LeftMotor_Go();
	RightMotor_Go();
	LeftMotorPWM(Speed);		  
	RightMotorPWM(Speed);	
}

/**
* Function       Car_Back
* @author        liusen
* @date          2017.07.17    
* @brief         小车后退
* @param[in]     Speed  （0~7200） 速度范围
* @param[out]    void
* @retval        void
* @par History   无
*/

void Car_Back(int Speed)
{
	LeftMotor_Back();
	RightMotor_Back();

	LeftMotorPWM(Speed);		  
	RightMotorPWM(Speed);	
}

/**
* Function       Car_Left
* @author        liusen
* @date          2017.07.17    
* @brief         小车左转
* @param[in]     Speed  （0~7200） 速度范围
* @param[out]    void
* @retval        void
* @par History   无
*/

void Car_Left(int Speed)
{
	LeftMotor_Stop();
	RightMotor_Go();

	LeftMotorPWM(0);		  
	RightMotorPWM(Speed);		
}

/**
* Function       Car_Right
* @author        liusen
* @date          2017.07.17    
* @brief         小车右转
* @param[in]     Speed  （0~7200） 速度范围
* @param[out]    void
* @retval        void
* @par History   无
*/

void Car_Right(int Speed)
{
	LeftMotor_Go();
	RightMotor_Stop();

	LeftMotorPWM(Speed);		  
	RightMotorPWM(0);		
}

/**
* Function       Car_Stop
* @author        liusen
* @date          2017.07.17    
* @brief         小车刹车
* @param[in]     void
* @param[out]    void
* @retval        void
* @par History   无
*/

void Car_Stop(void)
{
	LeftMotor_Stop();
	RightMotor_Stop();

	LeftMotorPWM(0);		  
	RightMotorPWM(0);		
}

/**
* Function       Car_SpinLeft
* @author        liusen
* @date          2017.07.17    
* @brief         小车左旋
* @param[in]     LeftSpeed：左电机速度  RightSpeed：右电机速度 取值范围：（0~7200）
* @param[out]    void
* @retval        void
* @par History   无
*/

void Car_SpinLeft(int LeftSpeed, int RightSpeed)
{
	LeftMotor_Back();
	RightMotor_Go();

	LeftMotorPWM(LeftSpeed);		  
	RightMotorPWM(RightSpeed);		
}

/**
* Function       Car_SpinRight
* @author        liusen
* @date          2017.07.17    
* @brief         小车右旋
* @param[in]     LeftSpeed：左电机速度  RightSpeed：右电机速度 取值范围：（0~7200）
* @param[out]    void
* @retval        void
* @par History   无
*/

void Car_SpinRight(int LeftSpeed, int RightSpeed)
{
	LeftMotor_Go();
	RightMotor_Back();

	LeftMotorPWM(LeftSpeed);		  
	RightMotorPWM(RightSpeed);		
}
