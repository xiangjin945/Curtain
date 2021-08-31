#include "adc_init.h"

bool Volatage_38 = FALSE;
u16 battery_voltage[100] = {0};
u16 tmp_voltage[100] = {0};

//vu32 gPotentiometerLevel;
//u8 gADC_Result[3] = {0};
//u8 gADC_CycleEndOfConversion;
//volatile bool gADC_SingleEndOfConversion;

u16 get_battery_voltage(void)
{
	int i,j,n,min,max;
	u16 average = 0;
	
	for(i=0;i<10;i++)
	{
		battery_voltage[i] = (HT_ADC->DR[0] & 0x0FFF);
	}
	
	n = 0;
	max = min = battery_voltage[0];
	for(i=0;i<10;i++)
	{
		for(j=1;j<10;j++)
		{
			if(battery_voltage[j]>max)
			{
				n++;
				max = battery_voltage[j];
			}
			if(battery_voltage[j]<min)
			{
				n++;
				min = battery_voltage[j];
			}
		}
	}
	for(i=0;i<10;i++)
	{
		if((tmp_voltage[j]==max)||(tmp_voltage[j]==min))
		{
			tmp_voltage[j] = 0;
		}
	}
	for(i=0;i<(10-n);i++)
	{
		average += tmp_voltage[i];
	}
	average = average/(10-n);
	
	return average;
}
extern bool lowpower_flag;

u16 check_battery_level(u32 value)
{
	static int flagVolage = 0 , count = 0;
	int tmp_silent = 0;
	int real_silent = 0; 
	tmp_silent = (HT_ADC->DR[0] & 0x0FFF); 
	//printf("\ntmp_silent = %d\n",tmp_silent);
	real_silent = ((float)tmp_silent/4096)*3300; //* 200;
	if(flagVolage == 1){
		real_silent -= 5;

	}
	printf("volatage value =  %d\r\n",real_silent);
	if(lowpower_flag)//低电量
	{
		printf("lowpower_flag power\r\n");
		update_battery_value(2);
		//mcu_dp_fault_update(12,1);
		return 0;
	}



	if(real_silent >= MEDIUM_BATTERY)
	{
		flagVolage = 0;
		printf("full\r\n");
		update_battery_value(0);
	}
	if(real_silent > LOW_BATTERY && real_silent < MEDIUM_BATTERY)
	{
		printf("Moderate\r\n");
		flagVolage = 1;
		update_battery_value(1);
	}
	if(real_silent < LOW_BATTERY)
	{
		lowpower_flag = TRUE;
		printf("low power\r\n");
		update_battery_value(2);
		mcu_dp_fault_update(12,1);
		motor_stop();
	}
	
	


	return 0;
}
//adc_val/4096*Vref=Value			// NOTE: 12.9 �Ƚ϶�ת������ֵ�ļ����������㷨
//u16 check_motor_current(u16 protect_current)
//{
//	int i,t=0,k=0;
//	float real_volt = 0;
//	u16 average_volt = 0;
//	
//	//ms_delay_systick(100);
//	for(i=0;i<10;i++)
//	{
//		tmp_voltage[i] = (HT_ADC->DR[1] & 0x0FFF);
//		real_volt = ((float)tmp_voltage[i]/4096)*3300;
//		printf("real_volt = %f\n",real_volt);
//		// NOTE: ÿ�β���ֵ��ת��Ϊ����ֵ
//		////notes%3.4f\n",real_volt);
//		if(real_volt >= protect_current) //��ȡ��real_volt��ֵΪ 1E 2E 2D ֵ���� 64С 
//		{
//			t++;
//			average_volt = average_volt + real_volt;	
//		}
//		else
//		{
//			k++;
//			average_volt = average_volt + real_volt;
//		}
//		if(t>=8)
//		{
//			average_volt = average_volt/t;
//			return average_volt;
//		}
//		ms_delay_systick(10);
//	}
//	
//	// NOTE: ����10�β���ֵ����3�����ϳ�����ֵ����ȡƽ��ֵΪ��ǰ����
//	average_volt = average_volt/k;
//	return average_volt;
//}

//adc_val/4096*Vref=Value			// NOTE: ȡƽ��ֵ�ļ����������㷨
u16 get_motor_current(void)
{
	int i,j,n,min,max;
	u16 average = 0;
	
	for(i=0;i<10;i++)
	{
		tmp_voltage[i] = (HT_ADC->DR[0] & 0x0FFF);
	}
	
	n = 0;
	max = min = tmp_voltage[0];
	for(i=0;i<10;i++)
	{
		for(j=1;j<10;j++)
		{
			if(tmp_voltage[j]>max)
			{
				n++;
				max = tmp_voltage[j];
			}
			if(tmp_voltage[j]<min)
			{
				n++;
				min = tmp_voltage[j];
			}
		}
	}
	for(i=0;i<10;i++)
	{
		if((tmp_voltage[j]==max)||(tmp_voltage[j]==min))
		{
			tmp_voltage[j] = 0;
		}
	}
	for(i=0;i<(10-n);i++)
	{
		average += tmp_voltage[i];
	}
	average = average/(10-n);
	
	return average;
}

void adc_init(void)
{
	ADC_DeInit(HT_ADC);
	/* Config AFIO mode as ADC function                                                                       */
	//����ָ�����ŵ�GPIOģʽ
	AFIO_GPxConfig(GPIO_PA, AFIO_PIN_4, AFIO_FUN_ADC);
	AFIO_GPxConfig(GPIO_PA, AFIO_PIN_5, AFIO_FUN_ADC);
	
	/* Continuous Mode, Length 1, SubLength 1                
	���ó������ת��ģʽ(�����͵���)���б���еĳ��ȡ�                                                 */
	ADC_RegularGroupConfig(HT_ADC, CONTINUOUS_MODE, 2, 1);

	/* ADC Channel n, Rank 0, Sampling clock is (1.5 + 0) ADCLK
	Conversion time = (sampling clock + 12.5) / ADCLK = 12.4 uS */
	//�ڶ�������Ϊ����Ƶ��������Ӧ�ĵȼ�
	ADC_RegularChannelConfig(HT_ADC, ADC_CH_6, 0);
	ADC_RegularChannelConfig(HT_ADC, ADC_CH_7, 1);
	

	/* Use Software Trigger as ADC trigger source 
	����ADC����Դ�Խ��г���ͨ��ת����                                                            */
	ADC_RegularTrigConfig(HT_ADC, ADC_TRIG_SOFTWARE);

	/* Enable ADC single end of conversion interrupt,
	The ADC ISR will store the ADC result into global variable gPotentiometerLevel.
	ʹ��ADC����ת���жϣ�ADC ISR��ADC����洢��ȫ�ֱ���gPotentiometerLevel */
	ADC_IntConfig(HT_ADC, ADC_INT_SINGLE_EOC, ENABLE);//���û����ADC�ж�

	ADC_Cmd(HT_ADC, ENABLE);//���û����ָ����ADC��

	/* Software trigger to start continuous mode                                                              */
	ADC_SoftwareStartConvCmd(HT_ADC, ENABLE);//���û����ѡ����ADC�����������ͨ��ת����
}

void adc_init_2(void)
{
	/* Configure specify IO's AFIO mode as ADC function */
	AFIO_GPxConfig(GPIO_PA, AFIO_PIN_5, AFIO_FUN_ADC);

	/* ADCLK frequency is set to CK_AHB / 64 */
	CKCU_SetADCPrescaler(CKCU_ADCPRE_DIV64);

	/* One Shot Mode, Conversion/Sequence Length 3, SubLength 0 */
	ADC_RegularGroupConfig(HT_ADC, ONE_SHOT_MODE, 3, 0);

	/* Set sampling time as 1.5 + 36 ADCCLK, Conversion = 12.5 + 1.5 + 36 = 50 ADCCLK */
	ADC_SamplingTimeConfig(HT_ADC, 36);

	/* Set ADC Channel of Sequence */
	ADC_RegularChannelConfig(HT_ADC, ADC_CH_7, 0);

	/* Enable ADC single/cycle end of conversion interrupt */
	ADC_IntConfig(HT_ADC, ADC_INT_SINGLE_EOC | ADC_INT_CYCLE_EOC, ENABLE);

	/* Enable the ADC Interrupts */
	NVIC_EnableIRQ(ADC_IRQn);

	/* Use GPTM0 CH3O as ADC trigger source */
	ADC_RegularTrigConfig(HT_ADC, ADC_TRIG_GPTM0_CH3O);
	
	ADC_Cmd(HT_ADC, ENABLE);
}

void adc_init_1(void)
{
#if 0
	/* Enable the ADC Interrupts */
	NVIC_EnableIRQ(ADC_IRQn);
	
	/* ADCLK frequency is set to 72/64 MHz = 1.125MHz */
	CKCU_SetADCPrescaler(CKCU_ADCPRE_DIV64);
	
	/* Config AFIO mode as ADC function */
	// AFIO_GPxConfig(ADC_800_AFIO_PROT, ADC_800_AFIO_PIN, AFIO_FUN_ADC);
	AFIO_GPxConfig(GPIO_PA, AFIO_PIN_5, AFIO_FUN_ADC);
	/* Continuous Mode, Length 6, SubLength 1 */
	ADC_RegularGroupConfig(HT_ADC, CONTINUOUS_MODE, 4, 1);
	
	ADC_RegularChannelConfig(HT_ADC, ADC_CH_7, 0);
#else
	/* Enable the ADC interrupts */
	NVIC_EnableIRQ(ADC_IRQn);
	
	/* Config AFIO mode as ADC function */
	AFIO_GPxConfig(GPIO_PA, AFIO_PIN_5, AFIO_FUN_ADC);

	/* Continuous Mode, Length 1, SubLength 1 */
	ADC_RegularGroupConfig(HT_ADC, CONTINUOUS_MODE, 1, 1);

	/* ADC Channel n, Rank 0, Sampling clock is (1.5 + 0) ADCLK
	Conversion time = (sampling clock + 12.5) / ADCLK = 12.4 uS */
	ADC_RegularChannelConfig(HT_ADC, ADC_CH_7, 0);

	/* Use Software Trigger as ADC trigger source */
	ADC_RegularTrigConfig(HT_ADC, ADC_TRIG_SOFTWARE);

	/* Enable ADC single end of conversion interrupt,
	The ADC ISR will store the ADC result into global variable gPotentiometerLevel. */
	ADC_IntConfig(HT_ADC, ADC_INT_SINGLE_EOC, ENABLE);

	ADC_Cmd(HT_ADC, ENABLE);

	/* Software trigger to start continuous mode */
	ADC_SoftwareStartConvCmd(HT_ADC, ENABLE);
	
	USART_SendData(HT_USART0,0xAA);
	while(USART_GetFlagStatus(HT_USART0, USART_FLAG_TXC) == RESET);
#endif
}
