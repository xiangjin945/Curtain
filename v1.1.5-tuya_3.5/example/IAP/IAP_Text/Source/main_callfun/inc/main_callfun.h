#ifndef _MAIN_CALLFUN_
#define _MAIN_CALLFUN_

#include "stdio.h"
#include "ht32f5xxxx_01.h"
#include "ht32f5xxxx_usart.h"
#include "ht32_board_configs.h"
#include "string.h"
#include "mcu_ota_handler.h"
#include "ap_flash.h"


extern uint32_t                image_lenth;         /**< Size of the image in the bank. */
extern uint32_t                image_crc;          /**< CRC of the image. If set to 0, the CRC is ignored. */
extern uint32_t                bank_version;          /**< Identifier code for the bank. */
extern uint32_t	md5_buf[4];
extern uint32_t updata_info;


u32 IAP_Flash_COPY(u32 saddr, u32 length,u32 readFlashAddr,u32 crc);

void get_AP_info(void);

uint8_t CMP_IAP_CRC(uint32_t len,uint32_t *crc,u32 addr);

/*从片区2拷贝到片区1的函数*/
u32 IAP_Write_Flash(u32 saddr, u32 length,u32 readFlashAddr,u32 crc);




#endif //_MAIN_CALLFUN_s
