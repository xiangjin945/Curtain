#include "pwm_init.h"

//PB2 pwm 输出
void  PB2_PWM_INIT(u16 duty_cycle)
{
    CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
    TM_TimeBaseInitTypeDef TM_TimeBaseInitStructure;
    TM_OutputInitTypeDef TM_OutputInitStructure;

    //Enable PCLK of BUZZER and AFIO
    CKCUClock.Bit.MCTM0 = 1;
    CKCUClock.Bit.AFIO       = 1;
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
	TM_OutputInitStructure.Compare = duty_cycle;
	TM_OutputInitStructure.OutputMode = TM_OM_PWM1;
    TM_OutputInitStructure.Control = TM_CHCTL_ENABLE;
    TM_OutputInitStructure.ControlN = TM_CHCTL_ENABLE;
    TM_OutputInitStructure.Polarity = TM_CHP_NONINVERTED;
    TM_OutputInitStructure.PolarityN = TM_CHP_NONINVERTED;
    TM_OutputInitStructure.IdleState = MCTM_OIS_LOW;
    TM_OutputInitStructure.IdleStateN = MCTM_OIS_HIGH;
    TM_OutputInitStructure.AsymmetricCompare = 0;
	TM_OutputInit(HT_MCTM0, &TM_OutputInitStructure);

    MCTM_CHMOECmd(HT_MCTM0, ENABLE);
    TM_Cmd(HT_MCTM0, ENABLE);
}


//PB3 PWM  OUT 
void  PB3_PWM_INIT(u16 duty_cycle)
{
    CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
    TM_TimeBaseInitTypeDef TM_TimeBaseInitStructure;
    TM_OutputInitTypeDef TM_OutputInitStructure;
    uint32_t wCRR = 0, wPSCR = 0, wBeeIndex = 0;

    /* Enable PCLK of BUZZER and AFIO */
    BUZZER_PB3_TM_CLK(CKCUClock) = 1;
    CKCUClock.Bit.AFIO       = 1;
    CKCU_PeripClockConfig(CKCUClock, ENABLE);

    /* Configure the BUZZER_GPIO_PIN as TM channel output AFIO function */
    HT32F_DVB_GPxConfig(BUZZER_PB3_GPIO_ID, BUZZER_PB3_AFIO_PIN, BUZZER_PB3_AFIO_MODE);

    /* Compute CRR and PSCR value */
    wCRR = (SystemCoreClock / PB3_BEE_FREQ) - 1;
    while ((wCRR / (wPSCR + 1)) > 0xFFFF)
    {
        wPSCR++;
    }
    wCRR = wCRR / (wPSCR + 1);

    /* Init BUZZER TM time-base */
    TM_TimeBaseInitStructure.CounterReload = wCRR;
    TM_TimeBaseInitStructure.Prescaler = wPSCR;
    TM_TimeBaseInitStructure.RepetitionCounter = 0;
    TM_TimeBaseInitStructure.CounterMode = TM_CNT_MODE_UP;
    TM_TimeBaseInitStructure.PSCReloadTime = TM_PSC_RLD_IMMEDIATE;
    TM_TimeBaseInit(BUZZER_PB3_TM, &TM_TimeBaseInitStructure);

    /* Clear Update Event Interrupt flag */
    TM_ClearFlag(BUZZER_PB3_TM, TM_FLAG_UEV);

    /* Init BUZZER TM Channel x to output PWM waveform with 33% duty */
    TM_OutputInitStructure.Channel = TM_CH_0;
    TM_OutputInitStructure.Compare = duty_cycle;
    TM_OutputInitStructure.OutputMode = TM_OM_PWM2;
    TM_OutputInitStructure.Control = TM_CHCTL_ENABLE;
    TM_OutputInitStructure.ControlN = TM_CHCTL_DISABLE;//TM_CHCTL_ENABLE;
    TM_OutputInitStructure.Polarity = TM_CHP_NONINVERTED;
    TM_OutputInitStructure.PolarityN = TM_CHP_NONINVERTED;
    TM_OutputInitStructure.IdleState = MCTM_OIS_LOW;
    TM_OutputInitStructure.IdleStateN = MCTM_OIS_HIGH;
    TM_OutputInit(BUZZER_PB3_TM, &TM_OutputInitStructure);

    /* BUZZER TM counter enable */
    TM_Cmd(BUZZER_PB3_TM, ENABLE);
}
