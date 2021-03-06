#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "motor.h"
#include "contro.h"
#include "timtick.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 

int main(void)
{ 
	int lspeed,rspeed;
	float pitch,roll,yaw; 		//欧拉角
	short aacx,aacy,aacz;		//加速度传感器原始数据
	short gyrox=0,gyroy=0,gyroz=0;	//陀螺仪原始数据 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);  //初始化延时函数
	uart_init(115200);		//初始化串口波特率为115200
	MPU_Init();					//初始化MPU6050
	motor_init();
	TimTick_Init();
	Encoder_Init();
	while(mpu_dmp_init())
	{
 		delay_ms(200);
	}
 	while(1)
	{
		AngleControl();
//		if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
//		{ 
//			MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//得到加速度传感器数据
//			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//得到陀螺仪数据
//			printf("Pitch: %f Roll: %f Yaw: %f\n",pitch,roll,yaw);
//			lspeed = rspeed = (int)(roll * 50 + gyrox * 0.3);
//			Motor_Speed(lspeed,rspeed);
//			delay_ms(10);
//			printf("accel: %d %d %d\n",aacx,aacy,aacz);
//			delay_ms(1000);
//			printf("gyro: %d %d %d\n",gyrox,gyroy,gyroz);
//			delay_ms(1000);
//			printf("Left: %d Right: %d\n",g_s16LeftMotorPulse,g_s16RightMotorPulse);
//			delay_ms(20);
//		}
	} 	
}
