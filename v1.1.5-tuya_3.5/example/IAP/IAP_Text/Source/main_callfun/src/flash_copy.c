#include "main_callfun.h"
static uint8_t buf[200]={0};


uint8_t mcu_flash_write_iap(uint32_t addr, uint8_t *p_data, uint32_t size)
{
    u32 i;
    uint32_t eaddr = addr + size; 
    uint8_t *buffer = buf;
    FLASH_OptionByte Option;

    FLASH_GetOptionByteStatus(&Option);

    /* Blank Check before programming                                                                       */
//    for (i = 0; i < size; i += 4)
//    {
//      if (rw(addr + i) != 0xFFFFFFFF )
//      {
//        //gu8ReturnBuffer[3] = 0x4;
//        return CMD_FAILED;
//      }
//    }
u8 u32ReturnBufferIndex = 0;
    while (addr < eaddr)
    {
      u32 tmp;
	
      memcpy((u8*)&tmp, (u8*)&buf[u32ReturnBufferIndex], 4);
      FLASH_ProgramWordData(addr, tmp);

			addr += 4;
			u32ReturnBufferIndex += 4;
//			&buf += 4;
    }
    return CMD_SUCCESS;
  
}


/*****************************************************************************
函数名称 : mcu_flash_read
功能描述 :flash读函数
输入参数 :addr 地址 size 大小 p_data待读出数据地址

返回参数 : 无
使用说明 : 用户自行完善
*****************************************************************************/

uint8_t mcu_flash_read_iap(uint32_t addr, uint8_t *p_data, uint32_t size)
{
	uint32_t data =0;
	u8 u32ReturnBufferIndex=0;
	u8 i=0;
	for(i=0;i<size;i+=4)
	{
	  data = rw(addr);
		memcpy((u8*)&buf[u32ReturnBufferIndex], (u8*)&data, 4);
    u32ReturnBufferIndex += 4;
		addr += 4;
	}
}


//*擦除函数*/
u32 _ZXW_IAP_Erase(u32 type, u32 saddr, u32 eaddr,u8 setFlag)
{
  u32 i, j;
  if(setFlag)
{
	
  if ( (type == IAP_MASS_ERASE) 
    || (saddr < SECOND_APP_CODE_START) // 做 FLASH  设置 
    || ((eaddr-saddr) > (LIBCFG_FLASH_SIZE - SECOND_APP_CODE_START)))
  {
    //gu8ReturnBuffer[3] = 0x3;
    return CMD_FAILED;
  }
}else
{
	if ( (type == IAP_MASS_ERASE) 
    || (saddr < IAP_CODE_SIZE) 
    || ((eaddr-saddr) > (LIBCFG_FLASH_SIZE - IAP_CODE_SIZE)))
	 {
    //gu8ReturnBuffer[3] = 0x3;
     return CMD_FAILED;
   }
	 
 } 
	
  for (i = saddr, j = 0; i <= eaddr; i += FLASH_PAGE_SIZE, j++)
  {
    FLASH_ErasePage(i);
  }
  //zxw_sp_address_backup = saddr;
  
  return CMD_SUCCESS;
}

#define eaddr 0x7000
u32 IAP_Write_Flash(u32 saddr, u32 length,u32 readFlashAddr,u32 crc)
{	
	u32 APP_Saddr=readFlashAddr;//IAP_APFLASH_START
	u32 tmp_crc;
	u32 result=0;
	u32 eaddrr=saddr+length;
	u32 write_saddr=saddr;
	u32 write_eaddr=saddr+199;
  
	
	//result=_ZXW_IAP_Erase(IAP_PAGE_ERASE,saddr,eaddr);// 擦除FLASH  存放APP_BIN 文件的地址空间
	if(saddr==APP_NEW_FW_START_ADR)
	{
	  result=_ZXW_IAP_Erase(IAP_PAGE_ERASE,saddr,eaddrr,1);// 擦除FLASH  存放APP_BIN 文件的地址空间
	}
	else
	{
	   result=_ZXW_IAP_Erase(IAP_PAGE_ERASE,saddr,eaddrr,0);// 擦除FLASH  存放APP_BIN 文件的地址空间
	}
  if(result==CMD_SUCCESS)// 擦除FLASH 成功 
	{
		  while(write_saddr<=eaddr)     
			{       
				 // WDT_Restart(); 进行 喂狗  
				//*==================stat 获取64个字节数据==========================================================
				  // 循环一次 获取64个字节 来存储copyBuffer中
          mcu_flash_read_iap(APP_Saddr,buf,200);
          //Get_Data(APP_Saddr);
				  
				//=======================end 获取64个字节数据========================================================
				  result = mcu_flash_write_iap(saddr,buf,200);
	        //result=_ZXW_IAP_Flash(CMD_PROGRAM,write_saddr,write_eaddr,copyBuffer);
				  if(result!=CMD_SUCCESS)
					{
					   return CMD_FAILED;
					}
          saddr += 200;
					APP_Saddr += 200;
					
					write_saddr=write_eaddr+1;
	        write_eaddr=write_saddr+200;
	
			}	
	}else
	  {
		
		   return CMD_FAILED;
		}	
		
		CMP_IAP_CRC(length,&tmp_crc,APP_NEW_FW_START_ADR);
		if(tmp_crc == crc)
		{
			return CMD_SUCCESS;
		}
	// 进行 CRC16 数据校验
	// 	tmp_crc=_ZXW_IAP_CRC(0, saddr, length);
   	// if(tmp_crc==crc)
	// 	{
	// 			result=app_set_iap_flag(tmp_crc,length,1);
	// 			if(result==CMD_SUCCESS)// 2á3yFLASH 3é1| 
    //     {
    //       return CMD_SUCCESS;
    //     }
		 
	// 		  return CMD_FAILED;
    // }else{
    //     return CMD_FAILED;
    // }
}
