/*
 * @Author: i4season.xiang
 * @Date: 2021-01-09 15:55:28
 * @LastEditTime: 2021-02-23 14:03:13
 * @FilePath: \Src_APc:\Users\24379\Desktop\v1.1.5-tuya-view_2.3\example\IAP\IAP_Text\Source\inc\rtc_init.h
 */
/* Define to prevent recursive inclusion -------------------------------------------------------------------*/
#ifndef __RTC_INIT_H
#define __RTC_INIT_H

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

#define MINUTE		(60)
#define HOUR		(MINUTE*60)
#define DATE		(HOUR*24)
#define WEEK		(DATE*7)

typedef struct
{
	u32 year;
	u32 month;
	u32 day;
	u32 hour;
	u32 minute;
	u32 second;
} date_time;

extern u32 current_time;
extern long long alarm_second;
extern vu32 rtc_interrupt;
extern vu32 second;
extern u8 second_flag;

extern vu32 wRtcCounterTmp; //rtc cnt计数
extern vu32 xRtcCounterTmp; //rtc cnt计数
extern vu32 Initial_count; //rtc cnt计数
extern vu32 Next_time;
extern void setup_interrupt(vu32 time);
extern void rtc_init(void);

extern void app_time_init(date_time* date_time);
extern u32 app_time_adjust(date_time* date_time);
extern void AP_Time_Show(void);
extern void calc_alarm(void);

#endif		/* __RTC_INIT_H */
