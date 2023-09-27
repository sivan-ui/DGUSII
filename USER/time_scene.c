/*
 * @Author: xw.qu
 * @Date: 2023-08-31 09:22:43
 * @LastEditors: xw.qu
 * @LastEditTime: 2023-09-26 15:30:39
 * @FilePath: \USER\time_scene.c
 * @Description: module time scene modify
 *
 * Copyright (c) 2023 by xw.qu, All Rights Reserved.
 */
#include "time_scene.h"
#include "sys.h"
#include "T5LLIB.h"
timing_content_t timing_content[TIMING_SCENE_NUB_SIGNLE] = {0};
timing_content_t temp_timing_content = {0};
// timing_content_t save_timing_content = {0};
time_scene_var_t time_scene_var;
// time_module_scene_set_t temp_time_module_scene_set;
// time_scene_set_t  temp_time_scene_set;
unsigned short time_scene_key_nb_bak = 0;
unsigned char time_scene_select_sta_bak = 0;
// 打印定时内容数组
void time_scene_init(void)
{
	//	memset((unsigned char *)&save_timing_content,1,sizeof(timing_content_t));
	//	time_scene_var.adr = TIMING_SCENE_INFOR_FLASH_ADR_SATRT;
	//	USER_PRINTF("%ld\n",time_scene_var.adr);
}
void printf_temp_timing_content(timing_content_t *p_timing_content)
{
	//	unsigned char i = 0;
	//	for( i = 0;i<index;i++)
	//	{
	USER_PRINTF("timing_content[%bd].time_scene_set.data_sta is %bd\n", p_timing_content->time_scene_set.index, p_timing_content->time_scene_set.data_sta);

	//	}
}
void printf_timing_content(unsigned char index)
{
	unsigned char i = 0;
	for (i = 0; i < index; i++)
	{
		USER_PRINTF("timing_content[%bd].time_scene_set.index is%bd, data_sta is %bd,week_enable is %bx,led_enable is %d,led_sta is %d\n",
								i, timing_content[i].time_scene_set.index, timing_content[i].time_scene_set.data_sta, timing_content[i].time_scene_set.week_enable,
								timing_content[i].time_scene_set.time_module_scene_set.led_enable, timing_content[i].time_scene_set.time_module_scene_set.led_sta);
	}
}
// 清除变量
void clear_timing_scene_var(void)
{
	unsigned char i = 0;
	for (i = 8; i < 11; i++)
	{
		write_dgus(0x108a + i, 0);
	}
	write_dgus(0x1098, 0);
	write_dgus(0x1099, 0);
	time_scene_var.module_select_sta = 0;
	time_scene_var.all_light_select_sta = 0;
	time_scene_var.scene_select_sta = 0;
	time_scene_var.all_light_on_sta = 0;
	time_scene_var.all_light_off_sta = 0;
	//	time_scene_var.page_bak = 0;
	//	memset(&timing_module,0,sizeof(timing_module));
}
// 清除单个定时内容变量
void clear_single_timing_scene_content(timing_content_t *p_timing_content)
{
	memset(p_timing_content, 0, sizeof(timing_content_t));
}
// 清除所有定时内容变量
void clear_all_timing_scene_content(timing_content_t *p_timing_content)
{
	unsigned char i = 0;
	for (i = 0; i < TIMING_SCENE_NUB_SIGNLE; i++)
	{
		clear_single_timing_scene_content(p_timing_content + i);
	}
}
unsigned char get_blank_timing_scene_infor_index(void)
{
	// return find_index(SCENE_INFOR_FLASH_ADR_SATRT,SCENC_INFOR_T_SIZE,SCENE_NUB);
	unsigned char i = 0;
	//	unsigned short temp_data = 0;
	for (i = 0; i < TIMING_SCENE_NUB_SIGNLE; i++)
	{
		if ((0 == timing_content[i].time_scene_set.data_sta))
		{
			USER_PRINTF("-->blank_timing_content_index is %bd\n", i);
			return i;
		}
	}
	return 0xff;
}
// 获取定时场景名称选中序号
unsigned char get_timing_scene_name_select_sequence_number(void)
{
	// return get_selected_sequence_number(0x10bb,16);
	return get_box_select_number(0x1500, 128);
}
// 设定定时场景名称选中序号
void set_timing_scene_name_select_sequence_number(void)
{
	// set_selected_sequence_number(0x10ba,0x10bb,16);
	set_box_select(0x10ba, 0x1500, 128, &time_scene_key_nb_bak, &time_scene_select_sta_bak);
}
// 获取日期使能
unsigned char get_timing_data_enable(void)
{
	return read_dgus(0x1091);
}
// 设置日期使能
void set_timing_data_enable(time_scene_set_t *p_time_scene_set)
{
	USER_PRINTF("-->data enable is %bd\n", p_time_scene_set->date_enable);
	write_dgus(0x1091, p_time_scene_set->date_enable);
}
// 获取星期使能 108a
unsigned char get_timing_week_enable(unsigned short week_enable_adr)
{

	return (read_dgus(week_enable_adr) | (read_dgus(week_enable_adr + 1) << 1) | (read_dgus(week_enable_adr + 2) << 2) |
					(read_dgus(week_enable_adr + 3) << 3) | (read_dgus(week_enable_adr + 4) << 4) | (read_dgus(week_enable_adr + 5) << 5) | (read_dgus(week_enable_adr + 6) << 6));
}
// 设置星期使能
void set_timing_week_enable(time_scene_set_t *p_time_scene_set)
{
	display_week_parameter(0x108a, p_time_scene_set->week_enable);
	//	unsigned char i = 0;
	//	USER_PRINTF("-->week enable is %bd\n",p_time_scene_set->week_enable);
	//	for( i = 0;i<7;i++)
	//	{
	//		write_dgus(0x108a+i,(p_time_scene_set->week_enable>>i)&0x01);
	//	}

	//	return (read_dgus(0x108a)|(read_dgus(0x108b)<<1)|(read_dgus(0x108c)<<2)|(read_dgus(0x108d)<<3)|(read_dgus(0x108e)<<4)|(read_dgus(0x108f)<<5)|(read_dgus(0x1090)<<6));;
}
// 得到定时的信息
void get_timing_set_infor(date_t *p_s_date, date_t *p_e_data, time_t *p_time, time_scene_set_t *p_time_scene_set)
{

	p_time_scene_set->date_enable = get_timing_data_enable();
	p_time_scene_set->week_enable = get_timing_week_enable(0x108a);
	p_time->hour = read_dgus(0x2007);
	p_time->min = read_dgus(0x2008);
	p_time->sec = 0;
	if (1 == p_time_scene_set->date_enable)
	{
		p_s_date->year = read_dgus(TIMING_SCENE_START_YEAR);
		p_s_date->month = read_dgus(TIMING_SCENE_START_MONTH);
		p_s_date->day = read_dgus(TIMING_SCENE_START_DAY);

		p_e_data->year = read_dgus(TIMING_SCENE_END_YEAR);
		p_e_data->month = read_dgus(TIMING_SCENE_END_MONTH);
		p_e_data->day = read_dgus(TIMING_SCENE_END_DAY);
	}
	// USER_PRINTF("-->data enable is %bd\n", p_time_scene_set->date_enable);
	// USER_PRINTF("-->week enable is %bd\n", p_time_scene_set->week_enable);
	// USER_PRINTF("-->execute time is %bd:%bd:%bd\n", p_time->hour, p_time->min, p_time->sec);
	// USER_PRINTF("-->execute start date is %d-%bd-%bd\n", p_s_date->year, p_s_date->month, p_s_date->day);
	// USER_PRINTF("-->execute end date is %d-%bd-%bd\n", p_e_data->year, p_e_data->month, p_e_data->day);
}
// unsigned char *get_scene_name(unsigned int adr,unsigned char index)
//{
//	unsigned char  scene_name[17] = {0};
//	read_dgusii_vp(adr+(index-1)*8,(unsigned char *)scene_name,8);
//	return scene_name;
// }
// 模块场景信息显示
void dis_module_scene_infor(time_module_scene_set_t *p_time_module_scene_set)
{
	unsigned char i = 0;
	unsigned char temp_tab[32] = {0};
	switch (p_time_module_scene_set->type)
	{
	case SINGLE_RELAY_MODULE:
	case SINGLE_TIME_RELAY_MODULE:
	case DOUBLE_RELAY_MODULE:
	case DOUBLE_TIME_RELAY_MODULE:
		for (i = 0; i < 16; i++)
		{
			temp_tab[2 * i + 1] = (p_time_module_scene_set->led_enable >> i) & 0x1;
			;
		}
		write_dgusii_vp(0x109a, (unsigned char *)&temp_tab, 16);
		for (i = 0; i < 16; i++)
		{
			temp_tab[2 * i + 1] = (p_time_module_scene_set->led_sta >> i) & 0x1;
			;
		}
		write_dgusii_vp(0x10aa, (unsigned char *)&temp_tab, 16);

		break;
	case VOL_DIM_MODULE:
		for (i = 0; i < 4; i++)
		{
			temp_tab[2 * i + 1] = p_time_module_scene_set->diming_enable >> i & 0x1;
		}
		write_dgusii_vp(0x10cd, (unsigned char *)&temp_tab, 4);
		write_dgusii_vp(0x10d1, (unsigned char *)&(p_time_module_scene_set->dimming_duty[0]), 4);

		break;
	case SCR_DIM_MODULE:
		for (i = 0; i < 4; i++)
		{
			temp_tab[2 * i + 1] = p_time_module_scene_set->diming_enable >> i & 0x1;
		}
		write_dgusii_vp(0x10d5, (unsigned char *)&temp_tab, 4);

		write_dgusii_vp(0x10d9, (unsigned char *)&(p_time_module_scene_set->dimming_duty[0]), 4);

		break;
	}
}
// 设置定时信息
void set_timing_set_infor(date_t *p_s_date, date_t *p_e_data, time_t *p_time, time_scene_set_t *p_time_scene_set)
{
	set_timing_data_enable(p_time_scene_set);
	set_timing_week_enable(p_time_scene_set);
	//	set_timing_set_infor(p_s_date,p_e_data,p_time,p_time_scene_set);
	write_dgus(0x2007, p_time->hour);
	write_dgus(0x2008, p_time->min);

	if (1 == p_time_scene_set->date_enable)
	{
		write_dgus(TIMING_SCENE_START_YEAR, p_s_date->year);
		write_dgus(TIMING_SCENE_START_MONTH, p_s_date->month);
		write_dgus(TIMING_SCENE_START_DAY, p_s_date->day);

		write_dgus(TIMING_SCENE_END_YEAR, p_e_data->year);
		write_dgus(TIMING_SCENE_END_MONTH, p_e_data->month);
		write_dgus(TIMING_SCENE_END_DAY, p_e_data->day);
	}
}

void timing_scene_infor_save_flash(timing_content_t *p_timing_content)
{
	// USER_PRINTF("--> save_flash_index is %bd\n", p_timing_content->time_scene_set.index);
	norflash_write(TIMING_SCENE_INFOR_FLASH_ADR_SATRT + (p_timing_content->time_scene_set.index) * SINGLE_TIMING_SCENC_INFOR_T_SIZE, (unsigned char *)p_timing_content, SINGLE_TIMING_SCENC_INFOR_T_SIZE);
	//	printf_temp_timing_content(&save_timing_content);
	//	norflash_write(abs(TIMING_SCENE_INFOR_FLASH_ADR_SATRT),(unsigned char *)(&save_timing_content),SINGLE_TIMING_SCENC_INFOR_T_SIZE);
	USER_PRINTF("--> timing scene flash store ok\n");
}
// 清空定时场景信息内容
void timing_scene_infor_clear(timing_content_t *p_timing_content)
{
	T5L_Flash(0xA5, 0Xe000, TIMING_SCENE_INFOR_FLASH_ADR_SATRT, ALL_TIMING_SCENC_INFOR_T_SIZE); // 写0FLASH清除
	T5L_Flash(0x5A, 0X6180, TIMING_SCENE_INFOR_FLASH_ADR_SATRT, 0x2300);													// 读为0场景信息

	clear_all_timing_scene_content(p_timing_content);
}
// 删除定时单个信息内容
void timing_scene_infor_delete(timing_content_t *p_timing_content, time_scene_set_t *p_time_scene)
{
	// USER_PRINTF("flash delet index is %bd\n", p_time_scene->index);
	T5L_Flash(0xA5, 0Xe000, TIMING_SCENE_INFOR_FLASH_ADR_SATRT + (p_time_scene->index) * SINGLE_TIMING_SCENC_INFOR_T_SIZE, SINGLE_TIMING_SCENC_INFOR_T_SIZE); // 写0FLASH清除
	T5L_Flash(0x5A, 0X6180 + (p_time_scene->index) * 80, TIMING_SCENE_INFOR_FLASH_ADR_SATRT, 0x50);																															// 读为0场景信息
	clear_single_timing_scene_content(p_timing_content);
}
// 继电器模块设置定时场景
void timing_module_relay_scene_set(module_t *p, time_module_scene_set_t *p_time_module_scene)
{
	//	unsigned short scene_led_enable = 0,scene_led_sta = 0;
	//	unsigned short led_hr_index_start = 0;
	unsigned char temp_tab[32] = {0};
	if ((SINGLE_RELAY_MODULE == p->type) || (SINGLE_TIME_RELAY_MODULE == p->type))
	{
		p_time_module_scene->led_hr_index_start = 337;
	}
	else if ((DOUBLE_RELAY_MODULE == p->type) || (DOUBLE_TIME_RELAY_MODULE == p->type))
	{
		p_time_module_scene->led_hr_index_start = 400;
	}
	else
	{
		return;
	}

	p_time_module_scene->type = p->type;
	p_time_module_scene->adr = p->adr;
	p_time_module_scene->channel_nb = p->channel_nb;
	p_time_module_scene->module_index = p->index;

	switch (p_time_module_scene->channel_nb)
	{
	case 4:
		read_dgusii_vp(0x109a, (unsigned char *)temp_tab, 16);
		p_time_module_scene->led_enable = temp_tab[1] | temp_tab[3] << 1 | temp_tab[5] << 2 | temp_tab[7] << 3;
		// USER_PRINTF("-->led_enable is -->0x%x\n", p_time_module_scene->led_enable);
		read_dgusii_vp(0x10aa, (unsigned char *)temp_tab, 16);
		p_time_module_scene->led_sta = temp_tab[1] | temp_tab[3] << 1 | temp_tab[5] << 2 | temp_tab[7] << 3;
		break;
	case 6:
		read_dgusii_vp(0x109a, (unsigned char *)temp_tab, 16);
		p_time_module_scene->led_enable = temp_tab[1] | temp_tab[3] << 1 | temp_tab[5] << 2 | temp_tab[7] << 3 | temp_tab[9] << 4 | temp_tab[11] << 5;
		// USER_PRINTF("-->led_enable is -->0x%x\n", p_time_module_scene->led_enable);
		read_dgusii_vp(0x10aa, (unsigned char *)temp_tab, 16);
		p_time_module_scene->led_sta = temp_tab[1] | temp_tab[3] << 1 | temp_tab[5] << 2 | temp_tab[7] << 3 | temp_tab[9] << 4 | temp_tab[11] << 5;
		break;
	case 8:
		read_dgusii_vp(0x109a, (unsigned char *)temp_tab, 16);
		p_time_module_scene->led_enable = temp_tab[1] | temp_tab[3] << 1 | temp_tab[5] << 2 | temp_tab[7] << 3 | temp_tab[9] << 4 | temp_tab[11] << 5 | temp_tab[13] << 6 | temp_tab[15] << 7;
		// USER_PRINTF("-->led_enable is -->0x%x\n", p_time_module_scene->led_enable);
		read_dgusii_vp(0x10aa, (unsigned char *)temp_tab, 16);
		p_time_module_scene->led_sta = temp_tab[1] | temp_tab[3] << 1 | temp_tab[5] << 2 | temp_tab[7] << 3 | temp_tab[9] << 4 | temp_tab[11] << 5 | temp_tab[13] << 6 | temp_tab[15] << 7;
		break;
	case 10:
		read_dgusii_vp(0x109a, (unsigned char *)temp_tab, 16);
		p_time_module_scene->led_enable = temp_tab[1] | temp_tab[3] << 1 | temp_tab[5] << 2 | temp_tab[7] << 3 | temp_tab[9] << 4 | temp_tab[11] << 5 | temp_tab[13] << 6 | temp_tab[15] << 7 |
																			temp_tab[17] << 8 | temp_tab[19] << 9;
		// USER_PRINTF("-->led_enable is -->0x%x\n", p_time_module_scene->led_enable);
		read_dgusii_vp(0x10aa, (unsigned char *)temp_tab, 16);
		p_time_module_scene->led_sta = temp_tab[1] | temp_tab[3] << 1 | temp_tab[5] << 2 | temp_tab[7] << 3 | temp_tab[9] << 4 | temp_tab[11] << 5 | temp_tab[13] << 6 | temp_tab[15] << 7 |
																	 temp_tab[17] << 8 | temp_tab[19] << 9;
	case 12:
		read_dgusii_vp(0x109a, (unsigned char *)temp_tab, 16);
		p_time_module_scene->led_enable = temp_tab[1] | temp_tab[3] << 1 | temp_tab[5] << 2 | temp_tab[7] << 3 | temp_tab[9] << 4 | temp_tab[11] << 5 | temp_tab[13] << 6 | temp_tab[15] << 7 |
																			temp_tab[17] << 8 | temp_tab[19] << 9 | temp_tab[21] << 10 | temp_tab[23] << 11;
		// USER_PRINTF("-->led_enable is -->0x%x\n", p_time_module_scene->led_enable);
		read_dgusii_vp(0x10aa, (unsigned char *)temp_tab, 16);
		p_time_module_scene->led_sta = temp_tab[1] | temp_tab[3] << 1 | temp_tab[5] << 2 | temp_tab[7] << 3 | temp_tab[9] << 4 | temp_tab[11] << 5 | temp_tab[13] << 6 | temp_tab[15] << 7 |
																	 temp_tab[17] << 8 | temp_tab[19] << 9 | temp_tab[21] << 10 | temp_tab[23] << 11;
		break;
	case 14:
		read_dgusii_vp(0x109a, (unsigned char *)temp_tab, 16);
		p_time_module_scene->led_enable = temp_tab[1] | temp_tab[3] << 1 | temp_tab[5] << 2 | temp_tab[7] << 3 | temp_tab[9] << 4 | temp_tab[11] << 5 | temp_tab[13] << 6 | temp_tab[15] << 7 |
																			temp_tab[17] << 8 | temp_tab[19] << 9 | temp_tab[21] << 10 | temp_tab[23] << 11 | temp_tab[25] << 12 | temp_tab[27] << 13;
		// USER_PRINTF("-->led_enable is -->0x%x\n", p_time_module_scene->led_enable);
		read_dgusii_vp(0x10aa, (unsigned char *)temp_tab, 16);
		p_time_module_scene->led_sta = temp_tab[1] | temp_tab[3] << 1 | temp_tab[5] << 2 | temp_tab[7] << 3 | temp_tab[9] << 4 | temp_tab[11] << 5 | temp_tab[13] << 6 | temp_tab[15] << 7 |
																	 temp_tab[17] << 8 | temp_tab[19] << 9 | temp_tab[21] << 10 | temp_tab[23] << 11 | temp_tab[25] << 12 | temp_tab[27] << 13;
	case 16:
		read_dgusii_vp(0x109a, (unsigned char *)temp_tab, 16);
		p_time_module_scene->led_enable = temp_tab[1] | temp_tab[3] << 1 | temp_tab[5] << 2 | temp_tab[7] << 3 | temp_tab[9] << 4 | temp_tab[11] << 5 | temp_tab[13] << 6 | temp_tab[15] << 7 |
																			temp_tab[17] << 8 | temp_tab[19] << 9 | temp_tab[21] << 10 | temp_tab[23] << 11 | temp_tab[25] << 12 | temp_tab[27] << 13 | temp_tab[29] << 14 | temp_tab[31] << 15;
		// USER_PRINTF("-->led_enable is -->0x%x\n", p_time_module_scene->led_enable);
		read_dgusii_vp(0x10aa, (unsigned char *)temp_tab, 16);
		p_time_module_scene->led_sta = temp_tab[1] | temp_tab[3] << 1 | temp_tab[5] << 2 | temp_tab[7] << 3 | temp_tab[9] << 4 | temp_tab[11] << 5 | temp_tab[13] << 6 | temp_tab[15] << 7 |
																	 temp_tab[17] << 8 | temp_tab[19] << 9 | temp_tab[21] << 10 | temp_tab[23] << 11 | temp_tab[25] << 12 | temp_tab[27] << 13 | temp_tab[29] << 14 | temp_tab[31] << 15;
		break;
	}

	// USER_PRINTF("-->led_sta is -->0x%x\n", p_time_module_scene->led_sta);
	USER_PRINTF("-->module_relay_scene_set\n");
}

// 设置场景调光模块信息
void timing_module_dim_scene_set(module_t *p, time_module_scene_set_t *p_time_module_scene)
{
	unsigned short scene_dim_enable = 0;
	unsigned short scene_dim_enable_adr = 0, scene_dim_gear_adr = 0;
	unsigned char temp_tab[8] = {0}, dim_tab[4] = {0};
	if ((VOL_DIM_MODULE == p->type))
	{
		scene_dim_enable_adr = 0x10cd;
		scene_dim_gear_adr = 0x10d1;
	}
	else if ((SCR_DIM_MODULE == p->type))
	{
		scene_dim_enable_adr = 0x10d5;
		scene_dim_gear_adr = 0x10d9;
	}
	else
	{
		return;
	}
	p_time_module_scene->type = p->type;
	p_time_module_scene->adr = p->adr;
	p_time_module_scene->channel_nb = p->channel_nb;
	p_time_module_scene->module_index = p->index;

	read_dgusii_vp(scene_dim_enable_adr, (unsigned char *)temp_tab, 4);
	p_time_module_scene->diming_enable = scene_dim_enable = temp_tab[1] | temp_tab[3] << 1 | temp_tab[5] << 2 | temp_tab[7] << 3;
	// USER_PRINTF("-->diming_enable is--->%x\n", scene_dim_enable);
	read_dgusii_vp(scene_dim_gear_adr, (unsigned char *)temp_tab, 4);
	p_time_module_scene->dimming_duty[0] = dim_tab[0] = temp_tab[1];
	p_time_module_scene->dimming_duty[1] = dim_tab[1] = temp_tab[3];
	p_time_module_scene->dimming_duty[2] = dim_tab[2] = temp_tab[5];
	p_time_module_scene->dimming_duty[3] = dim_tab[3] = temp_tab[7];
	// USER_PRINTF("-->dimming_duty is");
	// printf_tab(4, dim_tab);
}
// 继电器模块定时发送场景
void timing_module_relay_scene_send(time_module_scene_set_t *p_time_module_scene)
{
	if ((SINGLE_RELAY_MODULE == p_time_module_scene->type) || (SINGLE_TIME_RELAY_MODULE == p_time_module_scene->type) || (DOUBLE_RELAY_MODULE == p_time_module_scene->type) || (DOUBLE_TIME_RELAY_MODULE == p_time_module_scene->type))
	{
		set_master_send_hr(master_send_hr, 31, p_time_module_scene->led_enable);
		set_master_send_hr(master_send_hr, 32, p_time_module_scene->led_sta);
		set_master_send_hr(master_send_hr, 33, ENTERPRISE);
		pack_data_send(&user_modbus, p_time_module_scene->adr, MD_FR_MHR, 31, 3);
	}
	else
	{
		return;
	}
}
// 调光模块定时发送场景
void timing_module_dim_scene_send(time_module_scene_set_t *p_time_module_scene)
{
	if ((VOL_DIM_MODULE == p_time_module_scene->type) || (SCR_DIM_MODULE == p_time_module_scene->type))
	{
		set_master_send_hr(master_send_hr, 22, p_time_module_scene->diming_enable);
		set_master_send_hr(master_send_hr, 23, p_time_module_scene->dimming_duty[0]);
		set_master_send_hr(master_send_hr, 24, p_time_module_scene->dimming_duty[1]);
		set_master_send_hr(master_send_hr, 25, p_time_module_scene->dimming_duty[2]);
		set_master_send_hr(master_send_hr, 26, p_time_module_scene->dimming_duty[3]);
		pack_data_send(&user_modbus, p_time_module_scene->adr, MD_FR_MHR, 22, 6);
	}
	else
	{
		return;
	}
}
// 显示继电器模块界面
void display_relay_timing_scene_interface(module_t *p)
{
	write_dgus(0x2011, p->adr);
	write_dgus(0x2012, p->channel_nb);
	write_dgusii_vp(0x2013, (unsigned char *)&p->module_name, 8);
	write_dgusii_vp(0x3f0a, (unsigned char *)&p->channel_name, 8 * 16);
	// USER_PRINTF("display module index is %bd\n", p->index);
	switch (p->channel_nb)
	{
	case 4:
		time_scene_var.page_bak = 47;

		break;
	case 6:
		time_scene_var.page_bak = 74;

		break;
	case 8:
		time_scene_var.page_bak = 48;

		break;
	case 10:
		time_scene_var.page_bak = 72;

		break;
	case 12:
		time_scene_var.page_bak = 49;

		break;
	case 14:
		time_scene_var.page_bak = 73;

	case 16:
		time_scene_var.page_bak = 50;

		break;
	default:
		break;
	}
	pic_set(time_scene_var.page_bak);
}
// 显示调光模块界面
void display_dim_timing_scene_interface(module_t *p)
{
	write_dgus(0x2011, p->adr);
	write_dgus(0x2012, p->channel_nb);
	write_dgusii_vp(0x2013, (unsigned char *)&p->module_name, 8);
	write_dgusii_vp(0x3f0a, (unsigned char *)&p->channel_name, 8 * 16);
	// USER_PRINTF("display module index is %bd\n", p->index);
	switch (p->type)
	{
	case VOL_DIM_MODULE:
		pic_set(TIMING_SCENE_DIM_PAGE);
		time_scene_var.page_bak = TIMING_SCENE_DIM_PAGE;
		break;
	case SCR_DIM_MODULE:
		pic_set(TIMING_SCENE_SCE_PAGE);
		time_scene_var.page_bak = TIMING_SCENE_SCE_PAGE;
		break;
	default:
		break;
	}
}
// 显示具体定时场景执行信息
void display_timing_scene_infor(timing_content_t *p_timing_content)
{
	char mod_infom_tab[181] = {0};
	unsigned char module_name_1[17] = {0};
	unsigned char led_on_tb[49] = {0};
	unsigned char led_off_tb[49] = {0};
	unsigned short on_sta = 0, off_sta = 0;
	unsigned char i = 0, j = 0, k = 0, l = 0, m = 0;
	unsigned char dim_duty_tb[40] = {0};
	unsigned char dim_duty_str[10] = {0};
	unsigned char str_len_cal = 0;
	unsigned char str_adr = 0;
	unsigned short str_count = 0;
	char channel[48] = "01,02,03,04,05,06,07,08,09,10,11,12,13,14,15,16,";
	char week_tb[35] = "周一,周二,周三\xFD,周四,周五,周六,周日,"; // 具体方法：遇到无法显示字符时在其后加\xFD
	char temp_week_tb[36] = {0};
	module_t module_read = {0};
	unsigned char sce_name[16] = "";
	char scene_name1[20] = {""};

	//	p_timing_content->time_scene_set.index = index;
	//	printf("%s\n",week_tb);
	if (MODULE_TIME == p_timing_content->time_scene_set.time_scene_exectue_type)
	{
		if ((p_timing_content->time_scene_set.time_module_scene_set.type < 2) || (4 == p_timing_content->time_scene_set.time_module_scene_set.type) || (5 == p_timing_content->time_scene_set.time_module_scene_set.type))
		{
			on_sta = p_timing_content->time_scene_set.time_module_scene_set.led_enable & (p_timing_content->time_scene_set.time_module_scene_set.led_sta);
			// USER_PRINTF("-->on_sta is 0x%x\n", on_sta);
			off_sta = p_timing_content->time_scene_set.time_module_scene_set.led_enable & (~p_timing_content->time_scene_set.time_module_scene_set.led_sta);
			// USER_PRINTF("-->off_sta is 0x%x\n", off_sta);
			for (i = 0; i < 16; i++)
			{
				if (GET_BIT(on_sta, i))
				{
					strncpy(led_on_tb + j * 3, &channel[i * 3], 3);
					j++;
				}
			}
			for (i = 0; i < 16; i++)
			{
				if (GET_BIT(off_sta, i))
				{
					strncpy(led_off_tb + k * 3, &channel[i * 3], 3);
					k++;
				}
			}
			// USER_PRINTF("-->led_on_tb is %s\n", led_on_tb);
			// USER_PRINTF("-->led_off_tb is %s\n", led_off_tb);
			//		strncpy(module_name_1,&module_name_tab[p_scenc_infor->type],8);
		}
		else
		{
			for (i = 0; i < 4; i++)
			{
				if (GET_BIT(p_timing_content->time_scene_set.time_module_scene_set.diming_enable, i))
				{

					sprintf(dim_duty_str, "%d%%;", p_timing_content->time_scene_set.time_module_scene_set.type == 2 ? p_timing_content->time_scene_set.time_module_scene_set.dimming_duty[i] * 10 : p_timing_content->time_scene_set.time_module_scene_set.dimming_duty[i]);

					str_len_cal = strlen(dim_duty_str);
					// USER_PRINTF("-->dim_duty_str str_len_cal is %bd\n", str_len_cal);
					strncpy(dim_duty_tb + str_adr, &channel[i * 3], 3);
					str_adr += 3;
					// USER_PRINTF("-->dim_duty_str is %s\n", dim_duty_str);
					strncpy(dim_duty_tb + str_adr, &dim_duty_str, str_len_cal);
					str_adr += str_len_cal;
					l++;
				}
			}
			// USER_PRINTF("-->dim_duty_infor is %s\n", dim_duty_tb);
			//		strncpy(module_name_1,&module_name_tab[p_scenc_infor->type],10);
		}
	}
	else if (ALL_ON_OFF_TIME == p_timing_content->time_scene_set.time_scene_exectue_type)
	{
	}
	else if (SCENE_TIME == p_timing_content->time_scene_set.time_scene_exectue_type)
	{
	}
	else
	{
	}
	for (i = 0; i < 7; i++)
	{
		if ((p_timing_content->time_scene_set.week_enable >> i) & 0x01)
		{
			//			USER_PRINTF("week%bd enable\n",i+1);
			strncpy(temp_week_tb + m * 5, &week_tb[5 * i], 5);
			//			USER_PRINTF("week_tb is %s\n",week_tb[i]);
			m++;
		}
	}
	// USER_PRINTF("-->temp_week_tb is %s\n", temp_week_tb);
	// USER_PRINTF("-->display_scene_infor p_scenc_infor->module_index is %bd\n", p_timing_content->time_scene_set.time_module_scene_set.module_index);
	norflash_read((p_timing_content->time_scene_set.time_module_scene_set.module_index) * 140, (unsigned char *)&module_read, 140);

	strncpy(module_name_1, &module_read.module_name[0], 16);
	strrpl(module_name_1, " ", "");

	if (0 == p_timing_content->time_scene_set.date_enable)
	{
		if ((on_sta) && (off_sta))
		{
			sprintf(mod_infom_tab, "地址:%bd;%bd路;%s开;%s关,执行时间:%s%bd:%02bd;模块:%s", p_timing_content->time_scene_set.time_module_scene_set.adr, p_timing_content->time_scene_set.time_module_scene_set.channel_nb, led_on_tb, led_off_tb, temp_week_tb, p_timing_content->exectue_time.hour, p_timing_content->exectue_time.min, module_name_1);
		}
		else if ((on_sta) && (0 == off_sta))
		{
			sprintf(mod_infom_tab, "地址:%bd;%bd路;%s开,执行时间:%s%bd:%02bd;模块:%s", p_timing_content->time_scene_set.time_module_scene_set.adr, p_timing_content->time_scene_set.time_module_scene_set.channel_nb, led_on_tb, temp_week_tb, p_timing_content->exectue_time.hour, p_timing_content->exectue_time.min, module_name_1);
		}
		else if ((0 == on_sta) && (off_sta))
		{
			sprintf(mod_infom_tab, "地址:%bd;%bd路;%s关,执行时间:%s%bd:%02bd;模块:%s", p_timing_content->time_scene_set.time_module_scene_set.adr, p_timing_content->time_scene_set.time_module_scene_set.channel_nb, led_off_tb, temp_week_tb, p_timing_content->exectue_time.hour, p_timing_content->exectue_time.min, module_name_1);
		}
		else if (p_timing_content->time_scene_set.time_module_scene_set.diming_enable)
		{
			sprintf(mod_infom_tab, "地址:%bd;%bd路;%s,执行时间:%s%bd:%02bd;模块:%s", p_timing_content->time_scene_set.time_module_scene_set.adr, p_timing_content->time_scene_set.time_module_scene_set.channel_nb, dim_duty_tb, temp_week_tb, p_timing_content->exectue_time.hour, p_timing_content->exectue_time.min, module_name_1);
		}
		else if (ALL_ON_OFF_TIME == p_timing_content->time_scene_set.time_scene_exectue_type)
		{
			if (ALL_ON == p_timing_content->time_scene_set.all_on_off_sta)
			{
				sprintf(mod_infom_tab, "执行时间:%s%bd:%02bd;动作:全开", temp_week_tb, p_timing_content->exectue_time.hour, p_timing_content->exectue_time.min);
			}
			else if (ALL_OFF == p_timing_content->time_scene_set.all_on_off_sta)
			{
				sprintf(mod_infom_tab, "执行时间:%s%bd:%02bd;动作:全关", temp_week_tb, p_timing_content->exectue_time.hour, p_timing_content->exectue_time.min);
			}
			else
			{
				sprintf(mod_infom_tab, "执行时间:%s%bd:%02bd;无动作", temp_week_tb, p_timing_content->exectue_time.hour, p_timing_content->exectue_time.min);
			}
		}
		else if (SCENE_TIME == p_timing_content->time_scene_set.time_scene_exectue_type)
		{
			if (0 == p_timing_content->time_scene_set.scene_name_index)
			{
				sprintf(mod_infom_tab, "执行时间:%s%bd:%02bd;无执行场景", temp_week_tb, p_timing_content->exectue_time.hour, p_timing_content->exectue_time.min);
			}
			else
			{
				read_dgusii_vp(0x3320 + (p_timing_content->time_scene_set.scene_name_index - 1) * 8, (unsigned char *)sce_name, 8);
				// USER_PRINTF("-->scene_name is %s\n", sce_name);
				str_count = sprintf(mod_infom_tab, "执行时间:%s%bd:%02bd;执行场景:%s", temp_week_tb, p_timing_content->exectue_time.hour, p_timing_content->exectue_time.min, sce_name);
				//				USER_PRINTF("-->mod_infom_tab is %s\n",sce_name);
				//        USER_PRINTF("-->str_count is %d\n",str_count);
				//				sprintf(mod_infom_tab+str_count,"%s;执行场景:",sce_name);
			}
		}

		else
		{
			sprintf(mod_infom_tab, "地址:%bd;%bd路;未配置场景,执行时间:%s,%bd:%02bd;模块:%s", p_timing_content->time_scene_set.time_module_scene_set.adr, p_timing_content->time_scene_set.time_module_scene_set.channel_nb, temp_week_tb, p_timing_content->exectue_time.hour, p_timing_content->exectue_time.min, module_name_1);
		}
	}
	else
	{
		if ((on_sta) && (off_sta))
		{
			sprintf(mod_infom_tab, "地址:%bd;%bd路;%s开;%s关;时间:%d-%bd-%bd~%d-%bd-%bd,%s%bd:%02bd;模块:%s", p_timing_content->time_scene_set.time_module_scene_set.adr, p_timing_content->time_scene_set.time_module_scene_set.channel_nb, led_on_tb, led_off_tb, p_timing_content->start_date.year, p_timing_content->start_date.month, p_timing_content->start_date.day, p_timing_content->end_date.year, p_timing_content->end_date.month, p_timing_content->end_date.day, temp_week_tb, p_timing_content->exectue_time.hour, p_timing_content->exectue_time.min, module_name_1);
		}
		else if ((on_sta) && (0 == off_sta))
		{
			sprintf(mod_infom_tab, "地址:%bd;%bd路;%s开;时间:%d-%bd-%bd~%d-%bd-%bd,%s%bd:%02bd;模块:%s", p_timing_content->time_scene_set.time_module_scene_set.adr, p_timing_content->time_scene_set.time_module_scene_set.channel_nb, led_on_tb, p_timing_content->start_date.year, p_timing_content->start_date.month, p_timing_content->start_date.day, p_timing_content->end_date.year, p_timing_content->end_date.month, p_timing_content->end_date.day, temp_week_tb, p_timing_content->exectue_time.hour, p_timing_content->exectue_time.min, module_name_1);
		}
		else if ((0 == on_sta) && (off_sta))
		{
			sprintf(mod_infom_tab, "地址:%bd;%bd路;%s关;时间:%d-%bd-%bd~%d-%bd-%bd,%s%bd:%02bd;模块:%s", p_timing_content->time_scene_set.time_module_scene_set.adr, p_timing_content->time_scene_set.time_module_scene_set.channel_nb, led_off_tb, p_timing_content->start_date.year, p_timing_content->start_date.month, p_timing_content->start_date.day, p_timing_content->end_date.year, p_timing_content->end_date.month, p_timing_content->end_date.day, temp_week_tb, p_timing_content->exectue_time.hour, p_timing_content->exectue_time.min, module_name_1);
		}
		else if (p_timing_content->time_scene_set.time_module_scene_set.diming_enable)
		{
			sprintf(mod_infom_tab, "地址:%bd;%bd路;%s;时间:%d-%bd-%bd~%d-%bd-%bd,%s%bd:%02bd;模块:%s", p_timing_content->time_scene_set.time_module_scene_set.adr, p_timing_content->time_scene_set.time_module_scene_set.channel_nb, dim_duty_tb, p_timing_content->start_date.year, p_timing_content->start_date.month, p_timing_content->start_date.day, p_timing_content->end_date.year, p_timing_content->end_date.month, p_timing_content->end_date.day, temp_week_tb, p_timing_content->exectue_time.hour, p_timing_content->exectue_time.min, module_name_1);
		}
		else if (ALL_ON_OFF_TIME == p_timing_content->time_scene_set.time_scene_exectue_type)
		{
			if (ALL_ON == p_timing_content->time_scene_set.all_on_off_sta)
			{
				sprintf(mod_infom_tab, "执行时间:%s%bd:%02bd;动作:全开", temp_week_tb, p_timing_content->exectue_time.hour, p_timing_content->exectue_time.min);
			}
			else if (ALL_OFF == p_timing_content->time_scene_set.all_on_off_sta)
			{
				sprintf(mod_infom_tab, "执行时间:%s%bd:%02bd;动作:全关", temp_week_tb, p_timing_content->exectue_time.hour, p_timing_content->exectue_time.min);
			}
			else
			{
				sprintf(mod_infom_tab, "执行时间:%s%bd:%02bd;无动作", temp_week_tb, p_timing_content->exectue_time.hour, p_timing_content->exectue_time.min);
			}
		}
		else if (SCENE_TIME == p_timing_content->time_scene_set.time_scene_exectue_type)
		{
			if (0 == p_timing_content->time_scene_set.scene_name_index)
			{
				sprintf(mod_infom_tab, "执行时间:%s%bd:%02bd;无执行场景", temp_week_tb, p_timing_content->exectue_time.hour, p_timing_content->exectue_time.min);
			}
			else
			{
				sprintf(mod_infom_tab, "执行时间:%s%bd:%02bd;执行场景:%bd\n", temp_week_tb, p_timing_content->exectue_time.hour, p_timing_content->exectue_time.min, p_timing_content->time_scene_set.scene_name_index);
			}
		}
		else
		{
			sprintf(mod_infom_tab, "%s;地址:%bd;%bd路;未配置场景;时间:%d-%bd-%bd~%d-%bd-%bd,%s,%bd:%02bd", module_name_1, p_timing_content->time_scene_set.time_module_scene_set.adr, p_timing_content->time_scene_set.time_module_scene_set.channel_nb, p_timing_content->start_date.year, p_timing_content->start_date.month, p_timing_content->start_date.day, p_timing_content->end_date.year, p_timing_content->end_date.month, p_timing_content->end_date.day, temp_week_tb, p_timing_content->exectue_time.hour, p_timing_content->exectue_time.min);
		}
	}
	// USER_PRINTF("-->timing_message is %s\n", mod_infom_tab);
	//	USER_PRINTF("-->module->index %bd\n",p_scenc_infor->index);
	write_dgusii_vp(0x6180 + (p_timing_content->time_scene_set.index) * 80, (unsigned char *)mod_infom_tab, 80);
	// USER_PRINTF("-->current_time_scene_index is %bd\n", p_timing_content->time_scene_set.index);
}
// 选择模块进行场景设置
void module_timing_scene_select(module_t *p, unsigned char key)
{

	norflash_read((key - 1) * 140, (unsigned char *)p, 140);
	//	USER_PRINTF("-->batch_set_parameters_flag after read module flash is %bd\n",g_var_module.batch_set_parameters_flag);
	if (0 == p->data_sta)
	{
		return;
	}
	switch (p->type)
	{
	case SINGLE_RELAY_MODULE:
	case SINGLE_TIME_RELAY_MODULE:
	case DOUBLE_RELAY_MODULE:
	case DOUBLE_TIME_RELAY_MODULE:

		display_relay_timing_scene_interface(p);
		USER_PRINTF("display_relay_timing_scene_interface\n");
		//				relay_read(p);
		break;
	case VOL_DIM_MODULE:
	case SCR_DIM_MODULE:
		//				clear_touch_sta();
		display_dim_timing_scene_interface(p);
		USER_PRINTF("display_dim_timing_scene_interface\n");
		//				dim_read(p);
		break;
		break;
	default:

		break;
	}
}
// 选择需要设置定时的模块
void module_timing_scene_select_ctrl(module_t *p)
{
	unsigned short key_nb = 0;
	get_key_value(0x10cb, &key_nb);
	if (key_nb)
	{
		module_timing_scene_select(p, key_nb);
		clear_key_value(0x10cb, &key_nb);
	}
}
// 定时场景添加
void timing_scene_add(time_scene_set_t *p_time_scene)
{
	p_time_scene->index = get_blank_timing_scene_infor_index();
	// USER_PRINTF("-->temp_get_blank_scene_index is %bd\n", p_time_scene->index);
}
// 定时场景删除
void timing_scene_delete(timing_content_t *p_timing_content, time_scene_set_t *p_time_scene)
{
	unsigned short key_nb = 0;
	get_key_value(0x1089, &key_nb);
	if ((1 == key_nb))
	{
		clear_key_value(0x1089, &key_nb);
		p_time_scene->index = get_timing_scene_name_select_sequence_number() - 1;
		// USER_PRINTF("-->temp_get_select_sequence_number_scene_index is %bd\n", p_time_scene->index);
		timing_scene_infor_delete(p_timing_content + p_time_scene->index, p_time_scene);
		USER_PRINTF("-->timing scene delect success!\n");
	}
}
// 定时场景清空
void timing_scene_clear(timing_content_t *p_timing_content)
{
	unsigned short key_nb = 0;
	get_key_value(0x1088, &key_nb);
	if ((1 == key_nb))
	{
		timing_scene_infor_clear(p_timing_content);
		clear_key_value(0x1088, &key_nb);
		USER_PRINTF("-->timing scene clear success!\n");
	}
}
void copy_to_temp_timing_content(timing_content_t *p_timing_content)
{
	memcpy(&temp_timing_content, p_timing_content, sizeof(timing_content_t));
}
void display_all_on_off_interface(timing_content_t *p_timing_content)
{
	pic_set(46);
	if (1 == p_timing_content->time_scene_set.all_on_off_sta)
	{
		write_dgus(0x1098, 1);
		write_dgus(0x1099, 0);
	}
	else if (2 == p_timing_content->time_scene_set.all_on_off_sta)
	{
		write_dgus(0x1098, 0);
		write_dgus(0x1099, 1);
	}
	else
	{
		write_dgus(0x1098, 0);
		write_dgus(0x1099, 0);
	}
}

void display_scene_time_interface(timing_content_t *p_timing_content)
{
	unsigned char scene_name[16] = {0};
	pic_set(45);
	read_dgusii_vp(0x3320 + (p_timing_content->time_scene_set.scene_name_index - 1) * 8, (unsigned char *)scene_name, 8);
	write_dgusii_vp(0x2009, scene_name, 8);
}
// 定时信息修改
void timing_scene_modtify(timing_content_t *p_timing_content, time_scene_set_t *p_time_scene)
{
	if (0 == get_timing_scene_name_select_sequence_number())
		return;
	p_time_scene->index = get_timing_scene_name_select_sequence_number() - 1;
	if (BLANK == (p_timing_content + p_time_scene->index)->time_scene_set.data_sta)
		return;
	USER_PRINTF("-->temp_get_select_sequence_number_scene_index is %bd\n", p_time_scene->index);

	USER_PRINTF("-->time_scene_exectue_type is %bd\n", (p_timing_content + p_time_scene->index)->time_scene_set.time_scene_exectue_type);
	copy_to_temp_timing_content(p_timing_content + p_time_scene->index);
	set_timing_set_infor(&(p_timing_content + p_time_scene->index)->start_date, &(p_timing_content + p_time_scene->index)->end_date, &(p_timing_content + p_time_scene->index)->exectue_time, &(p_timing_content + p_time_scene->index)->time_scene_set);

	switch ((p_timing_content + p_time_scene->index)->time_scene_set.time_scene_exectue_type)
	{
	case MODULE_TIME:
		USER_PRINTF("-->time_module_scene_set.module_index is %bd\n", (p_timing_content + p_time_scene->index)->time_scene_set.time_module_scene_set.module_index);
		module_timing_scene_select(&timing_module, (p_timing_content + p_time_scene->index)->time_scene_set.time_module_scene_set.module_index + 1);
		//		  set_timing_set_infor(&(p_timing_content+p_time_scene->index)->start_date,&(p_timing_content+p_time_scene->index)->end_date,&(p_timing_content+p_time_scene->index)->exectue_time,&(p_timing_content+p_time_scene->index)->time_scene_set);
		dis_module_scene_infor(&(p_timing_content + p_time_scene->index)->time_scene_set.time_module_scene_set);
		time_scene_var.module_select_sta = 1;
		time_scene_var.all_light_select_sta = 0;
		time_scene_var.scene_select_sta = 0;
		break;
	case ALL_ON_OFF_TIME:
		USER_PRINTF("-->all on off\n");
		display_all_on_off_interface(p_timing_content + p_time_scene->index);
		time_scene_var.module_select_sta = 0;
		time_scene_var.all_light_select_sta = 1;
		time_scene_var.scene_select_sta = 0;
		break;
	case SCENE_TIME:
		USER_PRINTF("-->scene time\n");
		display_scene_time_interface(p_timing_content + p_time_scene->index);
		time_scene_var.module_select_sta = 0;
		time_scene_var.all_light_select_sta = 0;
		time_scene_var.scene_select_sta = 1;
		break;
	default:
		break;
	}
	write_dgus(0x1092, time_scene_var.module_select_sta);
	write_dgus(0x1093, time_scene_var.all_light_select_sta);
	write_dgus(0x1094, time_scene_var.scene_select_sta);
}
// 定时场景编辑
void timing_scene_edit(timing_content_t *p_timing_content, time_scene_set_t *p_time_scene)
{
	unsigned short key_nb = 0;
	get_key_value(0x1087, &key_nb);
	if ((key_nb))
	{
		switch (key_nb)
		{
		case ADD:
			timing_scene_add(p_time_scene);

			break;
		case MODIFY:
			timing_scene_modtify(p_timing_content, p_time_scene);
			break;
		default:

			break;
		}
		clear_key_value(0x1087, &key_nb);
	}
	timing_scene_clear(p_timing_content);
	timing_scene_delete(p_timing_content, p_time_scene);
}
// 定时模块场景设定
void timing_module_scene_set(module_t *p_module, timing_content_t *p_timing_content)
{
	//	unsigned char temp_tab[32] = {0};
	//	read_dgusii_vp(0x2013,(unsigned char *)temp_tab,16);
	//	if(0 == strcmp(temp_tab, "未选择模块"))
	//	{
	//		USER_PRINTF("-->don't select module ,please select one module\n");
	//		return;
	//	}

	timing_module_relay_scene_set(p_module, &p_timing_content->time_scene_set.time_module_scene_set);
	timing_module_dim_scene_set(p_module, &p_timing_content->time_scene_set.time_module_scene_set);
	p_timing_content->time_scene_set.data_sta = 1;
	USER_PRINTF("--> select a module ok\n");
}
void scene_infor_module_confim_key(module_t *p_module, timing_content_t *p_timing_content, time_scene_set_t *p_time_scene_set)
{
	p_timing_content->time_scene_set.index = p_time_scene_set->index;
	p_timing_content->time_scene_set.time_scene_exectue_type = p_time_scene_set->time_scene_exectue_type;
	get_timing_set_infor(&p_timing_content->start_date, &p_timing_content->end_date, &p_timing_content->exectue_time, &p_timing_content->time_scene_set);

	timing_module_scene_set(p_module, p_timing_content);
	display_timing_scene_infor(p_timing_content);
	timing_scene_infor_save_flash(p_timing_content);
	printf_temp_timing_content(p_timing_content);
	//	printf_timing_content(10);
	clear_timing_scene_var();
	// p_time_scene_set->index
	if (p_time_scene_set->index >= 5)
	{
		pic_page = TIMING_SCENE_MANGE_SECOND_PAGE + p_time_scene_set->index / 5 - 1;
		pic_set(pic_page);
		
	}
	else
	{
		pic_page = TIMING_SCENE_MANGE_FIRST_PAGE;
		pic_set(pic_page);
	}	
}
void scene_infor_cancle_key(void)
{
	clear_timing_scene_var();
	pic_set(pic_page);
}
void timing_specified_scene_set(time_scene_set_t *p_time_scene)
{
	//	norflash_read(SCENE_NAME_FLASH_ADR_SATRT+p_time_scene->index*NAME_SCENE_T_SIZE+2,(unsigned char *)p_time_scene->scene_name_index,1);//读flash
	if (0 == temp_timing_content.time_scene_set.scene_name_index)
	{
		USER_PRINTF("-->don't select scene ,please select one scene\n");
		return;
	}
	p_time_scene->data_sta = 1;
	p_time_scene->scene_name_index = temp_timing_content.time_scene_set.scene_name_index;
	USER_PRINTF("-->select scene index is %bd\n", p_time_scene->scene_name_index);
}
void specified_scene_confim_key(time_scene_set_t *p_time_scene, timing_content_t *p_timing_content, time_scene_set_t *p_time_scene_set)
{
	p_timing_content->time_scene_set.index = p_time_scene_set->index;
	p_timing_content->time_scene_set.time_scene_exectue_type = p_time_scene_set->time_scene_exectue_type;
	get_timing_set_infor(&p_timing_content->start_date, &p_timing_content->end_date, &p_timing_content->exectue_time, &p_timing_content->time_scene_set);

	timing_specified_scene_set(p_time_scene);
	display_timing_scene_infor(p_timing_content);
	clear_timing_scene_var();
	timing_scene_infor_save_flash(p_timing_content);
}

// 0x1098-0x1099
void timing_all_on_off_scene_set(time_scene_set_t *p_time_scene)
{
	unsigned char on_sta = 0, off_sta = 0;
	on_sta = read_dgus(0x1098);
	off_sta = read_dgus(0x1099);
	if ((0 == on_sta) && (0 == off_sta))
	{
		p_time_scene->all_on_off_sta = 0;
		p_time_scene->data_sta = 0;
	}
	else if ((1 == on_sta))
	{
		p_time_scene->all_on_off_sta = 1;
		p_time_scene->data_sta = 1;
	}
	else if ((1 == off_sta))
	{
		p_time_scene->all_on_off_sta = 2;
		p_time_scene->data_sta = 1;
	}
}
void all_on_off_scene_confim_key(time_scene_set_t *p_time_scene, timing_content_t *p_timing_content, time_scene_set_t *p_time_scene_set)
{
	p_timing_content->time_scene_set.index = p_time_scene_set->index;
	p_timing_content->time_scene_set.time_scene_exectue_type = p_time_scene_set->time_scene_exectue_type;
	get_timing_set_infor(&p_timing_content->start_date, &p_timing_content->end_date, &p_timing_content->exectue_time, &p_timing_content->time_scene_set);

	timing_all_on_off_scene_set(p_time_scene);
	display_timing_scene_infor(p_timing_content);
	timing_scene_infor_save_flash(p_timing_content);
	clear_timing_scene_var();
}
void timing_scene_infor_ctrl(module_t *p_module, timing_content_t *p_timing_content, time_scene_set_t *p_time_scene_set)
{
	unsigned short key_nb = 0;
	get_key_value(0x10CC, &key_nb);
	if ((key_nb))
	{
		switch (key_nb)
		{
		case 1: // 模块确定
			scene_infor_module_confim_key(p_module, p_timing_content, p_time_scene_set);

			break;
		case 2:
			scene_infor_cancle_key();
			break;
		case 3: // 所有灯光确定
			all_on_off_scene_confim_key(&p_timing_content->time_scene_set, p_timing_content, p_time_scene_set);

			//				scene_infor_confim_key();
			break;
		case 4: // 指定场景确定
			specified_scene_confim_key(&p_timing_content->time_scene_set, p_timing_content, p_time_scene_set);

			break;
		case 5:
			scene_infor_cancle_key();
			break;
		default:
			break;
		}
		clear_key_value(0x10CC, &key_nb);
	}
}
// 定时模块选择控制
void operand_ctrl(module_t *p_module, timing_content_t *p_timing_content)
{
	unsigned short key_nb = 0;

	get_key_value(0x1095, &key_nb);
	if ((key_nb))
	{
		switch (key_nb)
		{
		case 1:
			write_dgus(0x1092, time_scene_var.module_select_sta ^= 1);
			write_dgus(0x1093, 0);
			write_dgus(0x1094, 0);
			time_scene_var.all_light_select_sta = 0;
			time_scene_var.scene_select_sta = 0;

			if (0 == p_module->data_sta)
			{
				pic_set(71);
			}
			else
			{
				pic_set(time_scene_var.page_bak);
			}
			if (0 == time_scene_var.module_select_sta)
			{
				pic_set(70);
				p_timing_content->time_scene_set.time_scene_exectue_type = EMPTY_TIME;
			}
			else
			{
				p_timing_content->time_scene_set.time_scene_exectue_type = MODULE_TIME;
			}
			break;
		case 2:
			write_dgus(0x1092, 0);
			write_dgus(0x1093, time_scene_var.all_light_select_sta ^= 1);
			write_dgus(0x1094, 0);
			time_scene_var.module_select_sta = 0;
			time_scene_var.scene_select_sta = 0;

			if (0 == time_scene_var.all_light_select_sta)
			{
				pic_set(70);
				p_timing_content->time_scene_set.time_scene_exectue_type = EMPTY_TIME;
			}
			else
			{
				pic_set(46);
				p_timing_content->time_scene_set.time_scene_exectue_type = ALL_ON_OFF_TIME;
			}

			break;
		case 3:
			write_dgus(0x1092, 0);
			write_dgus(0x1093, 0);
			write_dgus(0x1094, time_scene_var.scene_select_sta ^= 1);

			time_scene_var.module_select_sta = 0;
			time_scene_var.all_light_select_sta = 0;
			if (0 == time_scene_var.scene_select_sta)
			{
				pic_set(70);
				p_timing_content->time_scene_set.time_scene_exectue_type = EMPTY_TIME;
			}
			else
			{
				pic_set(45);
				p_timing_content->time_scene_set.time_scene_exectue_type = SCENE_TIME;
			}
			break;
		}
		clear_key_value(0x1095, &key_nb);
	}
}
// 定时场景选择控制
void timing_scene_select_ctrl(time_scene_set_t *p_time_scene_set)
{
	unsigned short key_nb = 0;
	unsigned char scene_name[16] = {0};
	get_key_value(0x1096, &key_nb);
	if ((key_nb))
	{
		read_dgusii_vp(0x3320 + (key_nb - 1) * 8, (unsigned char *)scene_name, 8);
		write_dgusii_vp(0x2009, (unsigned char *)scene_name, 8);
		clear_key_value(0x1096, &key_nb);
		if ((0 != strcmp(scene_name, "未启用")) && (0 != strcmp(scene_name, "未选择场景")))
		{
			p_time_scene_set->scene_name_index = key_nb;
			USER_PRINTF("-->timing select scene index is %bd\n", p_time_scene_set->scene_name_index);
		}
		else
		{
			p_time_scene_set->scene_name_index = 0;
			USER_PRINTF("-->don't select scene ,please select one scene\n");
		}
		pic_set(45);
	}
}
// 定时全开全关选择控制
void timing_all_on_off_ctrl(void)
{
	unsigned short key_nb = 0;

	get_key_value(0x1097, &key_nb);
	if ((key_nb))
	{
		switch (key_nb)
		{
		case 1:
			write_dgus(0x1098, time_scene_var.all_light_on_sta ^= 1);
			write_dgus(0x1099, 0);
			time_scene_var.all_light_off_sta = 0;
			break;
		case 2:
			write_dgus(0x1099, time_scene_var.all_light_off_sta ^= 1);
			write_dgus(0x1098, 0);
			time_scene_var.all_light_on_sta = 0;
			break;
		}
		clear_key_value(0x1097, &key_nb);
	}
}
// 模块定时发送场景
void module_timing_send(time_module_scene_set_t *p)
{
	if ((DOUBLE_RELAY_MODULE == p->type) || (DOUBLE_TIME_RELAY_MODULE == p->type))
	{
		set_master_send_hr(master_send_hr, 31, p->led_enable);
	}
	else
	{
		set_master_send_hr(master_send_hr, 31, p->led_enable << 4 | 0x08);
	}
	USER_PRINTF("module_timing_send led enable is %d\n", p->led_enable);
	set_master_send_hr(master_send_hr, 32, p->led_sta);
	USER_PRINTF("module_timing_send led sta is %d\n", p->led_sta);
	set_master_send_hr(master_send_hr, 33, ENTERPRISE);
	pack_data_send(&user_modbus, p->adr, MD_FR_MHR, 31, 3);
}
// 场景设定定时发送
void scene_timing_send(time_scene_set_t *p)
{
	set_master_send_hr(master_send_hr, 7, p->scene_name_index);
	set_master_send_hr(master_send_hr, 8, 1);
	pack_data_send(&user_modbus, 0, MD_FR_MHR, 7, 2);
}
// 定时发送全开全关
void all_on_off_timing_send(time_scene_set_t *p)
{
	if (1 == p->all_on_off_sta)
	{
		set_master_send_hr(master_send_hr, 31, 0xffff);
		set_master_send_hr(master_send_hr, 32, 0xffff);
		set_master_send_hr(master_send_hr, 33, (2 << 14) | ENTERPRISE);
	}
	else if (2 == p->all_on_off_sta)
	{
		set_master_send_hr(master_send_hr, 31, 0xffff);
		set_master_send_hr(master_send_hr, 32, 0x0000);
		set_master_send_hr(master_send_hr, 33, (2 << 14) | ENTERPRISE);
	}
	pack_data_send(&user_modbus, 0, MD_FR_MHR, 31, 3);
}

void timing_auto_detect(void)
{
	unsigned char i = 0;
	unsigned char sys_time[8] = 0;
	static unsigned char sec_bak = 0;
	date_t temp_date = {0};
	time_t temp_time = {0};
	unsigned char week = 0;
	read_sys_time(sys_time);
	temp_date.year = sys_time[0];
	temp_date.month = sys_time[1];
	temp_date.day = sys_time[2];
	week = sys_time[3] > 0 ? (sys_time[3] - 1) : 6;
	temp_time.hour = sys_time[4];
	temp_time.min = sys_time[5];
	temp_time.sec = sys_time[6];
	//  printf_tab(8,sys_time);
	if (sec_bak != temp_time.sec)
	{
		sec_bak = temp_time.sec;
		for (i = 0; i < TIMING_SCENE_NUB_SIGNLE; i++)
		{
			if (BLANK == timing_content[i].time_scene_set.data_sta)
				continue;
			if (0 == timing_content[i].time_scene_set.date_enable)
			{
				//			USER_PRINTF("--->week is %bd \n",week);
				if (1 == GET_BIT(timing_content[i].time_scene_set.week_enable, week))
				{
					//				USER_PRINTF("--->%bd week enable\n",i);
					if ((temp_time.hour == timing_content[i].exectue_time.hour) && (temp_time.min == timing_content[i].exectue_time.min) && (0 == temp_time.sec) && (0 == GET_BIT(time_scene_var.scene_send_sta[i / 8], i)))
					{
						USER_PRINTF("--->it is time to send uart data\n");
						SET_BIT(time_scene_var.scene_send_sta[i / 8], i);
					}
				}
			}
			else
			{
				if ((timing_content[i].start_date.year <= temp_date.year) && (timing_content[i].start_date.month <= temp_date.month) && (timing_content[i].start_date.day <= temp_date.day) && (timing_content[i].end_date.year >= temp_date.year) && (timing_content[i].end_date.month >= temp_date.month) && (timing_content[i].end_date.day >= temp_date.day))
				{
					if (1 == GET_BIT(timing_content[i].time_scene_set.week_enable, week))
					{
						if ((temp_time.hour == timing_content[i].exectue_time.hour) && (temp_time.min == timing_content[i].exectue_time.min) && (0 == temp_time.sec) && (0 == GET_BIT(time_scene_var.scene_send_sta[i / 8], i)))
						{
							SET_BIT(time_scene_var.scene_send_sta[i / 8], i);
						}
					}
				}
			}
		}
	}
}
void timed_auto_send(void)
{
	static unsigned char i = 0;
	if (GET_BIT(time_scene_var.scene_send_sta[i / 8], i))
	{

		if ((0 == time_scene_var.send_delay_time))
		{
			if ((0 == time_scene_var.cyc_send_time) && (mbh_getState() == MBH_STATE_IDLE))
			{

				time_scene_var.cyc_send_time = 100;
				CLEAR_BIT(time_scene_var.scene_send_sta[i / 8], i);

				switch (timing_content[i].time_scene_set.time_scene_exectue_type)
				{
				case MODULE_TIME:
					USER_PRINTF("time_scene_exectue_type is MODULE_TIME\n");
					module_timing_send(&timing_content[i].time_scene_set.time_module_scene_set);
					break;
				case ALL_ON_OFF_TIME:
					USER_PRINTF("time_scene_exectue_type is ALL_ON_OFF_TIME\n");
					all_on_off_timing_send(&timing_content[i].time_scene_set);

					break;
				case SCENE_TIME:
					USER_PRINTF("time_scene_exectue_type is SCENE_TIME\n");
					scene_timing_send(&timing_content[i].time_scene_set);
					break;
				default:
					break;
				}
				i++;
			}
		}
	}
	else
	{
		i++;
		//		time_scene_var.cyc_send_time = 100;
	}
	if (i > TIMING_SCENE_NUB_SIGNLE)
	{
		i = 0;
		//		memset(time_scene_var.scene_send_sta,0,16);
		//		memset(time_scene_var.scene_send_sta);
	}
}
void set_the_right_number_of_days(unsigned short year_adr, unsigned short month_adr,unsigned short day_adr)
{
	unsigned char day = 0;
	unsigned char month = 0;
	unsigned short year = 0;
	year = read_dgus(year_adr);
	month = read_dgus(month_adr);
	sys_delay_about_ms(2);
	day = get_days_in_month(year,month);
	// sys_delay_about_ms(2);
	// USER_PRINTF("the year is %d\n",year);
	// USER_PRINTF("the month is %bd\n",month);
	// USER_PRINTF("the day is %bd\n",day);
	if(read_dgus(day_adr)>day)
	{
		sys_delay_about_ms(2);
		write_dgus(day_adr,day);
	}

}
void right_number_of_days_ctrl(void)
{
	set_the_right_number_of_days(TIMING_SCENE_START_YEAR,TIMING_SCENE_START_MONTH,TIMING_SCENE_START_DAY);
	set_the_right_number_of_days(TIMING_SCENE_END_YEAR,TIMING_SCENE_END_MONTH,TIMING_SCENE_END_DAY);	

}
void timing_scene_run(void)
{
	module_timing_scene_select_ctrl(&timing_module);
	operand_ctrl(&timing_module, &temp_timing_content);
	timing_scene_infor_ctrl(&timing_module, (timing_content + temp_timing_content.time_scene_set.index), &temp_timing_content.time_scene_set);
	timing_all_on_off_ctrl();
	timing_scene_select_ctrl(&temp_timing_content.time_scene_set);
	timing_scene_edit(&timing_content, &temp_timing_content.time_scene_set);

}
