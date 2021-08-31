/*
 * @Author: i4season.xiang
 * @Date: 2021-01-09 15:55:28
 * @LastEditTime: 2021-03-03 15:20:37
 * @FilePath: \Src_APc:\Users\24379\Desktop\v1.1.5-tuya_3.3\example\IAP\IAP_Text\Source\inc\gptm0_init.h
 */
/* Define to prevent recursive inclusion -------------------------------------------------------------------*/
#ifndef __GPTM0_INIT_H
#define __GPTM0_INIT_H

/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32f5xxxx_gpio.h"
#include "ht32.h"
#include "ht32_board.h"
#include "gpio_init.h"
#include "uart0_init.h"
#include "usart0_init.h"

// 2021.1.7 低功耗专用计时标记(单位: ms) 
extern int gptm0_4low;
//用于百分比运行时长计时
extern u32 cnt_switch;


// 2020.11.21 time cost for calibration(unit: ms) 
extern u32 DetectionThresholdCNT;

extern int gptm0_4clbr;
extern long long timer_second;
extern int gptm0_cnt;
extern u8 gptm0_second;
extern u8 gptm0_minute;
extern u8 gptm0_hour;

extern u32 time_monday;
extern u32 time_tuesday;
extern u32 time_wednesday;
extern u32 time_thursday;
extern u32 time_friday;
extern u32 time_saturtday;
extern u32 time_sunday;


void read_time(void);
void gptm0_onfiguration(void);

#endif /* __GPTM0_INIT_H ------------------------------------------------------------------------------------*/
