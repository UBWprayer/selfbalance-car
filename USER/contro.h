#ifndef _CONTRO_H
#define _CONTRO_H

#include "sys.h"

extern unsigned char g_u8MainEventCount;

extern short  g_s16LeftMotorPulse;
extern short  g_s16RightMotorPulse;

void GetMotorPulse(void);

#endif
