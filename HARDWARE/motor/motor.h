#ifndef _MOTOR_H
#define _MOTOR_H

#include "sys.h"

extern long cnt1,cnt2;

void motor_init(void);
void Encoder_Init(void);
void Motor_Speed(int lspeed,int rspeed);

#endif
