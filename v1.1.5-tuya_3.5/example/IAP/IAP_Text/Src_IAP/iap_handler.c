/*********************************************************************************************************//**
 * @file    IAP/IAP_Text/Src_IAP/iap_handler.c
 * @version $Rev:: 1720         $
 * @date    $Date:: 2017-08-23 #$
 * @brief   This file contains IAP handler.
 *************************************************************************************************************
 * @attention
 *
 * Firmware Disclaimer Information
 *
 * 1. The customer hereby acknowledges and agrees that the program technical documentation, including the
 *    code, which is supplied by Holtek Semiconductor Inc., (hereinafter referred to as "HOLTEK") is the
 *    proprietary and confidential intellectual property of HOLTEK, and is protected by copyright law and
 *    other intellectual property laws.
 *
 * 2. The customer hereby acknowledges and agrees that the program technical documentation, including the
 *    code, is confidential information belonging to HOLTEK, and must not be disclosed to any third parties
 *    other than HOLTEK and the customer.
 *
 * 3. The program technical documentation, including the code, is provided "as is" and for customer reference
 *    only. After delivery by HOLTEK, the customer shall use the program technical documentation, including
 *    the code, at their own risk. HOLTEK disclaims any expressed, implied or statutory warranties, including
 *    the warranties of merchantability, satisfactory quality and fitness for a particular purpose.
 *
 * <h2><center>Copyright (C) Holtek Semiconductor Inc. All rights reserved</center></h2>
 ************************************************************************************************************/

/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32.h"
#include "ht32_board.h"
#include "ht32_board_configs.h"
#include "iap_handler.h"
#include "iap_xmodem.h"
#include "iap_buffer.h"
#include "iap_crc16.h"

/** @addtogroup HT32_Series_Peripheral_Examples HT32 Peripheral Examples
  * @{
  */

/** @addtogroup IAP_Examples IAP
  * @{
  */

/** @addtogroup Text_mode Text mode
  * @{
  */

/** @addtogroup Text_mode_IAP IAP
  * @{
  */


/* Private function prototypes -----------------------------------------------------------------------------*/
void IAP_GoCMD(u32 address);
static void IAP_Download(void);
#if defined (__ICCARM__)
static int PutString(const char *pString);
#define printf PutString
extern int fputc (int ch, FILE *f);
#endif

/* Global functions ----------------------------------------------------------------------------------------*/
/*********************************************************************************************************//**
  * @brief  跳转到用户应用程序。
  * @retval None
  ***********************************************************************************************************/
void IAP_Go(void)
{
  u32 SP, PC;

  /*--------------------------------------------------------------------------------------------------------*/
  /* Check Stack Point in range检查范围内的堆栈点   ((u32)0x20000000) ~ (((u32)0x20000000) + 8192 )                                                                         */
  /*--------------------------------------------------------------------------------------------------------*/
  SP = rw(IAP_APFLASH_START);
  if (SP < IAP_APSRAM_START || SP > IAP_APSRAM_END)
  {
    return;      /* Stack Point is not in range, return and start IAP mode                                  */
  }

  /*--------------------------------------------------------------------------------------------------------*/
  /* Check PC in range   检查PC范围   4096~64511                                                                                 */
  /*--------------------------------------------------------------------------------------------------------*/
  PC = rw(IAP_APFLASH_START + 0x4);
  if (PC < IAP_APFLASH_START || PC > IAP_APFLASH_END)
  {
    return;      /* Program Counter is not in range, return and start IAP mode 
                    程序计数器不在范围内，返回并启动IAP模式                             */
  }

  /*--------------------------------------------------------------------------------------------------------*/
  /* SP and PC is correct, jump to user application  
      SP和PC正确，跳至用户应用程序                                                       */
  /*--------------------------------------------------------------------------------------------------------*/

  NVIC_DisableIRQ(HTCFG_UART_IRQn);   /* IAP UART SETTING: Disable UxRT interrupt to prevent unwanted interrupt
                                          IAP UART设置：禁用UxRT中断以防止意外中断 */
  USART_DeInit(HTCFG_UART_PORT);      /* IAP UART SETTING:                                                  */

  NVIC_SetVectorTable(NVIC_VECTTABLE_FLASH, IAP_APFLASH_START); /* Set Vector Table Offset                  */
  IAP_GoCMD(IAP_APFLASH_START);

  return;
}

#if defined (__CC_ARM)
/*********************************************************************************************************//**
  * @brief  Jump to user application by change PC.
  * @param  address: Start address of user application
  * @retval None
  ***********************************************************************************************************/
__asm void IAP_GoCMD(u32 address)
{
  LDR R1, [R0]
  MOV SP, R1
  LDR R1, [R0, #4]
  BX R1
}
#elif defined (__ICCARM__)
void IAP_GoCMD(u32 address)
{
  __asm("LDR R1, [R0]");
  __asm("MOV SP, R1");
  __asm("LDR R1, [R0, #4]");
  __asm("BX R1");
}
#endif

/*********************************************************************************************************//**
  * @brief  Download image to Flash.
  * @retval None
  ***********************************************************************************************************/
static void IAP_Download(void)
{
  u32 i;
  u32 result;
  u8 IAP_Buffer[134];                      /* Packet size 133 + 1 since Xmodem data put from 1 ~ 133        */

  //notes\r\nErasing Flash! Please Wait....\r\n");
  /*--------------------------------------------------------------------------------------------------------*/
  /* Erase Flash for application                                                                            */
  /*--------------------------------------------------------------------------------------------------------*/
  for (i = IAP_APFLASH_START; i < IAP_APFLASH_END; i += FLASH_PAGE_SIZE)
  {
    FLASH_ErasePage(i);
  }

  /*--------------------------------------------------------------------------------------------------------*/
  /* Start file download and programming                                                                    */
  /*--------------------------------------------------------------------------------------------------------*/
  result = xmodem(IAP_Buffer, IAP_APFLASH_START, XMODEM_PROGRAM);

  if (result == X_SSUCCESS)
  {
    //notes\r\n\r\nSuccessful!\r\n");
  }
  else
  {
    //notes\r\n\r\nFailed!\r\n");
    while (1);
  }

}

/*********************************************************************************************************//**
  * @brief  IAP mode handler.
  * @retval None
  ***********************************************************************************************************/
void IAP_Handler(void)
{
  u8 input;
  u32 isFlashProtection = FALSE;
  FLASH_OptionByte Option;

  /*--------------------------------------------------------------------------------------------------------*/
  /* Reset BOOT_MODE which set by user's application, to avoid start IAP again                              */
  /*--------------------------------------------------------------------------------------------------------*/
  BOOT_MODE = 0x0;

  while (1)
  {
    /* Get Option Byte status                                                                               */
    FLASH_GetOptionByteStatus(&Option);

    /* Check Application page is protection or not                                                          */
    isFlashProtection = Option.WriteProtect[0];
    isFlashProtection |= Option.WriteProtect[1];
    isFlashProtection |= Option.WriteProtect[2];
    isFlashProtection |= Option.WriteProtect[3];

    //notes\r\n\r\nHoltek IAP Example Code\r\n");
    //notes [1] Execute The New Program\r\n");
    //notes [2] Disable Write Protection\r\n");
    //notes [3] Update AP\r\n");

    input = ReadBuffer();
    //notes\r\n");

    switch (input)
    {
      case '1':
      {
        IAP_Go();
        //notes\r\nImage format is incorrect!\r\n");
        break;
      }
      case '2':
      {
        if (isFlashProtection != 0)
        {
          if (Option.MainSecurity == ENABLE || Option.OptionProtect == ENABLE)
          {
            //notesSecurity or Option Protection is enabled. Can not modify Option.\r\n");
          }
          else
          {
            FLASH_ErasePage(OPTION_BYTE_BASE);
            //Option.WriteProtect[0] = 0;
            //Option.WriteProtect[1] = 0;
            //Option.WriteProtect[2] = 0;
            //Option.WriteProtect[3] = 0;
            //FLASH_ProgramOptionByte(&Option);

            //notesWrite Protection disabled. Trigger reset to reload Option Byte\r\n");
            BOOT_MODE = BOOT_MODE_IAP;

            NVIC_SystemReset();
          }
        }
        else
        {
          //notesWrite Protection is not enabled!\r\n");
        }
        break;
      }
      case '3':
      {
        if (isFlashProtection == 0)
        {
          IAP_Download();
        }
        else
        {
          //notes\r\nWrite Protection is enabled. Can not update image.\r\n");
        }
        break;
      }
    }
  }
}

#if defined (__ICCARM__)
/*********************************************************************************************************//**
 * @brief Put string.
 * @param pString: String.
 * @retval String length.
 ************************************************************************************************************/
static int PutString(const char *pString)
{
  int i = 0;
  while (*pString != '\0')
  {
    fputc(*pString, (FILE *)1);
    pString++;
    i++;
  }

  return i;
}
#endif


/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
