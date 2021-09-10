/*************************************    ***Copyright (c)*************************
**                               版权所�?(C), 2015-2020, 涂鸦科技
**
**                                 http://www.tuya.com
**
**--------------文件信息-------------------------------------------------------
**�?  �?  �? protocol.c
**�?       �? 下发/上报数据处理函数
**�?�?�?�?:

                  *******非常重要，一定要看哦！！�?*******

** 1、用户在此文件中实现数据下发/上报功能
** 2、DP的ID/TYPE及数据处理函数都需要用户按照实际定义实�?
** 3、当开始某些宏定义后需要用户实现代码的函数内部�?err提示,完成函数后请删除�?err
**
**--------------版本修订记录---------------------------------------------------

** �? �?v2.0
** 日　�? 2020�?�?3�?
** 描　�? 
1.	增加cmd 0x09模块解绑接口支持
2.	增加cmd 0x0e rf射频测试接口支持
3.	增加cmd 0xe0 记录型数据上报接口支�?
4.	增加cmd 0xE1 获取实时时间接口支持
5.	增加 cmd 0xe2 修改休眠模式状态广播间隔支�?
6.	增加 cmd 0xe4 关闭系统时钟功能支持
7.	增加 cmd 0xe5 低功耗使能支�?
8.	增加 cmd 0xe6 获取一次性动态密码支�?
9.	增加 cmd 0xe7断开蓝牙连接支持
10.	增加 cmd 0xe8 查询MCU版本号支�?
11.	增加 cmd 0xe9 MCU主动发送版本号支持
12.	增加 cmd 0xea OTA升级请求支持
13.	增加 cmd 0xeb OTA升级文件信息支持
14.	增加 cmd 0xec OTA升级文件偏移请求支持
15.	增加 cmd 0xed OTA升级数据支持
16.	增加 cmd 0xee OTA升级结束支持
17.	增加 cmd 0xa0 MCU 获取模块版本信息支持
18.	增加 cmd 0xa1 恢复出厂设置通知支持
19.  增加MCU OTA demo
20. 优化串口解析�?

******************************************************************************/
#include "head.h"
timer timer_task[8] = {0};
countdown count_oper = {0};

u8 TimerDatagroup[96]={0};
u16 TimerDataLength = 0;
u8  CountdownGroup[4]={0};
u16  CountdownLength = 0;

u32 temporary_timing_buffer[65] = {0};
u8 Single_or_repeated_flag[8] = {0};
u8 Countdown_switch = 1;// count down curtain on or off flag   0:off  1:on
u8 Exchange_percentage = 0;
bool OperIfvalue = FALSE;

/******************************************************************************
                                移植须知:
1:MCU必须在while中直接调用mcu_api.c内的bt_uart_service()函数
2:程序正常初始化完成后,建议不进行关串口中断,如必须关中断,关中断时间必须短,关中断会引起串口数据包丢�?
3:请勿在中�?定时器中断内调用上报函数
******************************************************************************/

/******************************************************************************
                              第一�?初始�?
1:在需要使用到bt相关文件的文件中include "bt.h"
2:在MCU初始化中调用mcu_api.c文件中的bt_protocol_init()函数
3:将MCU串口单字节发送函数填入protocol.c文件中uart_transmit_output函数�?并删�?error
4:在MCU串口接收函数中调用mcu_api.c文件内的uart_receive_input函数,并将接收到的字节作为参数传入
5:单片机进入while循环后调用mcu_api.c文件内的bt_uart_service()函数
******************************************************************************/

/******************************************************************************
                        1:dp数据点序列类型对照表
          **此为自动生成代码,如在开发平台有相关修改请重新下载MCU_SDK**         
******************************************************************************/
const DOWNLOAD_CMD_S download_cmd[] =
    {
        {DPID_CONTROL, DP_TYPE_ENUM},
        {DPID_PERCENT_CONTROL, DP_TYPE_VALUE},
        {DPID_MODE, DP_TYPE_ENUM},
        {DPID_CONTROL_BACK, DP_TYPE_ENUM},
        {DPID_TIME_TOTAL, DP_TYPE_VALUE},
        {DPID_FAULT, DP_TYPE_BITMAP},
        {DPID_BATTERY_PERCENTAGE, DP_TYPE_VALUE},
        {DPID_BORDER, DP_TYPE_ENUM},
        {DPID_POSITION_BEST, DP_TYPE_VALUE},
        //{DPID_LED_SETTING, DP_TYPE_STRING},
        {DPID_CURTAIN_MODE, DP_TYPE_ENUM},
        {DPID_CALIBRATION_STATUS, DP_TYPE_ENUM},
        {DPID_CALIBRATION_OPERATION, DP_TYPE_ENUM},
        // {DPID_BATTERY_REMAIN, DP_TYPE_VALUE},
        {DPID_AUTO_ON_OFF, DP_TYPE_BOOL},
        {DPID_SILENT_MODE, DP_TYPE_BOOL},
        {DPID_TAIMED_TASK, DP_TYPE_RAW},
        {DPID_COUNTDOWN_ONOFF, DP_TYPE_BOOL},
        {DPID_COUNTDOWN_TIME, DP_TYPE_VALUE},
        {DPID_COUNTDOWN_OP, DP_TYPE_BOOL},
        {DPID_FAULT_REMOVAL, DP_TYPE_BOOL},

};

/******************************************************************************
                           2:串口单字节发送函�?
请将MCU串口发送函数填入该函数�?并将接收到的数据作为参数传入串口发送函�?
******************************************************************************/

/*****************************************************************************
函数名称 : uart_transmit_output
功能描述 : 发数据处�?
输入参数 : value:串口收到字节数据
返回参数 : �?
使用说明 : 请将MCU串口发送函数填入该函数�?并将接收到的数据作为参数传入串口发送函�?
*****************************************************************************/
void uart_transmit_output(unsigned char value)
{
    //#error "请将MCU串口发送函数填入该函数,并删除该�?

    uart_transmit_output_fun(value);
 

    /*
  //示例:
  extern void Uart_PutChar(unsigned char value);
  Uart_PutChar(value);	                                //串口发送函�?
*/
}
/******************************************************************************
                           第二�?实现具体用户函数
1:APP下发数据处理
2:数据上报处理
******************************************************************************/

/******************************************************************************
                            1:所有数据上报处�?
当前函数处理全部数据上报(包括可下�?可上报和只上�?
  需要用户按照实际情况实�?
  1:需要实现可下发/可上报数据点上报
  2:需要实现只上报数据点上�?
此函数为MCU内部必须调用
用户也可调用此函数实现全部数据上�?
******************************************************************************/

//自动化生成数据上报函�?

/*****************************************************************************
函数名称 : all_data_update
功能描述 : 系统所有dp点信息上�?实现APP和muc数据同步
输入参数 : �?
返回参数 : �?
使用说明 : 此函数SDK内部需调用;
           MCU必须实现该函数内数据上报功能;包括只上报和可上报可下发型数�?
*****************************************************************************/
void all_data_update(void)
{
    //#error "请在此处理可下发可上报数据及只上报数据示�?处理完成后删除该�?
    //此代码为平台自动生成，请按照实际数据修改每个可下发可上报函数和只上报函数

    // all_data_update_fun();
}

/******************************************************************************
                                WARNING!!!    
                            2:所有数据上报处�?
自动化代码模板函�?具体请用户自行实现数据处�?
******************************************************************************/




void update_current_runtime(u32 current_data)
{
    u8 crc8 = 0;

    dp_data_buf[0] = 0x55;
    dp_data_buf[1] = 0xAA;
    dp_data_buf[2] = VERSION;
    dp_data_buf[3] = 0x07;
    dp_data_buf[4] = 0x00; //数据长度
    dp_data_buf[5] = 0x08; //数据长度
    dp_data_buf[6] = 0x0A;
    dp_data_buf[7] = 0x02;
    dp_data_buf[8] = 0x00;
    dp_data_buf[9] = 0x04;
    dp_data_buf[10] = (current_data >> 24);  //电流
    dp_data_buf[11] = (current_data >> 16);  //电流
    dp_data_buf[12] = (current_data >> 8);   //电流
    dp_data_buf[13] = (current_data & 0xFF); //电流
    crc8 = get_check_sum(dp_data_buf, 14);
    dp_data_buf[14] = crc8;
    uart0_send(dp_data_buf, 15);
    //usart0_send(dp_data_buf,15);
}
/*****************************************************************************
函数名称 : dp_download_control_handle
功能描述 : 针对DPID_CONTROL的处理函�?
输入参数 : value:数据源数�?
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_control_handle(const unsigned char value[], unsigned short length)
{
    //示例:当前DP类型为ENUM
    //check_battery_level();//1.18: 每次涂鸦app控制时上报一次电池电量�?
    // //notes%#x\n", value[0]);
    switch (value[0])
    {
    case 0:
		if(crc_flag == 1 && !signFullonfulloff && !MotorRunStatus)//&& ty_control == 0)
		{
			KeyControl = FALSE;
			ptime.cnt_flag_on = 1;
			
			mcu_dp_enum_update(DPID_CONTROL, value[0]);

			timer_curtain_control(100);

		}
		else
        {
            SET_LED(RESET);
            ms_delay_systick(1000);
        }
        
        break;   

    case 1:
        KeyControl = TRUE;
        mcu_dp_enum_update(DPID_CONTROL, value[0]);
        Pause_handler();
		key_up_flag =0;
        motor_stop();
		signFullonfulloff = FALSE;
		MotorRunStatus = FALSE;
	
		mcu_dp_value_update(DPID_PERCENT_CONTROL, variable.global_percent);
		mcu_dp_value_update(DPID_PERCENT_UP_CONTROL, variable.global_percent);
        break;

    case 2:
		if(crc_flag == 1 && !signFullonfulloff && !MotorRunStatus)//&& ty_control == 0)
		{

			KeyControl = FALSE;
			ptime.cnt_flag_on = 10;

			mcu_dp_enum_update(DPID_CONTROL, value[0]);

			timer_curtain_control(0);

		}
		else
		{
			SET_LED(RESET);
			ms_delay_systick(1000);
		}
        
        break;

    default:
        break;
    }
}

void I4CourseDirection(u32 *global,u32 *local,u8 percent,u32 runtime)
{
	*global = variable.global_percent* runtime / 100 ; 
    *local = percent * runtime/ 100 ; 
}
/**
 * @description: 针对窗帘打开或关闭百分比的控制函�?
 * @param {u8} current_percent
 * @return {*}
 */
 u32 timerun = 0;
void timer_curtain_control(u8 current_percent)
{
    u32 global_time = 0;
    u32 local_time = 0;
	if(current_percent < variable.global_percent){
		IsignDirection = 1;
	}else{
		IsignDirection = 2;
	}

	
	if(MotorRunStatus)//针对一键执行的处理
	{
		Pause_handler();
		key_up_flag =0;
//		motor_stop();
//		printf("motor runing!!!!!!!!!!!\r\n");
//		ms_delay_systick(3000);
	}

	if(current_percent == variable.global_percent){//TODO:目标百分比跟当前百分比重复的情况处理
		mcu_dp_value_update(DPID_PERCENT_CONTROL, variable.global_percent);
		update_current_runtime(0);
		mcu_dp_value_update(DPID_PERCENT_UP_CONTROL, variable.global_percent);
		mcu_dp_enum_update(DPID_CONTROL, 1);
		key_up_flag = 0;
		KeyControl = TRUE;
        LEDFlashing_1s();
		return;
	}

	if(IsignStopPercent){

		variable.global_percent = vsignStopPercent;
		IsignStopPercent = FALSE;
	}



    if(IsignDirection == 1){
        I4CourseDirection(&global_time,&local_time,current_percent,variable.full_running_time);
    }
    if(IsignDirection == 2){
        I4CourseDirection(&global_time,&local_time,current_percent,variable.returntime);
    }



	
	
    ptime.runtime = local_time - global_time;
    //电机启动会有400ms延迟 ，这里补偿时间上去
    ptime.runtime += 400;
	
//	printf("global_percent = %d,current_percent = %d.\r\n",variable.global_percent,current_percent);
//	I4_LOG("runtime f=%d,r=%d\r\n",variable.full_running_time,variable.returntime);
	

    if (ptime.runtime < 0)
    {
    	percor.open = 0;
        ptime.runtime = ~(ptime.runtime - 1);
		I4_LOG("ptime.runtime = %d.\r\n",ptime.runtime);
        update_current_runtime(ptime.runtime); 							//1.Report runing time
        mcu_dp_enum_update(DPID_PERCENT_UP_CONTROL_STATUS, 1);			//2.Report operation status 
        mcu_dp_value_update(DPID_PERCENT_CONTROL, current_percent);		//3.Reported target percentage accepted
        
		//notescurtain_mode = %d\n",curtain_mode);
		if(curtain_mode==0){
			//notescurtain close time = %d!\n",ptime.runtime);	
			close_curtain();
		}else{
			//notescurtain open time = %d!\n",ptime.runtime);		
			open_curtain();
		}  
        
        ptime.cnt_flag_on = 1;
        // ms_delay_systick(1000);
        // SET_LED(RESET);
    }
    else
    {
        if (ptime.runtime != 0)
        {
        	percor.open = 1;
			I4_LOG("ptime.runtime = %d.\r\n",ptime.runtime);
            update_current_runtime(ptime.runtime); 								//1.Report runing time
            mcu_dp_enum_update(DPID_PERCENT_UP_CONTROL_STATUS, 0);				//2.Report operation status
            mcu_dp_value_update(DPID_PERCENT_CONTROL, current_percent);			//3.Reported target percentage accepted
            

            //notescurtain_mode = %d\n",curtain_mode);					
            if(curtain_mode==0){
				//notescurtain open time = %d!\n",ptime.runtime);	
				open_curtain();
			}else{
				//notescurtain close time = %d!\n",ptime.runtime);
				close_curtain();
			}
			
            ptime.cnt_flag_on = 10;
        }

        // SET_LED(RESET);
    }
    Exchange_percentage = current_percent; //更新本地百分比全局参数
	MotorRunStatus = TRUE;	
    ptime.cnt_flag = 1;
}
/*****************************************************************************
函数名称 : dp_download_percent_control_handle
功能描述 : 针对DPID_PERCENT_CONTROL的处理函�?
输入参数 : value:数据源数�?
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_percent_control_handle(const unsigned char value[], unsigned short length)
{
    //示例:当前DP类型为VALUE
    unsigned char ret;
    unsigned long percent_control;
    unsigned char current_percent;
	
    percent_control = mcu_get_dp_download_value(value, length);
    /*VALUE类型数据处理*/
    current_percent = value[3];
	if(CalibrMotor == TRUE)
	{
		CalibrMotor = FALSE;
	}else{
		//ty_control1;
		timer_curtain_control(current_percent);
	}
    
}

/*****************************************************************************
函数名称 : dp_download_mode_handle
功能描述 : 针对DPID_MODE的处理函�?
输入参数 : value:数据源数�?
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_mode_handle(const unsigned char value[], unsigned short length)
{
    //示例:当前DP类型为ENUM
    unsigned char ret;
    unsigned char mode;

    mode = mcu_get_dp_download_enum(value, length);
    switch (mode)
    {
    case 0:
        break;

    case 1:
        break;

    default:

        break;
    }

    //处理完DP数据后应有反�?
    ret = mcu_dp_enum_update(DPID_MODE, mode);
    if (ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}

/*****************************************************************************
函数名称 : dp_download_control_back_handle
功能描述 : 针对DPID_CONTROL_BACK的处理函�?
输入参数 : value:数据源数�?
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_control_back_handle(const unsigned char value[], unsigned short length)
{
    //示例:当前DP类型为ENUM
    unsigned char ret;
    unsigned char control_back;

    control_back = mcu_get_dp_download_enum(value, length);
    switch (control_back)
    {
    case 0:
        break;

    case 1:
        break;

    default:

        break;
    }

    //处理完DP数据后应有反�?
    ret = mcu_dp_enum_update(DPID_CONTROL_BACK, control_back);
    if (ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}

/*****************************************************************************
函数名称 : dp_download_border_handle
功能描述 : 针对DPID_BORDER的处理函�?
输入参数 : value:数据源数�?
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_border_handle(const unsigned char value[], unsigned short length)
{
    //示例:当前DP类型为ENUM
    unsigned char ret;
    unsigned char border;

    border = mcu_get_dp_download_enum(value, length);
    switch (border)
    {
    case 0:
        break;

    case 1:
        break;

    case 2:
        break;

    case 3:
        break;

    default:

        break;
    }

    //处理完DP数据后应有反�?
    ret = mcu_dp_enum_update(DPID_BORDER, border);
    if (ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}

/*****************************************************************************
函数名称 : dp_download_position_best_handle
功能描述 : 针对DPID_POSITION_BEST的处理函�?
输入参数 : value:数据源数�?
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_position_best_handle(const unsigned char value[], unsigned short length)
{
    //示例:当前DP类型为VALUE
    unsigned char ret;
    unsigned long position_best;

    position_best = mcu_get_dp_download_value(value, length);
    /*
    //VALUE类型数据处理
    
    */

    //处理完DP数据后应有反�?
    ret = mcu_dp_value_update(DPID_POSITION_BEST, position_best);
    if (ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}

/*****************************************************************************
函数名称 : dp_download_led_setting_handle
功能描述 : 针对DPID_LED_SETTING的处理函�?
输入参数 : value:数据源数�?
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_led_setting_handle(const unsigned char value[], unsigned short length)
{
    //示例:当前DP类型为STRING
    unsigned char ret;


    //处理完DP数据后应有反�?
    ret = mcu_dp_string_update(DPID_LED_SETTING, value, length);
    if (ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}




//自动开关模�?
static unsigned char dp_curtain_auto_mode(const unsigned char value[], unsigned short length)
{
    //示例:当前DP类型为STRING
    unsigned char ret;
    unsigned char string_data = 0;

    string_data = value[0];
    if (string_data == 1)
    {

        //variable.auto_run_mode = 1;
        variable.run_mode_flag = 1;
        arg_erase_write();
    }
    else
    {
        //variable.auto_run_mode = 0;
        variable.run_mode_flag = 0;
        arg_erase_write();
    }
    //处理完DP数据后应有反�?
    ret = mcu_dp_bool_update(DPID_AUTO_ON_OFF, string_data);
    if (ret == SUCCESS)
    {
        return SUCCESS;
    }
    else
        return ERROR;
}


#if 0
//静音模式
static unsigned char dp_curtain_silent_mode(const unsigned char value[], unsigned short length)
{
    //示例:当前DP类型为STRING
    unsigned char ret;
    unsigned char string_data = 0;

    variable.mute_mode = value[0];
    if (variable.mute_mode == 1)
    {
//    	if(Silenttime.)
        speed = 0x50;
        protect_current = MUTE_THRESHOLD_VALUE; // NOTE: 当静音模式启用，堵转检测阈值更新为 250ma
        mute_erase_write();
//		if()
		if(silenttime.Muteopentime == -1)
		{
			I4_LOG("Mute mode!\r\n");
			update_calibrstatus(0);
		}
    }
    else
    {
        speed = 0x64;
        protect_current = THRESHOLD_VALUE; // NOTE: 当静音模式关闭，堵转检测阈值更新为 310ma
        mute_erase_write();
		if(variable.full_running_time == -1){

		}else{
			update_calibrstatus(2);
		}
    }
    //处理完DP数据后应有反�?
    // ms_delay_systick(5000);
    ret = mcu_dp_bool_update(DPID_SILENT_MODE, variable.mute_mode);
    if (ret == SUCCESS)
    {
        return SUCCESS;
    }

    else
        return ERROR;
}
#endif


//定时任务
unsigned char dp_curtain_timer_task(const unsigned char value[], unsigned short length)
{

    // setup_interrupt(5);
    //示例:当前DP类型为STRING
    int time_cha = 0,flashcnt,addrcnt=4;
    unsigned char ret;
    unsigned char string_data = 0;
    u8 *tmp = (u8 *)value;

    memset(timing_buffer, 0, sizeof(int) * 64);
    memset(timer_buf, 0, sizeof(int) * 56);
    memset(timer_buf_single, 0, sizeof(int) * 8);
    memset(Single_or_repeated_flag, 0, sizeof(char) * 8);
	memcpy(TimerDatagroup,value,sizeof(char)*96);
	TimerDataLength = length;
	
    u32 timer_value;
    u8 timer_week_day; //标识当天为周�?
    u8 i, j, k;

    FLASH_ErasePage(FLASH_52K_GLOBAL_DATA);

	FLASH_ProgramWordData(FLASH_52K_GLOBAL_DATA, TimerDataLength);
	for(flashcnt=0;flashcnt<TimerDataLength;flashcnt++)
	{
		FLASH_ProgramWordData(FLASH_52K_GLOBAL_DATA+addrcnt, TimerDatagroup[flashcnt]);
		addrcnt += 4;
	}
	

    wRtcCounterTmp = RTC_GetCounter();
    printf("current RTCtime:%d \r\n", wRtcCounterTmp);
	
    for (i = 0; i < 8; i++) //8组数据，每一组数据由小时分钟，开关，百分比，有效性和七天标志位组成�?
    {
        timer_task[i].timer_thours = tmp[0];
        timer_task[i].timer_tminutes = tmp[1];
        timer_task[i].timer_ton_offs = tmp[2];
        timer_task[i].timer_percent = tmp[3];
        timer_task[i].Effectiveness = tmp[4];

        if (timer_task[i].Effectiveness == 1) //有效�?
        {

            for (j = 0; j < 7; j++) //七天
            {
                k = j + 5;
                timer_task[i].timer_buf1[j] = tmp[k];
                if (timer_task[i].timer_buf1[j] == 1)
                {
                    Single_or_repeated_flag[i] = 1; //进入这个if说明不是仅当日生效，将标志位�?，后面得if进行判断
                    timer_week_day = j;
                    timer_value = timer_task[i].timer_thours * 3600 + timer_task[i].timer_tminutes * 60;
                    timer_value += (86400 * timer_week_day);
                    if (timer_value <= lssued_time)
                    {
                        timer_value += 604800;
                        time_cha = timer_value - lssued_time; //算出当前时间跟定时时间相差多少秒
                        timer_buf[i][j] = time_cha + wRtcCounterTmp;
                    }
                    else
                    {
                        time_cha = timer_value - lssued_time; //算出当前时间跟定时时间相差多少秒
                        timer_buf[i][j] = time_cha + wRtcCounterTmp;
                    }
                }
            }
            if (Single_or_repeated_flag[i] == 0)
            {
                timer_value = timer_task[i].timer_thours * 3600 + timer_task[i].timer_tminutes * 60;
                time_cha = timer_value - single_timing; // + rtc_interrupt;
				//printf("%d single_timing:%d timervalue : %d time_cha:%d\n", __LINE__,single_timing,timer_value,time_cha);
                if (time_cha < 0)
                {
                    timer_buf_single[i] = 86400+wRtcCounterTmp;
//					for(i=0;i<12;i++){
//                        //notes%x\t", TimerDatagroup[i]);
//                    
//                    }
                    TimerDatagroup[i*12+4] = 1;
//					printf("TimerDatagroup %x \n",TimerDatagroup[i*12+4]);
//                    printf("xiao:%d\n", timer_buf_single[i]);
                }
                else
                {
                    timer_buf_single[i] = time_cha + wRtcCounterTmp;
//                    printf("696：Timing time:%d\n", timer_buf_single[i]);
                }
            }
        }

        if (timer_task[i].Effectiveness == 0)
        {
            for (j = 0; j < 7; j++) //七天
            {
                timer_buf[i][j] = 0;
            }
            timer_buf_single[i] = 0;
        }

        tmp += 12;
    }
    //处理完DP数据后应有反�?
    ret = mcu_dp_raw_update(DPID_TAIMED_TASK, TimerDatagroup, length);
    if (ret == SUCCESS)
    {
        temporary_timing_buffer[64] = count_oper.localtime;
        // //notescount_oper.time:%d\n",count_oper.time);

        memcpy(temporary_timing_buffer, ((int *)timer_buf), sizeof(u32) * 56);
        memcpy(&temporary_timing_buffer[56], timer_buf_single, sizeof(u32) * 8);

        for (i = 0; i < 65; i++) //对定时时间进行排序�?
        {
            for (int j = 0; j < 65 - 1 - i; j++)
            {
                if (temporary_timing_buffer[j] > temporary_timing_buffer[j + 1])
                {
                    int tmp = temporary_timing_buffer[j];
                    temporary_timing_buffer[j] = temporary_timing_buffer[j + 1];
                    temporary_timing_buffer[j + 1] = tmp;
                }
            }
        }
		printf("-----------------------------------\n");
        for (i = 0, j = 0; i < 65; i++) //
        {
            if (temporary_timing_buffer[i] > 0)
            {
                timing_buffer[j] = temporary_timing_buffer[i];
                j++;
            }
			
        }
		for(i = 0, j = 0; i < 65; i++)
		{
			printf("timer:%d\t",timing_buffer[i]);
		}
		
        flage_timer_on = 1;
        // Initial_count = RTC_GetCounter();
        // Next_time = 0;
        tmp = 0;
        return SUCCESS;
    }
    else
        return ERROR;
        
}
//倒计时任�?开�?
static unsigned char dp_curtain_countdown_on(const unsigned char value[], unsigned short length)
{
    //示例:当前DP类型为STRING
    unsigned char ret;
    unsigned char string_data;

    Countdown_switch = value[0];
    // //notestime_oo_off:%d\n",Countdown_switch);
    // //notestime_oo_off:%d\n",length);

    //处理完DP数据后应有反�?
    ret = mcu_dp_bool_update(DPID_COUNTDOWN_ONOFF, Countdown_switch);

    if (ret == SUCCESS)
    {
        count_oper.gswitch = Countdown_switch;
        Coundownswitch_EraseWrite();
        count_oper.gswitch = rw(FLASH_55K_DOUNTDOWN_SWITCH);
        printf("gswitch = %d\r\n", count_oper.gswitch);
        return SUCCESS;
    }

    else
        return ERROR;
}
//倒计时：有效�?
static unsigned char dp_download_curtain_countdown_opreation(const unsigned char value[], unsigned short length)
{
    //示例:当前DP类型为STRING
    unsigned char ret;
    unsigned char string_data;
   
    string_data = value[0];
	
    ret = mcu_dp_bool_update(DPID_COUNTDOWN_OP, string_data);
    if (ret == SUCCESS)
    {
        if (string_data == 1) //0，是关闭�?是打开
        {
            count_oper.flag = 1;
        }
        else
        {
            count_oper.flag = 0;

		} 

        Coundownflag_EraseWrite();
        count_oper.flag = rw(FLASH_55K_DOUNTDOWN_FLAG);
        printf("flag = %d\r\n", count_oper.flag);
        return SUCCESS;
    }

    else
        return ERROR;
}
//倒计时：时间
unsigned char dp_download_curtain_countdown_time(const unsigned char value[], unsigned short length)
{
#if 1	
    //示例:当前DP类型为STRING
    unsigned char ret;
    unsigned int string_data;

	u8 time[4]={0};
    u32 Swap_array[64] = {0};
    u32 i, j,k, flag = 0,flashp=0,position=0;

	CountdownLength = length;
	memcpy(CountdownGroup,value,sizeof(char)*CountdownLength);
	
    string_data = ((value[0]<<24) | (value[1]<<16)| (value[2] << 8) | value[3]);

	// printf("value = %x %x %x %x string = %ld\r\n",CountdownGroup[0],CountdownGroup[1],CountdownGroup[2],CountdownGroup[3],string_data);

	FLASH_ErasePage(FLASH_53K_GLOBAL_DATA);
	for(k=0;k<CountdownLength;k++)
	{

		FLASH_ProgramWordData(FLASH_53K_GLOBAL_DATA+flashp, CountdownGroup[k]);
		FLASH_ProgramWordData(FLASH_53K_CURRENT_TIME + flashp , VS_CurTime[k]);
		flashp+=4;

	}
	
    ret = mcu_dp_value_update(DPID_COUNTDOWN_TIME, string_data);
    count_oper.updatatime = string_data;
    if (ret == SUCCESS)
    {	
    	if(count_oper.flag == 1 && string_data)
    	{
// printf("count_oper.flag == 1\r\n");            
			for (i = 0; i < 56; i++)
			{
				if (timing_buffer[i] == count_oper.localtime)
				{
					timing_buffer[i] = 0;
				}
			}
// printf("count_oper.time = %d %d %d\r\n",count_oper.time,cFirstStartSign,VS_ResetCountTime);
	        u32 current_cnt = RTC_GetCounter();
			if(cFirstStartSign){
				count_oper.localtime = VS_ResetCountTime;
				
				cFirstStartSign = FALSE;
			}else{
				count_oper.localtime = (string_data * 60) + current_cnt;
			}
printf("count_oper.localtime = %d\r\n",count_oper.localtime);
	        for (i = 0; i < 64; i++)
	        {
	            Swap_array[i] = timing_buffer[i];
	        }
	        Swap_array[60] = count_oper.localtime;

	        for (i = 0; i < 64; i++) //对定时时间进行排序
	        {
	            for (int j = 0; j < 64 - 1 - i; j++)
	            {
	                if (Swap_array[j] > Swap_array[j + 1])
	                {
	                    int tmp = Swap_array[j];
	                    Swap_array[j] = Swap_array[j + 1];
	                    Swap_array[j + 1] = tmp;
	                }
	            }
	        }

	        for (i = 0, j = 0; i < 64; i++)
	        {
	            if (Swap_array[i] > 0)
	            {
	                timing_buffer[j] = Swap_array[i];
					
	                j++;
					
	            }
	        }

	        flage_timer_on = 1;    
		}
		
		if(count_oper.flag == 0){
//printf("count_oper.flag == 0\r\n");
			for (i = 0; i < 56; i++)
			{
				if (timing_buffer[i] == count_oper.localtime)
				{
                    count_oper.localtime = 0;
					timing_buffer[i] = 0;
                    position = i;
				}
			}
			
            for (j = position; j < 56; j++)
            {
                
                timing_buffer[j] = timing_buffer[j + 1];
            }	
		}

        for (i = 0; i < 56; i++)
        {
            printf("end:%d\t", timing_buffer[i]);
        }
        return SUCCESS;
    }

    else
        return ERROR;
#endif
}

static unsigned char dp_download_curtain_setting_handle(const unsigned char value[], unsigned short length)
{
    //示例:当前DP类型为STRING
    unsigned char ret;
    unsigned char string_data;

    string_data = value[0];

    //处理完DP数据后应有反�?
    ret = mcu_dp_enum_update(DPID_CURTAIN_MODE, string_data);

    if (ret == SUCCESS)
    {
        curtain_mode = string_data;
		CurtainModeEraseWrite();
        return SUCCESS;
    }

    else
       
        return ERROR;
}

static unsigned char dp_download_curtain_status_handle(const unsigned char value[], unsigned short length)
{
    //示例:当前DP类型为STRING
    unsigned char ret;
    unsigned char string_data;

    string_data = value[0];
    //处理完DP数据后应有反�?
    ret = mcu_dp_enum_update(DPID_CALIBRATION_STATUS, string_data);
    if (ret == SUCCESS)
    {
        calibration_status = string_data;
        printf("calibration status:%d\r\n ",calibration_status);
        if (calibration_status == 0)//返回上一�?
        {
        	motor_stop();
            run_state = 0;
            calibration_status = 0;
			IsignCalibrating = FALSE;
        }
        return SUCCESS;
    }

    else
        return ERROR;
}

static unsigned char dp_download_curtain_operation_handle(const unsigned char value[], unsigned short length)
{
    //示例:当前DP类型为STRING
    unsigned char ret;
    unsigned char string_data;

    string_data = value[0];

    //处理完DP数据后应有反�?
    ret = mcu_dp_enum_update(DPID_CALIBRATION_OPERATION, string_data);
    if (ret == SUCCESS)
    {
        calibration_operation = string_data;
//		printf("calibration_operation = %d\n",calibration_operation);
        calibration_operation_auto = 0; //设置为手动校�?
        return SUCCESS;
    }
    else
        return ERROR;
}

/******************************************************************************
                                WARNING!!!                     
此代码为SDK内部调用,请按照实际dp数据实现函数内部数据
******************************************************************************/

/******************************************************************************
                                WARNING!!!                     
以下函数用户请勿修改!!
******************************************************************************/

/*****************************************************************************
函数名称 : dp_download_handle
功能描述 : dp下发处理函数
输入参数 : dpid:DP序号
           value:dp数据缓冲区地址
           length:dp数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERRO
使用说明 : 该函数用户不能修�?
*****************************************************************************/
unsigned char dp_download_handle(unsigned char dpid, const unsigned char value[], unsigned short length)
{
    /*********************************
  当前函数处理可下�?可上报数据调�?                   
  具体函数内需要实现下发数据处�?
  完成用需要将处理结果反馈至APP�?否则APP会认为下发失�?
  ***********************************/
    gptm0_4low = 0;
    unsigned char ret;
	LEDFlashing_1s();
	printf("DP issued:%d\r\n",dpid);
    // printf(" key_control:%d run_mode_flag:%d\r\n",KeyControl,variable.run_mode_flag);
	
    switch (dpid)
    {
    case DPID_CONTROL:
        //设备控制处理函数
        ret = dp_download_control_handle(value, length);
        break;
    case DPID_PERCENT_CONTROL:
        //开启百分比控制处理函数
        ret = dp_download_percent_control_handle(value, length);
        break;
    case DPID_MODE:
        //工作模式处理函数
        ret = dp_download_mode_handle(value, length);
        break;
    case DPID_CONTROL_BACK:
        //电机反向处理函数
        ret = dp_download_control_back_handle(value, length);
        break;
    case DPID_BORDER:
        //设置限位处理函数
        ret = dp_download_border_handle(value, length);
        break;
    case DPID_POSITION_BEST:
        //最佳位置处理函�?
        ret = dp_download_position_best_handle(value, length);
        break;
    case DPID_CURTAIN_MODE: /*窗帘模式*/
        ret = dp_download_curtain_setting_handle(value, length);

        break;
    case DPID_CALIBRATION_STATUS: /*窗帘校准状�?*/
        ret = dp_download_curtain_status_handle(value, length);

        break;
    case DPID_CALIBRATION_OPERATION: /*窗帘手动校准*/
        ret = dp_download_curtain_operation_handle(value, length);

        break;
    case DPID_AUTO_ON_OFF:
        //自动开关模�?
        ret = dp_curtain_auto_mode(value, length);
        break;
    case DPID_SILENT_MODE:
        //静音模式
        //ret = dp_curtain_silent_mode(value, length);
        break;
    case DPID_TAIMED_TASK:
        //定时任务
        ret = dp_curtain_timer_task(value, length);
        break;
    case DPID_COUNTDOWN_ONOFF:
        ret = dp_curtain_countdown_on(value, length);
        break;
	case DPID_COUNTDOWN_OP:
        ret = dp_download_curtain_countdown_opreation(value, length);
        break;
    case DPID_COUNTDOWN_TIME:
        ret = dp_download_curtain_countdown_time(value, length);
        break;
	case DPID_FAULT_REMOVAL:
		mcu_dp_fault_update(12,0);
		//update_calibrstatus(0);
		motor_stop();
		calibration_status = 0;
		I4_LOG("Close the prompt window!\r\n");
		break;

    default:
        break;
    }
    return ret;
}

/*****************************************************************************
函数名称 : get_download_cmd_total
功能描述 : 获取所有dp命令总和
输入参数 : �?
返回参数 : 下发命令总和
使用说明 : 该函数用户不能修�?
*****************************************************************************/
unsigned char get_download_cmd_total(void)
{
    return (sizeof(download_cmd) / sizeof(download_cmd[0]));
}

/*========================================新旧功能分割===================================================*/

//////////////////////////////////当前MCU SDK版本较上一版本新增支持协议接口////////////////////
#ifdef TUYA_BCI_UART_COMMON_UNBOUND_REQ
/*****************************************************************************
函数名称 :  bt_unbound_req
功能描述 : 向模块发送解绑请求，模块收到该指令后会解除蓝牙绑定关�?
输入参数 : �?
返回参数 : �?
使用说明 : MCU主动解绑调用
*****************************************************************************/
void bt_unbound_req(void)
{
    bt_uart_write_frame(TUYA_BCI_UART_COMMON_UNBOUND_REQ, 0);
}
#endif

#ifdef TUYA_BCI_UART_COMMON_RF_TEST
/*****************************************************************************
函数名称 :  bt_rf_test_req
功能描述 : 向模块发射频测试请求
输入参数 : �?
返回参数 : �?
使用说明 : 
*****************************************************************************/
void bt_rf_test_req(void)
{
    bt_uart_write_frame(TUYA_BCI_UART_COMMON_RF_TEST, 0);
}
/*****************************************************************************
函数名称 : bt_rf_test_result
功能描述 : 蓝牙rf测试反馈
输入参数 : result:蓝牙rf测试结果;0:失败/1:成功
           rssi:测试成功表示蓝牙信号强度/测试失败值无意义
返回参数 : �?
使用说明 : MCU需要自行完善该函数功能
*****************************************************************************/
void bt_rf_test_result(unsigned char result, signed char rssi)
{
    //#error "请自行完善该功能,完成后请删除该行"
    if (result == 0)
    {
        //测试失败
        //
		IS_FullInspection = FALSE;
		USART_SendData(HT_USART0, 0);
		//notes0err\n");
    }
    else
    {
        //测试成功
        //rssi为信号强度，一般大�?70dbm 蓝牙信号都在正常范围�?
        //USART_SendData(HT_USART0, rssi);
		IS_FullInspection = TRUE;
		USART_SendData(HT_USART0, 1);
	}
}
#endif

#ifdef TUYA_BCI_UART_COMMON_SEND_STORAGE_TYPE
/*****************************************************************************
函数名称 : bt_send_recordable_dp_data
功能描述 : 记录型数据上�?
输入参数 : type -1�?蓝牙模块自带时间上报 -2�?只上报原始数据，无时�?3�?MCU自带时间上报
		dpid:  datapoint 序号
		dptype:对应开放平台上某datapoint 具体的数据类�?
		value:具体数据
		len:数据的长�?
返回参数 : �?
使用说明 : MCU需要自行完善该函数功能
	建议使用缓存队列，将要发给模块的所有数据放到MCU缓存队列中，上报成功一条后再上报下一条，记录型数据要确保每条数据都上报成�?
*****************************************************************************/
void bt_send_recordable_dp_data(unsigned char snedType, unsigned char dpid, unsigned char dpType, unsigned char value[], unsigned short len)
{
    //#error "请自行完善该功能,完成后请删除该行"
    if (snedType == 0x01) //格式1，蓝牙模块自带时间上�?
    {
    }
    else if (snedType == 0x02) //格式2，只上报原始数据，无时间  （备�?telink对接平台不支持该格式�?
    {
    }
    else if (snedType == 0x03) //格式3，MCU自带时间上报
    {
    }
}
/*****************************************************************************
函数名称 : bt_send_recordable_dp_data_result
功能描述 : 记录型数据上�?
输入参数 : result �?存储成功�?存储失败
返回参数 : �?
使用说明 : MCU需要自行完善该函数功能
*****************************************************************************/
void bt_send_recordable_dp_data_result(unsigned char result)
{
    //#error "请自行完善该功能,完成后请删除该行"
}
#ifdef TUYA_BCI_UART_COMMON_SEND_TIME_SYNC_TYPE
/*****************************************************************************
函数名称 : bt_send_time_sync_type
功能描述 : 向模块发送时间同步请�?
输入参数 :sync_time_type
0x00- 获取7字节时间时间类型+2
字节时区信息
0x01- 获取13字节ms级unix时间
+2字节时区信息
0x02- 获取7字节时间时间类型+2
字节时区信息

返回参数 : �?
使用说明 :
*****************************************************************************/
void bt_send_time_sync_req(unsigned char sync_time_type)
{
    unsigned short length = 0;

    length = set_bt_uart_byte(length, sync_time_type);

    bt_uart_write_frame(TUYA_BCI_UART_COMMON_SEND_TIME_SYNC_TYPE, length);
}
/*****************************************************************************
函数名称 : bt_time_sync_result
功能描述 : 向模块发送时间同步的结果
输入参数 :result同步结果 0成功，其他失�?
		sync_time_type时间格式
		bt_time自定义时间（如果是时间格�?或�?该值有效）
		time_zone_100 时区
		time_stamp_ms 时间戳（如果是时间格�?，该值有效）
返回参数 : �?
使用说明 : MCU需要自行完善该函数功能
*****************************************************************************/
void bt_time_sync_result(unsigned char result, unsigned char sync_time_type, bt_time_struct_data_t bt_time, unsigned short time_zone_100, long long time_stamp_ms)
{
    //#error "请自行完善该功能,完成后请删除该行"
    if (result == 0x00)
    {
        //同步时间成功
        if (sync_time_type == 0x00 || sync_time_type == 0x02)
        {
            //将bt_time中的自定义时间格式的数据填充到mcu时钟系统�?
            //time_zone_100时区
        }
        else if (sync_time_type == 0x01)
        {
            //将time_stamp_ms中的时间戳填充到mcu时钟系统�?
            //time_zone_100时区
        }
    }
    else
    {
        //同步时间失败
    }
}
#endif
#endif

#ifdef TUYA_BCI_UART_COMMON_MODIFY_ADV_INTERVAL
/*****************************************************************************
函数名称 : bt_modify_adv_interval_req
功能描述 : 向模块发送修改低功耗下模块广播间隔的请�?
输入参数 :value * 100ms等于要修改的广播间隔，value�?-20�?
返回参数 : �?
使用说明 : 
*****************************************************************************/
void bt_modify_adv_interval_req(unsigned char value)
{
    unsigned short length = 0;

    length = set_bt_uart_byte(length, value);

    bt_uart_write_frame(TUYA_BCI_UART_COMMON_MODIFY_ADV_INTERVAL, length);
}

/*****************************************************************************
函数名称 : bt_modify_adv_interval_result
功能描述 :向模块发送修改低功耗下模块广播间隔的结�?
输入参数 :result同步结果 0成功，其他失�?

返回参数 : �?
使用说明 : MCU需要自行完善该函数功能
*****************************************************************************/
void bt_modify_adv_interval_result(unsigned char result)
{
    //#error "请自行完善该功能,完成后请删除该行"
    if (result == 0x00)
    {
        //成功
    }
    else
    {
        //失败
    }
}
#endif

#ifdef TUYA_BCI_UART_COMMON_TURNOFF_SYSTEM_TIME
/*****************************************************************************
函数名称 : bt_close_timer_req
功能描述 : 向模块发送关闭系统时钟的请求（目前仅适用telink平台�?
输入参数 :value 0关闭�?打开
返回参数 : �?
使用说明 : 
*****************************************************************************/
void bt_close_timer_req(unsigned char value)
{
    unsigned short length = 0;

    length = set_bt_uart_byte(length, value);

    bt_uart_write_frame(TUYA_BCI_UART_COMMON_TURNOFF_SYSTEM_TIME, length);
}

/*****************************************************************************
函数名称 : bt_close_timer_result
功能描述 :向模块发送关闭系统时钟的结果
输入参数 :result同步结果 0成功，其他失�?

返回参数 : �?
使用说明 : MCU需要自行完善该函数功能
*****************************************************************************/
void bt_close_timer_result(unsigned char result)
{
    //#error "请自行完善该功能,完成后请删除该行"
    if (result == 0x00)
    {
        //成功
    }
    else
    {
        //失败
    }
}
#endif

#ifdef TUYA_BCI_UART_COMMON_ENANBLE_LOWER_POWER
/*****************************************************************************
函数名称 : bt_enable_lowpoer_req
功能描述 : 向模块发送使能低功耗的请求（目前仅适用telink平台�?
输入参数 :value 0关闭�?打开
返回参数 : �?
使用说明 :
*****************************************************************************/
void bt_enable_lowpoer_req(unsigned char value)
{
    unsigned short length = 0;

    length = set_bt_uart_byte(length, value);

    bt_uart_write_frame(TUYA_BCI_UART_COMMON_ENANBLE_LOWER_POWER, length);
}

/*****************************************************************************
函数名称 : bt_enable_lowpoer_result
功能描述 :向模块发送使能低功耗的结果
输入参数 :result同步结果 0成功，其他失�?

返回参数 : �?
使用说明 : MCU需要自行完善该函数功能
*****************************************************************************/
void bt_enable_lowpoer_result(unsigned char result)
{
    //#error "请自行完善该功能,完成后请删除该行"
    if (result == 0x00)
    {
        //成功
    }
    else
    {
        //失败
    }
}
#endif

#ifdef TUYA_BCI_UART_COMMON_SEND_ONE_TIME_PASSWORD_TOKEN
/*****************************************************************************
函数名称 : bt_send_one_time_password_token
功能描述 : 向模块发送获取一次性动态密码匹配结果的请求
输入参数 :value cmcu端输入的动态密码，len 8
返回参数 : �?
使用说明 :用于锁通用串口对接动态密码功�?
*****************************************************************************/
unsigned char bt_send_one_time_password_token(unsigned char value[], unsigned char len)
{
    unsigned short length = 0;
    if (len != 8)
        return 0;

    length = set_bt_uart_buffer(length, value, 8);

    bt_uart_write_frame(TUYA_BCI_UART_COMMON_SEND_ONE_TIME_PASSWORD_TOKEN, length);
}

/*****************************************************************************
函数名称 : bt_send_one_time_password_token_result
功能描述 :向模块获取一次性动态密码匹配的结果
输入参数 :result同步结果 0x00密码核对通过�?x01密码核对失败

返回参数 : �?
使用说明 : MCU需要自行完善该函数功能
*****************************************************************************/
void bt_send_one_time_password_token_result(unsigned char result)
{
    //#error "请自行完善该功能,完成后请删除该行"
    if (result == 0x00)
    {
        //密码核对通过
    }
    else
    {
        //密码核对失败
    }
}
#endif

#ifdef TUYA_BCI_UART_COMMON_ACTIVE_DISCONNECT
/*****************************************************************************
函数名称 : bt_enable_lowpoer_req
功能描述 : 向模块发送断开蓝牙连接的请�?
输入参数 :value 0关闭�?打开
返回参数 : �?
使用说明 :
*****************************************************************************/
void bt_disconnect_req(void)
{
    bt_uart_write_frame(TUYA_BCI_UART_COMMON_ACTIVE_DISCONNECT, 0);
}

/*****************************************************************************
函数名称 : bt_enable_lowpoer_result
功能描述 :向模块发送断开蓝牙连接的结�?
输入参数 :result结果 0成功，其他失�?

返回参数 : �?
使用说明 : MCU需要自行完善该函数功能
*****************************************************************************/
void bt_disconnect_result(unsigned char result)
{
    //#error "请自行完善该功能,完成后请删除该行"
    if (result == 0x00)
    {
        //成功
    }
    else
    {
        //失败
    }
}
#endif

#ifdef TUYA_BCI_UART_COMMON_MCU_SEND_VERSION
/*****************************************************************************
函数名称 : bt_send_mcu_ver
功能描述 :向模块主动发送MCU版本号，主要是为了模块能更及时获取到MCU的版本信�?
输入参数 :

返回参数 : �?
使用说明 : MCU可以在串口初始化后调用一�?
*****************************************************************************/
void bt_send_mcu_ver(void)
{

    unsigned short length = 0;

    length = set_bt_uart_buffer(length, (unsigned char *)versions, 3);

    length = set_bt_uart_buffer(length, (unsigned char *)versions, 3);

    bt_uart_write_frame(TUYA_BCI_UART_COMMON_MCU_SEND_VERSION, length);
}

void bt_sendE8_mcu_ver(void)
{

    unsigned short length = 0;
    u8 version[6] = {0x01, 0x01, 0x00, 0x01, 0x01, 0x00};
    length = set_bt_uart_buffer(length, (unsigned char *)version, 3);
    length = set_bt_uart_buffer(length, (unsigned char *)version, 3);
    bt_uart_write_frame(TUYA_BCI_UART_COMMON_QUERY_MCU_VERSION, length);
}

#endif




#ifdef TUYA_BCI_UART_COMMON_FACTOR_RESET_NOTIFY
/*****************************************************************************
函数名称 : bt_factor_reset_notify
功能描述 :模块恢复出厂设置后向mcu发送的通知
输入参数 :

返回参数 : �?
使用说明 : MCU可以在此处完成MCU恢复出厂设置的操�?
*****************************************************************************/
void bt_factor_reset_notify(void)
{
    //#error "请自行完善该功能,完成后请删除该行"
    u32 i;
    for (i = APP_NEW_FW_START_ADR; i < APP_UPDATA_INFO_REASED; i += 1024)
    {
        FLASH_ErasePage(i);
    }
	
    I4_LOG("factor reboot\r\n");
	FirstStartSign = FALSE;
	FLASH_ProgramWordData(APP_NEW_FW_START_ADR, 1);
	NVIC_SystemReset();
	
    // void resume_factory_up(void);
}
#endif

void tybn1_out_sleep_mode(void)
{
	//io_11
	GPIO_WriteOutBits(HT_GPIOA, GPIO_PIN_14,RESET);
}

