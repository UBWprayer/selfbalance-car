#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "systick.h"

void SysTick_Init(void)
{
	/* SystemFrequency / 100     10ms中断一次
	   SystemFrequency / 1000    1ms中断一次
	 * SystemFrequency / 100000	 10us中断一次
	 * SystemFrequency / 1000000 1us中断一次
	 */
	SystemCoreClockUpdate();

	if (SysTick_Config(SystemCoreClock / 1000))	// ST3.5.0库版本
	{ 
		/* Capture error */ 
		while (1);
	}
	// 开启滴答定时器  
	SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;
}
