#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "systick.h"

void SysTick_Init(void)
{
	/* SystemFrequency / 100     10ms�ж�һ��
	   SystemFrequency / 1000    1ms�ж�һ��
	 * SystemFrequency / 100000	 10us�ж�һ��
	 * SystemFrequency / 1000000 1us�ж�һ��
	 */
	SystemCoreClockUpdate();

	if (SysTick_Config(SystemCoreClock / 1000))	// ST3.5.0��汾
	{ 
		/* Capture error */ 
		while (1);
	}
	// �����δ�ʱ��  
	SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;
}
