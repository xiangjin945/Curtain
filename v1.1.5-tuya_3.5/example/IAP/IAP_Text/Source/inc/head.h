/*
 * @Author: i4season.xiang
 * @Date: 2021-03-24 09:47:48
 * @LastEditTime: 2021-09-02 10:25:58
 * @FilePath: \curtain\v1.1.5-tuya_3.5\example\IAP\IAP_Text\Source\inc\head.h
 */
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
#include "ht32f5xxxx_rstcu.h"
#include "rtc_init.h"
#include "systick_init.h"
#include "save_all_setting.h"
#include "adc_init.h"
#include "tst_function.h"
#include "bftm_init.h"
#include "low_power_iniit.h"
#include "protocol.h"
#include "mcu_api.h"
#include "system.h"
#include "main_callfun.h"
#include <time.h>
#include "string.h"
#include "service_logic_manage.h"
#include "bluetooth.h"
#include "stdbool.h"

#define THRESHOLD_VALUE				400
#define MUTE_THRESHOLD_VALUE 		250

#define FLASH_52K_GLOBAL_DATA		0xD000
#define FLASH_53K_GLOBAL_DATA		0xD400
#define FLASH_53K_CURRENT_TIME		0xD420



#define PRODUCTION_TEST_PRINTFINFO      1       //调试打印信息  产测时关闭 
#define FLASH_61K_GLOBAL_DATA       0xF400      //License 烧录地址
#define FLASH_63K_GLOBAL_DATA       0xFC00      //License 烧录地址

#define BURN_SUCCESS                  USART_SendData(HT_USART0, 0x01)
#define BURN_ERROR                  USART_SendData(HT_USART0, 0x00)	
#define BURN_WAIT                   while (USART_GetFlagStatus(HT_UART0, USART_FLAG_TXC) == RESET)
// extern u8 Production_test_uart_rx_buf[256];
#define I4_LOG(fmt,args...)		0//printf("%d:"fmt,__LINE__,##args)
typedef struct Link
{
	u32 timertime;
	u8 validity;
	u8 bufvalue;
	struct 	Link *next;
}link;
extern link *head;

extern u32 linkbuffer[64];
extern u8 icount ;
extern u8 VS_CurTime[3];

extern bool IsignStopPercent;    
extern u32 CountdownrData;
extern char vsignStopPercent;
extern u8 ledfalshswitch;
extern u8 bindflag ;
extern u8 lednetworkflag;
extern u32 ThereMinutes;
extern u32 ThereMinutesCnt;
extern u8 fiveflag;
extern u8 Stallsign;
extern bool FirstStartSign;
extern bool RtcCntSign;
extern bool cFirstStartSign;
extern bool MotorRunStatus;
extern u32 MiddleButtonTiming;
extern bool MiddleButtonStart;
extern bool muteflag;
extern u32 SaveFullruntime;
extern u32 MotorCDC;
extern u32 I4_ReturntimeCnt;
extern bool signFullonfulloff;
extern u8 signPositive_inversion ;
extern u8 IsignLaunchDirection;
extern u8 IsignDirection;
extern bool IsignCalibrating;
extern u32 IsignReset;
extern bool IsignIdling;
extern u16 VS_Timezonetime;
extern bool IS_Timezone;
extern u32 VS_TimeCalibraCnt;
extern bool IS_FullInspection;
extern u32 VS_ResetCountTime;
extern bool i4_LowVlotageflag;
extern bool lowpower_flag ;
extern bool lockturnflag;
extern u32 key_delay_process;
extern bool KeyControl;
extern bool AgeingFlag;

typedef struct Silent_running_time{
	u32 Muteopentime;
	u32 Muteclosetime;
}silent_runtime;
extern silent_runtime silenttime;

typedef struct Percent_correction{
	char key;
	char open;
}percent_correction;
extern percent_correction percor;




int check_time_zone(u16 timezone);

int check_battery(void);

void low_power_fun(void);
void rf_test(void);
void version_fun(void);
void fun_setup_interrupt(void);
void low_power_consump_fun(void);
void Production_test_input(unsigned char value);
unsigned char Production_test_Read_Byte(void);
void LEDFlashing_1s(void);
void LEDFlashingCALLBack(void);
void NoNetworkLedFlashing(void);
void TimerFlashDataHandle(void);
void CountdownFlashDataHandle(void);
void MuteModeCompensate(void);
void LockedRotorDetection(void);
void Pause_handler(void);

void DetectionCurrentValue(void);

void timer_curtain_control_callback(void);

void update_calibrstatus(u8 status);
void I4TimerDataClear(void);
void I4CountdownDataClear(void);

void I4_LedTwinkle(void);

void I4_ProductionTest(void);

void I4_MainTask(void);


