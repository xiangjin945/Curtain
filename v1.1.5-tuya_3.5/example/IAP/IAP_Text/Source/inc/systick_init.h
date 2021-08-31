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
#ifndef __SYSTICK_INIT_H
#define __SYSTICK_INIT_H

#include "ht32f5xxxx_tm.h"
#include "ht32f5xxxx_gpio.h"
#include "ht32f52231_41_libcfg.h"

#ifdef __cplusplus
 extern "C" {
#endif

#include "ht32.h"
#include "ht32_board.h"

extern void ms_delay_systick(u32 us);
extern void timing_delay(void);
void  systick_init(void);

#ifdef __cplusplus
}
#endif

#endif
