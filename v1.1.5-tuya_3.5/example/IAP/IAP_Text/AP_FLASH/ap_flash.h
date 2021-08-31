#ifndef __AP_FLASH_H
#define __AP_FLASH_H	 

#include "ht32_board.h"

#define SECOND_APP_CODE_START           (32 * 1024)       /*  存放第二段 升级程序代码的 开始地址    */
#define SECOND_APP_INFO_START           (58 * 1024)       /*  最后1K 用来存储数据 校验  长度  */


//#include "crc16.h"
//#include "iap_handler.h"
 u32  app_set_iap_flag(u32 tmp_crc,u32 length,u8 flag);
 // u32  app_set_iap_flag(u32 tmp_crc,u32 length,u8 flag)
 u32 IAP_Write_Flash(u32 saddr, u32 length,u32 readFlashAddr,u32 crc);
 u32 _ZXW_IAP_Flash(u32 type, u32 saddr, u32 eaddr,u8 *bufData);
 u32 _ZXW_IAP_Erase(u32 type, u32 saddr, u32 eaddr,u8 setFlag);
 u32 _ZXW_IAP_CRC(u32 crc, u32 saddr, u32 length);
 // short CRC_GetModbus16(unsigned char *pdata, int len);

#define IAP_CODE_SIZE                 (1024 * 4)
#define IAP_PAGE_ERASE          (0x8)
#define CMD_SUCCESS             ('O')
#define CMD_FAILED              ('F')
#define CMD_PROGRAM             (1)
#define IAP_MASS_ERASE          (0xA)
extern u8 copyBuffer[65]; 
 extern u8 CRC_Buffer[4]; 
 
  typedef struct {
 
  u32 lenth;
	u32 crc_value;
	u32 isInputIapData;
} ZXW_IAP_Info_TypeDef;

extern ZXW_IAP_Info_TypeDef ZXW_IAP_Info;
 extern u32 info_value;
extern u32 info_lenth;
extern u32 info_input_data;


  typedef struct {
 
  u32 lenth;
	u32 crc_value;
	u32 isInputIapData;
} IAP_Info_TypeDef;

extern IAP_Info_TypeDef ZXW_INFO;

 void get_AP_info(void);

#endif
