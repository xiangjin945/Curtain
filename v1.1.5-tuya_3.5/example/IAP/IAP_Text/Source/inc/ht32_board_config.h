/*********************************************************************************************************//**
 * @file    GPIO/InputOutput/ht32_board_config.h
 * @version $Rev:: 4728         $
 * @date    $Date:: 2020-04-07#$
 * @brief   The header file of board configuration.
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
#ifndef __HT32_BOARD_CONFIG_H
#define __HT32_BOARD_CONFIG_H

//#include "ht32f5xxxx_tm.h"
//#include "ht32f5xxxx_gpio.h"
#include "ht32f52231_41_libcfg.h"

#ifdef __cplusplus
 extern "C" {
#endif

#define  HTCFG_PWM_TM_PCLK                       (LIBCFG_MAX_SPEED)

/* Settings ------------------------------------------------------------------------------------------------*/
#if defined(USE_HT32F52241_SK)
  #define HTCFG_MCTM_CH0_GPIO_ID                      (GPIO_PA)
  #define HTCFG_MCTM_CH0N_GPIO_ID                     (GPIO_PA)
  #define HTCFG_MCTM_CH1_GPIO_ID                      (GPIO_PB)
  #define HTCFG_MCTM_CH1N_GPIO_ID                     (GPIO_PB)
  #define HTCFG_MCTM_CH2_GPIO_ID                      (GPIO_PB)
  #define HTCFG_MCTM_CH2N_GPIO_ID                     (GPIO_PB)
  #define HTCFG_MCTM_BREAK_GPIO_ID                    (GPIO_PB)

  #define HTCFG_MCTM_CH0_AFIO_PIN                     (AFIO_PIN_14)
  #define HTCFG_MCTM_CH0N_AFIO_PIN                    (AFIO_PIN_15)
  #define HTCFG_MCTM_CH1_AFIO_PIN                     (AFIO_PIN_0)
  #define HTCFG_MCTM_CH1N_AFIO_PIN                    (AFIO_PIN_1)
  #define HTCFG_MCTM_CH2_AFIO_PIN                     (AFIO_PIN_2)
  #define HTCFG_MCTM_CH2N_AFIO_PIN                    (AFIO_PIN_3)
  #define HTCFG_MCTM_BREAK_AFIO_PIN                   (AFIO_PIN_4)
  #define HTCFG_MCTM_RELOAD                           (4000000/1000)
  #define HTCFG_MCTM_DEAD_TIME                        (60)



  #define HTCFG_OUTPUT_LED0_ID                                  (GPIO_PC)
  #define HTCFG_OUTPUT_LED1_ID                                  (GPIO_PC)
  #define HTCFG_OUTPUT_LED2_ID                                  (GPIO_PB)
  #define HTCFG_INPUT_WAKE_ID                                   (GPIO_PB)
  #define HTCFG_INPUT_KEY1_ID                                   (GPIO_PB)
  #define HTCFG_INPUT_KEY2_ID                                   (GPIO_PB)

  #define HTCFG_OUTPUT_LED0_CLK(CK)                             (CK.Bit.PC)
  #define HTCFG_OUTPUT_LED1_CLK(CK)                             (CK.Bit.PC)
  #define HTCFG_OUTPUT_LED2_CLK(CK)                             (CK.Bit.PB)
  #define HTCFG_INPUT_WAKE_CLK(CK)                              (CK.Bit.PB)
  #define HTCFG_INPUT_KEY1_CLK(CK)                              (CK.Bit.PB)
  #define HTCFG_INPUT_KEY2_CLK(CK)                              (CK.Bit.PB)

  #define HTCFG_LED0                                            (HT_GPIOC)
  #define HTCFG_LED1                                            (HT_GPIOC)
  #define HTCFG_LED2                                            (HT_GPIOB)
  #define HTCFG_WAKE                                            (HT_GPIOB)
  #define HTCFG_KEY1                                            (HT_GPIOB)
  #define HTCFG_KEY2                                            (HT_GPIOB)

  #define HTCFG_OUTPUT_LED0_AFIO_PIN                            (AFIO_PIN_2)
  #define HTCFG_OUTPUT_LED1_AFIO_PIN                            (AFIO_PIN_3)
  #define HTCFG_OUTPUT_LED2_AFIO_PIN                            (AFIO_PIN_6)
  #define HTCFG_INPUT_WAKE_AFIO_PIN                             (AFIO_PIN_12)
  #define HTCFG_INPUT_KEY1_AFIO_PIN                             (AFIO_PIN_7)
  #define HTCFG_INPUT_KEY2_AFIO_PIN                             (AFIO_PIN_8)

  #define HTCFG_OUTPUT_LED0_GPIO_PIN                            (GPIO_PIN_2)
  #define HTCFG_OUTPUT_LED1_GPIO_PIN                            (GPIO_PIN_3)
  #define HTCFG_OUTPUT_LED2_GPIO_PIN                            (GPIO_PIN_6)
  #define HTCFG_INPUT_WAKE_GPIO_PIN                             (GPIO_PIN_12)
  #define HTCFG_INPUT_KEY1_GPIO_PIN                             (GPIO_PIN_7)
  #define HTCFG_INPUT_KEY2_GPIO_PIN                             (GPIO_PIN_8)
#endif

#ifdef __cplusplus
}
#endif
#endif
