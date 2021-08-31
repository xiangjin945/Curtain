#include "main_callfun.h"

uint32_t image_lenth;  /**< Size of the image in the bank. */
uint32_t image_crc;    /**< CRC of the image. If set to 0, the CRC is ignored. */
uint32_t bank_version; /**< Identifier code for the bank. */
uint32_t md5_buf[4];

uint32_t updata_info;

u8 copyBuffer[65];
int fputc(int ch, FILE *f)
{
    USART_SendData(HT_USART0, ch);
    while (USART_GetFlagStatus(HT_USART0, USART_FLAG_TXC) == RESET)
        ;
    return ch;
}

void read_Flash_Data(u32 addr)
{
    u32 data = 0;
    u32 u32ReturnBufferIndex = 0;
    u8 i = 0;
    for (i = 0; i < 16; i++) //0 4 8 12 16   20 24 28 32    36 40 44 48   52 56  60
    {
        data = rw(addr);
        memcpy((u8 *)&copyBuffer[u32ReturnBufferIndex], (u8 *)&data, 4);
        u32ReturnBufferIndex += 4;
        addr += 4;
    }
}
void get_AP_info(void)
{
    read_Flash_Data(APP_NEW_FW_SETTING_ADR + 72);
    //	  memcpy((u8*)&ZXW_INFO.crc_value, (u8*)&copyBuffer[0], 4);
    //    memcpy((u8*)&ZXW_INFO.lenth, (u8*)&copyBuffer[4], 4);
    //	  memcpy((u8*)&ZXW_INFO.isInputIapData, (u8*)&copyBuffer[8], 4);

    memcpy((u8 *)&bank_version, (u8 *)&copyBuffer[0], 4);
    memcpy((u8 *)&image_lenth, (u8 *)&copyBuffer[4], 4);
    memcpy((u8 *)&image_crc, (u8 *)&copyBuffer[8], 4);
    memcpy((u8 *)&md5_buf, (u8 *)&copyBuffer[12], 16);
    read_Flash_Data(APP_NEW_FW_SETTING_ADR + 1024);
    memcpy((u8 *)&updata_info, (u8 *)&copyBuffer[0], 4);
}

uint8_t mcu_flash_read_jiaoyan(uint32_t addr, uint8_t *p_data, uint32_t size)
{
    uint32_t data = 0;
    u32 u32ReturnBufferIndex = 0;
    u8 i = 0;
    for (i = 0; i < size; i += 4)
    {
        data = rw(addr);
        memcpy((uint32_t *)&p_data[u32ReturnBufferIndex], &data, 4);
        u32ReturnBufferIndex += 4;
        addr += 4;
    }
}

static uint32_t crc32_compute(uint8_t const *p_data, uint32_t size, uint32_t const *p_crc)
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
/*IAP通过长度去指定地址读取数据返回crc的值*/
uint8_t CMP_IAP_CRC(uint32_t len, uint32_t *crc, u32 addr)
{

    static uint8_t buf[UPDATA_SIZE] = {0};
    if (len == 0)
    {
        return 1;
    }
    uint32_t crc_temp = 0;
    uint32_t read_addr = addr;
    uint32_t cnt = len / UPDATA_SIZE;

    uint32_t remainder = len % UPDATA_SIZE;

    for (uint32_t i = 0; i < cnt; i++)
    {
        TUYA_OTA_LOG("%d", i);
        mcu_flash_read_jiaoyan(read_addr, buf, UPDATA_SIZE);
        crc_temp = crc32_compute(buf, UPDATA_SIZE, &crc_temp);
        read_addr += UPDATA_SIZE;
    }

    if (remainder > 0 && remainder < UPDATA_SIZE)
    {

        TUYA_OTA_LOG("%d", remainder);
        mcu_flash_read_jiaoyan(read_addr, buf, remainder);
        crc_temp = crc32_compute(buf, remainder, &crc_temp);

        read_addr += remainder;
    }

    *crc = crc_temp;
    return 0;
}

