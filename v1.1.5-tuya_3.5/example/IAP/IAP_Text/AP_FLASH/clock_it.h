#ifndef __CLOCK_IT_
#define __CLOCK_IT_

#include "ht32.h"
#include "ht32_board.h"
#include "ht32_board_configs.h"
#include "gpio_init.h"
#include "uart0_init.h"
#include "usart0_init.h"
#include "string.h"
#include "motor_init.h"
#include "ht32_board_config.h"
#include "gptm0_init.h"
#include "tybn1_init.h"
#include "ht32f5xxxx_rstcu.h"
#include "rtc_init.h"
#include "systick_init.h"
#include "save_all_setting.h"
#include "adc_init.h"
#include "tst_function.h"
#include "bftm_init.h"
#include "low_power_iniit.h"

#include "stdio.h"
#include "ht32f5xxxx_01.h"
#include "protocol.h"
#include "mcu_api.h"
#include "system.h"
#include "ap_flash.h"


#define APP_NEW_FW_START_ADR (0x00007000) //(0x44000)
#define LIBCFG_FLASH_SIZE_AP2 (1024 * 58)
extern u8 AP_BIN_Buf[2480];
/*时钟的配置*/
void clock_config(void);



#endif // __CLOCK_IT_
