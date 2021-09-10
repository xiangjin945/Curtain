/*
 * @Author: i4season.xiang
 * @Date: 2021-02-03 18:10:57
 * @LastEditTime: 2021-09-10 20:43:18
 * @FilePath: \curtain\v1.1.5-tuya_3.5\example\IAP\IAP_Text\Src_AP\main.c
 */
/*********************************************************************************************************/ /**
 * @file    IAP/IAP_Text/Src_AP/main.c
 * @version $Rev:: 4724         $
 * @date    $Date:: 2020-04-07 #$
 * @brief   The main program of IAP example.
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
#include "head.h"

#define HTCFG_PWRCU_LVDS             (PWRCU_LVDS_2V85)

u8 license_readback(void)
{
#if 1
	u8 buffer = 0;
	buffer = rw(FLASH_62K_LICESEN);
	if(buffer == 0xFF)
	{
		return 0;
	}

#endif
	return 1;

}
/**
 * @description: 主函数
 */

int main(void)
{
	NVIC_SetVectorTable(NVIC_VECTTABLE_FLASH, IAP_APFLASH_START);
	
//	printf("VERSION %s\r\n",MCU_VER);
	version_fun();
	//Low_power_wasteGpioSet();
	bt_protocol_init(); //协议串口的初始化
	clock_config(); //时钟配置
	uart0_init();	//B7.B8, TYBN1
 	usart0_init();	//C4.C5, 调试用
	key_init();			  //按键初始化
	gpio_init();		  //LED初始化
	adc_init();			  //ADC初始化
	gptm0_onfiguration(); //定时器初始化
	bftm_onfiguration();
	rtc_init(); 		//RTC初始化
	systick_init();
	
	global_arg_fun();//全局参数	
	bt_send_mcu_ver();
	fun_setup_interrupt();
	
    /* Enable NVIC LVD interrupt                                                                              */
	NVIC_EnableIRQ(LVD_BOD_IRQn);

	/* Enable BOD reset. The LDO will be turned off when VDD33 less then 2.18V                                 */
	PWRCU_BODRISConfig(PWRCU_BODRIS_RESET);
	PWRCU_BODCmd(ENABLE);
	
	PWRCU_SetLVDS(HTCFG_PWRCU_LVDS);
	PWRCU_LVDIntWakeupConfig(ENABLE);
	PWRCU_LVDCmd(ENABLE);
	
	// while(1){
	// 	printf("pa10 status %d\r\n",PA10_KEY());
	// 	ms_delay_systick(1000);
	// }
#if PRODUCE_MEASURE
	if(!PA10_KEY())
	{
		I4_ProductionTest();
		
	}else{
#endif
		rf_test();
		// if(license_readback())
		// {
			printf("VERSION %s!\r\n",MCU_VER);
			I4_MainTask();
		// }
		

#if PRODUCE_MEASURE		
	}
#endif

}





