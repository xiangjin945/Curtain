/*********************************************************************************************************//**
 * @file    TM/UpdateEvent/main.c
 * @version $Rev:: 4850         $
 * @date    $Date:: 2020-08-04 #$
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
#include "gptm0_init.h"

/** @addtogroup HT32_Series_Peripheral_Examples HT32 Peripheral Examples
  * @{
  */

/** @addtogroup TM_Examples TM
  * @{
  */

/** @addtogroup UpdateEvent
  * @{
  */

/* Settings ------------------------------------------------------------------*/
#define TM_FREQ_HZ           (1000)
#define TM_TIME_DELAY        (250)

/* Private function prototypes -----------------------------------------------*/

/* Global variables ----------------------------------------------------------*/

void gptm0_onfiguration(void)
{
	TM_TimeBaseInitTypeDef TimeBaseInit;
    
#if 0
	// Use following function to configure the IP clock speed.
	CKCU_SetPeripPrescaler(CKCU_PCLK_GPTM0, CKCU_APBCLKPRE_DIV2);
#endif

	/* Enable peripheral clock */
	CKCU_PeripClockConfig_TypeDef CKCUClock = {{ 0 }};
	CKCUClock.Bit.GPTM0 = 1;
	CKCU_PeripClockConfig(CKCUClock, ENABLE);

	/* Time base configuration */
	/* !!! NOTICE !!!
	Notice that the local variable (structure) did not have an initial value.
	Please confirm that there are no missing members in the parameter settings below in this function.
	*/

	TimeBaseInit.Prescaler = 1 - 1;                         // Timer clock = CK_AHB / 1
	TimeBaseInit.CounterReload = SystemCoreClock / TM_FREQ_HZ - 1;
	TimeBaseInit.RepetitionCounter = 0;
	TimeBaseInit.CounterMode = TM_CNT_MODE_UP;
	TimeBaseInit.PSCReloadTime = TM_PSC_RLD_IMMEDIATE;
	TM_TimeBaseInit(HT_GPTM0, &TimeBaseInit);

	/* Clear Update Event Interrupt flag since the "TM_TimeBaseInit()" writes the UEV1G bit */
	TM_ClearFlag(HT_GPTM0, TM_FLAG_UEV);

	/* Enable Update Event interrupt */
	NVIC_EnableIRQ(GPTM0_IRQn);
	TM_IntConfig(HT_GPTM0, TM_INT_UEV, ENABLE);

	TM_Cmd(HT_GPTM0, ENABLE);
}
