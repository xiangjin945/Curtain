#include "gpio_init.h"

void led_blink(u8 on_off)
{
	if(on_off==0)
	{
		//GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_2,~GPIO_ReadOutBit(HT_GPIOB, GPIO_PIN_2));
		//GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_3,~GPIO_ReadOutBit(HT_GPIOB, GPIO_PIN_3));
		GPIO_WriteOutBits(HT_GPIOB, GPIO_PIN_4,RESET);
	}
	if(on_off==1)
	{
		//GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_2,~GPIO_ReadOutBit(HT_GPIOB, GPIO_PIN_2));
		//GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_3,~GPIO_ReadOutBit(HT_GPIOB, GPIO_PIN_3));
		GPIO_WriteOutBits(HT_GPIOB, GPIO_PIN_4,SET);
	}
}

void key_init(void)
{
	EXTI_InitTypeDef EXTI_InitStruct = {0};
	
	
	    //PC1 ÖÐ¶ÏÅäÖÃ
    AFIO_GPxConfig(GPIO_PC,            AFIO_PIN_1,  AFIO_FUN_DEFAULT);
    GPIO_DirectionConfig(HT_GPIOC     ,GPIO_PIN_1,  GPIO_DIR_IN);
    GPIO_PullResistorConfig(HT_GPIOC,  GPIO_PIN_1,  GPIO_PR_UP);
    GPIO_InputConfig(HT_GPIOC,         GPIO_PIN_1,  ENABLE);
    /* Connect Button EXTI Channel to Button GPIO Pin */
    AFIO_EXTISourceConfig(AFIO_EXTI_CH_1, AFIO_ESS_PC);
    /* Configure button EXTI Line on falling edge */
    EXTI_InitStruct.EXTI_Channel = EXTI_CHANNEL_1;
    EXTI_InitStruct.EXTI_Debounce = EXTI_DEBOUNCE_DISABLE;
    EXTI_InitStruct.EXTI_DebounceCnt = 0;
    EXTI_InitStruct.EXTI_IntType = EXTI_BOTH_EDGE;
    EXTI_Init(&EXTI_InitStruct);
    /* Enable Button EXTI Interrupt */
    EXTI_IntConfig(EXTI_CHANNEL_1, ENABLE);
    NVIC_EnableIRQ(EXTI0_1_IRQn);

    //PC2 ÖÐ¶ÏÅäÖÃ
    AFIO_GPxConfig(GPIO_PC, AFIO_PIN_2, AFIO_FUN_DEFAULT);
    GPIO_DirectionConfig(HT_GPIOC ,GPIO_PIN_2, GPIO_DIR_IN);
    GPIO_PullResistorConfig(HT_GPIOC, GPIO_PIN_2, GPIO_PR_UP);
    GPIO_InputConfig(HT_GPIOC, GPIO_PIN_2, ENABLE);
    /* Connect Button EXTI Channel to Button GPIO Pin */
    AFIO_EXTISourceConfig(AFIO_EXTI_CH_2, AFIO_ESS_PC);
    /* Configure button EXTI Line on falling edge */
    EXTI_InitStruct.EXTI_Channel = EXTI_CHANNEL_2;
    EXTI_InitStruct.EXTI_Debounce = EXTI_DEBOUNCE_DISABLE;
    EXTI_InitStruct.EXTI_DebounceCnt = 0;
    EXTI_InitStruct.EXTI_IntType = EXTI_BOTH_EDGE;
    EXTI_Init(&EXTI_InitStruct);
    /* Enable Button EXTI Interrupt */
    EXTI_IntConfig(EXTI_CHANNEL_2, ENABLE);
    NVIC_EnableIRQ(EXTI2_3_IRQn);

    //PB5 ÖÐ¶ÏÅäÖÃ
    AFIO_GPxConfig(GPIO_PB, AFIO_PIN_5, AFIO_FUN_DEFAULT);
    GPIO_DirectionConfig(HT_GPIOB    ,GPIO_PIN_5,  GPIO_DIR_IN);
    GPIO_PullResistorConfig(HT_GPIOB,  GPIO_PIN_5,  GPIO_PR_UP);
    GPIO_InputConfig(HT_GPIOB,         GPIO_PIN_5,  ENABLE);
    /* Connect Button EXTI Channel to Button GPIO Pin                                                       */
    AFIO_EXTISourceConfig(AFIO_EXTI_CH_5, AFIO_ESS_PB);
    /* Configure button EXTI Line on falling edge                                                           */
    EXTI_InitStruct.EXTI_Channel = EXTI_CHANNEL_5;
    EXTI_InitStruct.EXTI_Debounce = EXTI_DEBOUNCE_DISABLE;
    EXTI_InitStruct.EXTI_DebounceCnt = 0;
    EXTI_InitStruct.EXTI_IntType = EXTI_POSITIVE_EDGE;
    EXTI_Init(&EXTI_InitStruct);
    /* Enable Button EXTI Interrupt                                                                         */
    EXTI_IntConfig(EXTI_CHANNEL_5, ENABLE);
    NVIC_EnableIRQ(EXTI4_15_IRQn);

}

static void i4_GPIO_PIN_SET(u32 gpio,u16 port,HT_GPIO_TypeDef *ht_gpio)
{
	AFIO_GPxConfig(gpio,           	port,     AFIO_MODE_1);
	GPIO_DirectionConfig(ht_gpio,    port,     GPIO_DIR_IN);
	GPIO_PullResistorConfig(ht_gpio, port, 		GPIO_PR_UP);
    GPIO_InputConfig(ht_gpio,        port,          ENABLE);
}

void gpio_init(void)
{
#if 1
	//EXTI_InitTypeDef EXTI_InitStruct = {0};
	//LED
	AFIO_GPxConfig(GPIO_PB, GPIO_PIN_4, AFIO_FUN_GPIO);//LED
	HT_GPIOB->RR = GPIO_PIN_4;
	GPIO_PullResistorConfig(HT_GPIOB, GPIO_PIN_4, GPIO_PR_DISABLE);
	GPIO_DriveConfig(HT_GPIOB, GPIO_PIN_4, GPIO_DV_16MA);
	GPIO_DirectionConfig(HT_GPIOB, GPIO_PIN_4, GPIO_DIR_OUT);

	
	//TYBN1==GPIO_11=????¡Á¨¦??????????????
	AFIO_GPxConfig(GPIO_PA, GPIO_PIN_14, AFIO_FUN_GPIO);
	HT_GPIOB->RR = GPIO_PIN_14;
	GPIO_PullResistorConfig(HT_GPIOA, GPIO_PIN_14, GPIO_PR_DISABLE);
	GPIO_DriveConfig(HT_GPIOA, GPIO_PIN_14, GPIO_DV_16MA);
	GPIO_DirectionConfig(HT_GPIOA, GPIO_PIN_14, GPIO_DIR_OUT);
	GPIO_WriteOutBits(HT_GPIOA, GPIO_PIN_14,RESET);
	
//	//TYBN1==GPIO_14=????¡Á¨¦????MCU????
	AFIO_GPxConfig(GPIO_PA, GPIO_PIN_15, AFIO_MODE_1);
	GPIO_DirectionConfig(HT_GPIOA, GPIO_PIN_15, GPIO_DIR_IN);
	GPIO_PullResistorConfig(HT_GPIOA, GPIO_PIN_15, GPIO_PR_UP);
	GPIO_InputConfig(HT_GPIOA, GPIO_PIN_15, ENABLE);

#if PRODUCE_MEASURE	

	i4_GPIO_PIN_SET(GPIO_PA,GPIO_PIN_10,HT_GPIOA);
#endif	
    //DC5V_EN
	AFIO_GPxConfig(GPIO_PB, GPIO_PIN_0, AFIO_FUN_GPIO);
	HT_GPIOB->RR = GPIO_PIN_0;
	GPIO_PullResistorConfig(HT_GPIOB, GPIO_PIN_0, GPIO_PR_DISABLE);
	GPIO_DriveConfig(HT_GPIOB, GPIO_PIN_0, GPIO_DV_8MA);
	GPIO_DirectionConfig(HT_GPIOB, GPIO_PIN_0, GPIO_DIR_OUT);
	
    //TM8837_EN
	AFIO_GPxConfig(GPIO_PB, GPIO_PIN_1, AFIO_FUN_GPIO);
	HT_GPIOB->RR = GPIO_PIN_1;
	GPIO_PullResistorConfig(HT_GPIOB, GPIO_PIN_1, GPIO_PR_DISABLE);
	GPIO_DriveConfig(HT_GPIOB, GPIO_PIN_1, GPIO_DV_8MA);
	GPIO_DirectionConfig(HT_GPIOB, GPIO_PIN_1, GPIO_DIR_OUT);

	//pc0
	AFIO_GPxConfig(GPIO_PC, GPIO_PIN_0, AFIO_FUN_GPIO);
	HT_GPIOC->RR = GPIO_PIN_0;
	GPIO_PullResistorConfig(HT_GPIOC, GPIO_PIN_0, GPIO_PR_DISABLE);
	GPIO_DriveConfig(HT_GPIOC, GPIO_PIN_0, GPIO_DV_8MA);
	GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_0, GPIO_DIR_OUT);
	GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_0,SET);

	i4_GPIO_PIN_SET(GPIO_PA,GPIO_PIN_8,HT_GPIOA);
	i4_GPIO_PIN_SET(GPIO_PB,GPIO_PIN_6,HT_GPIOB);
	i4_GPIO_PIN_SET(GPIO_PB,GPIO_PIN_9,HT_GPIOB);
	i4_GPIO_PIN_SET(GPIO_PB,GPIO_PIN_15,HT_GPIOB);
	i4_GPIO_PIN_SET(GPIO_PC,GPIO_PIN_3,HT_GPIOC);



	
	motor_pwr(PWR_OFF);
#endif
}


void Low_power_wasteGpioASet(void)
{
//	CKCU_PeripClockConfig_TypeDef CKCUClock = {0};
//	CKCUClock.Bit.PA = 1;
//	CKCUClock.Bit.AFIO = 1;
//	CKCU_PeripClockConfig(CKCUClock, ENABLE);

    AFIO_GPxConfig(GPIO_PA,        AFIO_PIN_0, AFIO_FUN_GPIO);
	GPIO_DriveConfig(HT_GPIOA,     GPIO_PIN_0, GPIO_DV_8MA);
	GPIO_DirectionConfig(HT_GPIOA, GPIO_PIN_0, GPIO_DIR_OUT);
	
    AFIO_GPxConfig(GPIO_PA,        AFIO_PIN_1, AFIO_FUN_GPIO);
	GPIO_DriveConfig(HT_GPIOA,     GPIO_PIN_1, GPIO_DV_8MA);
	GPIO_DirectionConfig(HT_GPIOA, GPIO_PIN_1, GPIO_DIR_OUT);

    AFIO_GPxConfig(GPIO_PA,        AFIO_PIN_2, AFIO_FUN_GPIO);
	GPIO_DriveConfig(HT_GPIOA,     GPIO_PIN_2, GPIO_DV_8MA);
	GPIO_DirectionConfig(HT_GPIOA, GPIO_PIN_2, GPIO_DIR_OUT);
	
    
    AFIO_GPxConfig(GPIO_PA,        AFIO_PIN_3, AFIO_FUN_GPIO);
	GPIO_DriveConfig(HT_GPIOA,     GPIO_PIN_3, GPIO_DV_8MA);
	GPIO_DirectionConfig(HT_GPIOA, GPIO_PIN_3, GPIO_DIR_OUT);


//    AFIO_GPxConfig(GPIO_PA,            AFIO_PIN_4,  AFIO_FUN_GPIO);
//    GPIO_DirectionConfig(HT_GPIOA     ,GPIO_PIN_4,  GPIO_DIR_IN);
//    GPIO_PullResistorConfig(HT_GPIOA,  GPIO_PIN_4,  GPIO_PR_DISABLE);
//    GPIO_InputConfig(HT_GPIOA,         GPIO_PIN_4,  ENABLE);

//    AFIO_GPxConfig(GPIO_PA,            AFIO_PIN_5,  AFIO_FUN_GPIO);
//    GPIO_DirectionConfig(HT_GPIOA     ,GPIO_PIN_5,  GPIO_DIR_IN);
//    GPIO_PullResistorConfig(HT_GPIOA,  GPIO_PIN_5,  GPIO_PR_DISABLE);
//    GPIO_InputConfig(HT_GPIOA,         GPIO_PIN_5,  ENABLE);

//    
//   
    AFIO_GPxConfig(GPIO_PA,        AFIO_PIN_6, AFIO_FUN_GPIO);
	GPIO_DriveConfig(HT_GPIOA,     GPIO_PIN_6, GPIO_DV_8MA);
	GPIO_DirectionConfig(HT_GPIOA, GPIO_PIN_6, GPIO_DIR_OUT); 
		
	AFIO_GPxConfig(GPIO_PA,        AFIO_PIN_7, AFIO_FUN_GPIO);
	GPIO_DriveConfig(HT_GPIOA,     GPIO_PIN_7, GPIO_DV_8MA);
	GPIO_DirectionConfig(HT_GPIOA, GPIO_PIN_7, GPIO_DIR_OUT); 

    AFIO_GPxConfig(GPIO_PA,        AFIO_PIN_8, AFIO_FUN_GPIO);
	GPIO_DriveConfig(HT_GPIOA,     GPIO_PIN_8, GPIO_DV_8MA);
	GPIO_DirectionConfig(HT_GPIOA, GPIO_PIN_8, GPIO_DIR_OUT); 
	
	AFIO_GPxConfig(GPIO_PA,        AFIO_PIN_10, AFIO_FUN_GPIO);
	GPIO_DriveConfig(HT_GPIOA,     GPIO_PIN_10, GPIO_DV_8MA);
	GPIO_DirectionConfig(HT_GPIOA, GPIO_PIN_10, GPIO_DIR_OUT); 
	
	AFIO_GPxConfig(GPIO_PA,        AFIO_PIN_11, AFIO_FUN_GPIO);
	GPIO_DriveConfig(HT_GPIOA,     GPIO_PIN_11, GPIO_DV_8MA);
	GPIO_DirectionConfig(HT_GPIOA, GPIO_PIN_11, GPIO_DIR_OUT); 
//	
//    AFIO_GPxConfig(GPIO_PA,        AFIO_PIN_12, AFIO_FUN_GPIO);
//	GPIO_DriveConfig(HT_GPIOA,     GPIO_PIN_12, GPIO_DV_8MA);
//	GPIO_DirectionConfig(HT_GPIOA, GPIO_PIN_12, GPIO_DIR_OUT); 
//	
//	AFIO_GPxConfig(GPIO_PA,        AFIO_PIN_13, AFIO_FUN_GPIO);
//	GPIO_DriveConfig(HT_GPIOA,     GPIO_PIN_13, GPIO_DV_8MA);
//	GPIO_DirectionConfig(HT_GPIOA, GPIO_PIN_13, GPIO_DIR_OUT); 




}

void Low_power_wasteGpioBSet(void)
{
	
//	AFIO_GPxConfig(GPIO_PB,        AFIO_PIN_1, AFIO_FUN_GPIO);
//	GPIO_DriveConfig(HT_GPIOB,     GPIO_PIN_1, GPIO_DV_8MA);
//	GPIO_DirectionConfig(HT_GPIOB, GPIO_PIN_1, GPIO_DIR_OUT); 
//	
//	AFIO_GPxConfig(GPIO_PB,        AFIO_PIN_2, AFIO_FUN_GPIO);
//	GPIO_DriveConfig(HT_GPIOB,     GPIO_PIN_2, GPIO_DV_8MA);
//	GPIO_DirectionConfig(HT_GPIOB, GPIO_PIN_2, GPIO_DIR_OUT);
//	
//	
//	AFIO_GPxConfig(GPIO_PB,        AFIO_PIN_3, AFIO_FUN_GPIO);
//	GPIO_DriveConfig(HT_GPIOB,     GPIO_PIN_3, GPIO_DV_8MA);
//	GPIO_DirectionConfig(HT_GPIOB, GPIO_PIN_3, GPIO_DIR_OUT);
//	
//	AFIO_GPxConfig(GPIO_PB,        AFIO_PIN_4, AFIO_FUN_GPIO);
//	GPIO_DriveConfig(HT_GPIOB,     GPIO_PIN_4, GPIO_DV_8MA);
//	GPIO_DirectionConfig(HT_GPIOB, GPIO_PIN_4, GPIO_DIR_OUT);
//	
//	
	AFIO_GPxConfig(GPIO_PB,        AFIO_PIN_6, AFIO_FUN_GPIO);
	GPIO_DriveConfig(HT_GPIOB,     GPIO_PIN_6, GPIO_DV_8MA);
	GPIO_DirectionConfig(HT_GPIOB, GPIO_PIN_6, GPIO_DIR_OUT);
	
//	AFIO_GPxConfig(GPIO_PB,        AFIO_PIN_7, AFIO_FUN_GPIO);
//	GPIO_DriveConfig(HT_GPIOB,     GPIO_PIN_7, GPIO_DV_8MA);
//	GPIO_DirectionConfig(HT_GPIOB, GPIO_PIN_7, GPIO_DIR_OUT);
//	
//	AFIO_GPxConfig(GPIO_PB,        AFIO_PIN_8, AFIO_FUN_GPIO);
//	GPIO_DriveConfig(HT_GPIOB,     GPIO_PIN_8, GPIO_DV_8MA);
//	GPIO_DirectionConfig(HT_GPIOB, GPIO_PIN_8, GPIO_DIR_OUT);

//	AFIO_GPxConfig(GPIO_PB,            AFIO_PIN_7,  AFIO_FUN_GPIO);
//    GPIO_DirectionConfig(HT_GPIOB     ,GPIO_PIN_7,  GPIO_DIR_IN);
//    GPIO_PullResistorConfig(HT_GPIOB,  GPIO_PIN_7,  GPIO_PR_DISABLE);
//    GPIO_InputConfig(HT_GPIOB,         GPIO_PIN_7,  ENABLE);

//    AFIO_GPxConfig(GPIO_PB,            AFIO_PIN_8,  AFIO_FUN_GPIO);
//    GPIO_DirectionConfig(HT_GPIOB     ,GPIO_PIN_8,  GPIO_DIR_IN);
//    GPIO_PullResistorConfig(HT_GPIOB,  GPIO_PIN_8,  GPIO_PR_DISABLE);
//    GPIO_InputConfig(HT_GPIOB,         GPIO_PIN_8,  ENABLE);
//	
	AFIO_GPxConfig(GPIO_PB,        AFIO_PIN_9, AFIO_FUN_GPIO);
	GPIO_DriveConfig(HT_GPIOB,     GPIO_PIN_9, GPIO_DV_8MA);
	GPIO_DirectionConfig(HT_GPIOB, GPIO_PIN_9, GPIO_DIR_OUT);

	AFIO_GPxConfig(GPIO_PB,        AFIO_PIN_12, AFIO_FUN_GPIO);
	GPIO_DriveConfig(HT_GPIOB,     AFIO_PIN_12, GPIO_DV_8MA);
	GPIO_DirectionConfig(HT_GPIOB, AFIO_PIN_12, GPIO_DIR_OUT);

	AFIO_GPxConfig(GPIO_PB,        AFIO_PIN_15, AFIO_FUN_GPIO);
	GPIO_DriveConfig(HT_GPIOB,     AFIO_PIN_15, GPIO_DV_8MA);
	GPIO_DirectionConfig(HT_GPIOB, AFIO_PIN_15, GPIO_DIR_OUT);


}

void Low_power_wasteGpioCSet(void)
{

//	AFIO_GPxConfig(GPIO_PC,        AFIO_PIN_0, AFIO_FUN_GPIO);
//	GPIO_DriveConfig(HT_GPIOC,     GPIO_PIN_0, GPIO_DV_8MA);
//	GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_0, GPIO_DIR_OUT);
//	
	AFIO_GPxConfig(GPIO_PC,        AFIO_PIN_3, AFIO_FUN_GPIO);
	GPIO_DriveConfig(HT_GPIOC,     GPIO_PIN_3, GPIO_DV_8MA);
	GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_3, GPIO_DIR_OUT);
//	
	AFIO_GPxConfig(GPIO_PC,        AFIO_PIN_4, AFIO_FUN_GPIO);
	GPIO_DriveConfig(HT_GPIOC,     GPIO_PIN_4, GPIO_DV_8MA);
	GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_4, GPIO_DIR_OUT);
//	
	AFIO_GPxConfig(GPIO_PC,        AFIO_PIN_5, AFIO_FUN_GPIO);
	GPIO_DriveConfig(HT_GPIOC,     GPIO_PIN_5, GPIO_DV_8MA);
	GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_5, GPIO_DIR_OUT);
//	
	AFIO_GPxConfig(GPIO_PC,        AFIO_PIN_6, AFIO_FUN_GPIO);
	GPIO_DriveConfig(HT_GPIOC,     GPIO_PIN_6, GPIO_DV_8MA);
	GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_6, GPIO_DIR_OUT);
	
	AFIO_GPxConfig(GPIO_PC,        AFIO_PIN_7, AFIO_FUN_GPIO);
	GPIO_DriveConfig(HT_GPIOC,     GPIO_PIN_7, GPIO_DV_8MA);
	GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_7, GPIO_DIR_OUT);
//	

}

void Low_power_wasteGpioSet(void)
{
	Low_power_wasteGpioASet();
	Low_power_wasteGpioBSet();
	Low_power_wasteGpioCSet();
	
}


void setGPIOStatus(void)
{
	GPIO_WriteOutBits(HT_GPIOA, GPIO_PIN_0,RESET);
	GPIO_WriteOutBits(HT_GPIOA, GPIO_PIN_1,RESET);
	GPIO_WriteOutBits(HT_GPIOA, GPIO_PIN_2,RESET);
	GPIO_WriteOutBits(HT_GPIOA, GPIO_PIN_3,RESET);
	GPIO_WriteOutBits(HT_GPIOA, GPIO_PIN_6,RESET);
	GPIO_WriteOutBits(HT_GPIOA, GPIO_PIN_7,RESET);
	GPIO_WriteOutBits(HT_GPIOA, GPIO_PIN_8,RESET);
	GPIO_WriteOutBits(HT_GPIOA, GPIO_PIN_10,RESET);
	GPIO_WriteOutBits(HT_GPIOA, GPIO_PIN_11,RESET);
//	GPIO_WriteOutBits(HT_GPIOA, GPIO_PIN_12,RESET);
//	GPIO_WriteOutBits(HT_GPIOA, GPIO_PIN_13,RESET);

//	GPIO_WriteOutBits(HT_GPIOB, GPIO_PIN_0,RESET);
//	GPIO_WriteOutBits(HT_GPIOB, GPIO_PIN_1,RESET);
//	GPIO_WriteOutBits(HT_GPIOB, GPIO_PIN_2,RESET);
//	GPIO_WriteOutBits(HT_GPIOB, GPIO_PIN_3,RESET);
	
	GPIO_WriteOutBits(HT_GPIOB, GPIO_PIN_6,RESET);
//	GPIO_WriteOutBits(HT_GPIOB, GPIO_PIN_7,RESET);
//	GPIO_WriteOutBits(HT_GPIOB, GPIO_PIN_8,RESET);
	GPIO_WriteOutBits(HT_GPIOB, GPIO_PIN_9,RESET);
	GPIO_WriteOutBits(HT_GPIOB, GPIO_PIN_12,RESET);
	GPIO_WriteOutBits(HT_GPIOB, GPIO_PIN_15,RESET);
	

//	GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_0,RESET);
	GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_3,RESET);
	GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_4,RESET);
	GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_5,RESET);
	GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_6,RESET);
	GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_7,RESET);
	
}
