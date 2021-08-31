#include "tst_function.h"
#include <time.h>
#include "protocol.h"
#include "head.h"


volatile u8 run_state = 0; //����״̬  ֻ�е�������ʱ�� �ֶ�У׼�������ƲŻᱻ�ſ�
volatile u8 rtc_motor_state = 0;
volatile u8 key_flag = 0;     //key_flag=1--���, key_flag=2--�Ҽ�
volatile u8 motor_status = 0; //motor_status=1--��������, motor_status=2--����

volatile u8 crc_flag=0;
bool CalibrMotor = FALSE;

// u8 //ty_control0;//1:Lock button 0:button release
u8 key_up_flag = 0;
u16 motor_current = 0;
u8 Moveflag = 0;//�����־���ڵ�һ��У׼������ͣʱ��¼��ǰ����ʱ��
bool IS_CailbrationBack = FALSE;
void delay(int i)
{
    for (; i > 0; i--)
    {
        ;
        ;
        ;
    }
}
void I4MutetimeFlashUpdate(void);
void Calibra_erase_write(void);
void update_battery_value(u8 current_data)
{
    u8 crc8 = 0;

    dp_data_buf[0] = 0x55;
    dp_data_buf[1] = 0xAA;
    dp_data_buf[2] = VERSION;
    dp_data_buf[3] = 0x07;
    dp_data_buf[4] = 0x00; //���ݳ���
    dp_data_buf[5] = 0x05; //���ݳ���
    dp_data_buf[6] = 0x6E;
    dp_data_buf[7] = 0x04;
    dp_data_buf[8] = 0x00;
    dp_data_buf[9] = 0x01;
    dp_data_buf[10] = current_data; //����

    crc8 = get_check_sum(dp_data_buf, 11);
    dp_data_buf[11] = crc8;
    uart0_send(dp_data_buf, 12);
//    usart0_send(dp_data_buf, 12);
}

void update_protect_current(u32 current_data)
{
    u8 crc8 = 0;

    dp_data_buf[0] = 0x55;
    dp_data_buf[1] = 0xAA;
    dp_data_buf[2] = VERSION;
    dp_data_buf[3] = 0x07;
    dp_data_buf[4] = 0x00; //���ݳ���
    dp_data_buf[5] = 0x08; //���ݳ���
    dp_data_buf[6] = 0X78;
    dp_data_buf[7] = 0x02;
    dp_data_buf[8] = 0x00;
    dp_data_buf[9] = 0x04;
    dp_data_buf[10] = (current_data >> 24);  //����
    dp_data_buf[11] = (current_data >> 16);  //����
    dp_data_buf[12] = (current_data >> 8);   //����
    dp_data_buf[13] = (current_data & 0xFF); //����
    crc8 = get_check_sum(dp_data_buf, 14);
    dp_data_buf[14] = crc8;
    uart0_send(dp_data_buf, 15);
    //usart0_send(dp_data_buf,15);
}

void update_motor_current(u32 current_data)
{
    u8 crc8 = 0;

    //55 AA 00 07 00 06 6E 00 00 02 00 64
    //dp_id=110 -- ��ת��������
    dp_data_buf[0] = 0x55;
    dp_data_buf[1] = 0xAA;
    dp_data_buf[2] = VERSION;
    dp_data_buf[3] = 0x07;
    dp_data_buf[4] = 0x00; //���ݳ���
    dp_data_buf[5] = 0x08; //���ݳ���
    dp_data_buf[6] = 0X6E;
    dp_data_buf[7] = 0x02;
    dp_data_buf[8] = 0x00;
    dp_data_buf[9] = 0x04;
    dp_data_buf[10] = (current_data >> 24);  //����
    dp_data_buf[11] = (current_data >> 16);  //����
    dp_data_buf[12] = (current_data >> 8);   //����
    dp_data_buf[13] = (current_data & 0xFF); //����
    crc8 = get_check_sum(dp_data_buf, 14);
    dp_data_buf[14] = crc8;
    uart0_send(dp_data_buf, 15);
    //usart0_send(dp_data_buf,15);
}

//gptm0���Ե������ת
void gptm0_test(void)
{
    u8 state = 0;

    //open_curtain();
    if (gptm0_second < 3)
    {
        if (state == 0)
        {
            state = 1;
            SET_LED(RESET);
            open_curtain();
        }
    }
    if (gptm0_second > 2)
    {
        if (state == 1)
        {
            state = 2;
            SET_LED(SET);
            close_curtain();
        }
    }
    if (gptm0_second > 5)
    {
        state = 0;
        gptm0_second = 0;
    }
}
#if 0
//ADC����
void motor_current_test(void)
{	
    if (rtc_interrupt > 10000) //Ϳѻ����֮���ȡʱ��ֵ ֮������if
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
			I4_LOG(" update coundown ------------------------------------!\r\n");
			if(FirstStartSign == TRUE)
			{
				printf(" update!\n");
				dp_curtain_timer_task(TimerDatagroup, TimerDataLength);
			}
			if(cFirstStartSign == TRUE)
			{
				printf(" update!\n");
				dp_download_curtain_countdown_time(CountdownGroup, 4);
			}
			
			RtcCntSign = TRUE;
		}
		
    }
	
}
#endif
/**
 * @description: ��ʱ����򿪻�رմ���
 */
static void Common_timing_data_processing(u32 i)
{
    float runtime = 0;
	// //variable.auto_run_mode = 0;
	// printf("//variable.auto_run_mode == %d\r\n", //variable.auto_run_mode);
    if (timer_task[i].timer_ton_offs == 1) //�رմ���
    {
		timer_curtain_control(0);
    }
    else
    {
        timer_curtain_control(timer_task[i].timer_percent);
    }
}
/**
 * @description: ����ʱ����򿪻�رմ���
 */
static void Countdown_switch_data_processing_function(void)
{
	// //variable.auto_run_mode = 0;
	// printf("//variable.auto_run_mode == %d\r\n", //variable.auto_run_mode);
    if (count_oper.gswitch == 1)
    {
        timer_curtain_control(100);
    }
    else
    {
        timer_curtain_control(0);
    }
}
/**
 * @description: ���ڵ���ʱ�����д���ٷֱȱ߽�0or100������
 * 
 */
void perc_Boundary_process(void)
{
	printf("count_oper.gswitch =%d\n",count_oper.gswitch );
    if (variable.global_percent == 0)
    {
        if (count_oper.gswitch  == 0)
        {
            SET_LED(RESET);
            ms_delay_systick(1000);
			 SET_LED(SET);
			 KeyControl = TRUE;
        }
        else
        {
            timer_curtain_control(100);
        }
    }
    if (variable.global_percent == 100)
    {
        if (count_oper.gswitch  == 1)
        {
            SET_LED(RESET);
            ms_delay_systick(1000);
			SET_LED(SET);
			KeyControl = TRUE;
        }
        else
        {
            timer_curtain_control(0);
        }
    }
}
/**
 * @description: ���ڶ�ʱ�����д���ٷֱȱ߽�0or100������
 */
void alarm_perc_Boundary_process(u32 i)
{
    /*�߽��Ϊ0�Ĵ���*/
	
    if (timer_task[i].timer_ton_offs == 1)
    {
        if (variable.global_percent == 0)
        {
            SET_LED(RESET);
            ms_delay_systick(1000);
			SET_LED(SET);
			KeyControl = TRUE;
        }
        else
        {   
           timer_curtain_control(0);

        }
    }
    /*�߽��Ϊ100�Ĵ���*/
    if (timer_task[i].timer_ton_offs == 0)
    {
        if (variable.global_percent == 100 && timer_task[i].timer_percent == 100)
        {
            SET_LED(RESET);
            ms_delay_systick(1000);
			SET_LED(SET);
			KeyControl = TRUE;
        }
        else
        {
           timer_curtain_control(timer_task[i].timer_percent);

        }
    }
}
void Timing_echofun(u32 value)
{	
	switch(value)
	{
		case 0:
			TimerDatagroup[4] = 0;
			break;
		case 1:
			TimerDatagroup[16] = 0;
			break;
		case 2:
			TimerDatagroup[28] = 0;
			break;
		case 3:
			TimerDatagroup[40] = 0;
			break;
		case 4:
			TimerDatagroup[52] = 0;
			break;
		case 5:
			TimerDatagroup[64] = 0;
			break;
		case 6:
			TimerDatagroup[76] = 0;
			break;
		case 7:
			TimerDatagroup[88] = 0;
			break;
		default:
			break;
	}
}
void Timing_task_program(void)
{
	u32 addrcnt =4,flashcnt;
	//��ʱ����������־
	if (flage_timer_on == 1)
	{
		RTC_SetCompare(timing_buffer[0]); //2.21����˯��֮ǰ����һ������˯��ʱ�� ���ԡ�
		flage_timer_on = 0;
    }
    /*��ʱʱ�䴥���ĺ�������*/
    if (flage_timer_on == 2)
    {
        u32 i, j, count_oper_flag = 0;
        u8 flag = 0;
        u32 current_cnt = RTC_GetCounter();
        for(i=1;i<56;i++){
            if (timing_buffer[0] == timing_buffer[i]){                
                timing_buffer[i] = timing_buffer[i+1];
            }           
        }
		//variable.auto_run_mode = 0;
		//countdown 
        if (current_cnt == count_oper.localtime)
        {
            timing_buffer[0] = 0;
            count_oper.localtime = 0;
            count_oper_flag = 1;
            KeyControl = FALSE;//����ʱ�����ص���������
            printf("Countdown task start!\r\n");
            if (variable.global_percent != 100 && variable.global_percent != 0)
            {
                Countdown_switch_data_processing_function();
            }
            else
            {
                perc_Boundary_process();
            }
            
        }
        if (count_oper_flag == 0)
        {
            for (i = 0; i < 8; i++)
            {
                /*���λ��ظ��Ĵ���*/
                if (Single_or_repeated_flag[i] == 1)
                {
                    timing_buffer[0] = 0;
                    timing_buffer[63] = current_cnt + 604800;
                }
                else
                {
                    timing_buffer[0] = 0;
                }

                /*��ʱʱ�䴥�� �Ƚ�����ǰ����ֵ ��ȡ��Ӧ�Ŀ���ֵ�Ϳ��ذٷֱ�ֵ*/
                if (current_cnt == timer_buf_single[i])
                {
					KeyControl = FALSE;//��ʱ�����ص���������
					Timing_echofun(i);
//					for(i=0;i<64;i++)
//					{
//						//notestest buffer %d = %d length\n",i,TimerDatagroup[i]);
//					}
					FirstStartSign = TRUE;
					//����ִ�к�ص�
					printf("current_cnt = %d timer_buf_single[i] = %d\r\n",current_cnt,timer_buf_single[i]);
                	mcu_dp_raw_update(DPID_TAIMED_TASK, TimerDatagroup, TimerDataLength);
					
					FLASH_ErasePage(FLASH_52K_GLOBAL_DATA);
					FLASH_ProgramWordData(FLASH_52K_GLOBAL_DATA, TimerDataLength);
					for(flashcnt=0;flashcnt<TimerDataLength;flashcnt++)
					{
						FLASH_ProgramWordData(FLASH_52K_GLOBAL_DATA+addrcnt, TimerDatagroup[flashcnt]);
						addrcnt += 4;
					}

                    if (variable.global_percent != 100 && variable.global_percent != 0)
                    {
                        Common_timing_data_processing(i);
                    }
                    else
                    {
                        alarm_perc_Boundary_process(i);
                    }
                    
                    continue;
                }
                for (j = 0; j < 7; j++)
                {
                    if (current_cnt == timer_buf[i][j])
                    {
                        if (variable.global_percent != 100 && variable.global_percent != 0)
                        {
                            Common_timing_data_processing(i);
                        }
                        else
                        {
                            alarm_perc_Boundary_process(i);
                        }
                    }
                }
            }
        }

        if (timing_buffer[1] == 0)
        {
            RTC_SetCompare(0xFFFFFFFF);
            flage_timer_on = 0;
        }
        else
        {
            for (i = 0; i < 64; i++) //�Զ�ʱʱ���������
            {
                for (int j = 0; j < 64 - 1 - i; j++)
                {
                    if (timing_buffer[j] > timing_buffer[j + 1])
                    {
                        int tmp = timing_buffer[j];
                        timing_buffer[j] = timing_buffer[j + 1];
                        timing_buffer[j + 1] = tmp;
                    }
                }
            }
            for (i = 0, j = 0; i < 64; i++)
            {
                if (timing_buffer[i] > 0)
                {
                    new_timing_buffer[j] = timing_buffer[i];
                    j++;
                }
            }
            memset(timing_buffer, 0, sizeof(int) * 64);
            for (i = 0; i < 56; i++)
            {
                timing_buffer[i] = new_timing_buffer[i];
            }
            memset(new_timing_buffer, 0, sizeof(int) * 56);

            printf("IRQ_event 0 timing_buffer:%d!\n", timing_buffer[0]);
//            //notesIRQ_event 1 timing_buffer:%d!\n", timing_buffer[1]);
//            //notesIRQ_event 2 timing_buffer:%d!\n", timing_buffer[2]);
//            //notesIRQ_event 3 timing_buffer:%d!\n", timing_buffer[3]);
//            //notesIRQ_event 4 timing_buffer:%d!\n", timing_buffer[4]);

            flage_timer_on = 1;
        }
    }

    if (xRtcCounterTmp == 1)
    {
        //�������͹���rtc_interrupt����ʱ�����⡣��ʱ�����ǵ͹���ģʽ���Ƿǵ͹���ģʽ�����ö�ʱ���������ʱ�����ʱ�䶼��׼ȷ��
        xRtcCounterTmp = RTC_GetCounter();
        // while (RTC_GetCounter() == xRtcCounterTmp);
        // //notesxRtcCounterTmp RTC count time!%d\n",xRtcCounterTmp);
        xRtcCounterTmp -= wRtcCounterTmp;
        // //notesCurrent xRtcCounterTmp RTC count time!%d\n",xRtcCounterTmp);
        rtc_interrupt += xRtcCounterTmp;
        // //notesCurrent count time!%d\n",xRtcCounterTmp);
        get_real_time(); //ÿ�λ��ѵ͹��ĺ� ��ȡһ��ʵʱʱ��
    }
    
}
//u32 crc32=0;
//0x09=�����
void dismiss_bind(void)
{
    u8 crc8 = 0;

    dp_data_buf[0] = 0x55;
    dp_data_buf[1] = 0xAA;
    dp_data_buf[2] = VERSION;
    dp_data_buf[3] = 0x09;
    dp_data_buf[4] = 0x00; //���ݳ���
    dp_data_buf[5] = 0x00; //���ݳ���

    crc8 = get_check_sum(dp_data_buf, 6);
    dp_data_buf[6] = crc8;

    uart0_send(dp_data_buf, 7);
    // usart0_send(dp_data_buf, 7);
#ifdef DEBUG_USART0_OUT
    usart0_send(dp_data_buf, 7);
#endif
}

//0xA1=恢�?�出厂�?�置通知
void resume_factory_up(void)
{
    u8 crc8 = 0;

    dp_data_buf[0] = 0x55;
    dp_data_buf[1] = 0xAA;
    dp_data_buf[2] = VERSION;
    dp_data_buf[3] = 0x04;
    dp_data_buf[4] = 0x00; //数据长度
    dp_data_buf[5] = 0x00; //数据长度

    crc8 = get_check_sum(dp_data_buf, 6);
    dp_data_buf[6] = crc8;

    uart0_send(dp_data_buf, 7);
    //usart0_send(dp_data_buf, 7);
#ifdef DEBUG_USART0_OUT
    usart0_send(dp_data_buf, 7);
#endif
}



//void rfs_test(void)
//{
//	u8 crc8 = 0;
//	char* tmp_buf = NULL;
//	
//	dp_data_buf[0] = 0x55;
//	dp_data_buf[1] = 0xAA;
//	dp_data_buf[2] = VERSION;
//	dp_data_buf[3] = 0x0E;
//	dp_data_buf[4] = 0x00;	//数据长度
//	dp_data_buf[5] = 0x00;	//数据长度
//	
//	crc8 = get_check_sum(dp_data_buf,6);
//	dp_data_buf[6] = crc8;
//	
//	uart0_send(dp_data_buf,7);
//}


bool key_interrupt = FALSE;
u32 keytime1 = 0,keytime2 = 0;
bool keyValue = FALSE;//����״̬
bool keyenter = TRUE;//��ֹһֱ��if

bool leftkey_up = FALSE,rightkey_up = FALSE;
void motor_turn(void)
{
	 if (!SET_KEY() && keyenter) 
	 {
	 	//printf("kaideng= %d\n",MiddleButtonTiming);
	 	SET_LED(RESET);
	 	KeyControl = TRUE;
		MiddleButtonStart = TRUE;
		if(MiddleButtonTiming > 5000)
		{
			lednetworkflag = 0;
			keyenter = FALSE;
			
		}	
		keyValue = TRUE;
		
	 }

	if(SET_KEY() && keyValue)
	{

		if(MiddleButtonTiming < 5000)
		{
			I4_LOG("stop\r\n");
			
			mcu_dp_enum_update(DPID_CONTROL, 1);
	        Pause_handler();
			key_up_flag =0;
			MotorRunStatus = FALSE;
			signFullonfulloff = FALSE;
			
			mcu_dp_value_update(DPID_PERCENT_CONTROL, variable.global_percent);
			mcu_dp_value_update(DPID_PERCENT_UP_CONTROL, variable.global_percent);
	        motor_stop();
		}
		else if(MiddleButtonTiming > 5000)
		{
			
			dismiss_bind();
			SET_LED(SET);
			ms_delay_systick(2000);
			
		}
		// else{
			
			
		// 	//printf("chong zhi\r\n");
		// 	dismiss_bind();
		// 	bt_factor_reset_notify();			
		// }
		SET_LED(SET);
		keyValue = FALSE;
		keyenter = TRUE;
		MiddleButtonStart = FALSE;
		MiddleButtonTiming = 0;
		MotorRunStatus = FALSE;
		gptm0_4low = 0;
		
	}
	 
	

    if (variable.run_mode_flag && !signFullonfulloff && KeyControl)
    {

		if(leftkey_up || rightkey_up){
			if(key_interrupt){
				keytime2 = RTC_GetCounter();
				keytime2 -= keytime1;
				if(keytime2<= 1)
				{
					printf("duanshijian changan 2s %d\n",keytime2);
					leftkey_up =FALSE;
					rightkey_up = FALSE;
				}
				key_interrupt = FALSE;
			}
			keytime1 = RTC_GetCounter();
			key_interrupt = TRUE;

		}
        if (leftkey_up)// Press the left key && App not controlled && The key flag is 0 or 2
        {
			if(lockturnflag)
			{
				lockturnflag = FALSE;
				return;
			}
			leftkey_up = FALSE;
            ms_delay_systick(10);
			if (LEFT_KEY()  && (key_up_flag ==0 || key_up_flag == 2))
            {
				percor.key = 0;
            	LEDFlashing_1s();
				if(key_up_flag == 2)
				{
				
					mcu_dp_enum_update(DPID_CONTROL, 1);
					Pause_handler();

					ptime.cnt_flag = 0;
					cnt_switch = 0;
					key_up_flag = 0;
					//signFullonfulloff = FALSE;
					MotorRunStatus = FALSE;
					return;
				}
				   
				key_up_flag = 1;		//key flag(left/right)
				gptm0_4low = 0;			//low power cnt time
				//key_flag = 0;
				motor_pwr(PWR_ON);
				motor_stop();
				delay(100);
				
				if(crc_flag==1)
				{
					if(curtain_mode == 0)
					{

						timer_curtain_control(0);

						
					}else{

						timer_curtain_control(100);
					}
				}else{
					LEDFlashing_1s(); 
					key_up_flag = 0;
				}
			}
     
            
        }
		
//		if (!RIGHT_KEY()){
//			rightkey_up = TRUE;
//			percor.key = 1;
//		}
        if (rightkey_up)			// Press the right key && App not controlled && The key flag is 0 or 1
        {
			if(lockturnflag)
			{
				lockturnflag = FALSE;
				return;
			}
			rightkey_up = FALSE;
            ms_delay_systick(10);
			if (RIGHT_KEY()  && (key_up_flag ==0||key_up_flag == 1))
            {
				percor.key = 0;
            	LEDFlashing_1s();
				if(key_up_flag == 1)
				{
					mcu_dp_enum_update(DPID_CONTROL, 1);
					Pause_handler();
//					motor_stop();
					ptime.cnt_flag = 0;
					cnt_switch = 0;
					key_up_flag = 0;
					//signFullonfulloff = FALSE;
					MotorRunStatus = FALSE;
					return;
				}	
				key_up_flag = 2;		//key flag(left/right)
				gptm0_4low = 0;			//low power cnt time
				//key_flag = 0;
				motor_pwr(PWR_ON);
				motor_stop();
				delay(100);
			   
				if(crc_flag==1)
				{
					if(curtain_mode == 0 )
					{
					
						timer_curtain_control(100);

					}
					else
					{		
						timer_curtain_control(0);

					}
					
				}
				else{
					LEDFlashing_1s();
					key_up_flag = 0;
				}
			}
        }
    }
}

void stop_fun(void)
{

    motor_stop(); //ֹͣ
	//notes716 variable.full_running_time = %d \n",variable.full_running_time);
    if (run_state == 1) //����
    {
        //0=����, 1=�Ҵ���
        if (curtain_mode == 0) //0=����
        {
        	if(variable.mute_mode == 1)
			{
				silenttime.Muteopentime -= gptm0_4clbr;
			}else{
				variable.full_running_time -= gptm0_4clbr;
			}
         
            gptm0_4clbr = 0; // ��ʱ����: ��ʱ�����
        }
        if (curtain_mode == 1) //1=�Ҵ���
        {
        	if(variable.mute_mode == 1)
			{
				silenttime.Muteopentime += gptm0_4clbr;
			}else{
				variable.full_running_time += gptm0_4clbr;
			}
            
            gptm0_4clbr = 0; // ��ʱ����: ��ʱ�����
        }
		printf("sum full_running_time = %d\r\n",variable.full_running_time);
    }
	
    if (run_state == 2) //����
    {
        //0=����, 1=�Ҵ���
        if (curtain_mode == 0) //0=����
        {
        	if(variable.mute_mode == 1)
			{
				silenttime.Muteopentime += gptm0_4clbr;
			}else{
				variable.full_running_time += gptm0_4clbr;
			}
           
            gptm0_4clbr = 0; // ��ʱ����: ��ʱ�����
                             /*variable.full_running_time -= gptm0_second*1000;
								variable.full_running_time -= gptm0_cnt;
								gptm0_cnt = 0;
								gptm0_second = 0;*/
        }

        if (curtain_mode == 1) //1=�Ҵ���
        {
        	if(variable.mute_mode == 1)
			{
				silenttime.Muteopentime -= gptm0_4clbr;
			}else{
				variable.full_running_time -= gptm0_4clbr;
			}
            
            gptm0_4clbr = 0; // ��ʱ����: ��ʱ�����

        }
		printf("sum full_running_time = %d\r\n",variable.full_running_time);
	}
	//notes755 variable.full_running_time = %d \n",variable.full_running_time);
    run_state = 3;
    //TODO: upload DP - calibration_operation: 1=ֹͣ
}

void I4ReturnManualHandler(void)
{
	//I4_LOG("run_state = %d , curtain_mode = %d\n",run_state,curtain_mode);
    motor_stop(); //ֹͣ
	//notes716 variable.full_running_time = %d \n",variable.full_running_time);
    if (run_state == 1) //����
    {
        //0=����, 1=�Ҵ���
        if (curtain_mode == 0) //0=����
        {
        	if(variable.mute_mode == 1)
			{
				silenttime.Muteclosetime += gptm0_4clbr;
			}else{
				variable.returntime += gptm0_4clbr;
				//I4_LOG("lmove lcuratin var = %d gtime = %d\n",variable.returntime,gptm0_4clbr);
			}
         
            gptm0_4clbr = 0; // ��ʱ����: ��ʱ�����
        }
        if (curtain_mode == 1) //1=�Ҵ���
        {
        	if(variable.mute_mode == 1)
			{
				silenttime.Muteclosetime -= gptm0_4clbr;
			}else{
				variable.returntime -= gptm0_4clbr;
				//I4_LOG("lmove rcuratin var = %d gtime = %d\n",variable.returntime,gptm0_4clbr);
			}
            
            gptm0_4clbr = 0; // ��ʱ����: ��ʱ�����
        }
    }
	
    if (run_state == 2) //����
    {
    	//I4_LOG("RIGHT MOVE !\n");
        //0=����, 1=�Ҵ���
        if (curtain_mode == 0) //0=����
        {
        	if(variable.mute_mode == 1)
			{
				silenttime.Muteclosetime -= gptm0_4clbr;
			}else{
				variable.returntime -= gptm0_4clbr;
				//I4_LOG("rmove lcuratin var = %d gtime = %d\n",variable.returntime,gptm0_4clbr);
			}
            gptm0_4clbr = 0; // ��ʱ����: ��ʱ�����
        }
        if (curtain_mode == 1) //1=�Ҵ���
        {
        	//I4_LOG("curtain_mode == 1\n");
        	if(variable.mute_mode == 1)
			{
				silenttime.Muteclosetime += gptm0_4clbr;
			}else{
				variable.returntime += gptm0_4clbr;
				//I4_LOG("rmove rcuratin var = %d gtime = %d\n",variable.returntime,gptm0_4clbr);
			}
            
            gptm0_4clbr = 0; // ��ʱ����: ��ʱ�����

        }
    }
   
    run_state = 3;
}
	 


static void CalibraCallBack(void)
{
	variable.global_percent = 100;
	Percentage_control();

	update_current_runtime(0);
	mcu_dp_value_update(DPID_PERCENT_UP_CONTROL_STATUS, 1);
	mcu_dp_value_update(DPID_PERCENT_CONTROL, variable.global_percent);
	mcu_dp_value_update(DPID_PERCENT_UP_CONTROL, variable.global_percent);
	mcu_dp_value_update(DPID_PERCENT_UP_CONTROL_STATUS, 2);
}


void I4CalibraERRHander(void)
{
	
	
	if(gptm0_4clbr>120000 && MotorRunStatus)//motor idling
	{
		mcu_dp_fault_update(12,2);
		motor_stop();	
		//idling = FALSE;
		gptm0_4clbr = 0;
	}

	if(gptm0_4clbr !=0 && gptm0_4clbr<3000 &&(motor_current > protect_current) && run_state == 0)
	{
		mcu_dp_fault_update(12,4);
		motor_stop();	
	}
}

static void ManualCalibraCallBack(void)
{
	if(calibration_status == 11){
		I4CalibraERRHander();
		
		//left curtain manual Calibration
		if(calibration_operation == 0 && calibration_operation_auto == 0 && run_state == 3)
		{
			
			run_state = 1;
			I4_LOG("open curtain %d\r\n ",curtain_mode);
			open_curtain();
			gptm0_4clbr = 0;
			
		}

		if(calibration_operation == 2 && calibration_operation_auto == 0 && run_state == 3)
		{
			
			run_state = 2;
			I4_LOG("close curtain =  %d\r\n",curtain_mode);
			close_curtain();
			gptm0_4clbr = 0;
			
		}

		if ((motor_current > protect_current || i4_LowVlotageflag)  || (calibration_operation == 1 && calibration_operation_auto == 0))
		{
			
			if(motor_current > protect_current)
			{
				printf("calibration Current locked rotor\r\n");
				motor_current = 0;
			}
			
			if(i4_LowVlotageflag )
			{
				printf("calibration Interrupt stall\r\n");
				i4_LowVlotageflag = FALSE;
			}
			MotorRunStatus = FALSE;
			//printf("calibra staop = %d\n",gptm0_4clbr);
			calibration_operation = 4;
			if(Moveflag == 0)
			{
				run_state = 3;
				motor_stop(); //ֹͣ

				//printf("calibration gptm0_4clbr = %d\n",gptm0_4clbr);
				if(IsignLaunchDirection == 1){
					//printf("calibration gptm0_4clbr = %d\n",gptm0_4clbr);
					variable.returntime = gptm0_4clbr;
				}else{
					variable.full_running_time = gptm0_4clbr;
				}
								
			}else{
				if(IsignLaunchDirection == 1){
					//I4_LOG("I4ReturnManualHandler\n");
					I4ReturnManualHandler();
					//IsignLaunchDirection = 0;
				}else{
					stop_fun();//��ʱ��
				}
				
			}
			Moveflag = 1;
			
			//I4_LOG("calibration variable.full_running_time = %d , variable.returntime = %d\r\n",variable.full_running_time,variable.returntime);
//			I4_LOG("MUTE variable.full_running_time = %d , variable.returntime = %d\r\n",silenttime.Muteclosetime, silenttime.Muteopentime);
		}
	}
}

void I4LaunchHandler(void)
{
	 if (calibration_status == 1) //��ʼУ׼
    {
		KeyControl = FALSE;
		//variable.auto_run_mode = 0;
		printf("start calibration. \r\n");
    	IsignCalibrating = TRUE;
		motor_stop();
    	Moveflag = 0;
		crc_flag = 0;
		
    	if(MotorRunStatus){
			variable.full_running_time = 0;
			variable.returntime= 0;
		
		    cnt_switch = 0;
            ptime.cnt_flag = 0;
			mcu_dp_enum_update(DPID_CONTROL, 1);
		}
		
		
		
        if(curtain_mode == 0)
       {
		   
        	close_curtain();
       }
		if(curtain_mode == 1)
       {
		   
        	open_curtain();
       }
		gptm0_4clbr = 0;
		calibration_operation_auto = 1;
		calibration_status = 11;
		MotorRunStatus = TRUE;	
		IsignStopPercent = FALSE;
    }

}

void I4ReturnJourneyHandler(void)
{
	if(calibration_status == 3)						
	{
		
		/*��¼��������ʱ��*/
		if(MotorRunStatus){							// Return without blocking	

			variable.full_running_time = gptm0_4clbr;	

			if(run_state == 1 ){	//�������ֶ�У׼��ʱ��ֱ�ӵ���� �Ĵ���	
				I4_LOG("maual run_state = %d\r\n",run_state);
				stop_fun();//��ʱ��
				ms_delay_systick(500);
				close_curtain();
				
			}
			if( run_state == 2){
				I4_LOG("maual run_state = %d\r\n",run_state);
				stop_fun();//��ʱ��
				ms_delay_systick(500);
				open_curtain();
			}
			//I4_LOG("full_running_time = %d\r\n",variable.full_running_time);

		}else{										//Back trip blocked
			
		}

		gptm0_4clbr = 0;
		Moveflag = 0;
		calibration_operation_auto  = 1;
		IS_CailbrationBack = TRUE;
		/*��ʼ����ʱ���ʱ*/
		if(curtain_mode == 0){

			open_curtain();
			
			I4_LOG("ReturnJourney start open= %d\r\n",variable.full_running_time);
			IsignLaunchDirection = 1;
		}
		if(curtain_mode == 1){
			close_curtain();
			I4_LOG("ReturnJourney start close= %d\r\n",variable.full_running_time);
			IsignLaunchDirection = 1;
		}

		calibration_status = 11;
	}

	
	
}

void I4CompleteHandler(void)
{
	if (calibration_status == 2) //���У׼
    {	
    	IsignCalibrating = FALSE;
    	
		crc_flag = 1;
		motor_stop();
		if(curtain_mode == 0){
			//I4_LOG("calibration_operation_auto = %d\r\n",calibration_operation_auto);
			if(calibration_operation_auto == 1){	//Auto Calibration

				variable.returntime = gptm0_4clbr;
				
				//notes839 variable.full_running_time = %d \n",variable.full_running_time);
			
			}else{
				if(calibration_operation == 0 || calibration_operation == 2){
					I4ReturnManualHandler();

				}
			}
			//notes847 curtain runtime = %d\n",variable.full_running_time);
				
		}
		if(curtain_mode == 1){
			if(calibration_operation_auto == 1){

				variable.returntime = gptm0_4clbr;
				
				//notes853 variable.full_running_time = %d \n",variable.full_running_time);
				
			}else{
				if(calibration_operation == 0 || calibration_operation == 2){
					I4ReturnManualHandler();
				}
			}
		}
		printf("%d ==============================\r\n",variable.full_running_time);
		//��ȫ��ģʽʱ���ȥ1s
		variable.full_running_time -= 1400;
		Calibra_erase_write();
		printf("%d =reduce=============================\r\n",variable.full_running_time);
		CalibraCallBack();//Update percentage status to 0 after calibration
		if(variable.full_running_time < 0){
			variable.full_running_time = 0;
		}
		if(variable.returntime < 0){
			variable.returntime = 0;
		}

		printf("qicheng = %d , return time = %d\r\n",variable.full_running_time,variable.returntime);
		//I4_LOG("MUTE qicheng = %d , return time = %d\r\n",silenttime.Muteopentime, silenttime.Muteclosetime);
		
		CalibrMotor = true;
       	
		IsignLaunchDirection = 0;
       	run_state = 0;  

       	gptm0_4low = 0;
		IS_CailbrationBack = FALSE;
		calibration_status = 0;
		
		//У׼�����ص�һ�µ��
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
		MotorRunStatus = FALSE;	

		variable.calibration_status = 2;
		Calibra_erase_write();

		KeyControl = TRUE;
		//variable.auto_run_mode = 1;
    }
}
/*
*��������ʱ��У׼
**curtain_mode 0Ϊ���� 1Ϊ�Ҵ���
*/
void calibration_test(void)
{

	I4LaunchHandler();		//start
	I4ReturnJourneyHandler();//return
	ManualCalibraCallBack();//Manual ��ʱ��
	I4CompleteHandler();	//end

} 

void cycle_fun(global_variable *arg)
{
    u32 i;
    for (i = 0; i <= 16; i += 4, arg++)
    {
        FLASH_ProgramWordData(FLASH_55K_GLOBAL_DATA + i, (u32)arg);
    }
}

/*
	Function description @: Write global parameters to flash ,Write the start address of the falsh:FLASH_55K_GLOBAL_DATA 55k(0xDC00)
*/
void WriteFlashGlobalCallBack()
{
#if 0
	//53k���ά������
	FLASH_ProgramWordData(FLASH_55K_GLOBAL_DATA, variable.full_running_time);
    FLASH_ProgramWordData(FLASH_55K_CURRENT_PERCENT, variable.global_percent); 
    FLASH_ProgramWordData(FLASH_55K_AUTO_MODE, //variable.auto_run_mode);
	FLASH_ProgramWordData(FLASH_55K_LEFT_RIGHT_CURTAIN,curtain_mode);
	FLASH_ProgramWordData(FLASH_55K_CALIBRA_STA,variable.calibration_status);
	FLASH_ProgramWordData(FLASH_55K_RETURN_TIME,variable.returntime);
#endif	
	//53k���ά������ - ����ʱ����
	FLASH_ProgramWordData(FLASH_55K_DOUNTDOWN_FLAG,count_oper.flag);
	FLASH_ProgramWordData(FLASH_55K_DOUNTDOWN_SWITCH,count_oper.gswitch);


	
}

void global_arg_fun(void)
{
#if 0
	//53k���ά������
	variable.global_percent = rw(FLASH_55K_CURRENT_PERCENT);
    variable.full_running_time = rw(FLASH_55K_GLOBAL_DATA);
    //variable.auto_run_mode = rw(FLASH_55K_AUTO_MODE);
	variable.calibration_status = rw(FLASH_55K_CALIBRA_STA);
	curtain_mode = rw(FLASH_55K_LEFT_RIGHT_CURTAIN);
	variable.returntime = rw(FLASH_55K_RETURN_TIME);
#endif
	//53k���ά������ - ����ʱ����
	count_oper.flag = rw(FLASH_55K_DOUNTDOWN_FLAG);
	count_oper.gswitch = rw(FLASH_55K_DOUNTDOWN_SWITCH);
	//printf("53:%d	%d	%d	%d	%d %d 53_count: %d %d\r\n",variable.global_percent,variable.full_running_time,//variable.auto_run_mode,variable.calibration_status,curtain_mode,variable.returntime,\
			count_oper.flag,count_oper.gswitch);
	
    //if (variable.full_running_time == -1) //���״���������ʱ�����һ�θ�ȫ�ֱ�������ֵ�������Ǵ�flash��ȡֵ���if���ٽ��롣
	if(1)
	{
		//printf("age calibration. \r\n");
        FLASH_ErasePage(FLASH_55K_GLOBAL_DATA);
        
		crc_flag = 0;
        variable.global_percent = 100;				//1.global percent
        variable.full_running_time = 0;				//2.full running time
        //variable.auto_run_mode = 1;					//3.run mode
		variable.run_mode_flag = 1;
		variable.calibration_status = 0;			//4.calibration_status
		curtain_mode = 0;							//5.Calibration mode (left curtain or right curtain)
		variable.returntime = 0;					//6.calibration return time

		count_oper.flag = 1;						//7.Coundown flag
		count_oper.gswitch = 1;						//8.Coundown switch. 


        WriteFlashGlobalCallBack();
    }
    else
    {
		if(variable.full_running_time != 0) crc_flag = 1;//Judge whether it has been calibrated
        mcu_dp_enum_update(DPID_CALIBRATION_STATUS, 2);
    }
}



//�ٷֱ�д��flashʱ����
void Percentage_control(void)
{
#if 0
    variable.full_running_time = rw(FLASH_55K_GLOBAL_DATA);  
    //variable.auto_run_mode = rw(FLASH_55K_AUTO_MODE);
	curtain_mode = rw(FLASH_55K_LEFT_RIGHT_CURTAIN);
	variable.calibration_status = rw(FLASH_55K_CALIBRA_STA);
	variable.returntime = rw(FLASH_55K_RETURN_TIME);

	count_oper.flag = rw(FLASH_55K_DOUNTDOWN_FLAG);
	count_oper.gswitch = rw(FLASH_55K_DOUNTDOWN_SWITCH);
	
	FLASH_ErasePage(FLASH_55K_GLOBAL_DATA);
	WriteFlashGlobalCallBack();
#endif
}
//�Զ�ģʽд��flashʱ����
void arg_erase_write(void)
{
#if  0
    variable.full_running_time = rw(FLASH_55K_GLOBAL_DATA);
    variable.global_percent = rw(FLASH_55K_CURRENT_PERCENT);
	curtain_mode = rw(FLASH_55K_LEFT_RIGHT_CURTAIN);
	variable.calibration_status = rw(FLASH_55K_CALIBRA_STA);
	variable.returntime = rw(FLASH_55K_RETURN_TIME);

	count_oper.flag = rw(FLASH_55K_DOUNTDOWN_FLAG);
	count_oper.gswitch = rw(FLASH_55K_DOUNTDOWN_SWITCH);

	FLASH_ErasePage(FLASH_55K_GLOBAL_DATA);
	WriteFlashGlobalCallBack();
#endif
}
//У׼ģʽдflash����
void Calibra_erase_write(void)
{ 
#if 0 
    variable.global_percent = rw(FLASH_55K_CURRENT_PERCENT);
    //variable.auto_run_mode = rw(FLASH_55K_AUTO_MODE);
	curtain_mode = rw(FLASH_55K_LEFT_RIGHT_CURTAIN);

	count_oper.flag = rw(FLASH_55K_DOUNTDOWN_FLAG);
	count_oper.gswitch = rw(FLASH_55K_DOUNTDOWN_SWITCH);
	
	FLASH_ErasePage(FLASH_55K_GLOBAL_DATA);
	WriteFlashGlobalCallBack();
#endif
}
//���Ҵ���ģʽѡ��
void CurtainModeEraseWrite(void)
{
#if 0
    variable.full_running_time = rw(FLASH_55K_GLOBAL_DATA);
    variable.global_percent = rw(FLASH_55K_CURRENT_PERCENT);
    //variable.auto_run_mode = rw(FLASH_55K_AUTO_MODE);
	variable.returntime = rw(FLASH_55K_RETURN_TIME);
	variable.calibration_status = rw(FLASH_55K_CALIBRA_STA);

	count_oper.flag = rw(FLASH_55K_DOUNTDOWN_FLAG);
	count_oper.gswitch = rw(FLASH_55K_DOUNTDOWN_SWITCH);

	FLASH_ErasePage(FLASH_55K_GLOBAL_DATA);
	WriteFlashGlobalCallBack();
#endif
}

void Coundownflag_EraseWrite(void)
{
    // variable.full_running_time = rw(FLASH_55K_GLOBAL_DATA);
    // variable.global_percent = rw(FLASH_55K_CURRENT_PERCENT);
    // //variable.auto_run_mode = rw(FLASH_55K_AUTO_MODE);
	// variable.returntime = rw(FLASH_55K_RETURN_TIME);
	// curtain_mode = rw(FLASH_55K_LEFT_RIGHT_CURTAIN);
	// variable.calibration_status = rw(FLASH_55K_CALIBRA_STA);

	
	count_oper.gswitch = rw(FLASH_55K_DOUNTDOWN_SWITCH);

	FLASH_ErasePage(FLASH_55K_GLOBAL_DATA);
	WriteFlashGlobalCallBack();
}

void Coundownswitch_EraseWrite(void)
{
#if 0
    variable.full_running_time = rw(FLASH_55K_GLOBAL_DATA);
    variable.global_percent = rw(FLASH_55K_CURRENT_PERCENT);
    //variable.auto_run_mode = rw(FLASH_55K_AUTO_MODE);
	variable.returntime = rw(FLASH_55K_RETURN_TIME);
	curtain_mode = rw(FLASH_55K_LEFT_RIGHT_CURTAIN);
	variable.calibration_status = rw(FLASH_55K_CALIBRA_STA);
#endif
	count_oper.flag = rw(FLASH_55K_DOUNTDOWN_FLAG);
	

	FLASH_ErasePage(FLASH_55K_GLOBAL_DATA);
	WriteFlashGlobalCallBack();
}

