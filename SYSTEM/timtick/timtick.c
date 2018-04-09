#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "timtick.h"
#include "contro.h"

void TimTick_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  	//TIM3ʱ��ʹ�� 
	
	TIM_TimeBaseStructure.TIM_Prescaler=8000;  //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=1000;   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);//��ʼ����ʱ��2
	
	TIM_ARRPreloadConfig(TIM2,ENABLE);//ARPEʹ��
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn; //��ʱ��3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x00; //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); //����ʱ��3�����ж�
	
	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIM8
}

void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)!=RESET) //����ж�
	{
		GetMotorPulse();
	}
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);  //����жϱ�־λ
}
