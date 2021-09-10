/* Define to prevent recursive inclusion -------------------------------------------------------------------*/
#ifndef __SAVE_ALL_SETTING_H
#define __SAVE_ALL_SETTING_H

#include "ht32f5xxxx_flash.h"
#include "service_logic_manage.h"

#define START_ADDRESS		0x2800//(1024*10)
#define END_ADDRESS			(1024*11)

extern u32 err;
extern u32 all_setting[100];

void read_all_setting(void);
void erase_all_setting(void);
void save_all_setting(u8 *data,u16 len);


#endif /* __SAVE_ALL_SETTING_H */
