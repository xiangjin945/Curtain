#include "systick_init.h"

static vu32 delay_time;

void ms_delay_systick(u32 us)
{
	/* Enable the SysTick Counter */
	SYSTICK_CounterCmd(SYSTICK_COUNTER_CLEAR);
	SYSTICK_CounterCmd(SYSTICK_COUNTER_ENABLE);

	delay_time = us;

	while(delay_time != 0);

	/* Disable SysTick Counter */
	SYSTICK_CounterCmd(SYSTICK_COUNTER_DISABLE);
	/* Clear SysTick Counter */
	SYSTICK_CounterCmd(SYSTICK_COUNTER_CLEAR);
}

void timing_delay(void)
{
	if(delay_time != 0)
	{
		delay_time--;
	}
}

void  systick_init(void)
{
	SYSTICK_ClockSourceConfig(SYSTICK_SRC_STCLK);       // Default : CK_AHB/8
	SYSTICK_SetReloadValue(SystemCoreClock / 8 / 1000); // (CK_AHB/8/1000) = 1ms on chip
	SYSTICK_IntConfig(ENABLE);                          // Enable SYSTICK Interrupt
}
