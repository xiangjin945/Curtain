/*
 * @Author: i4season.xiang
 * @Date: 2021-01-09 15:55:28
 * @LastEditTime: 2021-03-29 16:05:58
 * @FilePath: \新建文件夹c:\Users\24379\Desktop\smart_curtain\v1.1.5-tuya_3.5\example\IAP\IAP_Text\Source\inc\tst_function.h
 */
/* Define to prevent recursive inclusion -------------------------------------------------------------------*/
#ifndef __TEST_FUNCTION_H
#define __TEST_FUNCTION_H

/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32.h"
#include "ht32_board.h"
#include "ht32_board_config.h"
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
#include "service_logic_manage.h"
#include "mcu_api.h"
#include "protocol.h"
#include "stdbool.h"

extern u16 motor_current;
extern u8 ty_control;
extern u8 key_up_flag;
extern volatile u8 run_state;
extern u8 lift_key;
extern u8 right_key ;
extern volatile u8 crc_flag;
extern bool CalibrMotor;

void update_motor_current(u32 current_data);
void update_protect_current(u32 current_data);
void gptm0_test(void);
void motor_current_test(void);
void rtc_test(void);
void Timing_task_program(void);
void motor_turn(void);
void motor_10_test(void);
void calibration_test(void);
void tybn1_test(void);
void set_motor_current_78(u8 *data_buf);
void global_arg_fun(void);
extern void update_battery_value(u8 current_data);
void arg_erase_write(void);
void mute_erase_write(void);
void CurtainModeEraseWrite(void);
void CountDownEraseWrite(void);
void CountDownCurtainEraseWrite(void);

void Percentage_control(void);
void full_running_time_fun(void);
void resume_factory_up(void);//重置模组 04

#endif /* __TEST_FUNCTION_H */
