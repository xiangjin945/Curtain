#include "service_logic_manage.h"

global_variable variable;

percentage_time ptime;


u8 flage_timer_on = 0;
// u8 monday = 0;
// u8 tuesday = 0;
// u8 wednesday = 0;
// u8 thursday = 0;
// u8 friday = 0;
// u8 saturday = 0;
// u8 sunday = 0;
u8 count_down_action = 0;
volatile u16 protect_current = 400; // NOTE: ȱʡ��ת��������ֵ 310ma  TODO: ����ģʽ��ֵ 250ma
// NOTE: 0xFA=250ma  0x136=310ma  0x15E=350ma

u32 timer_wday = 0;
u8 timer_hours = 0;
u8 timer_minutes = 0;
u8 lssued_seconds = 0;
u8 lssued_minutes = 0;
u32 lssued_time = 0;
u32 single_timing = 0;
u32 timer_buf[8][7] = {0};	   //���ظ�
u32 timer_buf_single[8] = {0}; //��������Ч
u8 timing_start_flag = 0;

u32 timing_buffer[64] = {0};
u32 new_timing_buffer[56] = {0};

//����ģʽ
volatile u8 curtain_mode = 0; //0=����, 1=�Ҵ���
//У׼״̬
volatile u8 calibration_status = 0; //0=δ�趨, 1=��ʼ, 2=���
//У׼����
volatile u8 calibration_operation = 0; //0=����, 1=ֹͣ, 2=����
//ȫ������ʱ��
//volatile u32 variable.full_running_time = 0;
//У׼���� AutoMode
volatile u8 calibration_operation_auto = 0; //0=auto operation status, 1=user operation status

struct week_task week_task;
struct dp_data_buf_t dp_data_buf_t;
struct tlv_type tlv_type = {0};

//dp_id=0x01=�����������ؿ���
void curtain_control_01_pack(u8 *data_buf)
{
	int i = 0;

	//usart0_send(data_buf,5);
	dp_data_buf_t.dp_id = data_buf[0];
	dp_data_buf_t.dp_type = data_buf[1];
	dp_data_buf_t.dp_data_len = data_buf[2];
	dp_data_buf_t.dp_data_len = (dp_data_buf_t.dp_data_len << 8) & data_buf[3];
	dp_data_buf_t.dp_data_value[0] = data_buf[4];

	for (i = 0; i < dp_data_buf_t.dp_data_len; i++)
	{
		//dp_data_buf_t.dp_data_value[0] = data_buf[i+4];
	}

#ifdef DEBUG_STATE_LED
	SET_LED(RESET); //����
	GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_3, RESET);
#endif

	//if(data_buf[4]==0x01)
	if (dp_data_buf_t.dp_id == 0x01)
	{
		//GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_3,RESET);
		//SET_LED(RESET);			//����
		if (dp_data_buf_t.dp_data_value[0] == 0x00)
		{
#ifdef DEBUG_STATE_LED
			SET_LED(RESET); //����
			GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_3, RESET);
#endif
			open_curtain();
			uart0_len = 0;
			memset(uart0_buf, 0, 100);
		}
		if (dp_data_buf_t.dp_data_value[0] == 0x01)
		{
#ifdef DEBUG_STATE_LED
			SET_LED(RESET); //����
			GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_3, RESET);
#endif
			motor_pwr(PWR_OFF);
			motor_stop();
			uart0_len = 0;
			memset(uart0_buf, 0, 100);
		}
		if (dp_data_buf_t.dp_data_value[0] == 0x02)
		{
#ifdef DEBUG_STATE_LED
			SET_LED(RESET); //����
			GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_3, RESET);
#endif
			close_curtain();
			uart0_len = 0;
			memset(uart0_buf, 0, 100);
		}
	}
}

//dp_id=0x02=�ٷֱȿ���
void currain_percentage_02_pack(u8 *data_buf)
{
}

//dp_id=0x0C=12=���ϸ澯
void fault_alarm_0C_pack(u8 *data_buf)
{
}

//dp_id=0x0D=13=��ص���
void battery_level_0D_pack(u8 *data_buf)
{
}

//dp_id=0x65=101=�Զ�����ģʽ
void auto_open_close_65_pack(u8 *data_buf)
{
}

//dp_id=0x66=102=����ʱ
void count_down_66_pack(void)
{
	//02 08 00 01 FF FF 41 00 00 01
	if (tlv_type.value[0])
	{
		alarm_second = (tlv_type.value[0] * 60 * 60);
	}
	if (tlv_type.value[1])
	{
		alarm_second += (tlv_type.value[1] * 60);
	}

	count_down_action = tlv_type.value[5];

	setup_interrupt(alarm_second);
	rtc_interrupt = 0;
}
void timing_control_102_pack(const unsigned char value[])
{
	u32 timer_value;
	u32 week_value;
	if (value[5] == 1)
	{
		timer_value = value[0] * 3600 + value[2] * 60;
		week_value = 1;
		if (timer_wday > week_value) //�����ǰʱ����ڶ�ʱʱ��
		{
			timer_value += 604800;
			//timer = timer_value - lssued_time ;
		}
		else
		{
		}
	}
	if (value[6] == 1)
	{
		timer_value = (value[0] * 3600 + value[2] * 60) + 86400;
		week_value = 2;
	}
	if (value[7] == 1)
	{
		timer_value = value[0] * 3600 + value[2] * 60 + 172800;
		week_value = 3;
	}
	if (value[8] == 1)
	{
		timer_value = value[0] * 3600 + value[2] * 60 + 259200;
		week_value = 4;
	}
	if (value[9] == 1)
	{
		timer_value = value[0] * 3600 + value[2] * 60 + 345600;
		week_value = 5;
	}
	if (value[10] == 1)
	{
		timer_value = value[0] * 3600 + value[2] * 60 + 432000;
		week_value = 6;
	}
	if (value[11] == 1)
	{
		timer_value = value[0] * 3600 + value[2] * 60 + 518400;
		week_value = 7;
	}
}
//dp_id=0x66=102=��ʱ
void timing_control_66_pack(void)
{
	//01 08 00 01 FF FF 00 00 00 01
	int i = 0;
	int date[7] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40};

	if (tlv_type.value[4] == 0x00) //����
	{
		//tlv_type.value[0]*60*60
		//tlv_type.value[0] = current_time-tlv_type.value[0];
		count_down_66_pack(); //����ʱ
	}
	else //�ظ�
	{
		for (i = 0; i < 7; i++)
		{
			switch (tlv_type.value[4] & date[i])
			{
			case 0x01: //monday
				week_task.monday_timed_task.hour = tlv_type.value[0];
				week_task.monday_timed_task.minute = tlv_type.value[1];
				week_task.monday_timed_task.action = tlv_type.value[5];
				break;
			case 0x02: //tuesday
				week_task.tuesday_timed_task.hour = tlv_type.value[0];
				week_task.tuesday_timed_task.minute = tlv_type.value[1];
				week_task.tuesday_timed_task.action = tlv_type.value[5];
				break;
			case 0x04: //wednesday
				week_task.wednesday_timed_task.hour = tlv_type.value[0];
				week_task.wednesday_timed_task.minute = tlv_type.value[1];
				week_task.wednesday_timed_task.action = tlv_type.value[5];
				break;
			case 0x08: //thursday
				week_task.thursday_timed_task.hour = tlv_type.value[0];
				week_task.thursday_timed_task.minute = tlv_type.value[1];
				week_task.thursday_timed_task.action = tlv_type.value[5];
				break;
			case 0x10: //friday
				week_task.friday_timed_task.hour = tlv_type.value[0];
				week_task.friday_timed_task.minute = tlv_type.value[1];
				week_task.friday_timed_task.action = tlv_type.value[5];
				break;
			case 0x20: //saturday
				week_task.saturday_timed_task.hour = tlv_type.value[0];
				week_task.saturday_timed_task.minute = tlv_type.value[1];
				week_task.saturday_timed_task.action = tlv_type.value[5];
				break;
			case 0x40: //sunday
				week_task.sunday_timed_task.hour = tlv_type.value[0];
				week_task.sunday_timed_task.minute = tlv_type.value[1];
				week_task.sunday_timed_task.action = tlv_type.value[5];
				break;
			}
		}
	}
}




void dp_DPID_CALIBRATION_STATUS(const unsigned char value[])
{
	//STRING�������ݴ���
	unsigned char string_data;

	string_data = value[0];

	if (string_data == 0)
	{
		calibration_status = 0;
	}
	if (string_data == 1)
	{
		calibration_status = 1;
		//open_curtain();
	}
	if (string_data == 2)
	{
		calibration_status = 2;
		//close_curtain();
	}
}






void dp_DPID_CALIBRATION_operation(const unsigned char value[])
{
	//STRING�������ݴ���
	unsigned char string_data;

	string_data = value[0];

	if (string_data == 0)
	{
		calibration_operation = 0; //����
	}
	if (string_data == 1)
	{
		calibration_operation = 1; //ֹͣ
	}
	if (string_data == 2)
	{
		calibration_operation = 2; //����
	}
	calibration_operation_auto = 0;
}

//dp_id=0x78=120=���õ����������
void set_motor_current_78(u8 *data_buf)
{
	//78 02 00 04 00 00 02 F4 81
	// 55 AA 00 07 00 08 | 78 02 00 04 00 00 01 90 1D
	int i = 0;
	struct dp_data_buf_t dp_data_buf_t_01;

	usart0_send(data_buf, 8);

	dp_data_buf_t_01.dp_id = data_buf[0];
	dp_data_buf_t_01.dp_type = data_buf[1];
	dp_data_buf_t_01.dp_data_len = data_buf[2];
	dp_data_buf_t_01.dp_data_len = (dp_data_buf_t_01.dp_data_len << 8) | data_buf[3];

	for (i = 0; i < dp_data_buf_t_01.dp_data_len; i++)
	{
		dp_data_buf_t_01.dp_data_value[i] = data_buf[i + 4];
	}

	protect_current = data_buf[6] & 0xFFFF;
	protect_current = (protect_current << 8) | (data_buf[7] & 0xFFFF);
	//protect_current = dp_data_buf_t_01.dp_data_value[2]&0xFFFF;
	//protect_current = (protect_current<<8)|(dp_data_buf_t_01.dp_data_value[3]&0xFFFF);

	//USART_SendData(HT_USART0,(protect_current>>8));
	//USART_SendData(HT_USART0,(protect_current&0xFF));

	//USART_SendData(HT_USART0,protect_current);
	//while(USART_GetFlagStatus(HT_USART0, USART_FLAG_TXC) == RESET);
}

