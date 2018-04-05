#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "motor.h"
#include "contro.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 

unsigned char g_u8MainEventCount;

/******������Ʋ���******/
float g_fSpeedControlOut;
float g_fSpeedControlOutOld;
float g_fSpeedControlOutNew;
float g_fAngleControlOut;
float g_fLeftMotorOut;
float g_fRightMotorOut;
float g_fCarAngle;

/******�ٶȿ��Ʋ���******/

short  g_s16LeftMotorPulse;
short  g_s16RightMotorPulse;

int  g_s32LeftMotorPulseOld;
int  g_s32RightMotorPulseOld;
int  g_s32LeftMotorPulseSigma;
int  g_s32RightMotorPulseSigma;

float g_fCarSpeed;
float g_iCarSpeedSet;
float g_fCarSpeedOld;
float g_fCarPosition;

void GetMotorPulse(void)  //�ɼ�����ٶ�����
{ 
  g_s16LeftMotorPulse = TIM_GetCounter(TIM2);
  g_s16RightMotorPulse= TIM_GetCounter(TIM4);
  TIM2->CNT = 0;
  TIM4->CNT = 0;   //����
  g_s32LeftMotorPulseSigma +=  g_s16LeftMotorPulse;
  g_s32RightMotorPulseSigma += g_s16RightMotorPulse; 
}
