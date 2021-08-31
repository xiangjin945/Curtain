#include "motor_init.h"
#include "head.h"
vu32 speed = 0x64;



#ifdef USE_GPIO_PB2_PB3
void motor_init(void)
{
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
    //TM8837_IN1
    AFIO_GPxConfig(GPIO_PB, GPIO_PIN_2, AFIO_FUN_GPIO);
	HT_GPIOB->RR = GPIO_PIN_2;
	GPIO_PullResistorConfig(HT_GPIOB, GPIO_PIN_2, GPIO_PR_DISABLE);
	GPIO_DriveConfig(HT_GPIOB, GPIO_PIN_2, GPIO_DV_8MA);
	GPIO_DirectionConfig(HT_GPIOB, GPIO_PIN_2, GPIO_DIR_OUT);
    //TM8837_IN2
	AFIO_GPxConfig(GPIO_PB, GPIO_PIN_3, AFIO_FUN_GPIO);
	HT_GPIOB->RR = GPIO_PIN_3;
	GPIO_PullResistorConfig(HT_GPIOB, GPIO_PIN_3, GPIO_PR_DISABLE);
	GPIO_DriveConfig(HT_GPIOB, GPIO_PIN_3, GPIO_DV_8MA);
	GPIO_DirectionConfig(HT_GPIOB, GPIO_PIN_3, GPIO_DIR_OUT);
	
	motor_pwr(PWR_OFF);
}
#endif

#ifdef USE_MCTM_PB2_PB3
void motor_init_pb2(u16 duty_cycle)
{
    TM_TimeBaseInitTypeDef MCTM_TimeBaseInitStructure;
    TM_OutputInitTypeDef MCTM_OutputInitStructure;
    MCTM_CHBRKCTRInitTypeDef MCTM_CHBRKCTRInitStructure;

    CKCU_PeripClockConfig_TypeDef CKCUClock = {{ 0 }};
    CKCUClock.Bit.AFIO       = 1;
    CKCUClock.Bit.MCTM0      = 1;
    CKCU_PeripClockConfig(CKCUClock, ENABLE);

#if 1		//调试
	AFIO_GPxConfig(GPIO_PB, GPIO_PIN_1, AFIO_FUN_GPIO);
	HT_GPIOB->RR = GPIO_PIN_1;
	GPIO_PullResistorConfig(HT_GPIOB, GPIO_PIN_1, GPIO_PR_DISABLE);
	GPIO_DriveConfig(HT_GPIOB, GPIO_PIN_1, GPIO_DV_8MA);
	GPIO_DirectionConfig(HT_GPIOB, GPIO_PIN_1, GPIO_DIR_OUT);
#endif
	
    AFIO_GPxConfig(HTCFG_MCTM_CH2_GPIO_ID, HTCFG_MCTM_CH2_AFIO_PIN, AFIO_FUN_MCTM_GPTM);
    //AFIO_GPxConfig(HTCFG_MCTM_CH2N_GPIO_ID, HTCFG_MCTM_CH2N_AFIO_PIN, AFIO_FUN_MCTM_GPTM);
	
#if 0		//调试
	AFIO_GPxConfig(GPIO_PB, GPIO_PIN_3, AFIO_FUN_GPIO);
	HT_GPIOB->RR = GPIO_PIN_3;
	GPIO_PullResistorConfig(HT_GPIOB, GPIO_PIN_3, GPIO_PR_DISABLE);
	GPIO_DriveConfig(HT_GPIOB, GPIO_PIN_3, GPIO_DV_8MA);
	GPIO_DirectionConfig(HT_GPIOB, GPIO_PIN_3, GPIO_DIR_OUT);
#endif
	
    /* Configure MCTM Break pin */
    //AFIO_GPxConfig(HTCFG_MCTM_BREAK_GPIO_ID, HTCFG_MCTM_BREAK_AFIO_PIN, AFIO_FUN_MCTM_GPTM);

    /* MCTM Time Base configuration, 10KHz */
    MCTM_TimeBaseInitStructure.CounterReload = 3999;	//HTCFG_MCTM_RELOAD - 1
    MCTM_TimeBaseInitStructure.Prescaler = 0;
    MCTM_TimeBaseInitStructure.RepetitionCounter = 0;
    MCTM_TimeBaseInitStructure.CounterMode = TM_CNT_MODE_UP;
    MCTM_TimeBaseInitStructure.PSCReloadTime = TM_PSC_RLD_IMMEDIATE;
    TM_TimeBaseInit(HT_MCTM0, &MCTM_TimeBaseInitStructure);

    /* MCTM Channel 2 output configuration */
    MCTM_OutputInitStructure.Channel = TM_CH_2;
    MCTM_OutputInitStructure.Compare = duty_cycle*40;		//HTCFG_MCTM_RELOAD * 1 / 2
    MCTM_OutputInitStructure.OutputMode = TM_OM_PWM1;
    MCTM_OutputInitStructure.Control = TM_CHCTL_ENABLE;
    MCTM_OutputInitStructure.Polarity = TM_CHP_NONINVERTED;
    MCTM_OutputInitStructure.IdleState = MCTM_OIS_LOW;
    MCTM_OutputInitStructure.AsymmetricCompare = 0;
    TM_OutputInit(HT_MCTM0, &MCTM_OutputInitStructure);

    /* MCTM Off State, lock, Break, Automatic Output enable, dead time configuration */
    MCTM_CHBRKCTRInitStructure.OSSRState = MCTM_OSSR_STATE_DISABLE;	//MCTM_OSSR_STATE_ENABLE, MCTM_OSSR_STATE_DISABLE
    MCTM_CHBRKCTRInitStructure.OSSIState = MCTM_OSSR_STATE_DISABLE;	//MCTM_OSSR_STATE_ENABLE, MCTM_OSSR_STATE_DISABLE
    MCTM_CHBRKCTRInitStructure.LockLevel = MCTM_LOCK_LEVEL_3;
    MCTM_CHBRKCTRInitStructure.Break0 = MCTM_BREAK_DISABLE;	//MCTM_BREAK_ENABLE, MCTM_BREAK_DISABLE
    MCTM_CHBRKCTRInitStructure.Break0Polarity = MCTM_BREAK_POLARITY_LOW;
    MCTM_CHBRKCTRInitStructure.AutomaticOutput = MCTM_CHAOE_ENABLE;
    MCTM_CHBRKCTRInitStructure.DeadTime = HTCFG_MCTM_DEAD_TIME;
    MCTM_CHBRKCTRInitStructure.BreakFilter = 0;
    MCTM_CHBRKCTRConfig(HT_MCTM0, &MCTM_CHBRKCTRInitStructure);

    /* MCTM counter enable */
    TM_Cmd(HT_MCTM0, ENABLE);
    /* MCTM Channel Main Output enable */
    MCTM_CHMOECmd(HT_MCTM0, ENABLE);
}

void motor_init_pb3(u16 duty_cycle)
{
    TM_TimeBaseInitTypeDef MCTM_TimeBaseInitStructure;
    TM_OutputInitTypeDef MCTM_OutputInitStructure;
    MCTM_CHBRKCTRInitTypeDef MCTM_CHBRKCTRInitStructure;

    CKCU_PeripClockConfig_TypeDef CKCUClock = {{ 0 }};
    CKCUClock.Bit.AFIO       = 1;
    CKCUClock.Bit.MCTM0      = 1;
    CKCU_PeripClockConfig(CKCUClock, ENABLE);

#if 1		//调试
	AFIO_GPxConfig(GPIO_PB, GPIO_PIN_1, AFIO_FUN_GPIO);
	HT_GPIOB->RR = GPIO_PIN_1;
	GPIO_PullResistorConfig(HT_GPIOB, GPIO_PIN_1, GPIO_PR_DISABLE);
	GPIO_DriveConfig(HT_GPIOB, GPIO_PIN_1, GPIO_DV_8MA);
	GPIO_DirectionConfig(HT_GPIOB, GPIO_PIN_1, GPIO_DIR_OUT);
#endif
	
    //AFIO_GPxConfig(HTCFG_MCTM_CH2_GPIO_ID, HTCFG_MCTM_CH2_AFIO_PIN, AFIO_FUN_MCTM_GPTM);
    AFIO_GPxConfig(HTCFG_MCTM_CH2N_GPIO_ID, HTCFG_MCTM_CH2N_AFIO_PIN, AFIO_FUN_MCTM_GPTM);
	
#if 0		//调试
	AFIO_GPxConfig(GPIO_PB, GPIO_PIN_2, AFIO_FUN_GPIO);
	HT_GPIOB->RR = GPIO_PIN_2;
	GPIO_PullResistorConfig(HT_GPIOB, GPIO_PIN_2, GPIO_PR_DISABLE);
	GPIO_DriveConfig(HT_GPIOB, GPIO_PIN_2, GPIO_DV_8MA);
	GPIO_DirectionConfig(HT_GPIOB, GPIO_PIN_2, GPIO_DIR_OUT);
#endif
	
    /* Configure MCTM Break pin */
    //AFIO_GPxConfig(HTCFG_MCTM_BREAK_GPIO_ID, HTCFG_MCTM_BREAK_AFIO_PIN, AFIO_FUN_MCTM_GPTM);

    /* MCTM Time Base configuration, 10KHz */
    MCTM_TimeBaseInitStructure.CounterReload = 3999;	//HTCFG_MCTM_RELOAD - 1
    MCTM_TimeBaseInitStructure.Prescaler = 0;
    MCTM_TimeBaseInitStructure.RepetitionCounter = 0;
    MCTM_TimeBaseInitStructure.CounterMode = TM_CNT_MODE_UP;
    MCTM_TimeBaseInitStructure.PSCReloadTime = TM_PSC_RLD_IMMEDIATE;
    TM_TimeBaseInit(HT_MCTM0, &MCTM_TimeBaseInitStructure);

    /* MCTM Channel 2 output configuration */
    MCTM_OutputInitStructure.Channel = TM_CH_2;
    MCTM_OutputInitStructure.Compare = duty_cycle*40;		//HTCFG_MCTM_RELOAD * 1 / 2
    MCTM_OutputInitStructure.OutputMode = TM_OM_PWM1;
    MCTM_OutputInitStructure.Control = TM_CHCTL_ENABLE;
    //MCTM_OutputInitStructure.ControlN = TM_CHCTL_ENABLE;
    MCTM_OutputInitStructure.Polarity = TM_CHP_NONINVERTED;
    //MCTM_OutputInitStructure.PolarityN = TM_CHP_NONINVERTED;
    MCTM_OutputInitStructure.IdleState = MCTM_OIS_LOW;
    //MCTM_OutputInitStructure.IdleStateN = MCTM_OIS_HIGH;
	MCTM_OutputInitStructure.AsymmetricCompare = 0;
    TM_OutputInit(HT_MCTM0, &MCTM_OutputInitStructure);

    /* MCTM Off State, lock, Break, Automatic Output enable, dead time configuration */
    MCTM_CHBRKCTRInitStructure.OSSRState = MCTM_OSSR_STATE_DISABLE;	//MCTM_OSSR_STATE_ENABLE, MCTM_OSSR_STATE_DISABLE
    MCTM_CHBRKCTRInitStructure.OSSIState = MCTM_OSSR_STATE_DISABLE;	//MCTM_OSSR_STATE_ENABLE, MCTM_OSSR_STATE_DISABLE
    MCTM_CHBRKCTRInitStructure.LockLevel = MCTM_LOCK_LEVEL_3;
    MCTM_CHBRKCTRInitStructure.Break0 = MCTM_BREAK_DISABLE;	//MCTM_BREAK_ENABLE, MCTM_BREAK_DISABLE
    MCTM_CHBRKCTRInitStructure.Break0Polarity = MCTM_BREAK_POLARITY_LOW;
    MCTM_CHBRKCTRInitStructure.AutomaticOutput = MCTM_CHAOE_ENABLE;
    MCTM_CHBRKCTRInitStructure.DeadTime = HTCFG_MCTM_DEAD_TIME;
    MCTM_CHBRKCTRInitStructure.BreakFilter = 0;
    MCTM_CHBRKCTRConfig(HT_MCTM0, &MCTM_CHBRKCTRInitStructure);

    /* MCTM counter enable */
    TM_Cmd(HT_MCTM0, ENABLE);
    /* MCTM Channel Main Output enable */
    MCTM_CHMOECmd(HT_MCTM0, ENABLE);
}
#endif

#ifdef USE_PWM_PB2_PB3
//PB2 pwm 输出
void PB2_PWM_INIT(u16 duty_cycle,ControlStatus NewState)
{
    CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
    TM_TimeBaseInitTypeDef TM_TimeBaseInitStructure;
    TM_OutputInitTypeDef TM_OutputInitStructure;

    //Enable PCLK of BUZZER and AFIO
    CKCUClock.Bit.MCTM0 = 1;
    CKCUClock.Bit.AFIO = 1;
    CKCU_PeripClockConfig(CKCUClock, ENABLE);

    /* Configure the BUZZER_GPIO_PIN as TM channel output AFIO function */
    AFIO_GPxConfig(GPIO_PB, AFIO_PIN_2, AFIO_FUN_MCTM_GPTM);

    //=======================================
    TM_TimeBaseInitStructure.CounterReload = 3999;
    TM_TimeBaseInitStructure.Prescaler = 0;
    TM_TimeBaseInitStructure.RepetitionCounter = 0;
    TM_TimeBaseInitStructure.CounterMode = TM_CNT_MODE_UP;
    TM_TimeBaseInitStructure.PSCReloadTime = TM_PSC_RLD_IMMEDIATE;
    TM_TimeBaseInit(HT_MCTM0, &TM_TimeBaseInitStructure);

    TM_OutputInitStructure.Channel = TM_CH_2;     //
	TM_OutputInitStructure.Compare = (4000*(100-duty_cycle))/100;
	TM_OutputInitStructure.OutputMode = TM_OM_PWM1;
    TM_OutputInitStructure.Control = TM_CHCTL_ENABLE;
    //TM_OutputInitStructure.ControlN = TM_CHCTL_ENABLE;
    TM_OutputInitStructure.Polarity = TM_CHP_INVERTED;
    //TM_OutputInitStructure.PolarityN = TM_CHP_NONINVERTED;
    TM_OutputInitStructure.IdleState = MCTM_OIS_HIGH;
    //TM_OutputInitStructure.IdleStateN = MCTM_OIS_HIGH;
    TM_OutputInitStructure.AsymmetricCompare = 0;
	TM_OutputInit(HT_MCTM0, &TM_OutputInitStructure);

    MCTM_CHMOECmd(HT_MCTM0, ENABLE);
    TM_Cmd(HT_MCTM0, NewState);
    //MCTM_CHMOECmd(HT_MCTM0, ENABLE);
    //TM_Cmd(HT_MCTM0, ENABLE);
}

//PB3 PWM  OUT 
void PB3_PWM_INIT(u16 duty_cycle,ControlStatus NewState)
{
    CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
    TM_TimeBaseInitTypeDef TM_TimeBaseInitStructure;
    TM_OutputInitTypeDef TM_OutputInitStructure;

    /* Enable PCLK of BUZZER and AFIO */
    CKCUClock.Bit.SCTM1 = 1;
    CKCUClock.Bit.AFIO = 1;
    CKCU_PeripClockConfig(CKCUClock, ENABLE);

    /* Configure the BUZZER_GPIO_PIN as TM channel output AFIO function */
    AFIO_GPxConfig(GPIO_PB, AFIO_PIN_3, AFIO_FUN_SCTM);

    /* Init BUZZER TM time-base */
    TM_TimeBaseInitStructure.CounterReload = 3999;
    TM_TimeBaseInitStructure.Prescaler = 0;
    TM_TimeBaseInitStructure.RepetitionCounter = 0;
    TM_TimeBaseInitStructure.CounterMode = TM_CNT_MODE_UP;
    TM_TimeBaseInitStructure.PSCReloadTime = TM_PSC_RLD_IMMEDIATE;
    TM_TimeBaseInit(HT_SCTM1, &TM_TimeBaseInitStructure);

    /* Clear Update Event Interrupt flag */
    TM_ClearFlag(HT_SCTM1, TM_FLAG_UEV);

    /* Init BUZZER TM Channel x to output PWM waveform with 33% duty */
    TM_OutputInitStructure.Channel = TM_CH_0;
    TM_OutputInitStructure.Compare = (4000*(100-duty_cycle))/100;
    TM_OutputInitStructure.OutputMode = TM_OM_PWM2;
    TM_OutputInitStructure.Control = TM_CHCTL_ENABLE;
    TM_OutputInitStructure.ControlN = TM_CHCTL_DISABLE;//TM_CHCTL_ENABLE;
    TM_OutputInitStructure.Polarity = TM_CHP_NONINVERTED;
    TM_OutputInitStructure.PolarityN = TM_CHP_NONINVERTED;
    TM_OutputInitStructure.IdleState = MCTM_OIS_LOW;
    TM_OutputInitStructure.IdleStateN = MCTM_OIS_HIGH;
    TM_OutputInit(HT_SCTM1, &TM_OutputInitStructure);

    /* BUZZER TM counter enable */
    TM_Cmd(HT_SCTM1, NewState);
    //TM_Cmd(HT_SCTM1, ENABLE);
}
#endif

#if 0
void ADC_Configuration(void)
{
	/* Configure AFIO mode as ADC function */
	AFIO_GPxConfig(GPIO_PB, AFIO_PIN_6, AFIO_MODE_2);

	{
		/* ADC related settings */
		/* CK_ADC frequency is set to (CK_AHB / 64) */
		CKCU_SetADCnPrescaler(HTCFG_ADC_CKCU_ADCPRE, CKCU_ADCPRE_DIV64);

		/* Continuous mode, sequence length = 1 */
		ADC_RegularGroupConfig(HTCFG_ADC_PORT, CONTINUOUS_MODE, 1, 0);

		/* ADC conversion time = (Sampling time + Latency) / CK_ADC = (1.5 + ADST + 12.5) / CK_ADC */
		/* Set ADST = 0, sampling time = 1.5 + ADST */
		#if (LIBCFG_ADC_SAMPLE_TIME_BY_CH)
		// The sampling time is set by the last parameter of the function "ADC_RegularChannelConfig()".
		#else
		ADC_SamplingTimeConfig(HTCFG_ADC_PORT, 0);
		#endif

		/* Set ADC conversion sequence as channel n */
		ADC_RegularChannelConfig(HTCFG_ADC_PORT, HTCFG_VR_ADC_CH, 0, 0);

		/* Set software trigger as ADC trigger source                                                           */
		ADC_RegularTrigConfig(HTCFG_ADC_PORT, ADC_TRIG_SOFTWARE);
	}

	/* Enable ADC single end of conversion interrupt */
	ADC_IntConfig(HTCFG_ADC_PORT, ADC_INT_SINGLE_EOC, ENABLE);

	/* Enable the ADC interrupts                                                                              */
	NVIC_EnableIRQ(HTCFG_ADC_IRQn);
}
#endif

void delay_us(int i)
{
	for(;i>0;i--){;;;;;}
}

void motor_pwr(u8 x)
{
#if 0
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
#endif
	if(x==PWR_ON)
	{
		GPIO_WriteOutBits(HT_GPIOB, GPIO_PIN_0,SET);
		GPIO_WriteOutBits(HT_GPIOB, GPIO_PIN_1,SET);    	
	}
	if(x==PWR_OFF)
	{
		GPIO_WriteOutBits(HT_GPIOB, GPIO_PIN_0,RESET);
		GPIO_WriteOutBits(HT_GPIOB, GPIO_PIN_1,RESET);    	
	}
}

void motor_stop(void)
{
	//signPositive_inversion = 0;
	
	motor_pwr(PWR_OFF);
	PB2_PWM_INIT(0,ENABLE);
	PB3_PWM_INIT(0,ENABLE);
}

//开窗帘--反转
void open_curtain(void)
{
	MotorCDC = 0;
	MotorRunStatus = TRUE;
	motor_stop();

	motor_pwr(PWR_ON);
	ms_delay_systick(80);
	
	PB2_PWM_INIT(80,ENABLE);
	PB3_PWM_INIT(100,ENABLE);
	ms_delay_systick(80);
	
	PB2_PWM_INIT(60,ENABLE);
	PB3_PWM_INIT(100,ENABLE);
	ms_delay_systick(80);
	
	PB2_PWM_INIT(40,ENABLE);
	PB3_PWM_INIT(100,ENABLE);
	ms_delay_systick(80);
	
	PB2_PWM_INIT(20,ENABLE);
	PB3_PWM_INIT(100,ENABLE);
	ms_delay_systick(80);
	 
	PB2_PWM_INIT(0,ENABLE);
	PB3_PWM_INIT(100,ENABLE);
	// ms_delay_systick(100);

	signPositive_inversion = 1;
}

//关窗帘--正转
void close_curtain(void)
{
	MotorCDC = 0;
	MotorRunStatus = TRUE;
	motor_stop();
	
	
	motor_pwr(PWR_ON);
	ms_delay_systick(80);
	
	PB2_PWM_INIT(100,ENABLE);
	PB3_PWM_INIT(80,ENABLE);
	ms_delay_systick(80);
	
	PB2_PWM_INIT(100,ENABLE);
	PB3_PWM_INIT(60,ENABLE);
	ms_delay_systick(80);
	
	PB2_PWM_INIT(100,ENABLE);
	PB3_PWM_INIT(40,ENABLE);
	ms_delay_systick(80);
	
	PB2_PWM_INIT(100,ENABLE);
	PB3_PWM_INIT(20,ENABLE);
	ms_delay_systick(80);

	PB2_PWM_INIT(100,ENABLE);
	PB3_PWM_INIT(0,ENABLE);
	// ms_delay_systick(20);
	signPositive_inversion = 2;
}

void clock_config(void)
{
#if 1
	CKCU_PeripClockConfig_TypeDef CKCUClock = {{ 0 }};
	//CKCUClock.Bit.ADC0		= 1;
	CKCUClock.Bit.ADC		= 1;
	CKCUClock.Bit.PDMA		= 0;
	CKCUClock.Bit.USBD		= 0;
	CKCUClock.Bit.CKREF		= 0;
	CKCUClock.Bit.EBI		= 0;
	CKCUClock.Bit.CRC		= 0;
	CKCUClock.Bit.PA		= 0;
	CKCUClock.Bit.PB		= 0;
	CKCUClock.Bit.PC		= 0;
	CKCUClock.Bit.PD		= 0;
	CKCUClock.Bit.I2C0		= 0;
	CKCUClock.Bit.I2C1		= 0;
	CKCUClock.Bit.SPI0		= 0;
	CKCUClock.Bit.SPI1		= 0;
	CKCUClock.Bit.USART0	= 1;
	CKCUClock.Bit.USART1	= 0;
	CKCUClock.Bit.UART0		= 1;
	CKCUClock.Bit.UART1		= 0;
	CKCUClock.Bit.AFIO		= 1;
	CKCUClock.Bit.EXTI		= 1;
	CKCUClock.Bit.SCI0		= 0;
	CKCUClock.Bit.SCI1		= 0;
	CKCUClock.Bit.I2S		= 0;
	CKCUClock.Bit.MCTM0		= 1;
	CKCUClock.Bit.WDT		= 1;
	CKCUClock.Bit.BKP		= 1;
	CKCUClock.Bit.GPTM0		= 0;
	CKCUClock.Bit.GPTM1		= 0;
	CKCUClock.Bit.BFTM0		= 1;
	CKCUClock.Bit.BFTM1		= 1;
	CKCUClock.Bit.CMP		= 0;
	CKCUClock.Bit.SCTM0		= 0;
	CKCUClock.Bit.SCTM1		= 0;
	CKCUClock.Bit.SCTM2		= 0;
	CKCUClock.Bit.SCTM3		= 0;


	CKCU_PeripClockConfig(CKCUClock, ENABLE);
#else
	CKCU_PeripClockConfig_TypeDef CKCUClock = {0};
	
	CKCUClock.Bit.PA = 1;
	CKCUClock.Bit.AFIO = 1;
	CKCU_PeripClockConfig(CKCUClock, ENABLE);
	CKCUClock.Bit.PB = 1;
	CKCUClock.Bit.AFIO = 1;
	CKCU_PeripClockConfig(CKCUClock, ENABLE);
	CKCUClock.Bit.PC = 1;
	CKCUClock.Bit.AFIO = 1;
	CKCU_PeripClockConfig(CKCUClock, ENABLE);
#endif
}
