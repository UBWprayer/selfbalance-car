#include "sys.h"
#include "delay.h"
#include "usart.h"
//#include "motor.h"
//#include "contro.h"
//#include "mpu6050.h"
//#include "inv_mpu.h"
//#include "inv_mpu_dmp_motion_driver.h" 

int main(void)
{ 
//	float pitch,roll,yaw; 		//ŷ����
//	short aacx,aacy,aacz;		//���ٶȴ�����ԭʼ����
	short gyrox=0,gyroy=0,gyroz=0;	//������ԭʼ����
	u8 t;
	u8 len;	
	u16 times=0;  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);  //��ʼ����ʱ����
	uart_init(115200);		//��ʼ�����ڲ�����Ϊ115200
//	MPU_Init();					//��ʼ��MPU6050
//	motor_init();
//	TIM2_Encoder_Init();
//	TIM4_Encoder_Init();
//	while(mpu_dmp_init())
//	{
// 		delay_ms(200);
//	}
 	while(1)
	{
		printf("gyro: %d %d %d\n",gyrox,gyroy,gyroz);
			delay_ms(1000);
//		if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
//		{ 
//			MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//�õ����ٶȴ���������
//			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//�õ�����������
//			printf("Pitch: %f Roll: %f Yaw: %f\n",pitch,roll,yaw);
//			delay_ms(1000);
//			printf("accel: %d %d %d\n",aacx,aacy,aacz);
//			delay_ms(1000);
//			
//			//GetMotorPulse();
//			//printf("Left: %d Right: %d",g_s16LeftMotorPulse,g_s16RightMotorPulse);
//			delay_ms(1000);
//		}
	} 	
}
