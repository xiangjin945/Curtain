/*********************************************************************************************************//**
 * @file    IAP/IAP_Text/Src_IAP/iap_main.c
 * @version $Rev:: 4493         $
 * @date    $Date:: 2020-01-20 #$
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
#include "ht32.h"
#include "ht32_board.h"
#include "ht32_board_configs.h"
#include "iap_handler.h"
#include "main_callfun.h"



/* When change UxART port, remember to change the vector address and NVIC settings.                         */
/* Please search keyword "IAP UART SETTING".                                                                */
/* Vector address of UxART                                                                                  */
/* Keil MDK-ARM: startup_ht32fxxxx_iap.s -> AREA    |.ARM.__AT_0x000000xx|, CODE, READONLY                  */
/* IAR EWARM: linker_iap.icf -> define symbol usart_vector_start     = 0x000000xx;                          */

/* Private function prototypes -----------------------------------------------------------------------------*/
void NVIC_Configuration(void);
void CKCU_Configuration(void);
void GPIO_Configuration(void);
void USART_Configuration(void);
void usart0_init(void);

/* Global functions ----------------------------------------------------------------------------------------*/
/*********************************************************************************************************//**
  * @brief  Main program.
  * @retval None
  ***********************************************************************************************************/
u8 result;
uint32_t crc;
int main(void)
{

	

  NVIC_Configuration();               /* NVIC configuration                                                 */
  CKCU_Configuration();               /* System Related configuration                                       */                                 
  usart0_init();
//	IAP_Go(); 
	get_AP_info();
	u32 i;

	while(1)
	{
		if( (updata_info == BOOT_MODE_IAP ) && ((image_lenth+APP_NEW_FW_START_ADR)<APP_NEW_FW_SETTING_ADR))
		{
			////notesi am supman!\n");
			CMP_IAP_CRC(image_lenth,&crc,APP_NEW_FW_START_ADR);
			if(crc == image_crc)
			{
				printf("THIS start IAP_Write_Flash!\n");
				
				result = IAP_Write_Flash(IAP_APFLASH_START, image_lenth,APP_NEW_FW_START_ADR,image_crc);
				
				if(result == CMD_SUCCESS)
				{
					for (i = APP_NEW_FW_SETTING_ADR; i < APP_UPDATA_INFO_REASED; i+=1024)
					{
							FLASH_ErasePage(i);
					}
					IAP_Go(); 
				}else
				{
					////notesap2 to ap1 err!\n");
				}
				
			}
			else{
				IAP_Go(); 
			}
		}
		else{
			////noteszhengchangjinru!\n");
			IAP_Go(); 
			
		
		}
	}
				
		                                          
}

/*********************************************************************************************************//**
  * @brief  This function is used to configure NVIC.
  * @retval None
  ***********************************************************************************************************/
void NVIC_Configuration(void)
{
  NVIC_EnableIRQ(HTCFG_UART_IRQn);    /* IAP UART SETTING: Enable USART Interrupt                           */
}

/*********************************************************************************************************//**
  * @brief  Configure the system clocks.
  * @retval None
  ***********************************************************************************************************/
void CKCU_Configuration(void)
{
  CKCU_PeripClockConfig_TypeDef CKCUClock = {{ 0 }};
  CKCUClock.Bit.AFIO              = 1;
  CKCUClock.Bit.HTCFG_RX_GPIO_CLK = 1;
  CKCUClock.Bit.HTCFG_UART        = 1;
  CKCUClock.Bit.HTCFG_KEY1_CLK    = 1;
  CKCU_PeripClockConfig(CKCUClock, ENABLE);
}

void usart0_init(void)
{
	CKCU_PeripClockConfig_TypeDef CKCUClock ;
	USART_InitTypeDef USART_InitStructure;
	CKCUClock.Bit.AFIO = 1;
	CKCUClock.Bit.USART0 = 1;
	CKCUClock.Bit.PC = 1;
	CKCUClock.Bit.PB = 1;
	CKCUClock.Bit.PA = 1;
	CKCU_PeripClockConfig(CKCUClock, ENABLE);
	
	//TXD = C4, RXD = C5
	AFIO_GPxConfig(GPIO_PC, AFIO_PIN_4,  AFIO_FUN_USART_UART);
	AFIO_GPxConfig(GPIO_PC, AFIO_PIN_5, AFIO_FUN_USART_UART);

	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WORDLENGTH_8B;
	USART_InitStructure.USART_StopBits = USART_STOPBITS_1;
	USART_InitStructure.USART_Parity = USART_PARITY_NO;
	USART_InitStructure.USART_Mode = USART_MODE_NORMAL;
	USART_Init(HT_USART0 , &USART_InitStructure);

	USART_IntConfig(HT_USART0, USART_INT_RXDR, ENABLE);
	USART_TxCmd(HT_USART0, ENABLE);
	USART_RxCmd(HT_USART0, ENABLE);

	NVIC_EnableIRQ(USART0_IRQn);
}


/*********************************************************************************************************//**
  * @brief  Configure the GPIO ports.
  * @retval None
  ***********************************************************************************************************/
void GPIO_Configuration(void)
{
  /* Turn on UxART Rx internal pull up resistor to prevent unknow state                                     */
  GPIO_PullResistorConfig(HTCFG_RX_GPIO_PORT, HTCFG_RX_GPIO_PIN, GPIO_PR_UP);

  AFIO_GPxConfig(HTCFG_TX_GPIO_ID, HTCFG_TX_AFIO_PIN, AFIO_FUN_USART_UART);
  AFIO_GPxConfig(HTCFG_RX_GPIO_ID, HTCFG_RX_AFIO_PIN, AFIO_FUN_USART_UART);
}

/*********************************************************************************************************//**
  * @brief  This function is used to configure USART.
  * @retval None
  ***********************************************************************************************************/
void USART_Configuration(void)
{
  USART_InitTypeDef USART_InitStructure;
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WORDLENGTH_8B;
  USART_InitStructure.USART_StopBits = USART_STOPBITS_1;
  USART_InitStructure.USART_Parity = USART_PARITY_NO;
  USART_InitStructure.USART_Mode = USART_MODE_NORMAL;
  USART_Init(HTCFG_UART_PORT, &USART_InitStructure);

  /* USART Tx enable                                                                                        */
  USART_TxCmd(HTCFG_UART_PORT, ENABLE);
  /* USART Rx enable                                                                                        */
  USART_RxCmd(HTCFG_UART_PORT, ENABLE);

  #if (HTCFG_UART==USART0) || (HTCFG_UART==USART1)
          /* Set Rx FIFO Level                      */
  #endif
  USART_IntConfig(HTCFG_UART_PORT, USART_INT_RXDR, ENABLE);   /* Enable RX FIFO Interrupt               */

  #if (HTCFG_UART==USART0) || (HTCFG_UART==USART1)
  USART_SetTimeOutValue(HTCFG_UART_PORT, 40);                 /* Set Rx Timeout                         */
  USART_IntConfig(HTCFG_UART_PORT, USART_INT_TOUT, ENABLE);
  #endif
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

/**
  * @}
  */
