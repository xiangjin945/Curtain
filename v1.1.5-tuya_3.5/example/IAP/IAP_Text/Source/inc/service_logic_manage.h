/*********************************************************************************************************//**
 * @file    pwm.h
 * @version $Rev:: 4849         $
 * @date    $Date:: 2020-08-04 #$
 * @brief   The header file of PWM.
 *************************************************************************************************************
 * @attention
 *
 * Firmware Disclaimer Information
 *
 * 1. The customer hereby acknowledges and agrees that the program technical documentation, including the
 *    code, which is supplied by Holtek Semiconductor Inc., (hereinafter referred to as "HOLTEK") is the
 *    proprietary and confidential intellectual property of HOLTEK, and is protected by copyright law and
 *    other intellectual property laws.
 *
 * 2. The customer hereby acknowledges and agrees that the program technical documentation, including the
 *    code, is confidential information belonging to HOLTEK, and must not be disclosed to any third parties
 *    other than HOLTEK and the customer.
 *
 * 3. The program technical documentation, including the code, is provided "as is" and for customer reference
 *    only. After delivery by HOLTEK, the customer shall use the program technical documentation, including
 *    the code, at their own risk. HOLTEK disclaims any expressed, implied or statutory warranties, including
 *    the warranties of merchantability, satisfactory quality and fitness for a particular purpose.
 *
 * <h2><center>Copyright (C) Holtek Semiconductor Inc. All rights reserved</center></h2>
 ************************************************************************************************************/
/* Define to prevent recursive inclusion -------------------------------------------------------------------*/
#ifndef __SYSTERM_LOGIC_MANAGE_H
#define __SYSTERM_LOGIC_MANAGE_H

#include "ht32f5xxxx_tm.h"
#include "ht32f5xxxx_gpio.h"
#include "ht32f52231_41_libcfg.h"

#include "rtc_init.h"
#include "board_config.h"
#include "tst_function.h"

#ifdef __cplusplus
 extern "C" {
#endif
#define FLASH_55K_CURRENT_PERCENT 		0xDC00
#define FLASH_55K_GLOBAL_DATA			0xDC04					//55K 存放全局配置信息
#define FLASH_55K_AUTO_MODE				0xDC08
#define FLASH_55K_CALIBRA_STA			0xDC0C	
#define FLASH_55K_LEFT_RIGHT_CURTAIN	0xDC10			//Store curtain mode, left or right curtain
#define FLASH_55K_RETURN_TIME			0xDC14

#define FLASH_55K_DOUNTDOWN_FLAG		0xDC18
#define FLASH_55K_DOUNTDOWN_SWITCH		0xDC1C


#define FLASH_62K_LICESEN				0xFD00



#define PRINTFINFO 0
typedef struct Global_variable{
	volatile u32 full_running_time; //1.19：Motor running time(unit:ms)
	volatile char global_percent; //current percentage ,default is 100%
	volatile u8 auto_run_mode;
	volatile u8 mute_mode;
	volatile u8 calibration_status;
	volatile u32 returntime;
	volatile u8 run_mode_flag;
}global_variable;

typedef struct Percentage_time{
	volatile u32 cnt_time; //1.19：Motor running time(unit:ms)
	volatile int runtime; //current percentage ,default is 100%
	volatile u8 cnt_flag;
	volatile u8 cnt_flag_on;
	volatile u8 cnt_flag_off;
	volatile u8 calibration_status;
}percentage_time;

//保护电流阈值
extern volatile u16 protect_current;
//窗帘模式
extern volatile u8 curtain_mode;
//校准状态
extern volatile u8 calibration_status;
//校准操作
extern volatile u8 calibration_operation;
//全程运行时间
// extern volatile u32 variable.full_running_time;
extern global_variable variable;

extern percentage_time ptime;
//校准操作 AutoMode
extern volatile u8 calibration_operation_auto;

extern u32 timer_wday;
extern u8 lssued_seconds;
extern u8 lssued_minutes;
extern u32 lssued_time;
extern u32 single_timing ;
extern u32 timer_buf[8][7];
extern u32 timer_buf_single[8];
extern u8 timer_hours;
extern u8 timer_minutes;

extern u32 timing_buffer[64];
extern u32 new_timing_buffer[56];
/*
enum week_model
{
	monday = 0x01,
	tuesday = 0x02,
	wednesday = 0x04,
	thursday = 0x08,
	friday = 0x10,
	saturday = 0x20,
	sunday = 0x40
};
*/
// extern u8 monday;
// extern u8 tuesday;
// extern u8 wednesday;
// extern u8 thursday;
// extern u8 friday;
// extern u8 saturday;
// extern u8 sunday;
extern u8 flage_timer_on;
extern u8 count_down_action;
extern u8 timing_start_flag;

struct percentage
{
	u8 command;
	
};

struct tlv_type
{
	u8 tag;
	u8 len;
	u8 value[8];
};

struct dp_data_buf_t
{
	u8 dp_id;
	u8 dp_type;
	u16 dp_data_len;
	u8 dp_data_value[20];
};

struct tybn1_data_pack
{
	u16 head;
	u8 device_version;
	u8 cmd_type;
	u16 data_len;		//高在前低在后
	struct dp_data_buf_t dp_data_buf_t;
	u8 crc8;
};

struct timed_task
{
	u8 hour;
	u8 minute;
	u8 action;
	u8 taks_parameter;
};

struct week_task
{
	struct timed_task monday_timed_task;
	struct timed_task tuesday_timed_task;
	struct timed_task wednesday_timed_task;
	struct timed_task thursday_timed_task;
	struct timed_task friday_timed_task;
	struct timed_task saturday_timed_task;
	struct timed_task sunday_timed_task;
};

extern struct week_task week_task;
extern struct dp_data_buf_t dp_data_buf_t;
extern struct tlv_type tlv_type;
//extern struct dp_data_buf dp_data_buf;
//extern struct tybn1_data_pack tybn1_data_pack;

/* Settings ------------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
void curtain_control_01_pack(u8 *data_buf);
void currain_percentage_02_pack(u8 *data_buf);
void fault_alarm_0C_pack(u8 *data_buf);
void battery_level_0D_pack(u8 *data_buf);
void auto_open_close_65_pack(u8 *data_buf);

void set_motor_current_78(u8 *data_buf);

void dp_download_curtain(const unsigned char value[]);
void dp_DPID_CALIBRATION_STATUS(const unsigned char value[]);
void dp_DPID_CALIBRATION_operation(const unsigned char value[]);


#ifdef __cplusplus
}
#endif

#endif
