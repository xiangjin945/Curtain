/* Define to prevent recursive inclusion -------------------------------------------------------------------*/
#ifndef __UART0_INIT_H
#define __UART0_INIT_H

/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32f5xxxx_usart.h"
#include "ht32f5xxxx_gpio.h"
#include "ht32.h"
#include "ht32_board.h"
#include <stdio.h>
#include <string.h>

extern u8 uart0_len;
extern u8 uart0_flag;
extern u8 uart0_buf[100];

void uart0_send(u8 *data,int len);
void uart0_init(void);

#endif /* __UART0_INIT_H */
