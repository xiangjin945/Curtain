
//#include "ht32_board_config.h"
//#include "ht32_board_configs.h"

#include "ap_flash.h"
#include "string.h"
#include "mcu_ota_handler.h"



//#define IAP_PAGE_ERASE          (0x8)
//#define CMD_SUCCESS             ('O')
//#define CMD_FAILED              ('F')
//#define CMD_PROGRAM             (1)
//#define IAP_MASS_ERASE          (0xA)
 u8 copyBuffer[65]; 
 u8 CRC_Buffer[4]; 

ZXW_IAP_Info_TypeDef ZXW_IAP_Info;

//IAP_Info_TypeDef ZXW_INFO;
u32 info_value;
u32 info_lenth;
u32 info_input_data;


//????:??CRC16
//??1:*pData  16?CRC????,
//??2:nLength ?????
//??3:init    ????
//??4:ptable  16?CRC???

//??CRC??
unsigned short GetRevCrc_16(unsigned char * pData, int nLength, unsigned short init, const unsigned short *ptable)
{
  unsigned short cRc_16 = init;
  unsigned char temp;

  while(nLength-- > 0)
  {
    temp = cRc_16 & 0xFF; 
    cRc_16 = (cRc_16 >> 8) ^ ptable[(temp ^ *pData++) & 0xFF];
  }

  return cRc_16;
}

//??CRC??
unsigned short GetCrc_16(unsigned char * pData, int nLength, unsigned short init, const unsigned short *ptable)
{
  unsigned short cRc_16 = init;
  unsigned char temp;

  while(nLength-- > 0)
  {
    temp = cRc_16 >> 8; 
    cRc_16 = (cRc_16 << 8) ^ ptable[(temp ^ *pData++) & 0xFF];
  }

  return cRc_16;    
}

//Demo -- modbus-crc16??
unsigned short CRC_GetModbus16(unsigned char *pdata, int len)
{
//MODBUS CRC-16? 8005 ??
const unsigned short g_McRctable_16[256] =
{
0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040
};

  return GetRevCrc_16(pdata, len, 0xFFFF, g_McRctable_16);
}






///*********************************************************************************************************//**
//  * @brief  Download image for program or verify.
//  * @param  type: Program or verify
//  *         @arg CMD_PROGRAM: Program mode
//  *         @arg CMD_VERIFY: Verify mode
//  *         @arg CMD_BLANK: Blank check mode
//  *         @arg CMD_READ: Read mode
//  * @param  saddr: Start address
//  * @param  eaddr: End address
//  * @param  buffer: point of data buffer
//  * @retval CMD_SUCCESS or CMD_FAILED
//  ***********************************************************************************************************/
 u32 _ZXW_IAP_Flash(u32 type, u32 saddr, u32 eaddr,u8 *bufData)
{
  u32 i;
  u8 *buffer = bufData;
  FLASH_OptionByte Option;
	
	 if((eaddr-saddr)>=64)return CMD_FAILED;//防止数组越界 
  /*--------------------------------------------------------------------------------------------------------*/
  /* When Security protection is enabled, read operation is not allowed  
			启用安全保护后，不允许进行读取操作	*/
  /*--------------------------------------------------------------------------------------------------------*/
  FLASH_GetOptionByteStatus(&Option);
  if (type == CMD_PROGRAM)
  {
    /* Blank Check before programming                                                                       */
    for (i = 0; i < (eaddr-saddr + 1); i += 4)
    {
      if (rw(saddr + i) != 0xFFFFFFFF )
      {
        //gu8ReturnBuffer[3] = 0x4;
        return CMD_FAILED;
      }
    }
  }
  /*--------------------------------------------------------------------------------------------------------*/
  /* Program                                                                                                */
  /*--------------------------------------------------------------------------------------------------------*/
  if (type == CMD_PROGRAM)
  {
    while (saddr <= eaddr)
    {
      u32 tmp;
      memcpy((u8*)&tmp, (u8*)buffer, 4);
      FLASH_ProgramWordData(saddr, tmp);
      saddr += 4;
      buffer += 4;
    }
  }
  return CMD_SUCCESS;
}





/*********************************************************************************************************//**
  * @brief  Mass/Page Erase.
  * @param  type: Erase type
  *         @arg IAP_MASS_ERASE: Mass Erase (Not support in IAP mode)
  *         @arg IAP_PAGE_ERASE: Page Erase
  * @param  saddr: Start address
  * @param  eaddr: End address
  * @retval CMD_SUCCESS or CMD_FAILED
  ***********************************************************************************************************/
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
////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SECOND_APP_INFO_START地址 分别存放：
//AP文件的校验（4个字节） 
//AP文件的长度（4个字节）
//更新AP文件的标志位（4个字节）  0x55AAFAF0   没有AP文件 清理
//参数说明：
//tmp_crc：AP文件的检验
//length： AP文件的长度
//flag ：AP文件标志位    0 没有AP文件可以更新    1 有AP文件可以更新   
 u32 app_set_iap_flag(u32 tmp_crc,u32 length,u8 flag)
{
     	u32 result=0;
      result=_ZXW_IAP_Erase(IAP_PAGE_ERASE,SECOND_APP_INFO_START,(SECOND_APP_INFO_START+1024),1);// 擦除FLASH  存放APP_BIN  info 信息的地址空间
				if(result==CMD_SUCCESS)// 擦除FLASH 成功 
				{

					    memcpy((u8*)&copyBuffer[0], (u8*)&tmp_crc, 4);
					    memcpy((u8*)&copyBuffer[4], (u8*)&length, 4);
					      //(0x55AAFAF0)
					    //memcpy((u8*)&CRC_Buffer[0], (u8*)&length, 2);
					if(!flag)
					{
					    copyBuffer[8]=0; //
					    copyBuffer[9]=0;//
					    copyBuffer[10]=0;//
					    copyBuffer[11]=0;//
					}else
					  {
						  copyBuffer[8]=0xF0; // 
					    copyBuffer[9]=0xFA;//
					    copyBuffer[10]=0xAA;//
					    copyBuffer[11]=0x55;//
						
						}
					    result=_ZXW_IAP_Flash(CMD_PROGRAM,SECOND_APP_INFO_START,SECOND_APP_INFO_START+63,copyBuffer);
             	if(result==CMD_SUCCESS)// 擦除FLASH 成功 
				      {
								 return CMD_SUCCESS;
							}
			  }

		  return CMD_FAILED;	
}
// 一次 只读64 个字节  这里读的是升级版本信息
 void  read_Flash_Data(u32 addr)
{         
	    u32 data =0;
	    u32 u32ReturnBufferIndex=0;
	    u8 i=0;
	     for(i=0;i<16;i++)//0 4 8 12 16   20 24 28 32    36 40 44 48   52 56  60 
	      {
	         data = rw(addr);
				   memcpy((u8*)&copyBuffer[u32ReturnBufferIndex], (u8*)&data, 4);
           u32ReturnBufferIndex += 4;
					 addr+=4;
				}
}


  void Get_Data(u32 addr)
 {
   	   read_Flash_Data(addr); 
 }
 static uint32_t crc32_compute(uint8_t const * p_data, uint32_t size, uint32_t const * p_crc)
{
	uint32_t crc;
    crc = (p_crc == NULL) ? 0xFFFFFFFF : ~(*p_crc);
    for (uint32_t i = 0; i < size; i++)
    {
        crc = crc ^ p_data[i];
        for (uint32_t j = 8; j > 0; j--)
        {
            crc = (crc >> 1) ^ (0xEDB88320U & ((crc & 1) ? 0xFFFFFFFF : 0));
        }
    }
    return ~crc;
}

#define eaddr 0xB400
//falsh擦写函数
u32 mcu_flash_read_test(unsigned char * pData,u8 *buf, u32 length)
{
	int i;
	int x=length;
	u32 result;
	u8 copyBuff[]={0};

	for(i=0;i<length;i++)
	{
		copyBuffer[i] = pData[i];
	}
			
	 memcpy(buf, copyBuffer, length); 
		
}

//crc32算法
static uint8_t file_crc_check_in_flash(uint32_t len,uint32_t *crc)
{

    static uint8_t buf[257]={0};
    if(len == 0)
    {
        return 1;
    }
    uint32_t crc_temp = 0;
    uint32_t read_addr = APP_NEW_FW_START_ADR;
    uint32_t cnt = len/256;


    uint32_t remainder = len % 256;

    for(uint32_t i = 0; i<cnt; i++)
    {
        
        mcu_flash_read_test((u8 *)read_addr,buf,256);
        crc_temp = crc32_compute(buf, 256, &crc_temp);
        read_addr += 256;
        
    }

    if(remainder>0&&remainder<256)
    {
        mcu_flash_read_test((u8 *)read_addr,buf,remainder);
        crc_temp = crc32_compute(buf, remainder, &crc_temp);
   
        read_addr += remainder;
    }
    
    *crc = crc_temp;
    return 0;
}
 
//这个函数调用crc32算法函数，返回crc32校验值
/*********************************************************************************************************//**
  * @brief  Calculate CRC value.
  * @param  crc: Iinitial value of CRC (Usually as 0)
  * @param  saddr: Start address
  * @param  length: Length for CRC calculation
  * @retval Always success (CMD_SUCCESS)
  ***********************************************************************************************************/
 u32 _ZXW_IAP_CRC(u32 crc, u32 saddr, u32 length)
{
	u32 crc32=0;
	file_crc_check_in_flash(length,&crc32);
	return crc32;
	
	
//	  u32 testCRC;
//  //FLASH_ProgramWordData(sp_address_backup, sp_backup); //write sp

//	//FLASH_ProgramWordData(sp_address_backup, sp_backup); //write sp
//  //crc = CRC16(crc, (u8 *)saddr, length);
//	 testCRC =CRC_GetModbus16((u8 *)saddr, length);
// // memcpy((u8*)&gu8ReturnBuffer[u32ReturnBufferIndex], (u8*)&crc, 2);
//  //u32ReturnBufferIndex += 2;
//  	memcpy((u8*)&CRC_Buffer[0], (u8*)&crc, 2);
//  return testCRC;
	
	
	
}

 

///*******************************************************************************************************************//**
//  * 功能   : 向指定地址 写入APP_BIN 数据   
//  * saddr  ：APP_BIN 数据存储开始地址 
//  * length ：APP_BIN 文件的数据的长度
//  * crc    :APP_BIN数据的 crc16 的校验
//  * readFlashAddr : 读取指定地址的数据  IAP_APFLASH_START / SECOND_APP_CODE_START
//  * 备注 每次只能烧录64个字节数据  直到写完为止
//    获取数据在 Get_Data()函数中 执行一次 Get_Data函数 获取64个字节数据 存放在copyBuffer 中 即可 
//    备注 如果APP_BIN 文件 已经下载4G模块的中 ，只需修改 Get_Data 函数 给4G模块 发送指令 要64个字节存在copyBuffer中， 
//         每执行一次Get_Data 函数 获取64个字节数据 存放在copyBuffer 中 即可

//***********************************************************************************************************************/
 u32 IAP_Write_Flash(u32 saddr, u32 length,u32 readFlashAddr,u32 crc)
{	
	u32 APP_Saddr=readFlashAddr;//IAP_APFLASH_START
	u32 tmp_crc=0;
	u32 result=0;
	u32 eaddrr=saddr+length-1;
	u32 write_saddr=saddr;
	u32 write_eaddr=saddr+63;
	
	//result=_ZXW_IAP_Erase(IAP_PAGE_ERASE,saddr,eaddr);// 擦除FLASH  存放APP_BIN 文件的地址空间
	if(saddr==SECOND_APP_CODE_START)
	{
	  result=_ZXW_IAP_Erase(IAP_PAGE_ERASE,saddr,eaddrr,1);// 擦除FLASH  存放APP_BIN 文件的地址空间
	}
	else
	{
	   result=_ZXW_IAP_Erase(IAP_PAGE_ERASE,saddr,eaddrr,0);// 擦除FLASH  存放APP_BIN 文件的地址空间
	}
  if(result==CMD_SUCCESS)// 擦除FLASH 成功 
	{
		  while(write_saddr<eaddr)     
			{       
				 // WDT_Restart(); 进行 喂狗  
				//*==================stat 获取64个字节数据==========================================================
				  // 循环一次 获取64个字节 来存储copyBuffer中
          Get_Data(APP_Saddr);
				  APP_Saddr= APP_Saddr +64;
				//=======================end 获取64个字节数据========================================================
				
	        result=_ZXW_IAP_Flash(CMD_PROGRAM,write_saddr,write_eaddr,copyBuffer);
				  if(result!=CMD_SUCCESS)
					{
					   return CMD_FAILED;
					}
          write_saddr=write_eaddr+1;
	        write_eaddr=write_saddr+63;
					if(write_eaddr>=eaddr)
					{
					   write_eaddr=eaddr-1;
					}			
			}	
	}else
	  {
		
		   return CMD_FAILED;
		}	
	// 进行 CRC16 数据校验
		tmp_crc=_ZXW_IAP_CRC(0, saddr, length);
   	if(tmp_crc==crc)
		{
				result=app_set_iap_flag(tmp_crc,length,1);
				if(result==CMD_SUCCESS)// 2á3yFLASH 3é1| 
			{
				 return CMD_SUCCESS;
			}
		 
			return CMD_FAILED;
  }else
   {
        return CMD_FAILED;
   }
}


 void get_iap_info(void)
{
    read_Flash_Data(SECOND_APP_INFO_START);
	  memcpy((u8*)&ZXW_IAP_Info.crc_value, (u8*)&copyBuffer[0], 4);
    memcpy((u8*)&ZXW_IAP_Info.lenth, (u8*)&copyBuffer[4], 4);
	  memcpy((u8*)&ZXW_IAP_Info.isInputIapData, (u8*)&copyBuffer[8], 4); 
}



 void get_AP_info(void)
{
    read_Flash_Data(SECOND_APP_INFO_START);
//	  memcpy((u8*)&ZXW_INFO.crc_value, (u8*)&copyBuffer[0], 4);
//    memcpy((u8*)&ZXW_INFO.lenth, (u8*)&copyBuffer[4], 4);
//	  memcpy((u8*)&ZXW_INFO.isInputIapData, (u8*)&copyBuffer[8], 4); 
		
		memcpy((u8*)&info_value, (u8*)&copyBuffer[0], 4);
    memcpy((u8*)&info_lenth, (u8*)&copyBuffer[4], 4);
	  memcpy((u8*)&info_input_data, (u8*)&copyBuffer[8], 4); 
}

