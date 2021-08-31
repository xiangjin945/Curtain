#include "rtc_init.h"

u8 day_per_month[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

u32 current_time = 0;
vu32 second = 0;
long long alarm_second = 0;
vu32 rtc_interrupt = 1;

vu32 wRtcCounterTmp = 0;
vu32 xRtcCounterTmp = 0;
vu32 Initial_count = 0;
vu32 Next_time = 0;
static bool is_leap_year(u32 year)
{
	if(((year % 4 == 0) && (year % 100 != 0) ) || (year % 400 == 0) )
		return TRUE;
	else
		return FALSE;
}

u32 app_time_adjust(date_time* date_time)
{
	u32 i, temp, secsum = 0;

	temp = date_time->year - 1;
	for(i = 0; i < (date_time->year - 2020); i++)
	{
		if(is_leap_year(temp--) == TRUE)
		{
			secsum += (366 * 86400);
		}
		else
		{
			secsum += (365 * 86400);
		}
	}

	temp = 1;
	for(i = 0; i < (date_time->month - 1); i++)
	{
		if(temp == 2)
		{
			if(is_leap_year(date_time->year) == TRUE)
				secsum += (29 * 86400);
			else
				secsum += (28 * 86400);
		}
		else
		{
			secsum += (day_per_month[temp] * 86400);
		}
		temp++;
	}

	secsum += ((date_time->day - 1) * 86400);
	secsum += (date_time->hour * 3600 );
	secsum += (date_time->minute * 60);
	secsum += (date_time->second);

	return secsum;
}

u8 AP_Time_Count(date_time* date_time)
{
	u32 i, secsum = 0, temp = 0;

	//secsum = PWRCU_ReadBackupRegister((PWRCU_BAKREG_Enum) PWRCU_BAKREG_1_RTC_SECOND);
	secsum += RTC_GetCounter();

	temp = 0;
	while(secsum >= (365 * 86400))
	{
		if(is_leap_year(2014 + temp))
		{
			if(secsum >= (366 * 86400))
			{
				temp++;
				secsum -= (366 * 86400);
			}
			else
			{
				break;
			}
		}
		else
		{
			temp++;
			secsum -= (365 * 86400);
		}
	}
	date_time->year = 2014 + temp;

	for(i = 1; i <= 12; i++)
	{
		if(secsum >= (day_per_month[i] * 86400))
		{
			if(i == 2)  // February
			{
				if(is_leap_year(date_time->year))
				{
					if(secsum >= (29 * 86400))
						secsum -= (29 * 86400);
					else
						break;
				}
				else
				{
					secsum -= (28 * 86400);
				}
			}
			else
			{
				secsum -= (day_per_month[i] * 86400);
			}
		}
		else
		{
			break;
		}
	}
	date_time->month = i;

	date_time->day = secsum / 86400 + 1;
	secsum -= ((date_time->day - 1) * 86400);

	date_time->hour = secsum / 3600;
	date_time->minute = (secsum % 3600) / 60;
	date_time->second = (secsum % 3600) % 60;

	return 1;
}

void setup_interrupt(vu32 time)
{
	RTC_SetCompare(time);
	RTC_Cmd(ENABLE);
}
	
void rtc_init(void)
{
	NVIC_EnableIRQ(RTC_IRQn);
	// NVIC_SetPriority(EVWUP_IRQn, 0xF);
	// NVIC_EnableIRQ(EVWUP_IRQn);
	// EXTI_WakeupEventIntConfig(ENABLE);
	
	/* Reset Backup Domain */
	PWRCU_DeInit();

	/* Configure Low Speed External clock (LSE)  */
	RTC_LSESMConfig(RTC_LSESM_NORMAL);
	RTC_LSECmd(ENABLE);
	while (CKCU_GetClockReadyStatus(CKCU_FLAG_LSERDY) == RESET);

	/* Configure the RTC */
	RTC_ClockSourceConfig(RTC_SRC_LSE);
//	RTC_IntConfig(RTC_INT_CSEC, ENABLE);
	RTC_IntConfig(RTC_INT_CM, ENABLE);
	RTC_SetPrescaler(RTC_RPRE_32768);

	// /* Restart counter when match event occurred发生比较事件时重启计数器 */
	//RTC_CMPCLRCmd(ENABLE);
	

	/* Enable the RTC Compare Match wakeup event                                                              */
	RTC_WakeupConfig(RTC_WAKEUP_CM, ENABLE);
	
	/* Set Compare value                                                                                      */
	RTC_SetCompare(0xFFFFFFFF);

	/* Enable the RTC                                                                                         */
	RTC_Cmd(ENABLE);
}
