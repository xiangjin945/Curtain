#include "tybn1_init.h"

#define DEBUG_USART0_OUT

u8 mcu_state = 0;			// MCU��һ���ϵ�״̬, 1=һֱ����
u8 dp_data_buf[100] = {0};

void update_status(u8 *dp_state,u8 data_len);

/*****************************************************************************
�������� : get_check_sum
�������� : ����У���
������� : pack:����Դָ��
           pack_len:����У��ͳ���
���ز��� : У���
*****************************************************************************/
unsigned char get_check_sum(unsigned char *pack, unsigned short pack_len)
{
	unsigned short i;
	unsigned char check_sum = 0;
  
	for(i = 0; i < pack_len; i ++)
	{
		check_sum += *pack ++;
	}
  
	return check_sum;
}


//0x00=������
void heart_beat(void)
{
	u8 crc8 = 0;
	
	dp_data_buf[0] = 0x55;
	dp_data_buf[1] = 0xAA;
    dp_data_buf[2] = 0x00;
    dp_data_buf[3] = 0x0B;
	dp_data_buf[4] = 0x00;	//���ݳ���
    dp_data_buf[5] = 0x08;	//���ݳ���
    dp_data_buf[6] = 0x00;	//���ݳ���
    dp_data_buf[7] = 0x00;	//���ݳ���
    dp_data_buf[8] = 0x00;	//���ݳ���
    dp_data_buf[9] = 0x01;	//���ݳ���
    dp_data_buf[10] = mcu_state;
    
	crc8 = get_check_sum(dp_data_buf,11);
	dp_data_buf[11] = crc8;
    
	uart0_send(dp_data_buf,12);
#if 1
    usart0_send(dp_data_buf,12);
#endif

}

//0x01=��ȡMCU��Ϣ, ����ģ�鹤��ģʽ
void get_mcu_info(void)
{
    int i = 0;
	u8 crc8 = 0;
	u8 mcu_info[13] = {0x62,0x39,0x70,0x61,0x34,0x75,0x62,0x6E,0x31,0x2E,0x31,0x2E,0x30};
	
	dp_data_buf[0] = 0x55;
	dp_data_buf[1] = 0xAA;
    dp_data_buf[2] = VERSION;
    dp_data_buf[3] = 0x01;
    dp_data_buf[4] = 0x00;
    dp_data_buf[5] = 0x0d;
    
    for(i=0;i<13;i++)
    {
        dp_data_buf[i+6] = mcu_info[i];
    }
	crc8 = get_check_sum(dp_data_buf,19);
	dp_data_buf[19] = crc8;
    
	uart0_send(dp_data_buf,20);
#ifdef DEBUG_USART0_OUT
    usart0_send(dp_data_buf,20);
#endif
	//uart0_len = 0;
	//memset(uart0_buf,0,100);
}

//0x02=����ģ�鹤��ģʽ
void get_tybn1_mode(void)
{
	u8 crc8 = 0;
	
	dp_data_buf[0] = 0x55;
	dp_data_buf[1] = 0xAA;
    dp_data_buf[2] = VERSION;
    dp_data_buf[3] = 0x02;
    dp_data_buf[4] = 0x00;
    dp_data_buf[5] = 0x00;
    
	crc8 = get_check_sum(dp_data_buf,6);
	dp_data_buf[6] = crc8;
    
	uart0_send(dp_data_buf,7);
#ifdef DEBUG_USART0_OUT
    usart0_send(dp_data_buf,7);
#endif
	//uart0_len = 0;
	//memset(uart0_buf,0,100);
}

//0x03=ģ�鹤��״̬
void send_tybn1_state(void)
{
	u8 crc8 = 0;
	
	dp_data_buf[0] = 0x55;
	dp_data_buf[1] = 0xAA;
    dp_data_buf[2] = VERSION;
    dp_data_buf[3] = 0x03;
    dp_data_buf[4] = 0x00;
    dp_data_buf[5] = 0x00;
    
	crc8 = get_check_sum(dp_data_buf,6);
	dp_data_buf[6] = crc8;
    
	uart0_send(dp_data_buf,7);
#ifdef DEBUG_USART0_OUT
    usart0_send(dp_data_buf,7);
#endif
	
	if(uart0_buf[6]==0x00)
	{
		//δ��
	}
	if(uart0_buf[6]==0x01)
	{
		//��δ����
	}
	if(uart0_buf[6]==0x02)
	{
		//��������
	}
	//uart0_len = 0;
	//memset(uart0_buf,0,100);
}

//0x04=����ģ��
void reset_tybn1(void)
{
	u8 crc8 = 0;
	
	dp_data_buf[0] = 0x55;
	dp_data_buf[1] = 0xAA;
    dp_data_buf[2] = VERSION;
    dp_data_buf[3] = 0x04;
    dp_data_buf[4] = 0x00;
    dp_data_buf[5] = 0x00;
	
	crc8 = get_check_sum(dp_data_buf,6);
	dp_data_buf[6] = crc8;
    
	uart0_send(dp_data_buf,7);
#ifdef DEBUG_USART0_OUT
    usart0_send(dp_data_buf,7);
#endif
	
	gptm0_cnt = 0;
	gptm0_second = 0;
	while(gptm0_second<2){;;;}
		
	//uart0_len = 0;
	//memset(uart0_buf,0,100);
}

//0x06=�����·�
void send_command(void)
{
	int i=0;
	u16 len = 0;		//���ݳ���
	u8 tmp_buf[10] = {0};
	
	//�͹��ı�־
	bftm_cnt = 0;
	bftm_second =0;
	bftm_minute = 0;
	low_power_event_flag++;

	len = ((u16)uart0_buf[4]);
	len = (len<<8)|((u16)uart0_buf[5]);
	
#ifdef DEBUG_USART0_OUT
	//SET_LED(RESET);			//����
	for(i=0;i<uart0_len;i++)
	{
//		USART_SendData(COM_DEBUG,uart0_buf[i]);
//		while(!USART_GetFlagStatus(COM_DEBUG,USART_FLAG_TXC));
	}
#endif
	
	//dp_data���
	for(i=0;i<len;i++)
	{
		tmp_buf[i] = uart0_buf[i+6];
	}

	for(i=0;i<len;i++)
	{
		//USART_SendData(HT_USART0,tmp_buf[i]);
		//while (USART_GetFlagStatus(HT_USART0, USART_FLAG_TXC) == RESET);
	}
	
	/**************************************************************************/
#if 1
	if(uart0_buf[6] ==0x01)			//dp_id=0x01=���ƴ���, ״̬
	{
		update_status(tmp_buf,len);
		curtain_control_01_pack(tmp_buf);
	}
	if(uart0_buf[6] ==0x02)			//dp_id=0x02=�ٷֱȿ���
	{
		update_status(tmp_buf,len);
		currain_percentage_02_pack(tmp_buf);
	}
	if(uart0_buf[6] ==0x0C)			//dp_id=0x0C=12=���ϸ澯
	{
		update_status(tmp_buf,len);
		//fault_alarm_0C_pack(tmp_buf);
	}
	if(uart0_buf[6] ==0x0D)			//dp_id=0x0D=13=��ص���
	{
		update_status(tmp_buf,len);
		//battery_level_0D_pack(tmp_buf);
	}
	if(uart0_buf[6]==0x065)			//dp_id=0x65=101=�Զ�����ģʽ
	{
		//65 01 00 01 01
		update_status(tmp_buf,len);
		//auto_open_close_65_pack(tmp_buf);
	}
	if(uart0_buf[6] ==0x66)			//dp_id=0x66=102=��ʱ����
	{
		update_status(tmp_buf,len);
	}
	if(uart0_buf[6] ==0x67)			//dp_id=0x67=103=����ģʽ
	{
		update_status(tmp_buf,len);
		
	}
	if(uart0_buf[6] ==0x68)			//dp_id=0x68=1034У׼״̬
	{
		update_status(tmp_buf,len);
	}
	if(uart0_buf[6] ==0x69)			//dp_id=0x69=105=LED����
	{
		update_status(tmp_buf,len);
		
	}
	if(uart0_buf[6] ==0x6A)			//dp_id=0x6A=106=��ص������㱨��
	{
		update_status(tmp_buf,len);
		//battery_level_6A_pack(tmp_buf);
	}
	if(uart0_buf[6] ==0x6B)			//dp_id=0x6B=107=����ģʽ
	{
		update_status(tmp_buf,len);
		
	}
	if(uart0_buf[6] ==0x6C)			//dp_id=0x6C=108=����ٶ�����
	{
		update_status(tmp_buf,len);
		
	}
	if(uart0_buf[6] ==0x6D)			//dp_id=0x6D=109=У׼����
	{
		update_status(tmp_buf,len);
	}
	//55 AA 00 06 00 08 78 02 00 04 00 00 02 F4 81
	if(uart0_buf[6] ==0x78)			//dp_id=0x78=120=���õ����������
	{
		update_status(tmp_buf,len);
		//usart0_send(tmp_buf,len);
		set_motor_current_78(tmp_buf);
	}
#else
	switch(uart0_buf[6])
	{
		case 0x01:			//dp_id=0x01=���ƴ���, ״̬
			update_status(tmp_buf,len);
			curtain_control_01_pack(tmp_buf);
			break;
		case 0x02:			//dp_id=0x02=�ٷֱȿ���
			update_status(tmp_buf,len);
			currain_percentage_02_pack(tmp_buf);
			break;
		case 0x0C:			//dp_id=0x0C=12=���ϸ澯
			update_status(tmp_buf,len);
			//fault_alarm_0C_pack(tmp_buf);
			break;
		case 0x0D:			//dp_id=0x0D=13=��ص���
			update_status(tmp_buf,len);
			//battery_level_0D_pack(tmp_buf);
			break;
		case 0x65:			//dp_id=0x65=101=�Զ�����ģʽ
			update_status(tmp_buf,len);
			//auto_open_close_65_pack(tmp_buf);
			break;
		case 0x66:			//dp_id=0x66=102=��ʱ����
			update_status(tmp_buf,len);
			timed_tasks_66_pack(tmp_buf);
			break;
		case 0x67:			//dp_id=0x67=103=����ģʽ
			update_status(tmp_buf,len);
			curtain_mode_67_pack(tmp_buf);
			break;
		case 0x68:			//dp_id=0x68=1034У׼״̬
			update_status(tmp_buf,len);
			break;
		case 0x69:			//dp_id=0x69=105=LED����
			update_status(tmp_buf,len);
			control_led_69_pack(tmp_buf);
			break;
		case 0x6A:			//dp_id=0x6A=106=��ص������㱨��
			update_status(tmp_buf,len);
			//battery_level_6A_pack(tmp_buf);
			break;
		case 0x6B:			//dp_id=0x6B=107=����ģʽ
			update_status(tmp_buf,len);
			fast_mode_6B_pack(tmp_buf);
			break;
		case 0x6C://dp_id=0x6C=108=����ٶ�����
			update_status(tmp_buf,len);
			motor_speed_setting_6C_pack(tmp_buf);
			break;
		case 0x6D:			//dp_id=0x6D=109=У׼����
			update_status(tmp_buf,len);
			break;
	}
#endif

#ifdef DEBUG_USART0_OUT
    //usart0_send(dp_data_buf,9);
#endif
	
	usart0_len = 0;
    memset(uart0_buf,0,100);

	//gptm0_cnt = 0;
	//gptm0_second = 0;
	//while(gptm0_second<2){;;;}
	//while(usart0_len<5){;;;}
	
	//	USART_SendData(COM_DEBUG,uart0_buf[i]);
	//	while(!USART_GetFlagStatus(COM_DEBUG,USART_FLAG_TXC));
		
	if(uart0_buf[6]==0)
	{
		//�ɹ�
	}
	if(uart0_buf[6]==1)
	{
		//ʧ��
	}
	//uart0_len = 0;
	//memset(uart0_buf,0,100);
    
	//uart0_len = 0;
	//memset(uart0_buf,0,100);
	memset(tmp_buf,0,10);
}

//0x07=״̬�ϱ�
void update_status(u8 *dp_state,u8 data_len)
{
	u8 i = 0, crc8 = 0;
	
	dp_data_buf[0] = 0x55;
	dp_data_buf[1] = 0xAA;
    dp_data_buf[2] = VERSION;
    dp_data_buf[3] = 0x07;
    dp_data_buf[4] = 0x00;	//���ݳ���
    dp_data_buf[5] = data_len;	//���ݳ���
	
	for(i=0;i<data_len;i++)
	{
		dp_data_buf[i+6] = dp_state[i];
	}
	
	crc8 = get_check_sum(dp_data_buf,(data_len+6));
	dp_data_buf[(data_len+6)] = crc8;
    
	uart0_send(dp_data_buf,(data_len+7));
#ifdef DEBUG_USART0_OUT
    usart0_send(dp_data_buf,(data_len+7));
#endif
	
	usart0_len = 0;
    memset(uart0_buf,0,100);

	//gptm0_cnt = 0;
	//gptm0_second = 0;
	//while(gptm0_second<2){;;;}
	//while(usart0_len<5){;;;}
	
	//	USART_SendData(COM_DEBUG,uart0_buf[i]);
	//	while(!USART_GetFlagStatus(COM_DEBUG,USART_FLAG_TXC));
		
	if(uart0_buf[6]==0)
	{
		//�ɹ�
	}
	if(uart0_buf[6]==1)
	{
		//ʧ��
	}
	//uart0_len = 0;
	//memset(uart0_buf,0,100);
}

//0x08=״̬��ѯ
void check_tybn1_status(void)
{
	//��
	uart0_len = 0;
	memset(uart0_buf,0,100);
}

//0x09=ģ����
void unbind_tybn1(void)
{
	u8 crc8 = 0;
	
	dp_data_buf[0] = 0x55;
	dp_data_buf[1] = 0xAA;
	dp_data_buf[2] = VERSION;
	dp_data_buf[3] = 0x09;
	dp_data_buf[4] = 0x00;	//���ݳ���
	dp_data_buf[5] = 0x00;	//���ݳ���
	
	crc8 = get_check_sum(dp_data_buf,6);
	dp_data_buf[6] = crc8;
	
	uart0_send(dp_data_buf,7);
#ifdef DEBUG_USART0_OUT
    usart0_send(dp_data_buf,7);
#endif
	
	usart0_len = 0;
    memset(uart0_buf,0,100);

	gptm0_cnt = 0;
	gptm0_second = 0;
	while(gptm0_second<5){;;;}
		
	if(uart0_buf[6]==0x00)
	{
		//���ɹ�
	}
	if(uart0_buf[6]==0x01)
	{
		//���ʧ��
	}
	//uart0_len = 0;
	//memset(uart0_buf,0,100);
}



////0x0E=RF��Ƶ����
//void rf_test(void)
//{
//	u8 crc8 = 0;
//	char* tmp_buf = NULL;
//	
//	dp_data_buf[0] = 0x55;
//	dp_data_buf[1] = 0xAA;
//	dp_data_buf[2] = VERSION;
//	dp_data_buf[3] = 0x0E;
//	dp_data_buf[4] = 0x00;	//���ݳ���
//	dp_data_buf[5] = 0x00;	//���ݳ���
//	
//	crc8 = get_check_sum(dp_data_buf,6);
//	dp_data_buf[6] = crc8;
//	
//	uart0_send(dp_data_buf,7);
//#ifdef DEBUG_USART0_OUT
//    usart0_send(dp_data_buf,7);
//#endif
//	
//	usart0_len = 0;
//    memset(uart0_buf,0,100);

//	gptm0_cnt = 0;
//	gptm0_second = 0;
//	while(gptm0_second<2){;;;}
//	while(usart0_len<5){;;;}
//	
////	tmp_buf = strstr((char*)bt_queue_buf,"true");
////	if(tmp_buf==NULL)
////	{
////		//û�����������ź�
////	}

//}

//0xA1=�ָ���������֪ͨ
void resume_factory(void)
{
	//��
}

//0xB0=MCU�͹��Ļ�������
void mcu_lpr_wakeup_configuration(u8 wakeup_time)
{
	u8 crc8 = 0;
	
	dp_data_buf[0] = 0x55;
	dp_data_buf[1] = 0xAA;
    dp_data_buf[2] = VERSION;
    dp_data_buf[3] = 0xB0;
    dp_data_buf[4] = 0x00;
    dp_data_buf[5] = 0x01;
    dp_data_buf[6] = wakeup_time;
	
	crc8 = get_check_sum(dp_data_buf,7);
	dp_data_buf[7] = crc8;
    
	uart0_send(dp_data_buf,8);
#ifdef DEBUG_USART0_OUT
    usart0_send(dp_data_buf,8);
#endif
	
	usart0_len = 0;
    memset(uart0_buf,0,100);

	gptm0_cnt = 0;
	gptm0_second = 0;
	while(gptm0_second<2){;;;}
	while(usart0_len<5){;;;}
	if(uart0_buf[6]==0)
	{
		//���óɹ�
	}
	else
	{
		//����ʧ��
	}
	//uart0_len = 0;
	//memset(uart0_buf,0,100);
}

//0xE0=��¼�������ϱ�
void update_record_data(void)
{
	u8 crc8 = 0;
	
	dp_data_buf[0] = 0x55;
	dp_data_buf[1] = 0xAA;
	dp_data_buf[2] = VERSION;
	dp_data_buf[3] = 0xE0;
	dp_data_buf[4] = 0x00;	//���ݳ���
	dp_data_buf[5] = 0x00;	//���ݳ���
	//dp_data_buf[6] = ����
	
	crc8 = get_check_sum(dp_data_buf,7);
	dp_data_buf[7] = crc8;
	
	uart0_send(dp_data_buf,8);
#ifdef DEBUG_USART0_OUT
    usart0_send(dp_data_buf,8);
#endif
	
	usart0_len = 0;
    memset(uart0_buf,0,100);

	gptm0_cnt = 0;
	gptm0_second = 0;
	while(gptm0_second<2){;;;}
	while(usart0_len<6){;;;}
	if(uart0_buf[6]==0x00)
	{
		//�洢�ɹ�
	}
	else
	{
		//�洢ʧ��
	}
	//uart0_len = 0;
	//memset(uart0_buf,0,100);
}

u8 real_time_cnt = 0;
//0xE1=��ȡʵʱʱ��
void get_real_time(void)
{
    //55 AA 00 E1 00 0B 00 00 02 0B 0C 0E 10 38 04 03 20 81 
	//55 AA 00 E1 00 11 00 01 31 36 30 35 30 39 32 34 31 37 30 30 30 03 20 A8 
	//55 AA 00 E1 00 11 00 01 31 36 30 35 30 39 37 30 34 38 30 30 30 03 20 AD 
	u8 crc8 = 0;
	
	dp_data_buf[0] = 0x55;
	dp_data_buf[1] = 0xAA;
	dp_data_buf[2] = VERSION;
	dp_data_buf[3] = 0xE1;
	dp_data_buf[4] = 0x00;	//���ݳ���
	dp_data_buf[5] = 0x01;	//���ݳ���
	//0x00=��ȡ7�ֽ�ʱ��ʱ������+2�ֽ�, 0x01=��ȡ13�ֽ�ms��unixʱ��+2�ֽ�ʱ����Ϣ, 0x02=��ȡ7�ֽ�ʱ��ʱ������+2�ֽ�
	dp_data_buf[6] = 0x01;
	crc8 = get_check_sum(dp_data_buf,7);
	dp_data_buf[7] = crc8;

	uart0_send(dp_data_buf,8);

}

//0xE5=ʹ�ܵ͹��Ĺ���
void enable_low_pwr(u8 tybn1_pwr)
{
	u8 crc8 = 0;
	
	dp_data_buf[0] = 0x55;
	dp_data_buf[1] = 0xAA;
	dp_data_buf[2] = VERSION;
	dp_data_buf[3] = 0xE5;
	dp_data_buf[4] = 0x00;			//���ݳ���
	dp_data_buf[5] = 0x01;			//���ݳ���
	dp_data_buf[6] = tybn1_pwr;		//0x00=�رյ͹���, 0x01=�򿪵͹���
	
	crc8 = get_check_sum(dp_data_buf,7);
	dp_data_buf[7] = crc8;
	
	uart0_send(dp_data_buf,8);
#ifdef DEBUG_USART0_OUT
    usart0_send(dp_data_buf,8);
#endif
	
	usart0_len = 0;
    memset(uart0_buf,0,100);

	gptm0_cnt = 0;
	gptm0_second = 0;
	while(gptm0_second<2){;;;}
	if(uart0_buf[6]==0)
	{
		//���óɹ�
	}
	else
	{
		//����ʧ��
	}
	//uart0_len = 0;
	//memset(uart0_buf,0,100);
}

//0xE7=�����Ͽ���������
void disconnect_bluettooth(void)
{
	u8 crc8 = 0;
	
	dp_data_buf[0] = 0x55;
	dp_data_buf[1] = 0xAA;
    dp_data_buf[2] = VERSION;
    dp_data_buf[3] = 0xE7;
    dp_data_buf[4] = 0x00;
    dp_data_buf[5] = 0x00;
    
	crc8 = get_check_sum(dp_data_buf,6);
	dp_data_buf[6] = crc8;
    
	uart0_send(dp_data_buf,7);
#ifdef DEBUG_USART0_OUT
    usart0_send(dp_data_buf,7);
#endif
	
	usart0_len = 0;
    memset(uart0_buf,0,100);

	gptm0_cnt = 0;
	gptm0_second = 0;
	while(gptm0_second<2){;;;}
	if(uart0_buf[6]==0)
	{
		//���óɹ�
	}
	else
	{
		//����ʧ��
	}
	//uart0_len = 0;
	//memset(uart0_buf,0,100);
}

//0xE8=��ѯMCU�汾��
void check_mcu_version(void)
{
	int i = 0;
	u8 crc8 = 0;
	u8 version[6] = {0x01,0x01,0x00,0x01,0x01,0x00};
	
	dp_data_buf[0] = 0x55;
	dp_data_buf[1] = 0xAA;
    dp_data_buf[2] = VERSION;
    dp_data_buf[3] = 0xE8;
    dp_data_buf[4] = 0x00;
    dp_data_buf[5] = 0x06;
    
	for(i=0;i<6;i++)
	{
		dp_data_buf[i+6] = version[i];
	}
	
	crc8 = get_check_sum(dp_data_buf,12);
	dp_data_buf[12] = crc8;
    
	uart0_send(dp_data_buf,13);
#ifdef DEBUG_USART0_OUT
    usart0_send(dp_data_buf,13);
#endif
	//uart0_len = 0;
	//memset(uart0_buf,0,100);
}

//0xE9=MCU�������͵�ǰ�汾��
void mcu_send_version(void)
{
	int i = 0;
	u8 crc8 = 0;
	u8 version[6] = {0x01,0x01,0x00,0x01,0x01,0x00};
	
	dp_data_buf[0] = 0x55;
	dp_data_buf[1] = 0xAA;
    dp_data_buf[2] = VERSION;
    dp_data_buf[3] = 0xE9;
    dp_data_buf[4] = 0x00;
    dp_data_buf[5] = 0x06;
    
	for(i=0;i<6;i++)
	{
		dp_data_buf[i+6] = version[i];
	}
	
	crc8 = get_check_sum(dp_data_buf,12);
	dp_data_buf[12] = crc8;
    
	uart0_send(dp_data_buf,13);
#ifdef DEBUG_USART0_OUT
    usart0_send(dp_data_buf,13);
#endif
}

//0xEA=MCU-OTA��������
void mcu_Upgrade_request(void)
{
	u8 crc8 = 0;
	
	dp_data_buf[0] = 0x55;
	dp_data_buf[1] = 0xAA;
    dp_data_buf[2] = VERSION;
    dp_data_buf[3] = 0xEA;
    dp_data_buf[4] = 0x00;
    dp_data_buf[5] = 0x06;
    //δ���
	crc8 = get_check_sum(dp_data_buf,7);
	dp_data_buf[7] = crc8;
    
	uart0_send(dp_data_buf,8);
#ifdef DEBUG_USART0_OUT
    usart0_send(dp_data_buf,8);
#endif
	//uart0_len = 0;
	//memset(uart0_buf,0,100);
}

//0xEC=MCU-OTA�����ļ�ƫ������
void upgrade_file_offset_request(void)
{
	u8 crc8 = 0;
	
	dp_data_buf[0] = 0x55;
	dp_data_buf[1] = 0xAA;
    dp_data_buf[2] = VERSION;
    dp_data_buf[3] = 0xEC;
    dp_data_buf[4] = 0x00;
    dp_data_buf[5] = 0x04;
    //δ���
	crc8 = get_check_sum(dp_data_buf,10);
	dp_data_buf[10] = crc8;
    
	uart0_send(dp_data_buf,11);
#ifdef DEBUG_USART0_OUT
    usart0_send(dp_data_buf,11);
#endif
	//uart0_len = 0;
	//memset(uart0_buf,0,100);
}

//0xED=MCU-OTA��������
void updata_the_data(void)
{
	u8 crc8 = 0;
	
	dp_data_buf[0] = 0x55;
	dp_data_buf[1] = 0xAA;
    dp_data_buf[2] = VERSION;
    dp_data_buf[3] = 0xED;
    dp_data_buf[4] = 0x00;
    dp_data_buf[5] = 0x01;
	dp_data_buf[6] = 0x00;	//0x00=�ɹ�, 0x01=�����쳣, 0x02=���Ȳ�һ��, 0x03=CRCУ��ʧ��, 0x04=����
	crc8 = get_check_sum(dp_data_buf,7);
	dp_data_buf[7] = crc8;
    
	uart0_send(dp_data_buf,8);
#ifdef DEBUG_USART0_OUT
    usart0_send(dp_data_buf,8);
#endif
	//uart0_len = 0;
	//memset(uart0_buf,0,100);
}

//0xEE=MCU-OTA��������
void updata_end(void)
{
	u8 crc8 = 0;
	
	dp_data_buf[0] = 0x55;
	dp_data_buf[1] = 0xAA;
    dp_data_buf[2] = VERSION;
    dp_data_buf[3] = 0xEE;
    dp_data_buf[4] = 0x00;
    dp_data_buf[5] = 0x01;
	dp_data_buf[6] = 0x00;	//0x00=�ɹ�, 0x01=���ݳ��ȴ���, 0x02=���Ȳ�һ��, 0x03=����
	crc8 = get_check_sum(dp_data_buf,7);
	dp_data_buf[7] = crc8;
    
	uart0_send(dp_data_buf,8);
#ifdef DEBUG_USART0_OUT
    usart0_send(dp_data_buf,8);
#endif
	//uart0_len = 0;
	//memset(uart0_buf,0,100);
}

void update_all_status(void)
{
	//update_status(tmp_buf,len);		//��ص���
	//update_status(tmp_buf,len);		//��ʱ����
	//update_status(tmp_buf,len);		//�ٶ�����
	//update_status(tmp_buf,len);		//����״̬
	//update_status(tmp_buf,len);		//��������״̬
}

// 2021.1.7 ��ֲTODO: ����PB1(EN)�ţ���ʹ3.3v�������͹���(�ɰ�������)
void tybn1_join_sleep_mode(void)
{
	//io_11		2021.1.7����PA14֪ͨģ�����͹���
	GPIO_WriteOutBits(HT_GPIOA, GPIO_PIN_14,SET);
	ms_delay_systick(1000);			// NOTE: �ӳ�n��
}




void tybn1_init(void)
{
	//int i = 0;
	//int flag = 0;
	
	mcu_send_version();		//0xE9
	ms_delay_systick(100);
	check_mcu_version();	//0xE8
	ms_delay_systick(100);
#if 0
	do
	{
		if(uart0_len>6)
		{
			switch(uart0_buf[3])
			{
				case 0x00:
					heart_beat();			//0x00
					flag = 4;
					break;
				case 0x01:
					//get_mcu_info();			//0x01
					ms_delay_systick(100);
					flag = 1;
					i++;
					break;
				case 0x02:
					get_tybn1_mode();		//0x02
					flag = 2;
					i++;
					break;
				case 0x03:
					send_tybn1_state();		//0x03
					flag = 3;
					i++;
					break;
				case 0xE8:
					check_mcu_version();	//0xE8
					flag = 0;
					i++;
					break;
			}
		}
	} while(i!=4);
#else
	ms_delay_systick(100);
	get_mcu_info();			//0x01
	ms_delay_systick(100);
	get_tybn1_mode();		//0x02
	ms_delay_systick(100);
	send_tybn1_state();		//0x03
	ms_delay_systick(100);
	heart_beat();			//0x00
	//ms_delay_systick(100);
	//update_all_status();	//0x07
	//get_real_time();		//0xE1
#endif
}

void tybn1_ctrl(void)
{
	int i = 0;
	
	for(i=0;i<1000;i++){;;;}
		
    if(mcu_state==0)
    {
        mcu_state = 0;
    }
    
	switch(uart0_buf[3])
	{
		case 0x00:			        //������
			heart_beat();
			break;
		case 0x01:				    //��ȡMCU��Ϣ
			get_mcu_info();
			break;
        case 0x02:                  //����ģ�鹤��ģʽ
			get_tybn1_mode();
            break;
		case 0x03:					//����ģ�鹤��״̬
			send_tybn1_state();
			break;
		case 0x04:					//����ģ��
			reset_tybn1();
			break;
		case 0x05:					//
			break;
		case 0x06:					//�����·�
			send_command();
			break;
		case 0x07:					//״̬�ϱ�
			//update_status();
			break;
		case 0x08:					//״̬��ѯ
			check_tybn1_status();
			break;
		case 0x09:					//ģ����
			unbind_tybn1();
			break;
		case 0x0E:					//RF��Ƶ����
			//rf_test();
			break;
		case 0xA1:					//�ָ���������֪ͨ
			resume_factory();
			break;
		case 0xB0:					//MCU�͹��Ļ�������
			mcu_lpr_wakeup_configuration(10);
			break;
		case 0xE0:					//��¼�������ϱ�
			update_record_data();
			break;
		case 0xE1:					//��ȡʵʱʱ��
			get_real_time();
			break;
		case 0xE5:					//ʹ�ܵ͹��Ĺ���
			enable_low_pwr(0);
			break;
		case 0xE7:					//0xE7=�����Ͽ���������
			disconnect_bluettooth();
			break;
		case 0xE8:					//��ѯ MCU �汾�ţ�CMD-0xE8��
			check_mcu_version();
			break;
		case 0xE9:					//MCU�������͵�ǰ�汾��
			//mcu_send_version();
			break;
		case 0xEA:					//0xEA=MCU-OTA��������
			mcu_Upgrade_request();
			break;
		case 0xEC:					//0xEC=MCU-OTA�����ļ�ƫ������
			upgrade_file_offset_request();
			break;
		case 0xED:					//0xED=MCU-OTA��������
			updata_the_data();
			break;
		case 0xEE:					//0xEE=MCU-OTA��������
			updata_end();
			break;
	}
    
	//uart0_len = 0;
	//memset(uart0_buf,0,100);
	//memset(dp_data_buf,0,100);
}
