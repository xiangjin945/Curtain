/*
 * @Author: i4season.xiang
 * @Date: 2021-01-09 15:55:28
 * @LastEditTime: 2021-08-30 21:37:31
 * @FilePath: \v1.1.5-tuya_3.5\example\IAP\IAP_Text\OTA\SRC\mcu_transplant.c
 */
#include "head.h"
int uart_transmit_output_fun(unsigned char value)
{

	USART_SendData(HT_UART0, value);
	while (USART_GetFlagStatus(HT_UART0, USART_FLAG_TXC) == RESET)
		;
	ms_delay_systick(10);
	// USART_SendData(HT_USART0,value);
	// while (USART_GetFlagStatus(HT_USART0, USART_FLAG_TXC) == RESET);
	
	// if(value == 0x55)
	// {
	// 	//ms_delay_systick(100);
	// 	printf("\r\n");
	// }
	// printf("%x ", value);

}

// NOTE: 模块通讯初始化后，DP数据上报函数(预期: 收到模块状态下发指令 模块绑定已连接时，主动上报)
void all_data_update_fun(void)
{
	if(IsignReset == 1){//重置之后上报数据。
//		printf("----------------------------------------yyds\r\n");
		update_calibrstatus(0);
		I4TimerDataClear();
		I4CountdownDataClear();
		IsignReset = 0;
	}else{
		// printf("CountdownrData = %d\r\n",count_oper.time);
		printf("count_oper.updatatime = %d \r\n",count_oper.updatatime);

		
		mcu_dp_bool_update(DPID_COUNTDOWN_ONOFF, count_oper.gswitch);
		mcu_dp_bool_update(DPID_COUNTDOWN_OP, count_oper.flag);
		mcu_dp_value_update(DPID_COUNTDOWN_TIME, count_oper.updatatime);

		if(TimerDataLength != 0xffff){
			
			mcu_dp_raw_update(DPID_TAIMED_TASK, TimerDatagroup, TimerDataLength);
		}
	
	}
	mcu_dp_bool_update(DPID_AUTO_ON_OFF, variable.run_mode_flag);//08.18 每次上线上报的touch&go按键开关 1:打开 0:关闭
	
	mcu_dp_value_update(DPID_PERCENT_CONTROL, variable.global_percent);
	mcu_dp_value_update(DPID_PERCENT_UP_CONTROL, variable.global_percent);
	mcu_dp_enum_update(DPID_CALIBRATION_STATUS, variable.calibration_status);


	mcu_dp_enum_update(DPID_CONTROL, 1);
#if 0
	ms_delay_systick(1000);
	if(MotorRunStatus){
		if(curtain_mode){
			if(signPositive_inversion==1){
				update_current_runtime(5000); 								//1.Report runing time
				mcu_dp_enum_update(DPID_PERCENT_UP_CONTROL_STATUS, 0);				//2.Report operation status
				mcu_dp_value_update(DPID_PERCENT_CONTROL, variable.global_percent);			//3.Reported target percentage accepted
				mcu_dp_value_update(DPID_PERCENT_UP_CONTROL, variable.global_percent);//4.Report current actual location
				mcu_dp_value_update(DPID_PERCENT_UP_CONTROL_STATUS, 2); 			  //5.Report the end running status
			}
			if(signPositive_inversion==2){
				update_current_runtime(5000); 								//1.Report runing time
				mcu_dp_enum_update(DPID_PERCENT_UP_CONTROL_STATUS, 1);				//2.Report operation status
				mcu_dp_value_update(DPID_PERCENT_CONTROL, variable.global_percent);			//3.Reported target percentage accepted
				mcu_dp_value_update(DPID_PERCENT_UP_CONTROL, variable.global_percent);//4.Report current actual location
				mcu_dp_value_update(DPID_PERCENT_UP_CONTROL_STATUS, 2); 			  //5.Report the end running status

			}
		}else{
			if(signPositive_inversion==1){
				update_current_runtime(5000); 								//1.Report runing time
				mcu_dp_enum_update(DPID_PERCENT_UP_CONTROL_STATUS, 0);				//2.Report operation status
				mcu_dp_value_update(DPID_PERCENT_CONTROL, variable.global_percent);			//3.Reported target percentage accepted
				mcu_dp_value_update(DPID_PERCENT_UP_CONTROL, variable.global_percent);//4.Report current actual location
				mcu_dp_value_update(DPID_PERCENT_UP_CONTROL_STATUS, 2); 			  //5.Report the end running status
			}
			if(signPositive_inversion==2){
				update_current_runtime(5000); 								//1.Report runing time
				mcu_dp_enum_update(DPID_PERCENT_UP_CONTROL_STATUS, 1);				//2.Report operation status
				mcu_dp_value_update(DPID_PERCENT_CONTROL, variable.global_percent);			//3.Reported target percentage accepted
				mcu_dp_value_update(DPID_PERCENT_UP_CONTROL, variable.global_percent);//4.Report current actual location
				mcu_dp_value_update(DPID_PERCENT_UP_CONTROL_STATUS, 2); 			  //5.Report the end running status
			}
		}
	}

#endif
}
