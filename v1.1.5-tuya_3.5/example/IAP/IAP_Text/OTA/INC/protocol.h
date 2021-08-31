/****************************************Copyright (c)*************************
**                               版权所�?(C), 2015-2020, 涂鸦科技
**
**                                 http://www.tuya.com
**
**--------------文件信息-------------------------------------------------------
**�?  �?  �? protocol.h
**�?       �? 下发/上报数据处理函数
**�?�?�?�?:

                  *******非常重要，一定要看哦！！�?*******

** 1、用户在此文件中实现数据下发/上报功能
** 2、DP的ID/TYPE及数据处理函数都需要用户按照实际定义实�?
** 3、当开始某些宏定义后需要用户实现代码的函数内部�?err提示,完成函数后请删除�?err
**
**--------------历史版本修订---------------------------------------------------
** �? �? v1.0
** 日　�? 2017�?�?�?
** 描　�? 1:创建涂鸦bluetooth对接MCU_SDK
**
**-----------------------------------------------------------------------------
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
#ifndef __PROTOCOL_H_
#define __PROTOCOL_H_

#include "stdbool.h"

/******************************************************************************
                            用户相关信息配置
******************************************************************************/
/******************************************************************************
                            1:修改产品信息                
******************************************************************************/
#define PRODUCT_KEY "b9pa4ubn"    //开发平台创建产品后生成�?6位字符产品唯一标识



#define MCU_VER "1.3.2"                              //用户的软件版�?用于MCU固件升级,MCU升级版本需修改
#define MCU_APP_VER_NUM      0x010302			//用户的软件版�?用于MCU固件升级,MCU升级版本需修改     				1.0.0
#define MCU_HARD_VER_NUM     0x010100					//用户的硬件版�?当前没有实际用处
#define MCU_VERSION_1					0x01
#define	MCU_VERSION_2					0x03
#define	MCU_VERSION_3					0x02
extern unsigned char versions[3];
/******************************************************************************
                          2:MCU是否需要支固件升级                  
如需要支持MCU固件升级,请开启该�?
MCU可调用mcu_api.c文件内的mcu_firm_update_query()函数获取当前MCU固件更新情况
                        ********WARNING!!!**********
当前接收缓冲区为关闭固件更新功能的大�?固件升级包为256字节
如需要开启该功能,串口接收缓冲区会变大
******************************************************************************/
#define         SUPPORT_MCU_FIRM_UPDATE                 //开启MCU固件升级功能(默认开�?
/******************************************************************************
                         3:定义收发缓存:
                    如当前使用MCU的RAM不够,可修改为24
******************************************************************************/
#ifndef SUPPORT_MCU_FIRM_UPDATE
#define BT_UART_QUEUE_LMT             16              //数据接收队列大小,如MCU的RAM不够,可缩�?
#define BT_UART_RECV_BUF_LMT          128              //根据用户DP数据大小量定,必须大于32
#else
#define BT_UART_QUEUE_LMT             512             //数据接收队列大小,如MCU的RAM不够,可缩�?
#define BT_UART_RECV_BUF_LMT          256             //固件升级缓冲�?需大缓�?必须大于260
#endif

#define BT_UART_SEND_BUF_LMT         512              //根据用户DP数据大小量定,必须大于32    512
/******************************************************************************
                        4:定义模块工作方式
模块自处�?
          bt指示灯和bt复位按钮接在bt模块�?开启BT_CONTROL_SELF_MODE�?
          并正确定义BT_STATE_KEY和BT_RESET_KEY
MCU自处�?
          bt指示灯和bt复位按钮接在MCU�?关闭BT_CONTROL_SELF_MODE�?
          MCU在需要处理复位bt的地方调用mcu_api.c文件内的mcu_reset_bt()函数,并可调用mcu_get_reset_bt_flag()函数返回复位bt结果
          或调用设置bt模式mcu_api.c文件内的mcu_set_bt_mode(BT_CONFIG_E mode)函数,并可调用mcu_get_bt_work_state()函数返回设置bt结果
******************************************************************************/
//#define         BT_CONTROL_SELF_MODE                       //bt自处理按键及LED指示�?如为MCU外界按键/LED指示灯请关闭该宏
#ifdef          BT_CONTROL_SELF_MODE                      //模块自处�?
  #define     BT_STATE_KEY            14                    //bt模块状态指示按键，请根据实际GPIO管脚设置
  #define     BT_RESERT_KEY           0                     //bt模块重置按键，请根据实际GPIO管脚设置
#endif

/******************************************************************************
                      5:MCU是否需要支持校时功�?                    
如需要请开启该�?并在Protocol.c文件内mcu_write_rtctime实现代码
mcu_write_rtctime内部�?err提示,完成函数后请删除�?err
mcu在bt模块正确联网后可调用mcu_get_system_time()函数发起校时功能
******************************************************************************/
#define         SUPPORT_MCU_RTC_CHECK                //开启校时功�?

/******************************************************************************
                        1:dp数据点序列号重新定义
          **此为自动生成代码,如在开发平台有相关修改请重新下载MCU_SDK**         
******************************************************************************/
//设备控制(可下发可上报)
//备注:【必选项】该DP点用于控制电机的开启、暂停、关闭。该DP点枚举值参数不允许修改，不允许增减。其中，continue功能在APP面板无呈现，用于第三方（Google home）开通使用，下发指令后，需要MCU记住上一次的状态，并执行动作，注意，无该功能，无法通过WWGA认证�?
#define DPID_CONTROL 1
//开启百分比控制(可下发可上报)
//备注:如果窗帘电机可以监控窗帘目前的所处位置，例如窗帘现在开了一半，也就�?0%，那么可以通过该DP点进行窗帘位置的自定义控制。控制使用该DP点，如果选择了该DP点，必选选择“开启百分比状态显示”，该DP点数值范围值参数不允许修改，不允许增减�?
#define DPID_PERCENT_CONTROL 2
#define DPID_PERCENT_UP_CONTROL 3
#define DPID_PERCENT_UP_CONTROL_STATUS 7
//工作模式(可下发可上报)
//备注:
#define DPID_MODE 4
//电机反向(可下发可上报)
//备注:用于设置电机转向，顺时针，或者逆时针，forward代表顺时针，back代表逆时�?
#define DPID_CONTROL_BACK 5
//全程运行时间(只上�?
//备注:用于APP面板中窗帘全程的时间，单位是MS，例如上�?000，就代表 5S，也就是APP面板中窗帘动态全程的时间�?
#define DPID_TIME_TOTAL 10
//故障告警(只上�?
//备注:【必选】设备上报故障值，该DP点枚举值可增减，但不可修改枚举值名称，例如可以删除motor_fault，可以增加其他值，未选则不展示该功能�?如需要修改在APP面板中显示的内容，可以前往第四步的拓展中心，多语言管理修改代表值，新增的枚举值所代表的的内容也是在多语言中修改代表值�?
#define DPID_FAULT 12
//电池电量(只上�?
//备注:
#define DPID_BATTERY_PERCENTAGE 13
//设置限位(可下发可上报)
//备注:
#define DPID_BORDER 16
//最佳位�?可下发可上报)
//备注:
#define DPID_POSITION_BEST 19
//指示灯设�?可下发可上报)
//备注:
#define DPID_LED_SETTING 101

#define DPID_AUTO_ON_OFF 0x65 //自动开关模�?
//定时任务(可下发可上报)
//备注:透传协议用于支持�?
//定时任务(单次/周期 动作:开/�?百分比控�?
//倒计�?倒数小时�?分钟�?动作:开/�?
#define DPID_TAIMED_TASK 0x66 
#define DPID_SILENT_MODE 0x69 //静音模式
/*校准
DPID_CURTAIN_MODE               窗帘模式
DPID_CALIBRATION_STATUS         校准状�?
DPID_CALIBRATION_OPERATION      校准操作
*/
#define DPID_CURTAIN_MODE 0x67
#define DPID_CALIBRATION_STATUS 0x68
#define DPID_CALIBRATION_OPERATION 0x6D 

#define DPID_FAULT_REMOVAL 0x70

//daojishi
#define DPID_COUNTDOWN_ONOFF 106
#define DPID_COUNTDOWN_TIME 107
#define DPID_COUNTDOWN_OP 108


extern u8 TimerDatagroup[96];
extern u16 TimerDataLength;
extern u8  CountdownGroup[4];
extern u16  CountdownLength;




//定时任务下发数据结构�?
typedef struct time{
  u8 timer_thours;        //下发的定时时间：小时
  u8 timer_tminutes;      //下发的定时时间：分钟
  u8 timer_ton_offs;      //下发的窗帘控制命�?0 or 1 0:on 1:off
  u8 timer_percent;       //下发的窗帘打开(关闭)百分�?
  u8 Effectiveness;       //下发的数据有效�?
	u8 timer_buf1[7];      //7个标志位，为1代表当天定时任务打开
}timer;
extern timer timer_task[8];


extern u8 Single_or_repeated_flag[8];

typedef struct Countdown{
  u8 gswitch;
  u8 flag;
  u32 updatatime;
  u32 localtime;
  
}countdown;
extern countdown count_oper;
//volatile extern unsigned char global_percent;//当前百分比，默认�?00%

/***倒计时开关，用于上报给涂鸦更新面板状�?***/
extern u8 Countdown_switch;

extern u8 Exchange_percentage;

extern bool OperIfvalue;



/*****************************************************************************
函数名称 : all_data_update
功能描述 : 系统所有dp点信息上�?
输入参数 : �?
返回参数 : �?
使用说明 : MCU必须实现该函数内数据上报功能
*****************************************************************************/
void all_data_update(void);

void all_data_update_fun(void);

/*****************************************************************************
函数名称 : dp_download_handle
功能描述 : dp下发处理函数
输入参数 : dpid:DP序号
value:dp数据缓冲区地址
length:dp数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERRO
使用说明 : 该函数用户不能修�?
*****************************************************************************/
unsigned char dp_download_handle(unsigned char dpid,const unsigned char value[], unsigned short length);
/*****************************************************************************
函数名称 : get_download_cmd_total
功能描述 : 获取所有dp命令总和
输入参数 : �?
返回参数 : 下发命令总和
使用说明 : 该函数用户不能修�?
*****************************************************************************/
unsigned char get_download_cmd_total(void);


//////////////////////////////////当前MCU SDK版本较上一版本新增支持协议接口////////////////////
/*****************************************************************************
函数名称 :  bt_unbound_req
功能描述 : 向模块发送解绑请求，模块收到该指令后会解除蓝牙绑定关�?
输入参数 : �?
返回参数 : �?
使用说明 : MCU主动解绑调用
*****************************************************************************/
void bt_unbound_req(void);
/*****************************************************************************
函数名称 :  bt_unbound_req
功能描述 : 向模块发射频测试请求
输入参数 : �?
返回参数 : �?
使用说明 : 
*****************************************************************************/
void bt_rf_test_req(void);
/*****************************************************************************
函数名称 : bt_rf_test_result
功能描述 : 蓝牙rf测试反馈
输入参数 : result:蓝牙rf测试结果;0:失败/1:成功
           rssi:测试成功表示蓝牙信号强度/测试失败值无意义
返回参数 : �?
使用说明 : MCU需要自行完善该函数功能
*****************************************************************************/
void bt_rf_test_result(unsigned char result,signed char rssi);
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
void bt_send_recordable_dp_data(unsigned char snedType,unsigned char dpid,unsigned char dpType, unsigned char value[],unsigned short len);
/*****************************************************************************
函数名称 : bt_send_recordable_dp_data_result
功能描述 : 记录型数据上�?
输入参数 : result �?存储成功�?存储失败
返回参数 : �?
使用说明 : MCU需要自行完善该函数功能
*****************************************************************************/
void bt_send_recordable_dp_data_result(unsigned char result);
/*****************************************************************************
函数名称 : bt_modify_adv_interval_req
功能描述 : 向模块发送修改低功耗下模块广播间隔的请�?
输入参数 :value * 100ms等于要修改的广播间隔，value�?-20�?
返回参数 : �?
使用说明 : MCU需要自行完善该函数功能
*****************************************************************************/
void bt_modify_adv_interval_req(unsigned char value);
/*****************************************************************************
函数名称 : bt_modify_adv_interval_result
功能描述 :向模块发送修改低功耗下模块广播间隔的结�?
输入参数 :result同步结果 0成功，其他失�?

返回参数 : �?
使用说明 : MCU需要自行完善该函数功能
*****************************************************************************/
void bt_modify_adv_interval_result(unsigned char result);
/*****************************************************************************
函数名称 : bt_close_timer_req
功能描述 : 向模块发送关闭系统时钟的请求（目前仅适用telink平台�?
输入参数 :value 0关闭�?打开
返回参数 : �?
使用说明 : MCU需要自行完善该函数功能
*****************************************************************************/
void bt_close_timer_req(unsigned char value);
/*****************************************************************************
函数名称 : bt_close_timer_result
功能描述 :向模块发送关闭系统时钟的结果
输入参数 :result同步结果 0成功，其他失�?

返回参数 : �?
使用说明 : MCU需要自行完善该函数功能
*****************************************************************************/
void bt_close_timer_result(unsigned char result);
/*****************************************************************************
函数名称 : bt_enable_lowpoer_req
功能描述 : 向模块发送使能低功耗的请求（目前仅适用telink平台�?
输入参数 :value 0关闭�?打开
返回参数 : �?
使用说明 :
*****************************************************************************/
void bt_enable_lowpoer_req(unsigned char value);
/*****************************************************************************
函数名称 : bt_enable_lowpoer_result
功能描述 :向模块发送使能低功耗的结果
输入参数 :result同步结果 0成功，其他失�?

返回参数 : �?
使用说明 : MCU需要自行完善该函数功能
*****************************************************************************/
void bt_enable_lowpoer_result(unsigned char result);
/*****************************************************************************
函数名称 : bt_send_one_time_password_token
功能描述 : 向模块发送获取一次性动态密码匹配结果的请求
输入参数 :value cmcu端输入的动态密码，len 8
返回参数 : �?
使用说明 :用于锁通用串口对接动态密码功�?
*****************************************************************************/
unsigned char bt_send_one_time_password_token(unsigned char value[],unsigned char len);
/*****************************************************************************
函数名称 : bt_send_one_time_password_token_result
功能描述 :向模块获取一次性动态密码匹配的结果
输入参数 :result同步结果 0x00密码核对通过�?x01密码核对失败

返回参数 : �?
使用说明 : MCU需要自行完善该函数功能
*****************************************************************************/
void bt_send_one_time_password_token_result(unsigned char result);
/*****************************************************************************
函数名称 : bt_enable_lowpoer_req
功能描述 : 向模块发送断开蓝牙连接的请�?
输入参数 :value 0关闭�?打开
返回参数 : �?
使用说明 :
*****************************************************************************/
void bt_disconnect_req(void);
/*****************************************************************************
函数名称 : bt_enable_lowpoer_result
功能描述 :向模块发送断开蓝牙连接的结�?
输入参数 :result结果 0成功，其他失�?

返回参数 : �?
使用说明 : MCU需要自行完善该函数功能
*****************************************************************************/
void bt_disconnect_result(unsigned char result);
/*****************************************************************************
函数名称 : bt_factor_reset_notify
功能描述 :模块恢复出厂设置后向mcu发送的通知
输入参数 :

返回参数 : �?
使用说明 : MCU可以在此处完成MCU恢复出厂设置的操�?
*****************************************************************************/
void bt_factor_reset_notify(void);

void bt_send_mcu_ver(void);

void bt_sendE8_mcu_ver(void);

void timer_curtain_control(u8 current_percent);

void tybn1_out_sleep_mode(void);

unsigned char dp_curtain_timer_task(const unsigned char value[], unsigned short length);

unsigned char dp_download_curtain_countdown_time(const unsigned char value[], unsigned short length);




#endif




