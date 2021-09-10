#include "uart0_init.h"

u8 uart0_len = 0;
u8 uart0_flag = 0;
u8 uart0_buf[100] = {0};

void uart0_send(u8 *data,int len)
{
	int i;
	
	//COM_TYBN1 = HT_UART0
	for(i=0;i<len;i++)
	{
		USART_SendData(COM_TYBN1,data[i]);
    while(!USART_GetFlagStatus(COM_TYBN1,USART_FLAG_TXC));
	}
}

//TXD=B7.RXD=B8
void uart0_init(void)
{
	USART_InitTypeDef USART_InitStructure;
	CKCU_PeripClockConfig_TypeDef CKCUClock = {0};
	
	/* Enable peripheral clock of AFIO, USART0                                                                */
	CKCUClock.Bit.AFIO       = 1;
	COM1_CLK(CKCUClock)      = 1;
	CKCU_PeripClockConfig(CKCUClock, ENABLE);

	//TXD = B7, RXD = B8
	AFIO_GPxConfig(COM0_TX_GPIO_ID, COM0_TX_AFIO_PIN, AFIO_FUN_USART_UART);
	AFIO_GPxConfig(COM0_RX_GPIO_ID, COM0_RX_AFIO_PIN, AFIO_FUN_USART_UART);

	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WORDLENGTH_8B;
	USART_InitStructure.USART_StopBits = USART_STOPBITS_1;
	USART_InitStructure.USART_Parity = USART_PARITY_NO;
	USART_InitStructure.USART_Mode = USART_MODE_NORMAL;
	USART_Init(COM_TYBN1  , &USART_InitStructure);

	/* Seting COM1_PORT interrupt-flag                                                                        */
	USART_IntConfig(COM_TYBN1, USART_INT_RXDR, ENABLE);
	/* Enable COM1_PORT                                                                                       */
	USART_TxCmd(COM_TYBN1, ENABLE);
	USART_RxCmd(COM_TYBN1, ENABLE);
	/* Configure USART0 & USART1 interrupt                                                                    */
	NVIC_EnableIRQ(COM0_IRQn);
}
