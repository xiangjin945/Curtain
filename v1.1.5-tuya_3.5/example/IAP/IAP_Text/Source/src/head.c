/*
 * @Author: i4season.xiang
 * @Date: 2021-03-24 09:52:24
 * @LastEditTime: 2021-08-30 22:37:24
 * @FilePath: \v1.1.5-tuya_3.5\example\IAP\IAP_Text\Source\src\head.c
 */
#include "head.h"
#include "stdlib.h"

percent_correction percor;

bool KeyControl = TRUE;

vu32 DatalenIn = 0;
vu32 DatalenOUT = 0;

u32 linkbuffer[64] = {0};
u8 icount =0;
// static volatile uint16_t Test_UART_RX_Count = 0;
// u8 Production_test_uart_rx_buf[256] = {0};
unsigned char versions[3];


u8 VS_CurTime[3] = {0};
u8 ledflashingfalg[2] = {0};
u8 ledfalshswitch = 0;
u8 bindflag = 0;
u8 lednetworkflag = 0;
u8 fiveflag = 0;
u8 Stallsign =0;
u8 signPositive_inversion = 0;
char vsignStopPercent = 0;
u8 IsignLaunchDirection = 0;
u8 IsignDirection = 0;

u16 VS_Timezonetime = 0;

u32 ThereMinutes = 0;
u32 ThereMinutesCnt = 0;
u32 CountdownrData = 0;
u32 MiddleButtonTiming = 0;
u32 MotorCDC=0;//该变量保存电机电流值
u32 I4_ReturntimeCnt = 0;//此变量用于计数校准时返程运行的时间
u32 SaveFullruntime = 0;
u32 IsignReset = 0;
u32 VS_TimeCalibraCnt = 0;
u32 VS_ResetCountTime = 0;
u32 key_delay_process = 0;

bool IsignCalibrating = FALSE;//为真时不进入低功耗。
bool IsignStopPercent = FALSE;
bool signFullonfulloff = FALSE;
bool MiddleButtonStart = FALSE;
bool FirstStartSign = FALSE;
bool cFirstStartSign = FALSE;
bool RtcCntSign = FALSE;
bool MotorRunStatus = FALSE;//note:When it is true, it means that the motor is running and does not enter low power consumption
bool IsignIdling = TRUE;
bool IS_Timezone = TRUE;
bool IS_FullInspection = FALSE;
bool Stallfalag = FALSE;
bool ProductionTestFlag = FALSE;
bool lowpower_flag = FALSE;
bool lockturnflag = FALSE;
bool key_delay_process_flag = FALSE;


silent_runtime silenttime;

void I4_LockedRotorCallback(void);
void update_curtain_position(u8 percentage);
void key_delay_process_start(void);


void I4_LockedRotorCallback(void)
{
	KeyControl = FALSE;
	ms_delay_systick(1000);
	if(signPositive_inversion == 1)
	{
		close_curtain();
		ms_delay_systick(1000);
		motor_stop();
		signPositive_inversion = 0;
	}
	if(signPositive_inversion == 2)
	{
		open_curtain();
		ms_delay_systick(1000);
		motor_stop();
		signPositive_inversion = 0;
	}
	KeyControl = TRUE;
	percor.key = 0;

}



//0x0E=RF射频测试
void rf_test(void)
{
	u8 crc8 = 0;
	
	dp_data_buf[0] = 0x55;
	dp_data_buf[1] = 0xAA;
	dp_data_buf[2] = VERSION;
	dp_data_buf[3] = 0x0E;
	dp_data_buf[4] = 0x00;	//数据长度
	dp_data_buf[5] = 0x00;	//数据长度
	
	crc8 = get_check_sum(dp_data_buf,6);
	dp_data_buf[6] = crc8;
	//printf("rf crc = %d\r\n",crc8);
	uart0_send(dp_data_buf,7);
	//usart0_send(dp_data_buf,7);
}

int check_battery(void)
{
	int tmp_silent = 0;
	int real_silent = 0; 
	tmp_silent = (HT_ADC->DR[0] & 0x0FFF); 
	//printf("\ntmp_silent = %d\n",tmp_silent);
	
	real_silent = ((float)tmp_silent/4096)*330; //* 200;
	return real_silent;
}

int check_time_zone(u16 timezone )
{
	int sum = 0,time = 0;
	if(timezone == 400){
		sum = 4;
	}else if(timezone == 800){
		sum = 8;
	}else if(timezone == 900){
		sum = 9;
	}
	time = sum * 3600;
	return time;
}

void update_calibrstatus(u8 status)
{
	u8 crc8 = 0;

    dp_data_buf[0] = 0x55;
    dp_data_buf[1] = 0xAA;
    dp_data_buf[2] = VERSION;
    dp_data_buf[3] = 0x07;
    dp_data_buf[4] = 0x00; //数据长度
    dp_data_buf[5] = 0x05; //数据长度
    dp_data_buf[6] = 0x68;
    dp_data_buf[7] = 0x04;
    dp_data_buf[8] = 0x00;
    dp_data_buf[9] = 0x01;
    dp_data_buf[10] = status;
    crc8 = get_check_sum(dp_data_buf, 11);
    dp_data_buf[11] = crc8;
    uart0_send(dp_data_buf, 12);
	//usart0_send(dp_data_buf,12);
}

void I4TimerDataClear(void)
{
	u8 crc8 = 0;
	char* tmp_buf = NULL;
	
	dp_data_buf[0] = 0x55;
	dp_data_buf[1] = 0xAA;
	dp_data_buf[2] = VERSION;
	dp_data_buf[3] = 0x07;
	dp_data_buf[4] = 0x00;	//数据长度
	dp_data_buf[5] = 0x10;	//数据长度
	dp_data_buf[6] = 0x66;	//数据长度
	dp_data_buf[7] = 0x00;	//数据长度
	dp_data_buf[8] = 0x00;	//数据长度
	dp_data_buf[9] = 0x0c;	//数据长度
	dp_data_buf[10] = 0xff;	//数据长度
	dp_data_buf[11] = 0xff;	//数据长度
	dp_data_buf[12] = 0xff;	//数据长度
	dp_data_buf[13] = 0xff;	//数据长度
	dp_data_buf[14] = 0xff;	//数据长度
	dp_data_buf[15] = 0xff;	//数据长度
	dp_data_buf[16] = 0xff;	//数据长度
	dp_data_buf[17] = 0xff;	//数据长度
	dp_data_buf[18] = 0xff;	//数据长度
	dp_data_buf[19] = 0xff;	//数据长度
	dp_data_buf[20] = 0xff;	//数据长度
	dp_data_buf[21] = 0xff;	//数据长度
	
	
	crc8 = get_check_sum(dp_data_buf,22);
	dp_data_buf[22] = crc8;
	
	uart0_send(dp_data_buf,23);
	usart0_send(dp_data_buf,23);
	
}

void I4CountdownDataClear(void)
{
	u8 crc8 = 0;
	char* tmp_buf = NULL;
	
	dp_data_buf[0] = 0x55;
	dp_data_buf[1] = 0xAA;
	dp_data_buf[2] = VERSION;
	dp_data_buf[3] = 0x07;
	dp_data_buf[4] = 0x00;	//数据长度
	dp_data_buf[5] = 0x08;	//数据长度
	dp_data_buf[6] = 0x6b;	//数据长度
	dp_data_buf[7] = 0x02;	//数据长度
	dp_data_buf[8] = 0x00;	//数据长度
	dp_data_buf[9] = 0x04;
	dp_data_buf[10] = 0x00;
	dp_data_buf[11] = 0x00;
	dp_data_buf[12] = 0x00;
	dp_data_buf[13] = 0x00;

	
	
	crc8 = get_check_sum(dp_data_buf,14);
	dp_data_buf[14] = crc8;
	
	uart0_send(dp_data_buf,15);
	usart0_send(dp_data_buf,23);

}


void LEDFlashingCALLBack(void)
{
	

	if(DetectionThresholdCNT>250 && ledflashingfalg[1] <= 8)
	{
		DetectionThresholdCNT = 0;
		if(ledflashingfalg[0] == 0)
		{
			SET_LED(RESET);
			ledflashingfalg[0] = 1;
		}else
		{
			SET_LED(SET);
			ledflashingfalg[0] = 0;
		}
		ledflashingfalg[1] ++;
		////notesled flashing!\n");
	}
	if(ledflashingfalg[1] == 8)
	{
		ledfalshswitch = 0;
		ledflashingfalg[1] = 0;
	}
}

void NoNetworkLedFlashing(void)
{
	if(DetectionThresholdCNT>250 && ThereMinutesCnt <= 180000)
	{
		ThereMinutes = 1;
		DetectionThresholdCNT = 0;
		if(ledflashingfalg[0] == 0)
		{
			SET_LED(RESET);
			ledflashingfalg[0] = 1;
		}else
		{
			SET_LED(SET);
			ledflashingfalg[0] = 0;
		}
		
		////notesled flashing! : %d %d\n",ThereMinutes,ThereMinutesCnt);
	}
	if(ThereMinutesCnt >= 180000)
	{
		////notesdayu 18000\n");
		SET_LED(SET);
		lednetworkflag = 99;
		ThereMinutes = 0;
		ThereMinutesCnt = 0;
		
	}
}
void version_fun(void)
{
	versions[0] = MCU_VERSION_1;
	versions[1] = MCU_VERSION_2;
	versions[2] = MCU_VERSION_3;
}
void fun_setup_interrupt(void){
	
	get_real_time();
	TimerFlashDataHandle();
	CountdownFlashDataHandle();
	LEDFlashing_1s();
	//all_data_update_fun();
}

void low_power_consump_fun(void)
{
	if(MotorRunStatus){
		gptm0_4low = 0;
	}
    if (((gptm0_4low > 30000)&& (low_power_event_flag == 0) && ThereMinutes==0 && MotorRunStatus == FALSE && IsignCalibrating == FALSE)  || ProductionTestFlag)
	// if(gptm0_4low > 10000)
	{	
			low_power_init(); //低功耗初始化
			gptm0_4low = 0;
			motor_stop();
			low_power_event_flag = 1;
			tybn1_join_sleep_mode();
			GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_0,RESET);
			SET_LED(SET);
			ADC_Cmd(HT_ADC, DISABLE);//禁用ADC。 
			
			wRtcCounterTmp = RTC_GetCounter();

			TM_DeInit(HT_MCTM0);
			TM_DeInit(HT_SCTM1);

			// printf("enter slepp!\r\n");
			EXTI_ClearWakeupFlag(EXTI_CHANNEL_1);
			EXTI_ClearWakeupFlag(EXTI_CHANNEL_2);
			EXTI_ClearWakeupFlag(EXTI_CHANNEL_5);
			EXTI_ClearWakeupFlag(EXTI_CHANNEL_14);
			EXTI_ClearWakeupFlag(EXTI_CHANNEL_15); 
			EXTI_WakeupEventIntConfig(ENABLE);
			PWRCU_DeepSleep2(PWRCU_SLEEP_ENTRY_WFI);


			//printf("REBOOT slepp!\r\n");		
			adc_init();
			// uart0_init();	//B7.B8, TYBN1
			// usart0_init();	//C4.C5, 调试用
			key_init();			  //按键初始化
			
		}
}

void Production_test_input(unsigned char value)
{
	if(DatalenIn == 519)
	{
		my_memset(usart0_buf, 0, DatalenIn);
		DatalenIn = 0;
	}
    usart0_buf[DatalenIn] = value;
	DatalenIn++;
//	//notes%x\t",usart0_buf[DatalenIn]);

}

unsigned char Production_test_Read_Byte(void)
{
  	unsigned char value;
	if(DatalenIn != DatalenOUT)
	{
		if(DatalenOUT == 519)
		{
			my_memset(usart0_buf, 0, DatalenOUT);
			DatalenOUT = 0;
		}
		value = usart0_buf[DatalenOUT];
		DatalenOUT ++;
//		//notes%x\t",usart0_buf[DatalenOUT]);
	}
	
  	return value;
}

void LEDFlashing_1s(void)
{
	SET_LED(RESET);
	bftm_cnt = 0;
	fiveflag = 1;

	
}

void TimerFlashDataHandle(void)
{
	u32 i,k=4;
	TimerDataLength = rw(FLASH_52K_GLOBAL_DATA); 
	if(TimerDataLength >= 0xFF)
	{
		////noteslength err:%d\n",TimerDataLength);
		FirstStartSign = FALSE;
		
	}
	else
	{
		////noteslength:%d\n",TimerDataLength);
		for(i=0;i<TimerDataLength;i++)
		{
			TimerDatagroup[i] = rw(FLASH_52K_GLOBAL_DATA+k);
//			//notes8888888888888888888888888888:%x\n",TimerDatagroup[i]);
			k+=4;
		}
//		dp_curtain_timer_task(TimerDatagroup, TimerDataLength);
		FirstStartSign = TRUE;
	}
	
}

u32 I4_OTATime = 0 ,VS_CountfalshTime = 0;
void CountdownFlashDataHandle(void)
{
	//重启时，只有当倒计时.有效性标志为1的时候才会把倒计时任务重新上报
	if(!count_oper.flag){ 
		printf("count_oper flag =%d\r\n",count_oper.flag);
		return;
	}
	u32 i,k=0;
	u8 ts[4]={0};
	CountdownrData = rw(FLASH_53K_GLOBAL_DATA); 
	if(CountdownrData >= 0xFF)
	{
		////notescountdown err:%d\n",CountdownrData);
		cFirstStartSign = FALSE;
		
	}else{
		////notescountdown err:%d\n",CountdownrData);
		for(i=0;i<4;i++)
		{
			CountdownGroup[i] = rw(FLASH_53K_GLOBAL_DATA+k);
//			//notes-----------:%x\n",CountdownGroup[i]);
			k+=4;
		}
			
		ts[0] = rw(FLASH_53K_CURRENT_TIME);
		ts[1] = rw(FLASH_53K_CURRENT_TIME+4);
		ts[2] = rw(FLASH_53K_CURRENT_TIME+8);
		
		printf("ts = %d %d %d \r\n",ts[0],ts[1],ts[2]);
		I4_OTATime = (ts[0] * 3600) + (ts[1] * 60) + ts[2];
		printf("value = %x %x %x %x\r\n",CountdownGroup[0],CountdownGroup[1],CountdownGroup[2],CountdownGroup[3]);
		VS_CountfalshTime = (CountdownGroup[0] << 24 | CountdownGroup[1] << 16 | CountdownGroup[2] << 8 | CountdownGroup[3]);
		count_oper.updatatime = VS_CountfalshTime;
		VS_CountfalshTime *= 60;
		printf("I4_OTATime = %d , VS_CountfalshTime = %d  %d\r\n",I4_OTATime,VS_CountfalshTime,count_oper.updatatime);
		
		cFirstStartSign = TRUE;
			
	}
	
	
}

void LockedRotorMotorCallback(void)
{
	//ty_control1;
	ms_delay_systick(1000);
	if(signPositive_inversion== 1){
		close_curtain();
		ms_delay_systick(1000);
		motor_stop();
	}else if(signPositive_inversion == 2){
		open_curtain();
		ms_delay_systick(1000);
		motor_stop();
	}
	//ty_control0;
}

void LockedRotorDetection(void)
{
	if(!crc_flag){
		return;
	}
	//电压过低 
	if(i4_LowVlotageflag)
	{
		Stallfalag = TRUE;
		printf("LowVlotageflag !\r\n");
		mcu_dp_enum_update(DPID_CONTROL, 1);

		Pause_handler();

		MotorRunStatus = FALSE;
		i4_LowVlotageflag = FALSE;
		LEDFlashing_1s();

		// if (!LEFT_KEY() || !RIGHT_KEY())
		// {
		// 	lockturnflag = TRUE;
		// 	//ty_control1;
		// }

	}
#if 1
	//过流保护
	if ((motor_current > protect_current && !signFullonfulloff))
	{		
		Stallfalag = TRUE;

		printf("LockedRotorDetection\r\n");
		mcu_dp_enum_update(DPID_CONTROL, 1);
		
		Pause_handler();

		MotorRunStatus = FALSE;
		
		LEDFlashing_1s();
		motor_current = 0;
		
		// if (!LEFT_KEY() || !RIGHT_KEY())
		// {
		// 	lockturnflag = TRUE;
		// 	//ty_control1;
		// }
	}
#endif
}
void Pause_handler(void)
{
	bool callbackflag = FALSE;
	if(MotorRunStatus){
 
		update_current_runtime(0);


		if(IsignDirection == 1){
			vsignStopPercent = (cnt_switch * 100) / variable.full_running_time;
		}
		if(IsignDirection == 2){
			vsignStopPercent = (cnt_switch * 100) / variable.returntime;
		}
		printf("vsignStopPercent 1: %d\r\n", vsignStopPercent);	
		if (ptime.cnt_flag_on == 10)
        {
            vsignStopPercent += variable.global_percent;
            ptime.cnt_flag_on = 0; 
        }
        if (ptime.cnt_flag_on == 1)
        {
            vsignStopPercent = variable.global_percent - vsignStopPercent;
            ptime.cnt_flag_on = 0;
        }

		//堵转的标志 堵转后进行偏移修正
		if(Stallfalag){
			if(vsignStopPercent > 70 && variable.global_percent <= 70){
				vsignStopPercent = 100;
				callbackflag = TRUE;
				
			}
			if(vsignStopPercent < 30 && variable.global_percent >= 30){
				vsignStopPercent = 0;
			}
		}

		if(vsignStopPercent > 100){
			vsignStopPercent = 100;
		}
		if(vsignStopPercent < 0){
			vsignStopPercent = 0;
		}
		Stallfalag = FALSE;
		printf("vsignStopPercent 3: %d\r\n", vsignStopPercent);	
		//printf("\r\n cnt_switch = %d , variable.full_running_time = %d \r\n",cnt_switch,variable.full_running_time);
		ptime.cnt_flag = 0;
		cnt_switch = 0;
		

		printf("\r\n stop variable.global_percent = %d , vsignStopPercent = %d \r\n",variable.global_percent,vsignStopPercent);

		IsignStopPercent = TRUE;
		variable.global_percent = vsignStopPercent;
		mcu_dp_value_update(DPID_PERCENT_CONTROL, vsignStopPercent);
		mcu_dp_value_update(DPID_PERCENT_UP_CONTROL, vsignStopPercent);		

		motor_stop();
		Percentage_control();
		
		if(callbackflag){
			printf("Callback after offset correction\r\n");
			LockedRotorMotorCallback();
		}
		key_up_flag = 0;
		//ty_control0;
		key_delay_process_start();
	}
	
}



void update_curtain_position(u8 percentage)
{
	mcu_dp_value_update(DPID_PERCENT_CONTROL, percentage);
	
	mcu_dp_value_update(DPID_PERCENT_UP_CONTROL, percentage);//4.Report current actual location
	mcu_dp_value_update(DPID_PERCENT_UP_CONTROL_STATUS, 2); 			  //5.Report the end running status
	mcu_dp_enum_update(DPID_CONTROL, 1);
	update_current_runtime(0);

	variable.global_percent = percentage;
	Percentage_control();
	key_up_flag = 0;
	MotorRunStatus = FALSE; 
	gptm0_4low = 0;
	cnt_switch = 0;
	ptime.cnt_flag = 0;
	KeyControl = FALSE;

}

void key_delay_process_start(void){
	key_delay_process = 0;//计时得 计数一秒进key_delay_process_callback
	KeyControl = FALSE;
	key_delay_process_flag = TRUE;
}
void key_delay_process_callback(void){
	KeyControl = TRUE;
	key_delay_process_flag = FALSE;
	//printf("key_delay_process_callback ty_control:%d auto_run_mode:%d %d\r\n",ty_control,//variable.auto_run_mode ,variable.run_mode_flag);
}

u32 signStallCnt=0,signStallCntcur=0;
u32 VS_Calibrationtime = 0;
void timer_curtain_control_callback(void)
{
	if(MotorRunStatus){

		if (ptime.cnt_flag == 1 )
	    {

	        if (ptime.runtime  <= cnt_switch)
	        {
				//printf("482line:curtain run time = %d\n",cnt_switch);

	            variable.global_percent = Exchange_percentage;
				//printf("\r\n end variable.global_percent = %d  \r\n",variable.global_percent);
	            Exchange_percentage = 0;

				
				update_curtain_position(variable.global_percent);
				
				if(variable.global_percent == 100)
				{
					signFullonfulloff = TRUE;
					VS_TimeCalibraCnt = 0;
					//USART_RxCmd(COM_TYBN1, DISABLE);
					signStallCnt = VS_TimeCalibraCnt;
			
					VS_Calibrationtime =  (variable.returntime /2);
					
					printf("VS_Calibrationtime =%d\r\n",VS_Calibrationtime);
				}else{
					motor_stop();
					key_delay_process_start();
				}
				
				////variable.auto_run_mode = 1;
			}
				

		}
	}
    


	if(signFullonfulloff)
	{	  
 
		if(motor_current > protect_current || i4_LowVlotageflag)//|| VS_Calibrationtime < VS_TimeCalibraCnt)
		{
			//USART_RxCmd(COM_TYBN1, ENABLE);
			motor_stop();
			signFullonfulloff = FALSE;
			
			I4_LockedRotorCallback();
			MotorRunStatus = FALSE; 
			key_delay_process_start();
		}
		
		if( VS_Calibrationtime < VS_TimeCalibraCnt)
		{
			//USART_RxCmd(COM_TYBN1, ENABLE);
			motor_stop();
			signFullonfulloff = FALSE;
		
			I4_LockedRotorCallback();
			MotorRunStatus = FALSE; 
			key_delay_process_start();
		}
		
	}

	if(key_delay_process_flag){
		if(key_delay_process >= 1000){
			key_delay_process_callback();
		}
		//printf("key_delay_process:%d\r\n",key_delay_process);
	}
	
}

void I4_LedTwinkle(void){
	SET_LED(RESET);
	ms_delay_systick(1000);
	SET_LED(SET);
}

void I4_ProductionTest(void)
{
	I4_LedTwinkle();	
	//printf("test\n");
	while(1){
		
		bt_test_uart_service();//产测
		bt_uart_service();
		if(!LEFT_KEY()){
			I4_LedTwinkle();
			ProductionTestFlag = TRUE;
			low_power_consump_fun();
		}
		if(!RIGHT_KEY()){
			I4_LedTwinkle();
			close_curtain();
			ProductionTestFlag = FALSE;
		}
		if(!SET_KEY()){
			I4_LedTwinkle();
			motor_stop();
			ProductionTestFlag = FALSE;
		}
	}
}
void I4_FullInspection(void)
{
	SET_LED(SET);
	I4_LedTwinkle();
	SET_LED(SET);
	bool flag = TRUE;
	while(1)
	{			
		if (!SET_KEY()){
			motor_stop();
		}

		if (!LEFT_KEY()){
			close_curtain();
		}

		if (!RIGHT_KEY()){

			open_curtain();
		}

		if(motor_current > protect_current || i4_LowVlotageflag)
		{
			flag = TRUE;
			SET_LED(RESET);
			motor_stop();
			ms_delay_systick(3000);
			//printf("sign = %d \r\n",signPositive_inversion);
		if(signPositive_inversion == 2 && flag){
			open_curtain();
			flag = FALSE;
			SET_LED(SET);
		}
		if(signPositive_inversion == 1 && flag){
			close_curtain();
			flag = FALSE;
			SET_LED(SET);
		}
			i4_LowVlotageflag = FALSE;
			ms_delay_systick(1000);
		}

	
	
	}
	
	
}


void I4_Timecallback(void)
{
	 if (rtc_interrupt > 10000) 
    {
    
        time_t rawtime;
    	if(IS_Timezone){
			 rawtime = rtc_interrupt + check_time_zone(VS_Timezonetime);//+ 28800;
		}else{
			 rawtime = rtc_interrupt - check_time_zone(VS_Timezonetime);//+ 28800;
		}
        struct tm *info;
        info = localtime(&rawtime);
        timer_wday = info->tm_wday;
        timer_hours = info->tm_hour;
        timer_minutes = info->tm_min;
        lssued_time = (info->tm_hour * 3600) + (info->tm_min * 60) + info->tm_sec;

		VS_CurTime[0] = timer_hours;
		VS_CurTime[1] = timer_minutes;
		VS_CurTime[2] = info->tm_sec;
        single_timing = lssued_time;
		//notes155:connect time:%d hour %d minue %d \n",single_timing,info->tm_hour,info->tm_min);
        if (timer_wday == 0)
        {
            timer_wday = 7;
        }
        else
        {
            lssued_time += (24 * 3600 * (timer_wday - 1));
        }

		if(!RtcCntSign){
//printf(" FirstStartSign = %d , cFirstStartSign = %d !\n",FirstStartSign,cFirstStartSign);
			if(FirstStartSign == TRUE)
			{

				dp_curtain_timer_task(TimerDatagroup, TimerDataLength);
			}
			if(cFirstStartSign == TRUE)
			{

				if(single_timing < (I4_OTATime+VS_CountfalshTime))
				{
					VS_ResetCountTime = (I4_OTATime+VS_CountfalshTime) - single_timing;

					mcu_dp_bool_update(DPID_COUNTDOWN_ONOFF, count_oper.gswitch);
					dp_download_curtain_countdown_time(CountdownGroup, 4);

					//mcu_dp_value_update(DPID_COUNTDOWN_TIME, count_oper.updatatime);
				}else{

				}
				
			}
			
		RtcCntSign = TRUE;
		}
		
    }
}


void I4_LEDHandler(void)
{
	 //*********************************
	if(ledfalshswitch == 1)
	{
		LEDFlashingCALLBack();
	}
	if(lednetworkflag == 0)
	{
		NoNetworkLedFlashing();
	}
	else if(lednetworkflag == 2 || lednetworkflag == 1)
	{
		SET_LED(SET);
		lednetworkflag = 99;
		ThereMinutes = 0; 
		ThereMinutesCnt = 0;
	}
	if(fiveflag == 1)
	{
//		//notesbf:%d\n",bftm_cnt);
		if(bftm_cnt > 500)
		{
			SET_LED(SET);
			bftm_cnt = 0;
			fiveflag = 0;
		}
	}
	//***********************************

}

void I4_MainTask(void)
{
	while (1)
	{
		I4_LEDHandler();		//led灯处理
		motor_turn();		 	//正反转按键
		
		calibration_test();	  //校准
		bt_uart_service(); 	 //涂鸦协议处理
		if(IS_FullInspection)
		{
			I4_FullInspection();//整机全检
		}
		//bt_test_uart_service();//产测
		
		Timing_task_program();			//定时任务
		low_power_consump_fun();		//30s未操作进入低功耗模式

		timer_curtain_control_callback();//curtain position update
		LockedRotorDetection();			//Plugging Detection

		I4_Timecallback();		//时区时间处理

		
	}
}

