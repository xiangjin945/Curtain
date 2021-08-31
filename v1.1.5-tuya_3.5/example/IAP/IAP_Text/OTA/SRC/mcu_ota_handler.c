/****************************************Copyright (c)*************************
**                               版权所有 (C), 2015-2020, 涂鸦科技
**
**                                 http://www.tuya.com
**
**
**--------------版本修订记录---------------------------------------------------

** 版  本:v2.0
** 日　期: 2020年3月23日
** 描　述: 
1.	增加cmd 0x09模块解绑接口支持
2.	增加cmd 0x0e rf射频测试接口支持
3.	增加cmd 0xe0 记录型数据上报接口支持
4.	增加cmd 0xE1 获取实时时间接口支持
5.	增加 cmd 0xe2 修改休眠模式状态广播间隔支持
6.	增加 cmd 0xe4 关闭系统时钟功能支持
7.	增加 cmd 0xe5 低功耗使能支持
8.	增加 cmd 0xe6 获取一次性动态密码支持
9.	增加 cmd 0xe7断开蓝牙连接支持
10.	增加 cmd 0xe8 查询MCU版本号支持
11.	增加 cmd 0xe9 MCU主动发送版本号支持
12.	增加 cmd 0xea OTA升级请求支持
13.	增加 cmd 0xeb OTA升级文件信息支持
14.	增加 cmd 0xec OTA升级文件偏移请求支持
15.	增加 cmd 0xed OTA升级数据支持
16.	增加 cmd 0xee OTA升级结束支持
17.	增加 cmd 0xa0 MCU 获取模块版本信息支持
18.	增加 cmd 0xa1 恢复出厂设置通知支持
19.  增加MCU OTA demo
20. 优化串口解析器
**
**-----------------------------------------------------------------------------
******************************************************************************/

/******************************************************************************
                          特别注意！！！      
MCU OTA的方式和芯片强相关，该MCU OTA程序demo不一定适用所有芯片平台，但大同小异，用户可根据自己芯片平台的升级方式对该demo进行修改或参考该demo自行编写完成MCU OTA功能

******************************************************************************/
#include "ht32f5xxxx_flash.h"
#include "mcu_ota_handler.h"
#include "string.h"
#include "bluetooth.h"
#include "stdbool.h"
#include "main_callfun.h"
#include "gptm0_init.h"

#ifdef SUPPORT_MCU_FIRM_UPDATE
#define IAP_CODE_SIZE                 (1024 * 4)
#define IAP_PAGE_ERASE          (0x8)
#define CMD_SUCCESS             ('O')
#define CMD_FAILED              ('F')
#define CMD_PROGRAM             (1)
#define IAP_MASS_ERASE          (0xA)

void usart_fun(u8 data){
//    USART_SendData(HT_USART0,data);
//    while (USART_GetFlagStatus(HT_USART0, USART_FLAG_TXC) == RESET);
}

static dfu_settings_t s_dfu_settings;
/*****************************************************************************
函数名称 : mcu_flash_init
功能描述 :flash初始化函数
输入参数 :

返回参数 : 无
使用说明 : 用户需要将flash初始化函数在此完善，如果在其他处有flash初始化操作，该函数可以不被调用
*****************************************************************************/
uint8_t mcu_flash_init(void)
{
	//#error "è¯·è‡ªè¡Œå®Œå–„è¯¥åŠŸèƒ½,å®Œæˆ�å�Žè¯·åˆ é™¤è¯¥è¡Œ"
    
}
/*****************************************************************************
函数名称 : mcu_flash_erase
功能描述 :flash擦除函数
输入参数 :addr 地址 size 大小

返回参数 : 无
使用说明 : 用户自行完善
*****************************************************************************/
uint8_t mcu_flash_erase(uint32_t addr,uint32_t size)
{
	//#error "è¯·è‡ªè¡Œå®Œå–„è¯¥åŠŸèƒ½,å®Œæˆ�å�Žè¯·åˆ é™¤è¯¥è¡Œ"
    uint32_t i;
    uint32_t addr_end = addr + size;
    for (i = addr; i < addr_end; i += FLASH_PAGE_SIZE)
	{
		FLASH_ErasePage(i);
	}
}
/*****************************************************************************
函数名称 : mcu_flash_write
功能描述 :flash写函数
输入参数 :addr 地址 size 大小 p_data待写入数据地址

返回参数 : 无
使用说明 : 用户自行完善
*****************************************************************************/

uint8_t mcu_flash_write(uint32_t addr, uint8_t *p_data, uint32_t size)
{
    u32 i;
    uint32_t eaddr = addr + size; 
    u8 *buffer = p_data;
    FLASH_OptionByte Option;

    FLASH_GetOptionByteStatus(&Option);

    /* Blank Check before programming                                                                       */
    for (i = 0; i < size; i += 4)
    {
      if (rw(addr + i) != 0xFFFFFFFF )
      {
        //gu8ReturnBuffer[3] = 0x4;
        return CMD_FAILED;
      }
    }

    while (addr < eaddr)
    {
      u32 tmp;
      memcpy((u8*)&tmp, (u8*)buffer, 4);
      FLASH_ProgramWordData(addr, tmp);
      addr += 4;
      buffer += 4;
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

uint8_t mcu_flash_read(uint32_t addr, uint8_t *p_data, uint32_t size)
{
	uint32_t data =0;
	u32 u32ReturnBufferIndex=0;
	u8 i=0;
	for(i=0;i<size;i+= 4)
	{
	    data = rw(addr);
		memcpy((u8 *)&p_data[u32ReturnBufferIndex], (u8*)&data, 4);
    u32ReturnBufferIndex += 4;
		addr += 4;
	}
}
/*****************************************************************************
函数名称 : mcu_device_delay_restart
功能描述 :延迟重启函数，建议延迟500ms重启，以等待mcu完成一些必要的操作
输入参数 : 

返回参数 : 无
使用说明 : 用户自行完善
*****************************************************************************/
//延时函数
void delay1s(void)   
{
    unsigned int a,b;
    for(b=10100;b>0;b--)
        for(a=1200;a>0;a--);
}
void mcu_device_delay_restart(void)
{
    delay1s();
	//error "è¯·è‡ªè¡Œå®Œå–„è¯¥åŠŸèƒ½,å®Œæˆ�å�Žè¯·åˆ é™¤è¯¥è¡Œ"
    NVIC_SystemReset();
}







// static dfu_settings_t s_dfu_settings;

static volatile mcu_ota_status_t tuya_ota_status;
void mcu_ota_status_set(mcu_ota_status_t status)
{
	tuya_ota_status = status;
}
mcu_ota_status_t mcu_ota_status_get(void)
{
	return tuya_ota_status;
}


#define MAX_DFU_DATA_LEN  200
#define CODE_PAGE_SIZE	4096
#define MAX_DFU_BUFFERS   ((CODE_PAGE_SIZE / MAX_DFU_DATA_LEN) + 1)


static uint32_t m_firmware_start_addr;          /**< Start address of the current firmware image. */
static uint32_t m_firmware_size_req;


static uint16_t current_package = 0;
static uint16_t last_package = 0;

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

static void mcu_ota_start_req(uint8_t*recv_data,uint32_t recv_len)
{
    uint8_t p_buf[12];
    uint8_t payload_len = 0;
    uint32_t current_version = MCU_OTA_VERSION;
    //USART0_SendData(HT_USART0,current_version);
	uint16_t length = 0;

    if(mcu_ota_status_get()!=MCU_OTA_STATUS_NONE)
    {
        TUYA_OTA_LOG("current ota status is not MCU_OTA_STATUS_NONE  and is : %d !",mcu_ota_status_get());
        return;
    }

    p_buf[0] = MCU_OTA_TYPE;
    p_buf[1] = (current_version>>16)&0xff;
    p_buf[2] = (current_version>>8)&0xff;
    p_buf[3] = current_version&0xff;

    p_buf[4] = MAX_DFU_DATA_LEN>>8;
    p_buf[5] = MAX_DFU_DATA_LEN;
    
    mcu_ota_status_set(MCU_OTA_STATUS_START);
    payload_len = 6;

	length = set_bt_uart_buffer(length,(unsigned char *)p_buf,payload_len);
	bt_uart_write_frame(TUYA_BCI_UART_COMMON_MCU_OTA_REQUEST,length);
}

//这个函数会给crc_temp一个值 之后赋值给s_dfu_settings.progress.firmware_image_crc_last
static uint8_t file_crc_check_in_flash(uint32_t len,uint32_t *crc)
{

    static uint8_t buf[UPDATA_SIZE]={0};
    if(len == 0)
    {
        return 1;
    }
    uint32_t crc_temp = 0;
    uint32_t read_addr = APP_NEW_FW_START_ADR;
    uint32_t cnt = len/UPDATA_SIZE;


    uint32_t remainder = len % UPDATA_SIZE;

    for(uint32_t i = 0; i<cnt; i++)
    {
        TUYA_OTA_LOG("%d",i);
        mcu_flash_read(read_addr,buf,UPDATA_SIZE);
        crc_temp = crc32_compute(buf, UPDATA_SIZE, &crc_temp);
        read_addr += UPDATA_SIZE;
        
    }

    if(remainder>0&&remainder<UPDATA_SIZE)
    {
        
        TUYA_OTA_LOG("%d",remainder);
        mcu_flash_read(read_addr,buf,remainder);
        crc_temp = crc32_compute(buf, remainder, &crc_temp);
   
        read_addr += remainder;

    }
    
    *crc = crc_temp;
    return 0;
}



//0xEB
static void mcu_ota_file_info_req(uint8_t*recv_data,uint32_t recv_len)
{
    uint8_t p_buf[30];
    uint8_t payload_len = 0;
    uint32_t file_version;
    uint32_t file_length;
    uint32_t file_crc;
    bool file_md5;
    // uint8_t file_md5[16];
    uint16_t length = 0;
    uint8_t state;

    if(mcu_ota_status_get()!=MCU_OTA_STATUS_START)
    {
        TUYA_OTA_LOG("current ota status is not MCU_OTA_STATUS_START  and is : %d !",mcu_ota_status_get());
        return;
    }

    file_version = recv_data[0+8]<<16;
    file_version += recv_data[1+8]<<8;
    file_version += recv_data[2+8];

    if(memcmp(s_dfu_settings.progress.firmware_file_md5,&recv_data[3+8],16)==0)
    {
        file_md5 = true;
    }
    else
    {
        file_md5 = false;
    }

    file_length = recv_data[27]<<24;
    file_length += recv_data[28]<<16;
    file_length += recv_data[29]<<8;
    file_length += recv_data[30];

    file_crc = recv_data[31]<<24;
    file_crc += recv_data[32]<<16;
    file_crc += recv_data[33]<<8;
    file_crc += recv_data[34];

    if (memcmp(&recv_data[0], PRODUCT_KEY, 8) == 0)
    {
       
        if((file_version > MCU_OTA_VERSION)&&(file_length <= APP_NEW_FW_MAX_SIZE))
        {

            if(file_md5&&(s_dfu_settings.progress.firmware_file_version==file_version)&&(s_dfu_settings.progress.firmware_file_length==file_length)
                    &&(s_dfu_settings.progress.firmware_file_crc==file_crc))
            {
                state = 0;
            }
            else
            {
                memset(&s_dfu_settings.progress, 0, sizeof(dfu_progress_t));
                s_dfu_settings.progress.firmware_image_crc_last = 0;
                s_dfu_settings.progress.firmware_file_version = file_version;
                s_dfu_settings.progress.firmware_file_length = file_length;
                s_dfu_settings.progress.firmware_file_crc = file_crc;
                memcpy(s_dfu_settings.progress.firmware_file_md5,&recv_data[3+8],16);
                s_dfu_settings.write_offset = s_dfu_settings.progress.firmware_image_offset_last;
                state = 0;
                mcu_flash_write(DFU_SETTING_SAVE_ADDR,(uint8_t*)&s_dfu_settings,sizeof(s_dfu_settings));
            }

            m_firmware_start_addr = APP_NEW_FW_START_ADR;
            m_firmware_size_req = s_dfu_settings.progress.firmware_file_length;

        }
        else
        {
            if(file_version <= MCU_OTA_VERSION)
            {
                TUYA_OTA_LOG("ota file version error !");
                state = 2;
            }
            else
            {
                TUYA_OTA_LOG("ota file length is bigger than rev space !");
                state = 3;
            }
        }

    }
    else
    {
        usart_fun(0xdd);
        TUYA_OTA_LOG("ota pid error !");
        state = 1;//æ­¤å¤„æ˜¯1çš„.
    }

    memset(p_buf,0,sizeof(p_buf));
    p_buf[0] = state;
    if(state==0)
    {
        uint32_t crc_temp  = 0;
        if(file_crc_check_in_flash(s_dfu_settings.progress.firmware_image_offset_last,&crc_temp)==0)
        {
            if(crc_temp != s_dfu_settings.progress.firmware_image_crc_last)
            {
                s_dfu_settings.progress.firmware_image_offset_last = 0;
                s_dfu_settings.progress.firmware_image_crc_last = 0;
                s_dfu_settings.write_offset = s_dfu_settings.progress.firmware_image_offset_last;
                mcu_flash_write(DFU_SETTING_SAVE_ADDR,(uint8_t*)&s_dfu_settings,sizeof(s_dfu_settings));
            }
        }

        p_buf[1] = s_dfu_settings.progress.firmware_image_offset_last>>24;
        p_buf[2] = s_dfu_settings.progress.firmware_image_offset_last>>16;
        p_buf[3] = s_dfu_settings.progress.firmware_image_offset_last>>8;
        p_buf[4] = (uint8_t)s_dfu_settings.progress.firmware_image_offset_last;
        
        p_buf[5] = s_dfu_settings.progress.firmware_image_crc_last>>24;
        p_buf[6] = s_dfu_settings.progress.firmware_image_crc_last>>16;
        p_buf[7] = s_dfu_settings.progress.firmware_image_crc_last>>8;
        p_buf[8] = (uint8_t)s_dfu_settings.progress.firmware_image_crc_last;
        mcu_ota_status_set(MCU_OTA_STATUS_FILE_INFO);
        current_package = 0;
        last_package = 0;

        TUYA_OTA_LOG("ota file length  : 0x%04x",s_dfu_settings.progress.firmware_file_length);
        TUYA_OTA_LOG("ota file  crc    : 0x%04x",s_dfu_settings.progress.firmware_file_crc);
        TUYA_OTA_LOG("ota file version : 0x%04x",s_dfu_settings.progress.firmware_file_version);
        TUYA_OTA_LOG("ota firmware_image_offset_last : 0x%04x",s_dfu_settings.progress.firmware_image_offset_last);
        TUYA_OTA_LOG("ota firmware_image_crc_last    : 0x%04x",s_dfu_settings.progress.firmware_image_crc_last);
        TUYA_OTA_LOG("ota firmware   write offset    : 0x%04x",s_dfu_settings.write_offset);

    }
    payload_len = 25;

	length = set_bt_uart_buffer(length,(unsigned char *)p_buf,payload_len);
	bt_uart_write_frame(TUYA_BCI_UART_COMMON_MCU_OTA_FILE_INFO,length);


}


//ec
static void mcu_ota_offset_req(uint8_t*recv_data,uint32_t recv_len)
{
    uint8_t p_buf[5];
    uint8_t payload_len = 0;
    uint32_t offset;
	uint16_t length = 0;
	
    if(mcu_ota_status_get()!=MCU_OTA_STATUS_FILE_INFO)
    {
        TUYA_OTA_LOG("current ota status is not MCU_OTA_STATUS_FILE_INFO  and is : %d !",mcu_ota_status_get());
        return;
    }


    offset  = recv_data[0]<<24;
    offset += recv_data[1]<<16;
    offset += recv_data[2]<<8;
    offset += recv_data[3];

    if((offset==0)&&(s_dfu_settings.progress.firmware_image_offset_last!=0))
    {
        s_dfu_settings.progress.firmware_image_crc_last = 0;
        s_dfu_settings.progress.firmware_image_offset_last = 0;
        s_dfu_settings.write_offset = s_dfu_settings.progress.firmware_image_offset_last;
        mcu_flash_write(DFU_SETTING_SAVE_ADDR,(uint8_t*)&s_dfu_settings,sizeof(s_dfu_settings));
    }

    p_buf[0] = s_dfu_settings.progress.firmware_image_offset_last>>24;
    p_buf[1] = s_dfu_settings.progress.firmware_image_offset_last>>16;
    p_buf[2] = s_dfu_settings.progress.firmware_image_offset_last>>8;
    p_buf[3] = (uint8_t)s_dfu_settings.progress.firmware_image_offset_last;

    mcu_ota_status_set(MCU_OTA_STATUS_FILE_OFFSET);

    payload_len = 4;

	length = set_bt_uart_buffer(length,(unsigned char *)p_buf,payload_len);
	bt_uart_write_frame(TUYA_BCI_UART_COMMON_MCU_OTA_FILE_OFFSET,length);


}


//0xED升级数据传输
static void mcu_ota_data_req(uint8_t*recv_data,uint32_t recv_len)
{
	// TUYA_OTA_LOG("%s",__func__);
    uint8_t p_buf[2];
    uint8_t payload_len = 0;
    uint8_t state = 0;
    uint16_t len;
    uint8_t p_balloc_buf[256];
    uint16_t length = 0;
    gptm0_4low = 0;
  
    
    if((mcu_ota_status_get()!=MCU_OTA_STATUS_FILE_OFFSET)&&(mcu_ota_status_get()!=MCU_OTA_STATUS_FILE_DATA))
    {
        usart_fun(0xdd);
        //TUYA_OTA_LOG("current ota status is not MCU_OTA_STATUS_FILE_OFFSET  or MCU_OTA_STATUS_FILE_DATA and is : %d !",mcu_ota_status_get());
        return;
    }

    state = 0;


    current_package = (recv_data[0]<<8)|recv_data[1];       /*数据包号*/
    len = (recv_data[2]<<8)|recv_data[3];                   /*数据包长度*/

    // USART_SendData(HT_USART0,current_package);
  

    if((current_package!=(last_package+1))&&(current_package!=0))
    {
       usart_fun(0xdd);
        //TUYA_OTA_LOG("ota received package number error.received package number : %d",current_package);
        state = 1;
    }
    else  if(len>MAX_DFU_DATA_LEN)
    {
        
        //TUYA_OTA_LOG("ota received package data length error : %d",len);
        state = 5;
    }
    else
    {
        uint32_t const write_addr = APP_NEW_FW_START_ADR +  s_dfu_settings.write_offset;

        if(write_addr>=APP_NEW_FW_END_ADR)
        {
            
            //TUYA_OTA_LOG("ota write addr error.");
            state = 1;
        }

        if(write_addr%CODE_PAGE_SIZE==0)
        {
            if (mcu_flash_erase(write_addr,4096)!= 0)
            {
                //notesqita err!\n");
                //TUYA_OTA_LOG("ota Erase page operation failed");
                state = 4;
            }
        }

        if(state==0)
        {

            len = (recv_data[2]<<8)|recv_data[3];
             //USART_SendData(HT_USART0,len);

            memcpy(p_balloc_buf, &recv_data[6], len);
            
            int i;
            // for(i=0;i<len;i++){
            //     usart_fun(p_balloc_buf[i]);
            // }

            /*flash写函数*/
            uint8_t ret = mcu_flash_write(write_addr, p_balloc_buf, len);
			//TUYA_OTA_LOG("ota save len :%d",len);
            //uint8_t ret = 0;
            if (ret == CMD_FAILED)
            {
                //notesqita two err!\n");
                state = 4;
            }
            else
            {
                /*这里会将升级的信息写入定义好的宏地址，之后重启完后IAP会去读取这个地址的信息*/
                s_dfu_settings.progress.firmware_image_crc_last = crc32_compute(p_balloc_buf, len, &s_dfu_settings.progress.firmware_image_crc_last);
                s_dfu_settings.write_offset    += len;
                s_dfu_settings.progress.firmware_image_offset_last += len;

                if((current_package+1)%32==0)
                {
                    mcu_flash_write(DFU_SETTING_SAVE_ADDR,(uint8_t*)&s_dfu_settings,sizeof(s_dfu_settings));
                }


            }
			
        }

    }

    p_buf[0] = state;

    mcu_ota_status_set(MCU_OTA_STATUS_FILE_DATA);

    payload_len = 1;

	length = set_bt_uart_buffer(length,(unsigned char *)p_buf,payload_len);
	bt_uart_write_frame(TUYA_BCI_UART_COMMON_MCU_OTA_DATA,length);



    if(state!=0)//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ö¸ï¿½ï¿½ï¿½Ê¼×´Ì¬
    {
         usart_fun(0xdd);
        //TUYA_OTA_LOG("ota error so free!");
        mcu_ota_status_set(MCU_OTA_STATUS_NONE);
        mcu_ota_init_disconnect();
        memset(&s_dfu_settings, 0, sizeof(dfu_settings_t));
        mcu_flash_write(DFU_SETTING_SAVE_ADDR,(uint8_t*)&s_dfu_settings,sizeof(s_dfu_settings));
    }
    else
    {
        last_package = current_package;
    }


}


static void reset_after_flash_write(void * p_context)
{
    TUYA_OTA_LOG("start reset~~~.");
    mcu_device_delay_restart();
}

//这个函数将下载下来的固件包拷贝到片区1后，重启。
static void on_dfu_complete(void)
{
	u32 i;

	u32 info = BOOT_MODE_IAP;
		
    u32 size = s_dfu_settings.bank_1.image_size;
    for (i = APP_NEW_FW_SETTING_ADR+1024; i < APP_NEW_FW_SETTING_ADR+1025; i ++)
    {
        FLASH_ErasePage(i);
    }
		
		
    FLASH_ProgramWordData(APP_NEW_FW_SETTING_ADR+1024, info);
		

    // u32 crc = s_dfu_settings.bank_1.image_crc;
    //IAP_Flash_COPY(IAP_APFLASH_START,(uint8_t*)&s_dfu_settings.bank_1.image_size,SECOND_APP_CODE_START,(uint8_t*)&s_dfu_settings.bank_1.image_crc);

    reset_after_flash_write(NULL);
}


//ee结束时检查crc
static void on_data_write_request_sched(void * data)
{
    uint8_t ret;
    uint8_t p_buf[1];
    uint8_t payload_len = 0;
    uint8_t state;
	uint16_t length = 0;

    if (s_dfu_settings.progress.firmware_image_offset_last == m_firmware_size_req)
    {    
       // usart_fun(s_dfu_settings.progress.firmware_image_offset_last);
        uint32_t crc_temp  = 0;
        /*检查crc*/
        if(file_crc_check_in_flash(s_dfu_settings.progress.firmware_image_offset_last,&crc_temp)==0)
        {
            if(s_dfu_settings.progress.firmware_image_crc_last != crc_temp)
            {
              
                s_dfu_settings.progress.firmware_image_crc_last = crc_temp;
            }
            
        }

        if(s_dfu_settings.progress.firmware_image_crc_last!=s_dfu_settings.progress.firmware_file_crc)
        {
            state = 2;
        }
        else
        {
            s_dfu_settings.bank_1.image_crc = s_dfu_settings.progress.firmware_image_crc_last;
            s_dfu_settings.bank_1.image_size = m_firmware_size_req;
            s_dfu_settings.bank_1.bank_code = NRF_DFU_BANK_VALID_APP;

            memset(&s_dfu_settings.progress, 0, sizeof(dfu_progress_t));

            s_dfu_settings.write_offset                  = 0;
            s_dfu_settings.progress.update_start_address = APP_NEW_FW_START_ADR;

            state = 0;
        }

    }
    else
    {
        state = 1;
    }

    p_buf[0] = state;
    mcu_ota_status_set(MCU_OTA_STATUS_NONE);
    payload_len = 1;

	length = set_bt_uart_buffer(length,(unsigned char *)p_buf,payload_len);
	bt_uart_write_frame(TUYA_BCI_UART_COMMON_MCU_OTA_END,length);

    
    if(state==0)
    {
        //在此处打断点可以查看结构体中固件包的长度和校验码等信息
    	mcu_flash_write(DFU_SETTING_SAVE_ADDR,(uint8_t*)&s_dfu_settings,sizeof(s_dfu_settings)); //将升级固件包的信息写到指定地址
        on_dfu_complete();
    }
    else
    {
        mcu_ota_status_set(MCU_OTA_STATUS_NONE);
        mcu_ota_init_disconnect();
        memset(&s_dfu_settings, 0, sizeof(dfu_settings_t));
        mcu_flash_write(DFU_SETTING_SAVE_ADDR,(uint8_t*)&s_dfu_settings,sizeof(s_dfu_settings));
    }

}


//0xEE升级结束标志
static void mcu_ota_end_req(uint8_t*recv_data,uint32_t recv_len)
{
    if(mcu_ota_status_get()==MCU_OTA_STATUS_NONE)
    {
        TUYA_OTA_LOG("current ota status is MCU_OTA_STATUS_NONE!");
        return;
    }
    on_data_write_request_sched(NULL);

}

//OTA升级总函数
void mcu_ota_proc(uint16_t cmd,uint8_t*recv_data,uint32_t recv_len)
{
    //  USART_SendData(HT_USART0,cmd>>8);
    // USART_SendData(HT_USART0,cmd);
 
    switch(cmd)
    {
    case TUYA_BCI_UART_COMMON_MCU_OTA_REQUEST:
        mcu_ota_start_req(recv_data,recv_len);
        break;
    case TUYA_BCI_UART_COMMON_MCU_OTA_FILE_INFO:
        mcu_ota_file_info_req(recv_data,recv_len);
        break;
    case TUYA_BCI_UART_COMMON_MCU_OTA_FILE_OFFSET:
        mcu_ota_offset_req(recv_data,recv_len);
        break;
    case TUYA_BCI_UART_COMMON_MCU_OTA_DATA:
        mcu_ota_data_req(recv_data,recv_len);
        break;
    case TUYA_BCI_UART_COMMON_MCU_OTA_END:
        mcu_ota_end_req(recv_data,recv_len);
        break;
    default:
    	TUYA_OTA_LOG("tuya_ota_proc cmd err.");
        break;
    }

}


uint8_t mcu_ota_init_disconnect(void)
{
    if(mcu_ota_status_get() != MCU_OTA_STATUS_NONE)
    {
        mcu_flash_write(DFU_SETTING_SAVE_ADDR,(uint8_t*)&s_dfu_settings,sizeof(s_dfu_settings));
        mcu_ota_status_set(MCU_OTA_STATUS_NONE);
    }
    current_package = 0;
    last_package = 0;
	return 0;
}

uint32_t mcu_ota_init(void)
{
    mcu_ota_status_set(MCU_OTA_STATUS_NONE);

    current_package = 0;
    last_package = 0;
	//mcu_flash_read(DFU_SETTING_SAVE_ADDR,(uint8_t*)&s_dfu_settings,sizeof(s_dfu_settings));


    return 0;
}
#endif

