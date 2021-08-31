/*********************************************************************************************************//**
 * @file    GPIO/InputOutput/main.c
 * @version $Rev:: 4473         $
 * @date    $Date:: 2020-01-14 #$
 * @brief   Main program.
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
#include "bftm_init.h"
#include "low_power_iniit.h"

/** @addtogroup HT32_Series_Peripheral_Examples HT32 Peripheral Examples
  * @{
  */

/** @addtogroup GPIO_Examples GPIO
  * @{
  */

/** @addtogroup InputOutput
  * @{
  */

/* Private function prototypes -----------------------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------------------------------------*/
FlagStatus TmpStatus = RESET;

/*********************************************************************************************************//**
  * @brief  Configure the system clocks.
  * @retval None
  ***********************************************************************************************************/
void clock_config(void)
{
#if 1
	CKCU_PeripClockConfig_TypeDef CKCUClock = {{ 0 }};
	//CKCUClock.Bit.ADC0		= 1;
	CKCUClock.Bit.ADC		= 1;
	CKCUClock.Bit.PDMA		= 0;
	CKCUClock.Bit.USBD		= 0;
	CKCUClock.Bit.CKREF		= 0;
	CKCUClock.Bit.EBI		= 0;
	CKCUClock.Bit.CRC		= 0;
	CKCUClock.Bit.PA		= 0;
	CKCUClock.Bit.PB		= 0;
	CKCUClock.Bit.PC		= 0;
	CKCUClock.Bit.PD		= 0;
	CKCUClock.Bit.I2C0		= 0;
	CKCUClock.Bit.I2C1		= 0;
	CKCUClock.Bit.SPI0		= 0;
	CKCUClock.Bit.SPI1		= 0;
	CKCUClock.Bit.USART0	= 1;
	CKCUClock.Bit.USART1	= 0;
	CKCUClock.Bit.UART0		= 1;
	CKCUClock.Bit.UART1		= 0;
	CKCUClock.Bit.AFIO		= 1;
	CKCUClock.Bit.EXTI		= 1;
	CKCUClock.Bit.SCI0		= 0;
	CKCUClock.Bit.SCI1		= 0;
	CKCUClock.Bit.I2S		= 0;
	CKCUClock.Bit.MCTM0		= 1;
	CKCUClock.Bit.WDT		= 1;
	CKCUClock.Bit.BKP		= 1;
	CKCUClock.Bit.GPTM0		= 0;
	CKCUClock.Bit.GPTM1		= 0;
	CKCUClock.Bit.BFTM0		= 1;
	CKCUClock.Bit.BFTM1		= 1;
	CKCUClock.Bit.CMP		= 0;
	CKCUClock.Bit.SCTM0		= 0;
	CKCUClock.Bit.SCTM1		= 0;
	CKCUClock.Bit.SCTM2		= 0;
	CKCUClock.Bit.SCTM3		= 0;
	CKCU_PeripClockConfig(CKCUClock, ENABLE);
#else
	CKCU_PeripClockConfig_TypeDef CKCUClock = {0};
	
	CKCUClock.Bit.PA = 1;
	CKCUClock.Bit.AFIO = 1;
	CKCU_PeripClockConfig(CKCUClock, ENABLE);
	CKCUClock.Bit.PB = 1;
	CKCUClock.Bit.AFIO = 1;
	CKCU_PeripClockConfig(CKCUClock, ENABLE);
	CKCUClock.Bit.PC = 1;
	CKCUClock.Bit.AFIO = 1;
	CKCU_PeripClockConfig(CKCUClock, ENABLE);
#endif
}
#define IAP_CODE_SIZE                 (1024 * 4)

#define IAP_APFLASH_START             (IAP_CODE_SIZE + 0)
#define IAP_APSRAM_START              (HT_SRAM_BASE)

#define BOOT_MODE_AP                  (0x55AAFAF0)
#define BOOT_MODE_IAP                 (0x55AAFAF5)
#define BOOT_MODE                     ((*((u32 volatile *) (HT_SRAM_BASE))))

void exti_process(void)
{
	char *exti_0 = "hello exti_1\r\n";
	char *exti_1 = "hello exti_2\r\n";
	char *exti_2 = "hello exti_3\r\n";
	char *exti_3 = "hello exti_13\r\n";
	char *exti_4 = "hello exti_15\r\n";
	
	if (guKeyState[0] == TRUE)
	{
		guKeyState[0] = FALSE;
		usart0_send((u8*)exti_0,12);
	}
	if (guKeyState[1] == TRUE)
	{
		guKeyState[1] = FALSE;
		usart0_send((u8*)exti_0,12);
	}
	if (guKeyState[2] == TRUE)
	{
		guKeyState[2] = FALSE;
		usart0_send((u8*)exti_0,12);
	}
	if (guKeyState[3] == TRUE)
	{
		guKeyState[3] = FALSE;
		usart0_send((u8*)exti_0,12);
	}
	if (guKeyState[4] == TRUE)
	{
		guKeyState[4] = FALSE;
		usart0_send((u8*)exti_0,12);
	}
}

/* Global functions ----------------------------------------------------------------------------------------*/
/*********************************************************************************************************//**
  * @brief  Main program.
  * @retval None
  * @note At this stage the microcontroller clock setting should has already been configured.
  *       This can be done through SystemInit() function which is called from startup
  *       file (startup_ht32fxxxx.s) before branching to application main().
  *       To reconfigure the default setting of SystemInit() function, please refer to
  *       system_ht32fxxxx.c file
  * @details The main function works as the following:
  *    - Check if the backup domain is ready for access so as to disable isolation.
  *      Because some GPIO pins are located in the backup domain.
  *    - Enable AFIO peripheral clock
  *    - Configure WAKEUP, KEY1, KEY2 pins as the input function
  *      - Enable GPIO peripheral clock of input pins
  *      - Configure AFIO mode of input pins
  *      - Configure GPIO direction of input pins
  *    - Configure LED1, LED2, LED3 pins as the output function
  *      - Enable GPIO peripheral clock of output pins
  *      - Configure AFIO mode of output pins
  *      - Configure GPIO direction of output pins
  *    - Infinite loop to read data from the input pin and then output to LED
  *      - Read WAKEUP and then output via LED1
  *      - Read KEY1 and then output via LED2
  *      - Read KEY2 and then output via LED3
  *
  ***********************************************************************************************************/
#define IAP_CODE_SIZE                 (1024 * 4)

#define IAP_APFLASH_START             (IAP_CODE_SIZE + 0)
int main(void)
{
	
	////notesjinglaiguo\n");
	
//	//int i;
	//char *hello = "hello world!\r\n";
	
	clock_config();				//时钟配置
	uart0_init();				//B7.B8, TYBN1
	usart0_init();				//C4.C5, 调试用
	//I2C_EEPROM_Init();		//GSENSOR
    //SPI_FLASH_Init();			//GSENSOR
	key_init();					//按键初始化
	gpio_init();				//LED初始化
	adc_init();					//ADC初始化
	gptm0_onfiguration();			//定时器初始化
	bftm_onfiguration();
	rtc_init();					//RTC初始化
	systick_init();
	//low_power_init();
	tybn1_init();				//tybn1初始化
//	BOOT_MODE = BOOT_MODE_IAP;
//  NVIC_SystemReset();
#ifdef USE_PWM_PB2_PB3
	//PB2_PWM_INIT(0,ENABLE);				//电机初始化
	//PB3_PWM_INIT(0,ENABLE);				//电机初始化
#endif

	//usart0_send((u8*)hello,18);
	//ms_delay_systick(1000);
	
#ifdef FLASH
	if(*(u32*)START_ADDRESS==0xFF)
	{
		//erase_all_setting();
	}
	else
	{
		//read_all_setting();
	}
	
	//usart0_send((u8*)hello,16);
	//ms_delay_systick(100);
	//save_all_setting((u8*)hello,1);
	
	//usart0_send((u8*)all_setting,16);
	for(i=0;i<18;i++)
	{
		//USART_SendData(HT_USART0,(u8)all_setting);
		//while(USART_GetFlagStatus(HT_USART0, USART_FLAG_TXC) == RESET);
	}
	for(i=0;i<18;i++)
	{
	//	USART_SendData(HT_USART0,hello[i]);
	//	while(USART_GetFlagStatus(HT_USART0, USART_FLAG_TXC) == RESET);
	}
#endif

	mcu_state = 1;							//第一次上电
    //heart_beat();							//第一条心跳
	//motor_pwr(PWR_ON);
	motor_pwr(PWR_OFF);
	SET_LED(SET);
	
	//RTC闹钟配置
	setup_interrupt(alarm_second);
	rtc_interrupt = 0;
	
	//GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_3,RESET);
	//USART_SendData(HT_USART0,rtc_interrupt);
	//while(USART_GetFlagStatus(HT_USART0, USART_FLAG_TXC) == RESET);
    
    curtain_mode = 0;
    calibration_status = 0;
    calibration_operation = 0;
    variable.full_running_time = 0;
    motor_current = 0;
    protect_current = 0x40;		// 0x190 400mA		0x40 64mA		0x30 48mA
	
	while (1)
	{
		low_power_event_flag = 0;

		//current_time += RTC_GetCounter();
        motor_current_test();		//电机电流
        motor_turn();				//正反转
        calibration_test();			//校验
        tybn1_test();				//涂鸦模块
		
		//进入低功耗模式
		//if(((low_power_time2-low_power_time1)>10)&&(low_power_event_flag==0))
		//if((bftm_second>30)&&(low_power_event_flag==0))
		/*if((bftm_minute>0)&&(low_power_event_flag==0))	// 2020.11.26 注解进入低功耗处理调用: 暂无唤醒支持
		{
			bftm_cnt = 0;
			bftm_second = 0;
			bftm_minute = 0;
			motor_stop();
			//enable_low_pwr(OPEN);
			tybn1_join_sleep_mode();
			//__WFI();																// 备注: 该低功耗模式实测仍然有11.9ma电流
			//Request to enter Deep Sleep mode 1 & Wait For Interrupt	|2020.11.26
			PWRCU_DeepSleep1(PWRCU_SLEEP_ENTRY_WFI);	// 备注: 该低功耗模式实测仍然有1.28ma电流(预期 150ua)
			//Request to enter Deep Sleep mode 2
			//PWRCU_DeepSleep2(PWRCU_SLEEP_ENTRY_WFI);
		}*/
		
		//过流保护
		//if(motor_current>0x190)		//400mA
		if(motor_current>protect_current)
		{
			motor_current = 0;
			motor_stop();
		}
	}
}

#if (HT32_LIB_DEBUG == 1)
/*********************************************************************************************************//**
  * @brief  Report both the error name of the source file and the source line number.
  * @param  filename: pointer to the source file name.
  * @param  uline: error line source number.
  * @retval None
  ***********************************************************************************************************/
void assert_error(u8* filename, u32 uline)
{
  /*
     This function is called by IP library that the invalid parameters has been passed to the library API.
     Debug message can be added here.
     Example: //notesParameter Error: file %s on line %d\r\n", filename, uline);
  */

  while (1)
  {
  }
}
#endif


/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
