#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "motor.h"
#include "stm32f4xx_exti.h"

long cnt1,cnt2;

void motor_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  	//TIM3时钟使能 
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_TIM3); 
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource0,GPIO_AF_TIM3); 
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource1,GPIO_AF_TIM3); 
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_TIM3); 
	
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_7 | GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_Init(GPIOA,&GPIO_InitStructure);              //初始化PC7,8,9
	
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;      //推挽复用输出
	GPIO_Init(GPIOB,&GPIO_InitStructure);              //初始化PC7,8,9
	
	TIM_TimeBaseStructure.TIM_Prescaler=3;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=499;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);//初始化定时器8
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1OC1
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM8OC2
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM8OC3
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM8OC4
	
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM1在CCR1上的预装载寄存器
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM8在CCR2上的预装载寄存器
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM8在CCR3上的预装载寄存器
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM8在CCR4上的预装载寄存器
	
	TIM_ARRPreloadConfig(TIM3,ENABLE);//ARPE使能
	
	TIM_Cmd(TIM3, ENABLE);  //使能TIM8
	
	TIM_SetCompare1(TIM3,0);
	TIM_SetCompare2(TIM3,0);
	TIM_SetCompare3(TIM3,0);
	TIM_SetCompare4(TIM3,0);
}

void Motor_LeftSpeed(int speed)
{
	if(speed >= 0)
	{
		TIM_SetCompare1(TIM3,speed);
		TIM_SetCompare2(TIM3,0);
	}
	else
	{
		TIM_SetCompare1(TIM3,0);
		TIM_SetCompare2(TIM3,-speed);
	}
}

void Motor_RightSpeed(int speed)
{
	if(speed >= 0)
	{
		TIM_SetCompare3(TIM3,speed);
		TIM_SetCompare4(TIM3,0);
	}
	else
	{
		TIM_SetCompare3(TIM3,0);
		TIM_SetCompare4(TIM3,-speed);
	}
}

void Motor_Speed(int lspeed,int rspeed)
{
	Motor_LeftSpeed(lspeed);
	Motor_RightSpeed(rspeed);
}

void GPIO_Encoder_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); 	//使能PORTA时钟	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1; //GPIOA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; //下拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA0
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4; //GPIOA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; //下拉
	GPIO_Init(GPIOD,&GPIO_InitStructure); //初始化PA0
}



/*
	正交解码初始化(电机测速)
*/
void EXTI_Encoder_Init(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource1);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource3);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource4);
  
	/* 配置EXTI_Line0 */
  EXTI_InitStructure.EXTI_Line = EXTI_Line0 | EXTI_Line1;//LINE0
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //上升沿触发 
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能LINE0
  EXTI_Init(&EXTI_InitStructure);//配置
	
	/* 配置EXTI_Line0 */
  EXTI_InitStructure.EXTI_Line = EXTI_Line3 | EXTI_Line4;//LINE0
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //上升沿触发 
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能LINE0
  EXTI_Init(&EXTI_InitStructure);//配置
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void Encoder_Init(void)
{
	GPIO_Encoder_Init();
	EXTI_Encoder_Init();
}

void EXTI0_IRQHandler(void)
{ 		    		  			    
	if(EXTI_GetITStatus(EXTI_Line0) != RESET)
	{
		if(PAin(0) == 1)
		{
			if(PAin(1) == 0) cnt1 += 1;
			else cnt1 -= 1;
		}
		else
		{
			if(PAin(1) == 0) cnt1 -= 1;
			else cnt1 += 1;
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line0);
}

void EXTI1_IRQHandler(void)
{ 		    		  			    
	if(EXTI_GetITStatus(EXTI_Line1) != RESET)
	{
		if(PAin(1) == 1)
		{
			if(PAin(0) == 1) cnt1 += 1;
			else cnt1 -= 1;
		}
		else
		{
			if(PAin(0) == 1) cnt1 -= 1;
			else cnt1 += 1;
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line1);
}

void EXTI3_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line3) != RESET)
	{
		if(PDin(3) == 1)
		{
			if(PDin(4) == 0) cnt2 += 1;
			else cnt2 -= 1;
		}
		else
		{
			if(PDin(4) == 0) cnt2 -= 1;
			else cnt2 += 1;
		}
	}	
	EXTI_ClearITPendingBit(EXTI_Line3);
}
void EXTI4_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line4) != RESET)
	{
		if(PDin(4) == 1)
		{
			if(PDin(3) == 1) cnt2 += 1;
			else cnt2 -= 1;
		}
		else
		{
			if(PDin(3) == 1) cnt2 -= 1;
			else cnt2 += 1;
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line4);
}
