/*
 * @Author: i4season.xiang
 * @Date: 2021-02-03 18:10:57
 * @LastEditTime: 2021-03-24 14:35:37
 * @FilePath: \Src_IAPc:\Users\24379\Desktop\smart_curtain\v1.1.5-tuya_3.5\example\IAP\IAP_Text\Source\src\usart0_init.c
 */
#include "usart0_init.h"

u8 usart0_len = 0;
u8 usart0_flag = 0;
u8 usart0_buf[519] = {0};

void usart0_send(u8 *data,int len)
{
	int i;
	
	//COM_DEBUG = HT_USART0
	for(i=0;i<len;i++)
	{
		USART_SendData(HT_USART0,data[i]);
		while (USART_GetFlagStatus(HT_USART0, USART_FLAG_TXC) == RESET);
	}
}

void usart0_sendNum(u8 data)
{
	  USART_SendData(HT_USART0,data);
		while (USART_GetFlagStatus(HT_USART0, USART_FLAG_TXC) == RESET);

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
