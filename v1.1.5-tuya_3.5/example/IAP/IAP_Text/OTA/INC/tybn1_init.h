#ifndef __TYBN1_INIT_H_
#define __TYBN1_INIT_H_

//#include "system.h"
#include <stdio.h>
#include <string.h>
#include "uart0_init.h"
#include "usart0_init.h"
#include "motor_init.h"
#include "gptm0_init.h"
#include "service_logic_manage.h"
#include "systick_init.h"
#include "tst_function.h"
#include "bftm_init.h"
#include "low_power_iniit.h"

#define VERSION     0x00
#define OPEN		0x01
#define CLOSE		0x00

//CMD_TYPE
#define HEAD					0x55AA
#define HEART_BEAT				0x00		//������
#define MCU_INFO				0x01		//��ȡ MCU ��Ϣ
#define GET_TYBN1_MODE			0x02		//����ģ�鹤��ģʽ,���ӻ�δ����
#define TYBN1_OPERATION_MODE	0x03		//ģ�鹤��״̬
#define RESET_TYBN1				0x04		//����ģ��
#define TYBN1_ISSUED_COMMAND	0x06		//�����·�
#define STATUS_REPORT			0x07		//״̬�ϱ�
#define CHECK_STATUS			0x08		//״̬��ѯ
#define UNBUND_TYBN1			0x09		//ģ����
#define RECORD_DATA_REPORT		0xE0		//��¼�������ϱ�
#define GET_REAL_TIME			0xE1		//��ȡʵʱʱ��

extern u8 mcu_state;
extern u8 dp_data_buf[100];

extern unsigned char get_check_sum(unsigned char *pack, unsigned short pack_len);
extern void heart_beat(void);
extern void get_mcu_info(void);
extern void mcu_send_version(void);
extern void tybn1_init(void);
extern void tybn1_ctrl(void);
extern void update_status(u8 *dp_state,u8 data_len);
extern void full_running_time_0A_pack(u32 run_time_ms);
extern void enable_low_pwr(u8 tybn1_pwr);
extern void tybn1_join_sleep_mode(void);
extern void tybn1_out_sleep_mode(void);
extern void get_real_time(void);

#endif
