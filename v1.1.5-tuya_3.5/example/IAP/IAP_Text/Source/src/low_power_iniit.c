#include "low_power_iniit.h"
#include  "gpio_init.h"


u32 low_power_event_flag = 0;
vu32 guKeyState[5] = {0};

#if 0
void RTC_Configuration(void)
{
	/* Check the backup domain(RTC & PWRCU) is ready for access */
	if(PWRCU_CheckReadyAccessed() != PWRCU_OK)
	{
		while (1);
	}
	/* Reset Backup Domain */
	PWRCU_DeInit();

	/* Enable the LSE OSC */
	RTC_LSESMConfig(RTC_LSESM_NORMAL);
	RTC_LSECmd(ENABLE);
	/* Wait till LSE is ready */
	while (CKCU_GetClockReadyStatus(CKCU_FLAG_LSERDY) == RESET);

	/* Select the RTC Clock Source as LSE */
	RTC_ClockSourceConfig(RTC_SRC_LSE);

	/* Set the RTC time base to 1s */
	RTC_SetPrescaler(RTC_RPRE_32768);

	/* Set Compare value */
	RTC_SetCompare(0xFFFFFFFF);

	/* Enable the RTC */
	RTC_Cmd(ENABLE);

	/* Enable the RTC Compare Match wakeup event */
	RTC_WakeupConfig(RTC_WAKEUP_CM, ENABLE);
}
#endif

void key_exti_init(void)
{
    EXTI_InitTypeDef EXTI_InitStruct = {0};

    //PC1 �ж�����
    AFIO_GPxConfig(GPIO_PC, AFIO_PIN_1, AFIO_FUN_DEFAULT);
    GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_1, GPIO_DIR_IN);
    GPIO_PullResistorConfig(HT_GPIOC, GPIO_PIN_1, GPIO_PR_UP);
    GPIO_InputConfig(HT_GPIOC, GPIO_PIN_1, ENABLE);
    /* Connect Button EXTI Channel to Button GPIO Pin */
    AFIO_EXTISourceConfig(AFIO_EXTI_CH_1, AFIO_ESS_PC);
    /* Configure button EXTI Line on falling edge */
    EXTI_InitStruct.EXTI_Channel = EXTI_CHANNEL_1;
    EXTI_InitStruct.EXTI_Debounce = EXTI_DEBOUNCE_DISABLE;
    EXTI_InitStruct.EXTI_DebounceCnt = 0;
    EXTI_InitStruct.EXTI_IntType = EXTI_POSITIVE_EDGE;
    EXTI_Init(&EXTI_InitStruct);
    /* Enable Button EXTI Interrupt */
    EXTI_IntConfig(EXTI_CHANNEL_1, ENABLE);
    NVIC_EnableIRQ(EXTI0_1_IRQn);

    //PC2 �ж�����
    AFIO_GPxConfig(GPIO_PC, AFIO_PIN_2, AFIO_FUN_DEFAULT);
    GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_2, GPIO_DIR_IN);
    GPIO_PullResistorConfig(HT_GPIOC, GPIO_PIN_2, GPIO_PR_UP);
    GPIO_InputConfig(HT_GPIOC, GPIO_PIN_2, ENABLE);
    /* Connect Button EXTI Channel to Button GPIO Pin */
    AFIO_EXTISourceConfig(AFIO_EXTI_CH_2, AFIO_ESS_PC);
    /* Configure button EXTI Line on falling edge */
    EXTI_InitStruct.EXTI_Channel = EXTI_CHANNEL_2;
    EXTI_InitStruct.EXTI_Debounce = EXTI_DEBOUNCE_DISABLE;
    EXTI_InitStruct.EXTI_DebounceCnt = 0;
    EXTI_InitStruct.EXTI_IntType = EXTI_POSITIVE_EDGE;
    EXTI_Init(&EXTI_InitStruct);
    /* Enable Button EXTI Interrupt */
    EXTI_IntConfig(EXTI_CHANNEL_2, ENABLE);
    NVIC_EnableIRQ(EXTI2_3_IRQn);

    //PB5 �ж�����
    AFIO_GPxConfig(GPIO_PB, AFIO_PIN_5, AFIO_FUN_DEFAULT);
    GPIO_DirectionConfig(HT_GPIOB, GPIO_PIN_5, GPIO_DIR_IN);
    GPIO_PullResistorConfig(HT_GPIOB, GPIO_PIN_5, GPIO_PR_UP);
    GPIO_InputConfig(HT_GPIOB, GPIO_PIN_5, ENABLE);
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

    //PA14 �ж�����
    AFIO_GPxConfig(GPIO_PA, AFIO_PIN_14, AFIO_FUN_DEFAULT);
    GPIO_DirectionConfig(HT_GPIOA, GPIO_PIN_14, GPIO_DIR_IN);
    GPIO_PullResistorConfig(HT_GPIOA, GPIO_PIN_14, GPIO_PR_UP);
    GPIO_InputConfig(HT_GPIOA, GPIO_PIN_14, ENABLE);
    /* Connect Button EXTI Channel to Button GPIO Pin                                                       */
    AFIO_EXTISourceConfig(AFIO_EXTI_CH_14, AFIO_ESS_PA);
    /* Configure button EXTI Line on falling edge                                                           */
    EXTI_InitStruct.EXTI_Channel = EXTI_CHANNEL_14;
    EXTI_InitStruct.EXTI_Debounce = EXTI_DEBOUNCE_DISABLE;
    EXTI_InitStruct.EXTI_DebounceCnt = 0;
    EXTI_InitStruct.EXTI_IntType = EXTI_POSITIVE_EDGE;
    EXTI_Init(&EXTI_InitStruct);
    /* Enable Button EXTI Interrupt                                                                         */
    EXTI_IntConfig(EXTI_CHANNEL_14, ENABLE);
    NVIC_EnableIRQ(EXTI4_15_IRQn);

    //PA15 �ж�����
    AFIO_GPxConfig(GPIO_PA, AFIO_PIN_15, AFIO_FUN_DEFAULT);
    GPIO_DirectionConfig(HT_GPIOA, GPIO_PIN_15, GPIO_DIR_IN);
    GPIO_PullResistorConfig(HT_GPIOA, GPIO_PIN_15, GPIO_PR_UP);
    GPIO_InputConfig(HT_GPIOA, GPIO_PIN_15, ENABLE);
    /* Connect Button EXTI Channel to Button GPIO Pin                                                       */
    AFIO_EXTISourceConfig(AFIO_EXTI_CH_15, AFIO_ESS_PA);
    /* Configure button EXTI Line on falling edge                                                           */
    EXTI_InitStruct.EXTI_Channel = EXTI_CHANNEL_15;
    EXTI_InitStruct.EXTI_Debounce = EXTI_DEBOUNCE_DISABLE;
    EXTI_InitStruct.EXTI_DebounceCnt = 0;
    EXTI_InitStruct.EXTI_IntType = EXTI_POSITIVE_EDGE;
    EXTI_Init(&EXTI_InitStruct);
    /* Enable Button EXTI Interrupt                                                                         */
    EXTI_IntConfig(EXTI_CHANNEL_15, ENABLE);
    NVIC_EnableIRQ(EXTI4_15_IRQn);

#if 0
	GPIO_PullResistorConfig(HT_GPIOC, GPIO_PIN_1, GPIO_PR_UP);
	GPIO_PullResistorConfig(HT_GPIOC, GPIO_PIN_2, GPIO_PR_UP);
	GPIO_PullResistorConfig(HT_GPIOB, GPIO_PIN_5, GPIO_PR_UP);
	GPIO_PullResistorConfig(HT_GPIOA, GPIO_PIN_14, GPIO_PR_UP);
	GPIO_PullResistorConfig(HT_GPIOA, GPIO_PIN_15, GPIO_PR_UP);
	
	AFIO_GPxConfig(GPIO_PC, GPIO_PIN_1, AFIO_FUN_GPIO);
	AFIO_GPxConfig(GPIO_PC, GPIO_PIN_2, AFIO_FUN_GPIO);
	AFIO_GPxConfig(GPIO_PB, GPIO_PIN_5, AFIO_FUN_GPIO);
	AFIO_GPxConfig(GPIO_PA, GPIO_PIN_14, AFIO_FUN_GPIO);
	AFIO_GPxConfig(GPIO_PA, GPIO_PIN_15, AFIO_FUN_GPIO);
	
	GPIO_InputConfig(HT_GPIOC, GPIO_PIN_1, ENABLE);
	GPIO_InputConfig(HT_GPIOC, GPIO_PIN_2, ENABLE);
	GPIO_InputConfig(HT_GPIOB, GPIO_PIN_5, ENABLE);
	GPIO_InputConfig(HT_GPIOA, GPIO_PIN_14, ENABLE);
	GPIO_InputConfig(HT_GPIOA, GPIO_PIN_15, ENABLE);
	
    AFIO_EXTISourceConfig(AFIO_EXTI_CH_1, AFIO_ESS_PC);
    AFIO_EXTISourceConfig(AFIO_EXTI_CH_2, AFIO_ESS_PC);
    AFIO_EXTISourceConfig(AFIO_EXTI_CH_5, AFIO_ESS_PB);
    AFIO_EXTISourceConfig(AFIO_EXTI_CH_14, AFIO_ESS_PA);
    AFIO_EXTISourceConfig(AFIO_EXTI_CH_15, AFIO_ESS_PA);
	
	EXTI_WakeupEventConfig(EXTI_CHANNEL_1, EXTI_WAKEUP_LOW_LEVEL, ENABLE);
	EXTI_WakeupEventConfig(EXTI_CHANNEL_2, EXTI_WAKEUP_LOW_LEVEL, ENABLE);
	EXTI_WakeupEventConfig(EXTI_CHANNEL_5, EXTI_WAKEUP_LOW_LEVEL, ENABLE);
	EXTI_WakeupEventConfig(EXTI_CHANNEL_14, EXTI_WAKEUP_LOW_LEVEL, ENABLE);
	EXTI_WakeupEventConfig(EXTI_CHANNEL_15, EXTI_WAKEUP_LOW_LEVEL, ENABLE);
	
	
	EXTI_WakeupEventIntConfig(ENABLE);
	NVIC_EnableIRQ(EVWUP_IRQn);
#endif
}

// ???? 12.1
void Exit_Init(void)
{
    CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
    //EXTI_InitTypeDef EXTI_InitStruct = {0};

    RSTCU_PeripReset_TypeDef rst = {0};
    rst.Bit.EXTI = 1;
    RSTCU_PeripReset(rst, ENABLE);

    CKCUClock.Bit.AFIO = 1;
    CKCUClock.Bit.PC = 1;
    CKCUClock.Bit.PB = 1;
    CKCUClock.Bit.PA = 1;
    CKCUClock.Bit.EXTI = 1;
    CKCU_PeripClockConfig(CKCUClock, ENABLE);
    extern bool ProductionTestFlag;
    if(!ProductionTestFlag){
        //PC1
        /* Configure GPIO pull down, AFIO mode, and Input Enable                                                  */
        GPIO_PullResistorConfig(HT_GPIOC, GPIO_PIN_1, GPIO_PR_UP);
        AFIO_GPxConfig(GPIO_PC, AFIO_PIN_1, AFIO_MODE_1);
        GPIO_InputConfig(HT_GPIOC, GPIO_PIN_1, ENABLE);
        /* Select Port as EXTI Trigger Source                                                                     */
        AFIO_EXTISourceConfig(AFIO_EXTI_CH_1, AFIO_ESS_PC);

        /* EXTI Event Related Configuration                                                                       */
        if(LEFT_KEY())
        {
            EXTI_WakeupEventConfig(EXTI_CHANNEL_1, EXTI_EDGE_NEGATIVE, ENABLE);
        }else{
            EXTI_WakeupEventConfig(EXTI_CHANNEL_1, EXTI_EDGE_POSITIVE, ENABLE);
        }
        EXTI_WakeupEventIntConfig(ENABLE);
        NVIC_EnableIRQ(EVWUP_IRQn);
    }

    //PC2
    /* Configure GPIO pull down, AFIO mode, and Input Enable                                                  */
    GPIO_PullResistorConfig(HT_GPIOC, GPIO_PIN_2, GPIO_PR_UP);
    AFIO_GPxConfig(GPIO_PC, AFIO_PIN_2, AFIO_MODE_1);
    GPIO_InputConfig(HT_GPIOC, GPIO_PIN_2, ENABLE);
    /* Select Port as EXTI Trigger Source                                                                     */
    AFIO_EXTISourceConfig(AFIO_EXTI_CH_2, AFIO_ESS_PC);

    /* EXTI Event Related Configuration                                                                       */
    if(RIGHT_KEY()){
       EXTI_WakeupEventConfig(EXTI_CHANNEL_2, EXTI_EDGE_NEGATIVE, ENABLE);
    }else{   
        EXTI_WakeupEventConfig(EXTI_CHANNEL_2, EXTI_EDGE_POSITIVE, ENABLE);
    }
    EXTI_WakeupEventIntConfig(ENABLE);
    NVIC_EnableIRQ(EVWUP_IRQn);

    //PB5
    /* Configure GPIO pull down, AFIO mode, and Input Enable                                                  */
    GPIO_PullResistorConfig(HT_GPIOB, GPIO_PIN_5, GPIO_PR_UP);
    AFIO_GPxConfig(GPIO_PB, AFIO_PIN_5, AFIO_MODE_1);
    GPIO_InputConfig(HT_GPIOB, GPIO_PIN_5, ENABLE);
    /* Select Port as EXTI Trigger Source                                                                     */
    AFIO_EXTISourceConfig(AFIO_EXTI_CH_5, AFIO_ESS_PB);

    /* EXTI Event Related Configuration                                                                       */
    EXTI_WakeupEventConfig(EXTI_CHANNEL_5, EXTI_EDGE_NEGATIVE, ENABLE);
    EXTI_WakeupEventIntConfig(ENABLE);
    NVIC_EnableIRQ(EVWUP_IRQn);
#if 1
    //	//PA14
    /* Configure GPIO pull down, AFIO mode, and Input Enable                                                  */
    GPIO_PullResistorConfig(HT_GPIOA, GPIO_PIN_14, GPIO_PR_UP);
    AFIO_GPxConfig(GPIO_PA, AFIO_PIN_14, AFIO_MODE_1); // AFIO_MODE_1
    GPIO_InputConfig(HT_GPIOA, GPIO_PIN_14, ENABLE);
    /* Select Port as EXTI Trigger Source                                                                     */
    AFIO_EXTISourceConfig(AFIO_EXTI_CH_14, AFIO_ESS_PA);

    /* EXTI Event Related Configuration                                                                       */
    EXTI_WakeupEventConfig(EXTI_CHANNEL_14, EXTI_WAKEUP_LOW_LEVEL, ENABLE);
    EXTI_WakeupEventIntConfig(ENABLE);
    NVIC_EnableIRQ(EVWUP_IRQn);

    //PA15
    // 2020.12.15 ??PA15?????????????????MCU?????PA15?????
    /* Configure GPIO pull down, AFIO mode, and Input Enable                                                  */
    GPIO_PullResistorConfig(HT_GPIOA, GPIO_PIN_15, GPIO_PR_UP);
    AFIO_GPxConfig(GPIO_PA, AFIO_PIN_15, AFIO_MODE_1);
    GPIO_InputConfig(HT_GPIOA, GPIO_PIN_15, ENABLE);
    /* Select Port as EXTI Trigger Source                                                                     */
    AFIO_EXTISourceConfig(AFIO_EXTI_CH_15, AFIO_ESS_PA);

    /* EXTI Event Related Configuration                                                                       */
    EXTI_WakeupEventConfig(EXTI_CHANNEL_15, EXTI_WAKEUP_LOW_LEVEL, ENABLE);
    EXTI_WakeupEventIntConfig(ENABLE);
    NVIC_EnableIRQ(EVWUP_IRQn);
#endif

    AFIO_GPxConfig(GPIO_PA,           	GPIO_PIN_10,     AFIO_MODE_1);
	GPIO_DirectionConfig(HT_GPIOA,    GPIO_PIN_10,     GPIO_DIR_IN);
	GPIO_PullResistorConfig(HT_GPIOA, GPIO_PIN_10, 		GPIO_PR_DISABLE);
    GPIO_InputConfig(HT_GPIOA,        GPIO_PIN_10,          ENABLE);

}
//PWRCU_DeepSleep2(PWRCU_SLEEP_ENTRY_WFI);
void low_power_init(void)
{
    //RTC_Configuration();
    //key_exti_init();
    Exit_Init();
    //PWRCU_WakeupPinCmd(ENABLE);
}
