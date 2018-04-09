#include "sys.h"
#include "stdlib.h"
#include "delay.h"
#include "usart.h"
#include "motor.h"
#include "contro.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 

#define P 41
#define I 0
#define D 0.2

long OScount = 0;
int lspeed,rspeed;
int lspeedold = 0,rspeedold = 0;
long speedsigma = 0;
float pitch,roll,yaw; 		//欧拉角
float rollold = 0;
float smissroll = 0.1719;
float rollsigma = 0;
short gyrox=0,gyroy=0,gyroz=0;	//陀螺仪原始数据 
short gyroxold = 0;
short smissgyrox = -17;

/******速度控制参数******/

short  g_s16LeftMotorPulse;
short  g_s16RightMotorPulse;

int  g_s32LeftMotorPulseOld;
int  g_s32RightMotorPulseOld;
int  g_s32LeftMotorPulseSigma;
int  g_s32RightMotorPulseSigma;


void GetMotorPulse(void)  //采集电机速度脉冲
{ 
  g_s16LeftMotorPulse = -cnt1;
  g_s16RightMotorPulse= cnt2;
  cnt1 = 0;
  cnt2 = 0;   //清零
  g_s32LeftMotorPulseSigma +=  g_s16LeftMotorPulse;
  g_s32RightMotorPulseSigma += g_s16RightMotorPulse; 
}

void SpeedControl(int* ls,int* rs)
{
	*ls = *ls + (*ls - g_s16LeftMotorPulse);
	*rs = *ls + (*ls - g_s16RightMotorPulse) * 0.4;
	if(*ls > 499) *ls = 499;
	if(*ls < -499) *ls = -499;
	if(*rs > 499) *rs = 499;
	if(*rs < -499) *rs = -499;
	
	Motor_Speed(*ls,*rs);
}

void AngleControl(void)
{
	if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
	{
		MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//得到陀螺仪数据
		rollsigma = rollsigma * 0.7  + roll * 0.3;
//		if((speedsigma + (lspeed + rspeed)/2) / 2 < 499) speedsigma = (speedsigma + (lspeed + rspeed)/2)/2;
//		else 
//		{
//			speedsigma = 0;
//			smissroll = 0.179;
//		}
//		if(speedsigma > 400)
//		{
//			OScount++;
//			if(OScount >= 100000 && OScount <= 120000)
//			{
//				if(OScount % 5000 == 0) 
//				{
//					smissroll -= 0.0005;
//					rollsigma = 0;
//				}
//				lspeed = rspeed = (int)((roll - smissroll) * P + (gyrox - smissgyrox) * D + rollsigma * I +speedsigma * 0.3);
//			}
//		}
//		else if(speedsigma < -400)
//		{
//			OScount++;
//			if(OScount >= 100000 && OScount <= 120000)
//			{
//				if (OScount % 5000 == 0) 
//				{	
//					smissroll += 0.0005;
//					rollsigma = 0;
//				}
//				lspeed = rspeed = (int)((roll - smissroll) * P -+ (gyrox - smissgyrox) * D + rollsigma * I +speedsigma * 0.3);
//			}
//		}
//		else
//		{
			lspeed = rspeed = (int)((roll - smissroll) * P + (gyrox - smissgyrox) * D + rollsigma * I);
			OScount = 0;
//		}
		lspeedold = lspeed;
		rspeedold = rspeed;
		SpeedControl(&lspeed,&rspeed);
	}
}
