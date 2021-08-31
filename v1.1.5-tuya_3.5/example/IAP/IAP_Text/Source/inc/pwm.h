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
#ifndef __PWM_H
#define __PWM_H

#include "ht32f5xxxx_tm.h"
#include "ht32f5xxxx_gpio.h"
#include "ht32f52231_41_libcfg.h"

#ifdef __cplusplus
 extern "C" {
#endif

/* Settings ------------------------------------------------------------------------------------------------*/
#define HTCFG_PWM_TM_PRESCALER                    (1)         // 1 ~ 65535
#define HTCFG_PWM_FREQ_HZ                         (50000)

#define HTCFG_PWM_TM_RELOAD                       (HTCFG_PWM_TM_PCLK / HTCFG_PWM_TM_PRESCALER / HTCFG_PWM_FREQ_HZ)

#define HTCFG_PWM0_EN                             (1)
#define HTCFG_PWM1_EN                             (1)
#define HTCFG_PWM2_EN                             (1)
#define HTCFG_PWM3_EN                             (1)

/* Exported constants --------------------------------------------------------------------------------------*/
#define PWM_DUTY_0       (0)
#define PWM_DUTY_25      (HTCFG_PWM_TM_RELOAD * 0.25)
#define PWM_DUTY_50      (HTCFG_PWM_TM_RELOAD * 0.5)
#define PWM_DUTY_75      (HTCFG_PWM_TM_RELOAD * 0.75)
#define PWM_DUTY_100     (HTCFG_PWM_TM_RELOAD * 1.00)

#define PWM_FREQ_100K	 (HTCFG_PWM_TM_PCLK / HTCFG_PWM_TM_PRESCALER / 100000)
#define PWM_FREQ_50K     (HTCFG_PWM_TM_PCLK / HTCFG_PWM_TM_PRESCALER / 50000)
#define PWM_FREQ_40K     (HTCFG_PWM_TM_PCLK / HTCFG_PWM_TM_PRESCALER / 40000)
#define PWM_FREQ_12K     (HTCFG_PWM_TM_PCLK / HTCFG_PWM_TM_PRESCALER / 12000)

/* Exported functions --------------------------------------------------------------------------------------*/
void PWM_Init(void);

#if 1
#define PWM_Cmd(st)              TM_Cmd(HTCFG_PWM_TM_PORT, st)
#define PWM_SetFreq(uReload)     TM_SetCounterReload(HTCFG_PWM_TM_PORT, uReload)
#define PWM_UpdateDuty(CH, CMP)  TM_SetCaptureCompare(HTCFG_PWM_TM_PORT, CH, CMP)
#else
void PWM_Cmd(ControlStatus NewState);
void PWM_SetFreq(u32 uReload);
void PWM_UpdateDuty(TM_CH_Enum TM_CH_n, u32 uCompare);
#endif

#ifdef __cplusplus
}
#endif

#endif
