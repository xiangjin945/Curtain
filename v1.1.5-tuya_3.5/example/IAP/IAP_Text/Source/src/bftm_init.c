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
#include "bftm_init.h"

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

/* Private function prototypes -----------------------------------------------*/

/* Global variables ----------------------------------------------------------*/
u32 bftm_cnt = 0;
u8 bftm_second = 0;
u8 bftm_minute = 0;
u8 bftm_hour = 0;

void bftm_onfiguration(void)
{
    NVIC_EnableIRQ(BFTM0_IRQn);

    /* toggel LED1 every 0.5 second by match interrupt */
    BFTM_SetCompare(HT_BFTM0, SystemCoreClock/1000);
    BFTM_SetCounter(HT_BFTM0, 0);
    BFTM_IntConfig(HT_BFTM0, ENABLE);
    BFTM_EnaCmd(HT_BFTM0, ENABLE);

    /* toggel LED2 every second by polling */
    BFTM_SetCompare(HT_BFTM1, SystemCoreClock);
    BFTM_SetCounter(HT_BFTM1, 0);
    BFTM_OneShotModeCmd(HT_BFTM1, ENABLE);
}
