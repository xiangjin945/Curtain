/* Define to prevent recursive inclusion -------------------------------------------------------------------*/
#ifndef __LOW_POWER_INIT_H
#define __LOW_POWER_INIT_H

/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32f5xxxx_usart.h"
#include "ht32f5xxxx_gpio.h"
#include "ht32.h"
#include "ht32_board.h"
#include <stdio.h>
#include <string.h>
#include "ht32f5xxxx_pwrcu.h"
#include "ht32f5xxxx_01.h"
#include "usart0_init.h"
#include "rtc_init.h"

extern vu32 guKeyState[5];
extern u32  low_power_event_flag;

void low_power_init(void);
void key_exti_init(void);

#endif		/* __LOW_POWER_INIT_H */
