/*
 * @Author: xw.qu
 * @Date: 2023-08-31 09:22:42
 * @LastEditors: xw.qu
 * @LastEditTime: 2023-10-27 09:45:12
 * @FilePath: \USER\sys.c
 * @Description: about system function
 *
 * Copyright (c) 2023 by xw.qu, All Rights Reserved.
 */
#include "Sys.h"
unsigned char pic_page = 0;
u16 xdata TimVal = 0;
u16 xdata Count_num1 = 0;
u16 xdata Count_for_tx = 0;
u32 xdata enter_code_time_cnt = 120000u;
u16 xdata SysTick_RTC = 0;
bit RTC_Flog = 0;
u16 xdata time_tick = 0;
u16 xdata re_send_time = 0;
void Sys_Timer2_isr() interrupt 5 // 1MS
{

	TF2 = 0; // 清除定时器2的中断标志位
	if (TimVal < 255)
		TimVal++;
	time_tick++;
	//	retransmission_time++;
	SysTick_RTC++;
	if (T_O5 > 0)
		T_O5--;
	if (T_O4 > 0)
		T_O4--;
	if (T_O3 > 0)
		T_O3--;
	if (T_O2 > 0)
		T_O2--;
	if (Count_num1 > 0)
		Count_num1--;
	if (Count_for_tx > 0)
		Count_for_tx--;
	if (enter_code_time_cnt < 120000u)
		enter_code_time_cnt++;
	if (mbHost.scene_send_cyc_time > 0)
		mbHost.scene_send_cyc_time--;
	if (time_scene_var.cyc_send_time > 0)
		time_scene_var.cyc_send_time--;
	if (time_scene_var.send_delay_time > 0)
		time_scene_var.send_delay_time--;
	if (g_var_module.batch_send_cyc_time > 0)
		g_var_module.batch_send_cyc_time--;
	//	if(g_var_module.batch_relay_send_cyc_time>0) g_var_module.batch_relay_send_cyc_time--;
	if (g_var_module.auto_retrieve_cyc_time > 0)
		g_var_module.auto_retrieve_cyc_time--;
	if (scene_data.delete_scenc_send_cyc_time > 0)
		scene_data.delete_scenc_send_cyc_time--;

	mbh_timer3T5Isr();
	mbh_resend_time_run();
}
// 软件大致延时,单位ms
// 如果修改了优化等级,那么此函数内部的参数需要重新调试
/**
 * @brief 软件大致延时,单位ms
 * @param {u16} ms
 * @return {*}
 */
void sys_delay_about_ms(u16 ms)
{
	u16 i, j;
	for (i = 0; i < ms; i++)
		for (j = 0; j < 3000; j++)
			;
}
//=====================================
void Sys_Cpu_Init()
{
	IEN0 = 0x00;	// IEN0.7=EA 中断总控制 .6=0，.5=ET2，.4=ES0，.3=ET1，.2=EX1，.1=ET0，.0=EX0   ES0 串口2 接受+发送中断
	IEN1 = 0x00;	// IEN1.7=0，.6=0，.5=串口5接受中断,.4=串口5发送中断,.3=串口4接受中断,.2=串口4发送中断,.1=can中断
	IEN2 = 0x00;	// .7-.1=0 , .0=ES1 串口3 接受+发送中断
								//========================================MODE
	DPC = 0x00;		// DPTR 是否+1 C51=0
	CKCON = 0x00; // CPU运行=0, 1T模式

	IP0 = 0x00;
	IP1 = 0x00; // 中断优先级默认
	RAMMODE = 0x00;
	//========================================IO初始化
	P0MDOUT = 0x53; // p0.0 p0.1 强推 用于485方向控制 P0.4引出到串口2 P0.6引出到串口3
	P2MDOUT = 0xC0;
	P3MDOUT = 0x0C; // 开启P3_2和P3_3的输出 P3_2=RTC_SCL P3_3=RTC_SDA
	PORTDRV = 0x01; // IO驱动能力设定 4mA=0,8mA=1,16mA=2,32mA=3
	P2 = 0XC0;
	P3 = 0x00;
	WDT_OFF(); // 关闭开门狗
	//========================================
	TH2 = 0x00;
	TL2 = 0x00;
	T2CON = 0x70;
	TRL2H = 0xBC; // 1ms的定时器
	TRL2L = 0xCD;
	IEN0 |= 0x20; // 启动定时器2
	TR2 = 0x01;
	ET2 = 1; // T2定时器中断使能控制位
	EA = 1;
}

u16 read_dgus(u16 Dgus_Addr)
{
	u16 R_Dgus = 0;
	//			EA = 0;
	ADR_H = 0x00;
	ADR_M = (u8)((Dgus_Addr / 2) >> 8);
	ADR_L = (u8)(Dgus_Addr / 2);
	RAMMODE = 0xAF; // 读操作
	// while(!APP_ACK);	//test LKC
	APP_EN = 1;
	// while(APP_EN);	//test LKC
	if (Dgus_Addr % 2)
		R_Dgus = (DATA1 << 8) + DATA0;
	else
		R_Dgus = (DATA3 << 8) + DATA2;
	RAMMODE = 0;
	//			EA = 1;
	return R_Dgus;
}
/****************************************************************************/
void write_dgus(u16 Dgus_Addr, u16 Val)
{
	//    	EA = 0;
	ADR_H = 0x00;
	ADR_M = (u8)((Dgus_Addr / 2) >> 8);
	ADR_L = (u8)(Dgus_Addr / 2);
	RAMMODE = 0xAF; // 读操作
	while (!APP_ACK)
		;
	APP_EN = 1;
	while (APP_EN)
		;
	RAMMODE = 0;
	ADR_M = (u8)((Dgus_Addr / 2) >> 8);
	ADR_L = (u8)(Dgus_Addr / 2);
	RAMMODE = 0x8F; // 写操作
	while (!APP_ACK)
		;
	if (Dgus_Addr % 2)
	{
		DATA1 = (u8)(Val >> 8);
		DATA0 = (u8)(Val);
	}
	else
	{
		DATA3 = (u8)(Val >> 8);
		DATA2 = (u8)(Val);
	}
	APP_EN = 1;
	while (APP_EN)
		;
	RAMMODE = 0;
	//			EA = 1;
}

// 打印数组
void printf_tab(unsigned short nb, unsigned char *p)
{
	unsigned short i = 0;
	for (i = 0; i < nb; i++)
	{
		USER_PRINTF(" %bx ", p[i]);
	}
	USER_PRINTF("\n");
}
/**
 * @brief 添加数组数据
 * @param {unsigned char} *t_arr 添加目标数组
 * @param {unsigned char} element 添加数据
 * @param {unsigned char} pos 添加位置
 * @return {*} 0:添加失败 1:添加成功
 */
unsigned char add_arr_data(unsigned char *t_arr, unsigned char element, unsigned char pos)
{
	if (pos > MAX_DEVICE_ADR)
	{
		USER_PRINTF("-->arr is oversize!\n");
		return 0;
	}
	else
	{
		*(t_arr + pos) = element;
		return 1;
	}
}
/**
 * @brief 删除数组数据
 * @param {unsigned char} *t_arr 删除目标数组
 * @param {unsigned char} element 删除数据
 * @param {unsigned char} pos 删除位置
 * @return {*} 0:删除失败 1:删除成功
 */
unsigned char delete_arr_data(unsigned char *t_arr, unsigned char pos)
{
	if (pos > MAX_DEVICE_ADR)
	{
		return 0;
	}
	else
	{
		*(t_arr + pos) = 0;
		return 1;
	}
}
/**
 * @brief 清空数组
 * @param {unsigned char} *t_arr
 * @return {*}
 */
void clear_arr_data(unsigned char *t_arr,unsigned char size)
{
	unsigned char i = 0;
	for(i = 0;i<size;i++)
	{
		delete_arr_data(t_arr,i);
	}
}
/**
 * @brief 查找数据是否存在数组中
 * @param {unsigned char} *t_arr 目标数组
 * @param {unsigned char} num_to_find 需查找数据
 * @return {*}1:存在数据 0:不存在数据
 */
unsigned char find_nub_index(unsigned char *t_arr, unsigned char num_to_find)
{
	unsigned char i = 0;
	for (i = 0; i < MAX_DEVICE_ADR; i++)
	{
		if (t_arr[i] == num_to_find)
		{
			return 1;
		}
	}
	return 0;
}

/**
 * @brief 查找存在的字符替换为指定的字符
 * @param {char} *s 字符串
 * @param {char} *old_str 替换的字符
 * @param {char} *new_str 新的字符
 * @return {*}
 */
char *strrpl(char *s, const char *old_str, const char *new_str)
{
	char *ptr;
	while (ptr = strstr(s, old_str)) /* 如果在s中找到s1 */
	{
		memmove(ptr + strlen(new_str), ptr + strlen(old_str), strlen(ptr) - strlen(old_str) + 1);
		memcpy(ptr, &new_str[0], strlen(new_str));
	}
	return s;
}
/**
 * @brief 返回正确的天数
 * @param {unsigned short} year 输入年
 * @param {unsigned char} month 输入月
 * @return {*}当月天数
 */
unsigned char get_days_in_month(unsigned short year, unsigned char month)
{
	unsigned char days = 0;
	switch (month)
	{
	case 1:
	case 3:
	case 5:
	case 7:
	case 8:
	case 10:
	case 12:
		days = 31;
		break;
	case 4:
	case 6:
	case 9:
	case 11:
		days = 30;
		break;
	case 2:
		if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
		{
			days = 29;
		}
		else
		{
			days = 28;
		}
		break;
	default:
		USER_PRINTF("invalid month!\n");
		return -1;
	}
	return days;
}
/**
 * @brief 判断输入值和之前值是否一致
 * @param {unsigned short} var 输入的值
 * @param {unsigned char} *pre_var 之前的值
 * @return {*}
 */
unsigned char check_constant(unsigned short var,unsigned char *pre_var)
{
	// static unsigned short previous_value = 0;
	// USER_PRINTF("--new var is %bd\n", var);
	// USER_PRINTF("--pre var is %bd\n", previous_value);
	if (var != *pre_var)
	{
		*pre_var = var;
		return 1; // 变量的值已经发生变化
	}
	else
	{
		return 0; // 变量的值一直没有发生变化
	}
}
