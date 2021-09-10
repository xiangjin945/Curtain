#include "save_all_setting.h"

u32 err = 0;
u32 all_setting[100] = {0};

void read_all_setting(void)
{
	int i = 0;
	u8 data = 0;
	u32 addr = START_ADDRESS;
	
	for(i=0;i<100;i++)
	{
		data = (u8)((*(u32*)addr)>>24);
		all_setting[i] = (u32)data;
		data = (u8)((*(u32*)addr)>>16);
		all_setting[i] = (u32)data;
		data = (u8)((*(u32*)addr)>>8);
		all_setting[i] = (u32)data;
		addr++;
	}
}

void erase_all_setting(void)
{
	FLASH_State flash_state;
	
	flash_state = FLASH_ErasePage(START_ADDRESS);
	if(flash_state != FLASH_COMPLETE)
	{
		err++;
	}
}

void save_all_setting(u8 *data,u16 len)
{
	int i = 0, addr = 0;
	FLASH_State flash_state;
	
	addr = START_ADDRESS;
	for(i=0;i<len;i++)
	{
		flash_state = FLASH_ProgramWordData(addr,(u32)data++);
		addr++;
		if(flash_state != FLASH_COMPLETE)
		{
			err++;
		}
	}
}


