#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "motor.h"


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
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
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
	
	TIM_SetCompare1(TIM3,999);
	TIM_SetCompare2(TIM3,0);
	TIM_SetCompare3(TIM3,999);
	TIM_SetCompare4(TIM3,0);
}

void TIM2_Encoder_Init(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_ICInitTypeDef TIM_ICInitStructure;
  
/* Encoder unit connected to TIM2, 4X mode */    
  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* TIM2 clock source enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  /* Enable GPIOA, clock */
  
  GPIO_StructInit(&GPIO_InitStructure);
  /* Configure PA.00,01 as encoder input */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  /* Enable the TIM3 Update Interrupt */
  
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  /* Timer configuration in Encoder mode */
  TIM_DeInit(TIM2);
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  
  TIM_TimeBaseStructure.TIM_Prescaler = 0x0;  // No prescaling 
  TIM_TimeBaseStructure.TIM_Period = 0xFFFF;  
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  //设置时钟分频系数：不分频
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数模式 
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
	//TIM_ICPolarity_Rising上升沿捕获
  TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, 
                             TIM_ICPolarity_Rising, 
														 TIM_ICPolarity_Rising); 
  TIM_ICStructInit(&TIM_ICInitStructure);
  TIM_ICInitStructure.TIM_ICFilter = 6; //无滤波器
  TIM_ICInit(TIM2, &TIM_ICInitStructure);
  
 // Clear all pending interrupts
  TIM_ClearFlag(TIM2, TIM_FLAG_Update);
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);	  //使能中断
  
  TIM_Cmd(TIM2, ENABLE);  	   //使能定时器3
}



/*
	正交解码初始化(电机测速)
*/
void TIM4_Encoder_Init(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_ICInitTypeDef TIM_ICInitStructure;
  
/* Encoder unit connected to TIM3, 4X mode */    
  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* TIM3 clock source enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
  /* Enable GPIOA, clock */
  
  GPIO_StructInit(&GPIO_InitStructure);
  /* Configure PA.06,07 as encoder input */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  /* Enable the TIM3 Update Interrupt */
  
  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  /* Timer configuration in Encoder mode */
  TIM_DeInit(TIM4);
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  
  TIM_TimeBaseStructure.TIM_Prescaler = 0x0;  // TIM4时钟预分频值
  TIM_TimeBaseStructure.TIM_Period = 0xFFFF;  //设定计时器重装值
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  //设置时钟分频系数：不分频
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数模式 
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
 
  //TIM_ICPolarity_Rising上升沿捕获
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, 
                             TIM_ICPolarity_Rising, 
														 TIM_ICPolarity_Rising); 
  TIM_ICStructInit(&TIM_ICInitStructure);
  TIM_ICInitStructure.TIM_ICFilter = 6; //无滤波器
  TIM_ICInit(TIM4, &TIM_ICInitStructure);
  
 // Clear all pending interrupts
  TIM_ClearFlag(TIM4, TIM_FLAG_Update);	 //清除TIM4的更新标志位
  TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);	  //开启TIM4的更新标志位
  
  TIM_Cmd(TIM4, ENABLE);  	   //使能定时器3
}

void TIM2_IRQHandler(void)
{ 		    		  			    
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);	    
}
void TIM4_IRQHandler(void)
{ 		    		  			    
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);	    
}
