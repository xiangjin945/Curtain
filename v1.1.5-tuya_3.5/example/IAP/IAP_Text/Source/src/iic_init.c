#include "adc_init.h"

void adc_init(void)
{
    /* !!! NOTICE !!!
    Notice that the local variable (structure) did not have an initial value.
    Please confirm that there are no missing members in the parameter settings below in this function.
    */
    I2C_InitTypeDef I2C_InitStructure;

    /* Enable AFIO & I2C clock                                                                                */
    CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
    EEPROM_I2C_CLK(CKCUClock) = 1;
    CKCUClock.Bit.AFIO        = 1;
    CKCU_PeripClockConfig(CKCUClock, ENABLE);

    /* Configure I2C SCL pin, I2C SDA pin */
    HT32F_DVB_GPxConfig(GPIO_PC, EEPROM_I2C_SCL_AFIO_PIN, EEPROM_I2C_SCL_AFIO_MODE);
    HT32F_DVB_GPxConfig(GPIO_PC, EEPROM_I2C_SDA_AFIO_PIN, EEPROM_I2C_SDA_AFIO_MODE);

    /* I2C configuration */
    I2C_InitStructure.I2C_GeneralCall = I2C_GENERALCALL_DISABLE;
    I2C_InitStructure.I2C_AddressingMode = I2C_ADDRESSING_7BIT;
    I2C_InitStructure.I2C_Acknowledge = I2C_ACK_DISABLE;
    I2C_InitStructure.I2C_OwnAddress = 0x00;
    I2C_InitStructure.I2C_Speed = I2C_EEPROM_SPEED;
    I2C_InitStructure.I2C_SpeedOffset = 0;
    I2C_Init(EEPROM_I2C, &I2C_InitStructure);

    I2C_Cmd(EEPROM_I2C, ENABLE);
}
