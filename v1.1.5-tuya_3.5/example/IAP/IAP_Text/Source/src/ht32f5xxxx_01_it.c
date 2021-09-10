/*********************************************************************************************************//**
 * @file    USART/HyperTerminal_TxRx_Interrupt/ht32f5xxxx_01_it.c
 * @version $Rev:: 2970         $
 * @date    $Date:: 2018-08-03 #$
 * @brief   This file provides all interrupt service routine.
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
#include "uart0_init.h"
#include "usart0_init.h"
#include "gptm0_init.h"
#include "tybn1_init.h"
#include "rtc_init.h"
#include "systick_init.h"
#include "adc_init.h"
#include "low_power_iniit.h"

/** @addtogroup HT32_Series_Peripheral_Examples HT32 Peripheral Examples
  * @{
  */

/** @addtogroup USART_Examples USART
  * @{
  */

/** @addtogroup HyperTerminal_TxRx_Interrupt
  * @{
  */


/* Private define ------------------------------------------------------------------------------------------*/
#define USART_TIMEOUT_ON          ((u32)0x00000080)

// 2021.1.7 ???????(??: ms) 
int gptm0_4low = 0;
// 2020.11.21 time cost for calibration(unit: ms) 
int gptm0_4clbr = 0;

int gptm0_cnt = 0;
u8 gptm0_second = 0;
u8 gptm0_minute = 0;
u8 gptm0_hour = 0;
u8 gptm0_date = 0;

/* Global functions ----------------------------------------------------------------------------------------*/
/*********************************************************************************************************//**
 * @brief   This function handles NMI exception.
 * @retval  None
 ************************************************************************************************************/
void NMI_Handler(void)
{
}

/*********************************************************************************************************//**
 * @brief   This function handles Hard Fault exception.
 * @retval  None
 ************************************************************************************************************/
void HardFault_Handler(void)
{
  #if 1

  static vu32 gIsContinue = 0;
  /*--------------------------------------------------------------------------------------------------------*/
  /* For development FW, MCU run into the while loop when the hardfault occurred.                           */
  /* 1. Stack Checking                                                                                      */
  /*    When a hard fault exception occurs, MCU push following register into the stack (main or process     */
  /*    stack). Confirm R13(SP) value in the Register Window and typing it to the Memory Windows, you can   */
  /*    check following register, especially the PC value (indicate the last instruction before hard fault).*/
  /*    SP + 0x00    0x04    0x08    0x0C    0x10    0x14    0x18    0x1C                                   */
  /*           R0      R1      R2      R3     R12      LR      PC    xPSR                                   */
  while (gIsContinue == 0)
  {
  }
  /* 2. Step Out to Find the Clue                                                                           */
  /*    Change the variable "gIsContinue" to any other value than zero in a Local or Watch Window, then     */
  /*    step out the HardFault_Handler to reach the first instruction after the instruction which caused    */
  /*    the hard fault.                                                                                     */
  /*--------------------------------------------------------------------------------------------------------*/

  #else

  /*--------------------------------------------------------------------------------------------------------*/
  /* For production FW, you shall consider to reboot the system when hardfault occurred.                    */
  /*--------------------------------------------------------------------------------------------------------*/
  NVIC_SystemReset();

  #endif
}

/*********************************************************************************************************//**
 * @brief   This function handles SVCall exception.
 * @retval  None
 ************************************************************************************************************/
void SVC_Handler(void)
{
}

/*********************************************************************************************************//**
 * @brief   This function handles PendSVC exception.
 * @retval  None
 ************************************************************************************************************/
void PendSV_Handler(void)
{
}

/*********************************************************************************************************//**
 * @brief   This function handles ADC interrupt.
 * @retval  None
 ************************************************************************************************************/
void ADC_IRQHandler(void)
{
	//gADC_SingleEndOfConversion++;
	//ADC_ClearIntPendingBit(HT_ADC, ADC_FLAG_SINGLE_EOC);
	//gPotentiometerLevel = (HT_ADC->DR[0] & 0x0FFF);
}

/*********************************************************************************************************//**
 * @brief   This function handles UARTx interrupt.
 * @retval  None
 ************************************************************************************************************/
void COM0_IRQHandler(void)
{
	if (USART_GetFlagStatus(HT_UART0, USART_FLAG_RXDR))
	{
		uart0_buf[uart0_len] = HT_UART0->DR;
		//USART_SendData(HT_USART0,uart0_buf[uart0_len]);
        //while(USART_GetFlagStatus(HT_USART0,USART_FLAG_TXC));
		uart0_len++; 
		USART_ClearFlag(HT_UART0, USART_FLAG_RXDR);
	}
}

void COM1_IRQHandler(void)
{
	
}

//µ÷ÊÔÓÃ
void USART0_IRQHandler(void)
{
	if (USART_GetFlagStatus(HT_USART0, USART_FLAG_RXDR))
	{
		usart0_buf[usart0_len] = HT_USART0->DR;
		//USART_SendData(HT_UART0,usart0_buf[usart0_len]);
        //while(USART_GetFlagStatus(HT_UART0,USART_FLAG_TXC));
		usart0_len++; 
		USART_ClearFlag(HT_USART0, USART_FLAG_RXDR);
	}
}

/*********************************************************************************************************//**
 * @brief   This function handles BFTM0 interrupt.
 * @retval  None
 ************************************************************************************************************/
void BFTM0_IRQHandler(void)
{
    //HT32F_DVB_LEDToggle(HT_LED1);
    BFTM_ClearFlag(HT_BFTM0);
    bftm_cnt++;
    
	if(bftm_cnt>999)
	{
		bftm_cnt = 0;
		bftm_second++;
	}
	if(bftm_second>59)
	{
		bftm_second = 0;
		bftm_minute++;
	}
	if(bftm_minute>59)
	{
		bftm_minute = 0;
		bftm_hour++;
	}
}

/*********************************************************************************************************//**
 * @brief   This function handles MCTM interrupt.
 * @retval  None
 ************************************************************************************************************/
void GPTM0_IRQHandler(void)
{
	TM_ClearFlag(HT_GPTM0, TM_INT_UEV);
	
	gptm0_4low ++;		// NOTE: ???????
	gptm0_4clbr ++;
	gptm0_cnt++;
		
	if(gptm0_cnt>999)
	{
		gptm0_cnt = 0;
		gptm0_second++;
	}
	if(gptm0_second>59)
	{
		gptm0_second = 0;
		gptm0_minute++;
	}
	if(gptm0_minute>59)
	{
		gptm0_minute = 0;
		gptm0_hour++;
	}

	// NOTE: ??????(????????24?????0?????)
	if(gptm0_4low>86400000)
	{
		gptm0_4low = 0;
	}
	if(gptm0_4clbr>86400000)
	{
		gptm0_4clbr = 0;
	}
}

void RTC_IRQHandler(void)
{
	u8 bFlags;
	
    alarm_second = RTC_GetCounter();
	bFlags = RTC_GetFlagStatus();
	if((bFlags & 0x2) != 0x0) /* Match flag */
	{
		//RTC_SetCompare(1);
		rtc_interrupt++;
	}
}

/*********************************************************************************************************//**
 * @brief   This function handles WAKEUP button interrupt.
 * @retval  None
 ************************************************************************************************************/
__STATIC_INLINE void WAKEUP_Button_Process(void)
{
#if 0
	if (EXTI_GetEdgeFlag(WAKEUP_BUTTON_EXTI_CHANNEL))
	{
		EXTI_ClearEdgeFlag(WAKEUP_BUTTON_EXTI_CHANNEL);
		guKeyState[0] = TRUE;
	}
#else
	if(EXTI_GetEdgeStatus(EXTI_CHANNEL_1, EXTI_LOW_LEVEL))
	{
		EXTI_ClearEdgeFlag(EXTI_CHANNEL_1);
		guKeyState[0] = TRUE;
	}
	if(EXTI_GetEdgeStatus(EXTI_CHANNEL_2, EXTI_LOW_LEVEL))
	{
		EXTI_ClearEdgeFlag(EXTI_CHANNEL_2);
		guKeyState[1] = TRUE;
	}
	if(EXTI_GetEdgeStatus(EXTI_CHANNEL_5, EXTI_LOW_LEVEL))
	{
		EXTI_ClearEdgeFlag(EXTI_CHANNEL_5);
		guKeyState[2] = TRUE;
	}
	if(EXTI_GetEdgeStatus(EXTI_CHANNEL_14, EXTI_EDGE_POSITIVE))
	{
		EXTI_ClearEdgeFlag(EXTI_CHANNEL_14);
		guKeyState[3] = TRUE;
	}
	if(EXTI_GetEdgeStatus(EXTI_CHANNEL_15, EXTI_LOW_LEVEL))
	{
		EXTI_ClearEdgeFlag(EXTI_CHANNEL_15);
		guKeyState[4] = TRUE;
	}
#endif
}

/*********************************************************************************************************//**
 * @brief   This function handles KEY1 button interrupt.
 * @retval  None
 ************************************************************************************************************/
__STATIC_INLINE void KEY1_Button_Process(void)
{
#if 0
	if (EXTI_GetEdgeFlag(KEY1_BUTTON_EXTI_CHANNEL))
	{
		EXTI_ClearEdgeFlag(KEY1_BUTTON_EXTI_CHANNEL);
		guKeyState[1] = TRUE;
	}
#else
	if(EXTI_GetEdgeStatus(EXTI_CHANNEL_1, EXTI_LOW_LEVEL))
	{
		EXTI_ClearEdgeFlag(EXTI_CHANNEL_1);
		guKeyState[0] = TRUE;
	}
	if(EXTI_GetEdgeStatus(EXTI_CHANNEL_2, EXTI_LOW_LEVEL))
	{
		EXTI_ClearEdgeFlag(EXTI_CHANNEL_2);
		guKeyState[1] = TRUE;
	}
	if(EXTI_GetEdgeStatus(EXTI_CHANNEL_5, EXTI_LOW_LEVEL))
	{
		EXTI_ClearEdgeFlag(EXTI_CHANNEL_5);
		guKeyState[2] = TRUE;
	}
	if(EXTI_GetEdgeStatus(EXTI_CHANNEL_14, EXTI_EDGE_POSITIVE))
	{
		EXTI_ClearEdgeFlag(EXTI_CHANNEL_14);
		guKeyState[3] = TRUE;
	}
	if(EXTI_GetEdgeStatus(EXTI_CHANNEL_15, EXTI_LOW_LEVEL))
	{
		EXTI_ClearEdgeFlag(EXTI_CHANNEL_15);
		guKeyState[4] = TRUE;
	}
#endif
}

/*********************************************************************************************************//**
 * @brief   This function handles KEY2 button interrupt.
 * @retval  None
 ************************************************************************************************************/
__STATIC_INLINE void KEY2_Button_Process(void)
{
#if 0
	if(EXTI_GetEdgeFlag(KEY2_BUTTON_EXTI_CHANNEL))
	{
		EXTI_ClearEdgeFlag(KEY2_BUTTON_EXTI_CHANNEL);
		guKeyState[2] = TRUE;
	}
#else
	if(EXTI_GetEdgeStatus(EXTI_CHANNEL_1, EXTI_LOW_LEVEL))
	{
		EXTI_ClearEdgeFlag(EXTI_CHANNEL_1);
		guKeyState[0] = TRUE;
	}
	if(EXTI_GetEdgeStatus(EXTI_CHANNEL_2, EXTI_LOW_LEVEL))
	{
		EXTI_ClearEdgeFlag(EXTI_CHANNEL_2);
		guKeyState[1] = TRUE;
	}
	if(EXTI_GetEdgeStatus(EXTI_CHANNEL_5, EXTI_LOW_LEVEL))
	{
		EXTI_ClearEdgeFlag(EXTI_CHANNEL_5);
		guKeyState[2] = TRUE;
	}
	if(EXTI_GetEdgeStatus(EXTI_CHANNEL_14, EXTI_EDGE_POSITIVE))
	{
		EXTI_ClearEdgeFlag(EXTI_CHANNEL_14);
		guKeyState[3] = TRUE;
	}
	if(EXTI_GetEdgeStatus(EXTI_CHANNEL_15, EXTI_LOW_LEVEL))
	{
		EXTI_ClearEdgeFlag(EXTI_CHANNEL_15);
		guKeyState[4] = TRUE;
	}
#endif
}

/*********************************************************************************************************//**
 * @brief   This function handles EXTI interrupt.
 * @retval  None
 ************************************************************************************************************/
void EXTI0_1_IRQHandler(void)
{
	//USART_SendData(HT_USART0,0xAA);
	//KEY1_Button_Process();
	//KEY2_Button_Process();
	WAKEUP_Button_Process();	//Fixed in PB12
}

/*********************************************************************************************************//**
 * @brief   This function handles EXTI interrupt.
 * @retval  None
 ************************************************************************************************************/
void EXTI2_3_IRQHandler(void)
{
	//USART_SendData(HT_USART0,0xBB);
	//KEY1_Button_Process();
	//KEY2_Button_Process();
	WAKEUP_Button_Process();	//Fixed in PB12
}

/*********************************************************************************************************//**
 * @brief   This function handles EXTI interrupt.
 * @retval  None
 ************************************************************************************************************/
void EXTI4_15_IRQHandler(void)
{
	//USART_SendData(HT_USART0,0xCC);
	//KEY1_Button_Process();
	//KEY2_Button_Process();
	WAKEUP_Button_Process();	//Fixed in PB12
}

/*********************************************************************************************************//**
 * @brief   This function handles Event Wake Up interrupt.
 * @retval  None
 ************************************************************************************************************/
void EVWUP_IRQHandler(void)
{
	//USART_SendData(HT_USART0,0xDD);
	
	/* Disable Wakeup Event Interrupt to avoid entering ISR again                                             */
	//EXTI_WakeupEventIntConfig(ENABLE);
	
	/* Clear the specified EXTI channel wakeup flag                                                           */
	EXTI_ClearWakeupFlag(EXTI_CHANNEL_1); // Note: The clear operation may not work since the IO still active
	EXTI_ClearWakeupFlag(EXTI_CHANNEL_2); // Note: The clear operation may not work since the IO still active
	EXTI_ClearWakeupFlag(EXTI_CHANNEL_4); // Note: The clear operation may not work since the IO still active
	EXTI_ClearWakeupFlag(EXTI_CHANNEL_14); // Note: The clear operation may not work since the IO still active
	EXTI_ClearWakeupFlag(EXTI_CHANNEL_15); // Note: The clear operation may not work since the IO still active
}

/*********************************************************************************************************//**
 * @brief   This function handles SysTick Handler.
 * @retval  None
 ************************************************************************************************************/
void SysTick_Handler(void)
{
	timing_delay();
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
