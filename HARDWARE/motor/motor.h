#ifndef _MOTOR_H
#define _MOTOR_H

void motor_init(void);
void TIM2_Encoder_Init(void);
void TIM4_Encoder_Init(void);
void GetMotorPulse(void);

extern int g_LeftMotorPulse;
extern int g_RightMotorPulse;

#endif
