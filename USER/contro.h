#ifndef _CONTRO_H
#define _CONTRO_H

#include "sys.h"

extern unsigned long g_u8MainEventCount;

extern short  g_s16LeftMotorPulse;
extern short  g_s16RightMotorPulse;
extern int  g_s32LeftMotorPulseSigma;
extern int  g_s32RightMotorPulseSigma;

void GetMotorPulse(void);
void AngleControl(void);

#endif
