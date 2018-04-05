#ifndef _MOTOR_H
#define _MOTOR_H

#include "sys.h"

void motor_init(void);
void TIM2_Encoder_Init(void);
void TIM4_Encoder_Init(void);

extern u32	TIM2CH1_CAPTURE_VAL;
extern u32	TIM4CH1_CAPTURE_VAL;	//输入捕获值(TIM2/TIM5是32位)

#endif
