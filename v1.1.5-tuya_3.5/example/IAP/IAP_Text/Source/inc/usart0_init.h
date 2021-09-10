/* Define to prevent recursive inclusion -------------------------------------------------------------------*/
#ifndef __USART0_INIT_H
#define __USART0_INIT_H

/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32f5xxxx_usart.h"
#include "ht32f5xxxx_gpio.h"
#include "ht32.h"
#include <stdio.h>
#include <string.h>
#include "ht32_board.h"

#define COM_DEBUG				HT_USART0

extern u8 usart0_len;
extern u8 usart0_flag;
extern u8 usart0_buf[519];

void usart0_send(u8 *data,int len);
void usart0_init(void);

#endif /* __USART0_INIT_H */
