
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
#include "head.h"
bool i4_LowVlotageflag = FALSE;
u8 second_flag =0;
u32 second_LS = 0;
u32 DetectionThresholdCNT = 0;

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
bool StopSumFlag = FALSE;
// 2021.1.7 ???????(??: ms) 
int gptm0_4low = 0;
// 2020.11.21 time cost for calibration(unit: ms) 
int gptm0_4clbr = 0;

//用于百分比运行时长�?��??
u32 cnt_switch = 0;


int gptm0_cnt = 0;
u8 gptm0_second = 0;
long long timer_second = 0;
u8 gptm0_minute = 0;
u8 gptm0_hour = 0;
u8 gptm0_date = 0;


u8 i_timer = 0;

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
u8 buffer[200]={0};
/*********************************************************************************************************//**
 * @brief   This function handles UARTx interrupt.
 * @retval  None
 ************************************************************************************************************/
void COM0_IRQHandler(void)
{
	if (USART_GetFlagStatus(HT_UART0, USART_FLAG_RXDR))
	{
		uart_receive_input(HT_UART0->DR);
		
//		if(HT_UART0->DR == 0x55)
//		{
//			printf("\r\n");
//		}

//		printf("%x",HT_UART0->DR);
//		USART_ClearFlag(HT_UART0, USART_FLAG_RXDR);
#if PRINTFINFO
		//notes%x ",HT_UART0->DR);
#endif

		
	}
}

void COM1_IRQHandler(void)
{
	
}

//rxʵ��
void USART0_IRQHandler(void)
{
	if (USART_GetFlagStatus(HT_USART0, USART_FLAG_RXDR))
	{
		Production_test_input(HT_USART0->DR);
		// uart_receive_input(HT_USART0->DR);
//		//notesrx INFO:%x ",HT_USART0->DR);
		//USART_SendData(HT_UART0,usart0_buf[usart0_len]);
        //while(USART_GetFlagStatus(HT_UART0,USART_FLAG_TXC));
		// usart0_len++; 
		USART_ClearFlag(HT_USART0, USART_FLAG_RXDR);
	}
}

/*********************************************************************************************************//**
 * @brief   This function handles BFTM0 interrupt.
 * @retval  None
 ************************************************************************************************************/
void BFTM0_IRQHandler(void)
{
//	if(PA11_KEY()){
//		printf("PA11_KEY\r\n");
//		motor_stop();
//	}
    //HT32F_DVB_LEDToggle(HT_LED1);
    BFTM_ClearFlag(HT_BFTM0);
    bftm_cnt++;
	DetectionThresholdCNT++;
	if(ThereMinutes == 1)
	{
		ThereMinutesCnt ++;
	}
	

}

/*********************************************************************************************************//**
 * @brief   This function handles MCTM interrupt.
 * @retval  None
 ************************************************************************************************************/
u32 savetimecnt=0,dianliucnt=0;
u32 voltage_test = 0,agecnt = 0;
void GPTM0_IRQHandler(void)
{
	float real_volt = 0;

	TM_ClearFlag(HT_GPTM0, TM_INT_UEV);
	VS_TimeCalibraCnt++;
	key_delay_process++;
	gptm0_4low ++;		// NOTE: 每�??秒加�?

	if(MotorRunStatus && (calibration_status==11)){
		gptm0_4clbr ++;
		
		//I4_LOG("gptm0_4clbr = %d\n",gptm0_4clbr);
	}


	if(MiddleButtonStart)
		{
		MiddleButtonTiming++;
	
	}
	
	gptm0_cnt++;
	MotorCDC++;

	if(MotorCDC>400)
	{
		u8 i=0,t=0,k=0;
		u32 tsum=0,ksum;
		for(i=0;i<10;i++)
		{
			voltage_test = (HT_ADC->DR[1] & 0x0FFF);
			real_volt = ((float)voltage_test/4096)*3300;
			if(real_volt > protect_current)
			{
				t++;
				tsum += real_volt;
			}else{
				k++;
				ksum += real_volt;
			}
			
		}
		if(t>5)
		{
			
			motor_current = tsum /t;
			//printf("protect_current = %d\n",motor_current);
		}
		if(k>8)
		{
			motor_current = ksum / k;
		}
		printf("real_volt = %d %d\r\n",(int)real_volt,motor_current);
		MotorCDC = 0;

		if(motor_current > 300){
			agecnt ++;
			if(agecnt > 12){
				AgeingFlag = TRUE;
			}
		}else{
			AgeingFlag = FALSE;
			agecnt = 0;
		}
		
	}


	if(ptime.cnt_flag == 1)
	{
		cnt_switch++;
	}
	
	if(gptm0_cnt>999)
	{
		gptm0_cnt = 0;
		gptm0_second++;
		timer_second++;
		rtc_interrupt++;
		
		////notesprintf rtc:%d\n",rtc_interrupt);


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

	bFlags = RTC_GetFlagStatus();
	if((bFlags & 0x2) != 0x0) /* Match flag */
	{

		//notesIRQ_event!\n");	
		
		flage_timer_on = 2;
		
		
		
	}
	
	
}
extern bool leftkey_up ,rightkey_up;
static void Left_key_interrupt(void)
{
	if(variable.run_mode_flag){
		if(KeyControl)
			{
				leftkey_up = TRUE;
				printf(" left true!\r\n");
			}else{
				leftkey_up = FALSE;
				printf(" left false\r\n");
			}
		}else{
				leftkey_up = FALSE;
				printf("run_mode_flag off  left false\r\n");
	}
}
static void Right_key_interrupt(void)
{
	if(variable.run_mode_flag){
		if(KeyControl)
			{
				rightkey_up = TRUE;
				printf(" rightkey_up true!\r\n");
			}else{
				rightkey_up = FALSE;
				printf(" rightkey_up false\r\n");
			}
		}else{
				rightkey_up = FALSE;
				printf("run_mode_flag off  rightkey_up false\r\n");
	}
}


int key_cnt, timediff = 0;
bool down_flag = FALSE;
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
	if(EXTI_GetEdgeStatus(EXTI_CHANNEL_1, EXTI_HIGH_LEVEL) && !down_flag){
			key_cnt = RTC_GetCounter();
			down_flag = TRUE;
			EXTI_ClearEdgeFlag(EXTI_CHANNEL_1);
	}	
	if(EXTI_GetEdgeStatus(EXTI_CHANNEL_1, EXTI_LOW_LEVEL))//&& down_flag)
	{
		down_flag = FALSE;
		EXTI_ClearEdgeFlag(EXTI_CHANNEL_1);
		timediff = RTC_GetCounter();
		timediff -= key_cnt;
		//printf("key_cnt = %d\r\n",timediff);
		if(timediff < 2)
		{
			Left_key_interrupt();
			
		}


		guKeyState[0] = TRUE;
	}

	if(EXTI_GetEdgeStatus(EXTI_CHANNEL_2, EXTI_HIGH_LEVEL) && !down_flag){
			key_cnt = RTC_GetCounter();
			down_flag = TRUE;
			EXTI_ClearEdgeFlag(EXTI_CHANNEL_2);
	}	
	if(EXTI_GetEdgeStatus(EXTI_CHANNEL_2, EXTI_LOW_LEVEL))
	{
		down_flag = FALSE;
		EXTI_ClearEdgeFlag(EXTI_CHANNEL_2);
		timediff = RTC_GetCounter();
		timediff -= key_cnt;
		//printf("Rkey_cnt = %d\r\n",timediff);
		if(timediff < 2)
		{
			Right_key_interrupt();
		}
		guKeyState[1] = TRUE;
	}
	if(EXTI_GetEdgeStatus(EXTI_CHANNEL_5, EXTI_LOW_LEVEL))
	{
		EXTI_ClearEdgeFlag(EXTI_CHANNEL_5);
		// printf("EXTI_CHANNEL_5\r\n");
		guKeyState[2] = TRUE;
	}
	if(EXTI_GetEdgeStatus(EXTI_CHANNEL_14, EXTI_EDGE_POSITIVE))
	{
		EXTI_ClearEdgeFlag(EXTI_CHANNEL_14);
//		printf("EXTI_CHANNEL_14\r\n");
		guKeyState[3] = TRUE;
	}
	if(EXTI_GetEdgeStatus(EXTI_CHANNEL_15, EXTI_LOW_LEVEL))
	{
		EXTI_ClearEdgeFlag(EXTI_CHANNEL_15);
//		printf("EXTI_CHANNEL_15\r\n");
		guKeyState[4] = TRUE;
	}
	
	// if(EXTI_GetEdgeStatus(EXTI_CHANNEL_11, EXTI_POSITIVE_EDGE))
	// {
	// 	EXTI_ClearEdgeFlag(EXTI_CHANNEL_11);
	// 	//printf("EXTI_CHANNEL_11\r\n");
	// 	motor_stop();
	// 	i4_LowVlotageflag = TRUE;
	// 	guKeyState[4] = TRUE;
	// }
	
	
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
//void EXTI11_IRQHandler(void)
//{
//	//motor_stop();
//	printf("GPIO11\r\n");
//	if(EXTI_GetEdgeStatus(EXTI_CHANNEL_11, EXTI_NEGATIVE_EDGE))
//	{
//		motor_stop();
//		printf("GPIO11\r\n");
//		EXTI_ClearEdgeFlag(EXTI_CHANNEL_11);
//		
//	}

//}
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
	//printf("EXTI0_1_IRQHandler\r\n");
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
	//printf("EXTI2_3_IRQHandler\r\n");
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
	//motor_stop();
}


static void WAKEUP_key_Process(void)
{
	if(EXTI_GetEdgeStatus(EXTI_CHANNEL_2, EXTI_HIGH_LEVEL) || EXTI_GetEdgeStatus(EXTI_CHANNEL_2, EXTI_LOW_LEVEL))
	{
		Right_key_interrupt();
	}

	if(EXTI_GetEdgeStatus(EXTI_CHANNEL_1, EXTI_HIGH_LEVEL) || EXTI_GetEdgeStatus(EXTI_CHANNEL_1, EXTI_LOW_LEVEL))
	{
		Left_key_interrupt();
	}
}
/*********************************************************************************************************//**
 * @brief   This function handles Event Wake Up interrupt.
 * @retval  None
 ************************************************************************************************************/
void EVWUP_IRQHandler(void)
{
	//USART_SendData(HT_USART0,0xDD);
	// NOTE: 2020.12.1 reset timing when IRQ
	// NOTE: 2020.12.2 捕获唤醒�?�?时，a.重置低功耗时间标�?		b.重置低功耗状态标�?	
	
	gptm0_4low = 0;
	if(low_power_event_flag == 1)
	{	
		
		xRtcCounterTmp = 1;
		// //notesxRtcCounterTmp%d\n",xRtcCounterTmp);
		low_power_event_flag = 0;
		//printf("huan\r\n");
		
		key_cnt = RTC_GetCounter();
		
		tybn1_out_sleep_mode();
		GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_0,SET);
		
		// get_real_time();
		// unsigned char cmd_type = bt_uart_rx_buf[0];
		// //noteshuanxiang:%d\n",cmd_type);
	}

	
	/* Disable Wakeup Event Interrupt to avoid entering ISR again                                             */
	EXTI_WakeupEventIntConfig(DISABLE);		// ENABLE DISABLE
	
	/* Clear the specified EXTI channel wakeup flag                                                           */
	EXTI_ClearWakeupFlag(EXTI_CHANNEL_1); // Note: The clear operation may not work since the IO still active
	EXTI_ClearWakeupFlag(EXTI_CHANNEL_2); // Note: The clear operation may not work since the IO still active
	EXTI_ClearWakeupFlag(EXTI_CHANNEL_5); // Note: The clear operation may not work since the IO still active
	// PA14属于MCU通知模块进入低功耗，无需注册�?�?
	EXTI_ClearWakeupFlag(EXTI_CHANNEL_14); // Note: The clear operation may not work since the IO still active
	// 2020.12.1 模块PA15会有持续的脉冲信号，导致不停的唤醒MCU，暂时注�?PA15�?�?注册
	EXTI_ClearWakeupFlag(EXTI_CHANNEL_15); // Note: The clear operation may not work since the IO still active

	key_init();			  //?????
	WAKEUP_Button_Process();
	
}




/*********************************************************************************************************//**
 * @brief   This function handles SysTick Handler.
 * @retval  None
 ************************************************************************************************************/
void SysTick_Handler(void)
{
	timing_delay();
}


void LVD_BOD_IRQHandler(void)
{
  /* Wait till the VDD33 above 2.8V */
  while (PWRCU_GetLVDFlagStatus() != RESET)
  {
	GPIO_WriteOutBits(HT_GPIOB, GPIO_PIN_0,RESET);
	GPIO_WriteOutBits(HT_GPIOB, GPIO_PIN_1,RESET);  
	  printf("LVD_BOD_IRQHandler\r\n");
	//   motor_stop();
	  i4_LowVlotageflag = TRUE;
//    SET_LED(RESET);
//    ms_delay_systick(3000);
//    SET_LED(SET);
//    ms_delay_systick(3000);
  }
}