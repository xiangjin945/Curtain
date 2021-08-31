/* Define to prevent recursive inclusion -------------------------------------------------------------------*/
#ifndef __BFTM_INIT_H
#define __BFTM_INIT_H

/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32f5xxxx_gpio.h"
#include "ht32.h"
#include "ht32_board.h"
#include "gpio_init.h"
#include "uart0_init.h"
#include "usart0_init.h"

extern u32 bftm_cnt;
extern u8 bftm_second;
extern u8 bftm_minute;
extern u8 bftm_hour;

void bftm_onfiguration(void);

#endif /* __BFTM_INIT_H ------------------------------------------------------------------------------------*/
