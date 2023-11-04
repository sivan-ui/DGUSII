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

	TF2 = 0; // �����ʱ��2���жϱ�־λ
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
// ���������ʱ,��λms
// ����޸����Ż��ȼ�,��ô�˺����ڲ��Ĳ�����Ҫ���µ���
/**
 * @brief ���������ʱ,��λms
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
	IEN0 = 0x00;	// IEN0.7=EA �ж��ܿ��� .6=0��.5=ET2��.4=ES0��.3=ET1��.2=EX1��.1=ET0��.0=EX0   ES0 ����2 ����+�����ж�
	IEN1 = 0x00;	// IEN1.7=0��.6=0��.5=����5�����ж�,.4=����5�����ж�,.3=����4�����ж�,.2=����4�����ж�,.1=can�ж�
	IEN2 = 0x00;	// .7-.1=0 , .0=ES1 ����3 ����+�����ж�
								//========================================MODE
	DPC = 0x00;		// DPTR �Ƿ�+1 C51=0
	CKCON = 0x00; // CPU����=0, 1Tģʽ

	IP0 = 0x00;
	IP1 = 0x00; // �ж����ȼ�Ĭ��
	RAMMODE = 0x00;
	//========================================IO��ʼ��
	P0MDOUT = 0x53; // p0.0 p0.1 ǿ�� ����485������� P0.4����������2 P0.6����������3
	P2MDOUT = 0xC0;
	P3MDOUT = 0x0C; // ����P3_2��P3_3����� P3_2=RTC_SCL P3_3=RTC_SDA
	PORTDRV = 0x01; // IO���������趨 4mA=0,8mA=1,16mA=2,32mA=3
	P2 = 0XC0;
	P3 = 0x00;
	WDT_OFF(); // �رտ��Ź�
	//========================================
	TH2 = 0x00;
	TL2 = 0x00;
	T2CON = 0x70;
	TRL2H = 0xBC; // 1ms�Ķ�ʱ��
	TRL2L = 0xCD;
	IEN0 |= 0x20; // ������ʱ��2
	TR2 = 0x01;
	ET2 = 1; // T2��ʱ���ж�ʹ�ܿ���λ
	EA = 1;
}

u16 read_dgus(u16 Dgus_Addr)
{
	u16 R_Dgus = 0;
	//			EA = 0;
	ADR_H = 0x00;
	ADR_M = (u8)((Dgus_Addr / 2) >> 8);
	ADR_L = (u8)(Dgus_Addr / 2);
	RAMMODE = 0xAF; // ������
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
	RAMMODE = 0xAF; // ������
	while (!APP_ACK)
		;
	APP_EN = 1;
	while (APP_EN)
		;
	RAMMODE = 0;
	ADR_M = (u8)((Dgus_Addr / 2) >> 8);
	ADR_L = (u8)(Dgus_Addr / 2);
	RAMMODE = 0x8F; // д����
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

// ��ӡ����
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
 * @brief �����������
 * @param {unsigned char} *t_arr ���Ŀ������
 * @param {unsigned char} element �������
 * @param {unsigned char} pos ���λ��
 * @return {*} 0:���ʧ�� 1:��ӳɹ�
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
 * @brief ɾ����������
 * @param {unsigned char} *t_arr ɾ��Ŀ������
 * @param {unsigned char} element ɾ������
 * @param {unsigned char} pos ɾ��λ��
 * @return {*} 0:ɾ��ʧ�� 1:ɾ���ɹ�
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
 * @brief �������
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
 * @brief ���������Ƿ����������
 * @param {unsigned char} *t_arr Ŀ������
 * @param {unsigned char} num_to_find ���������
 * @return {*}1:�������� 0:����������
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
 * @brief ���Ҵ��ڵ��ַ��滻Ϊָ�����ַ�
 * @param {char} *s �ַ���
 * @param {char} *old_str �滻���ַ�
 * @param {char} *new_str �µ��ַ�
 * @return {*}
 */
char *strrpl(char *s, const char *old_str, const char *new_str)
{
	char *ptr;
	while (ptr = strstr(s, old_str)) /* �����s���ҵ�s1 */
	{
		memmove(ptr + strlen(new_str), ptr + strlen(old_str), strlen(ptr) - strlen(old_str) + 1);
		memcpy(ptr, &new_str[0], strlen(new_str));
	}
	return s;
}
/**
 * @brief ������ȷ������
 * @param {unsigned short} year ������
 * @param {unsigned char} month ������
 * @return {*}��������
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
 * @brief �ж�����ֵ��֮ǰֵ�Ƿ�һ��
 * @param {unsigned short} var �����ֵ
 * @param {unsigned char} *pre_var ֮ǰ��ֵ
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
		return 1; // ������ֵ�Ѿ������仯
	}
	else
	{
		return 0; // ������ֵһֱû�з����仯
	}
}
