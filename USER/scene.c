/*
 * @Author: xw.qu
 * @Date: 2023-08-31 09:22:42
 * @LastEditors: xw.qu
 * @LastEditTime: 2023-10-30 08:26:58
 * @FilePath: \USER\scene.c
 * @Description: module scene modify
 *
 * Copyright (c) 2023 by xw.qu, All Rights Reserved.
 */
#include "scene.h"
#include "sys.h"
#include "T5LLIB.h"
#include "time_scene.h"
// unsigned short mbHost.scene_send_cyc_time;
name_scene_t module_scene; //
scenc_infor_t scenc_infor; //
scenc_infor_t temp_scenc_infor[SINGLE_SCENE_NAME_INFOR_SIZE];
scene_data_t scene_data;
unsigned char default_scene_name[17] = {"场景1"};
unsigned char module_scene_select_key_bak[8] = {0};
unsigned char temp_module_index = 0;
unsigned char infor_clear_sta = 0;
unsigned char scene_vaild[8] = {0};
// unsigned char name_space[16] = "";
unsigned char name_space[140] = "";
unsigned short scene_name_key_nb_bak = 0;
unsigned char scene_name_select_sta_bak = 0;
unsigned short scene_infor_key_nb_bak = 0;
unsigned char scene_infor_select_sta_bak = 0;
unsigned char scene_infor_page = 0;
unsigned char scene_index = 0;
void pop_menu_key_ctrl(unsigned char menu_key_value);
// static void get_led_set_sta(unsigned short *led_enable,unsigned short *led_sta);
// module_infor_t temp_module_infor[SINGLE_SCENE_NAME_INFOR_SIZE];
void set_default_scene_name(void)
{
	write_dgusii_vp(0x4089, (unsigned char *)&default_scene_name, 8);
}
// 获取场景名字编辑状态
unsigned char get_module_scene_edit_sta(void)
{
	// USER_PRINTF("name_scene.edit_sta is %bd\n", module_scene.edit_sta);
	return module_scene.edit_sta;
}
// 获取模块编辑状态
unsigned char *get_module_scene_module_select(void)
{
	//	USER_PRINTF("name_scene.edit_sta is %bd\n",module_scene.edit_sta);
	printf_tab(8, &module_scene.module_select[0]);
	return &module_scene.module_select[0];
}
void scene_init(scenc_infor_t *p_scenc_infor, name_scene_t *p)
{
	p->scene_name_index = 1;
	p->edit_sta = 1;
	p_scenc_infor->edit_sta = 1;
	p_scenc_infor->index = 1;
}
void printf_temp_scenc_infor(unsigned char index)
{
	unsigned char i = 0;
	for (i = 0; i < index; i++)
	{
		//		USER_PRINTF("temp_scenc_infor[%bd].edit_sta is %bd,data_sta is %bd,type is %bd,index is %d,adr is %bd,led_enable is %d,led_sta is %d\n",i,temp_scenc_infor[i].edit_sta,temp_scenc_infor[i].data_sta,temp_scenc_infor[i].type,temp_scenc_infor[i].index,temp_scenc_infor[i].adr,temp_scenc_infor[i].led_enable,temp_scenc_infor[i].led_sta);
	}
}
// 清除单选框
void clear_single_module_var_en(unsigned short adr)
{
	write_dgus(adr, 0);
}
// 清除所有单选框
void clear_all_module_var_en(unsigned short adr, unsigned char len)
{
	unsigned char i = 0;
	for (i = 0; i < len; i++)
	{
		clear_single_module_var_en(adr + i);
	}
}
// 清空临时变量场景信息内容
void clear_temp_scenc_infor(void)
{
	memset((unsigned char *)temp_scenc_infor, 0, sizeof(scenc_infor_t) * SINGLE_SCENE_NAME_INFOR_SIZE);
}
// 获取空白场景名字序号
unsigned char get_blank_scene_name_index(void)
{
	// return find_index(SCENE_NAME_FLASH_ADR_SATRT,NAME_SCENE_T_SIZE,SCENE_NAME_NUB);
	unsigned char i = 0;
	unsigned short temp_data = 0;
	for (i = 0; i < SCENE_NAME_NUB_LIMIT; i++)
	{
		read_dgusii_vp(0x4100 + i * 32, (unsigned char *)&temp_data, 1);
		if (0 == temp_data)
			return i;
	}
	return 0xff;
}
// 判断场景所有信息内容是为空
unsigned char if_all_scene_infor_blank(void)
{
	unsigned char i = 0;
	unsigned short temp_data = 0;
	for (i = 0; i < SINGLE_SCENE_NAME_INFOR_SIZE; i++)
	{
		//		read_dgusii_vp(0x5000+i*70,(unsigned char *)&temp_data,1);
		if (1 == temp_scenc_infor[i].data_sta)
			return i;
	}
	return 0xff;
}
// 清空一个场景信息内容
void clear_single_scene(unsigned char index)
{

	write_dgusii_vp(0x5000 + index * 70, (unsigned char *)&name_space, 70);
}
// 清空所有场景信息内容
void clear_all_scene_infor(void)
{
	unsigned char i = 0;
	for (i = 0; i < SINGLE_SCENE_NAME_INFOR_SIZE; i++)
	{
		clear_single_scene(i);
	}
}
// 获取空白的场景内容序号
unsigned char get_blank_scene_infor_index(void)
{

	unsigned char i = 0;
	//	unsigned short temp_data = 0;
	for (i = 0; i < SINGLE_SCENE_NAME_INFOR_SIZE; i++)
	{
		//		read_dgusii_vp(0x5000+i*70,(unsigned char *)&temp_data,1);
		//		USER_PRINTF("-->temp_data_value is %d\n",temp_data);
		if ((0 == temp_scenc_infor[i].data_sta)) //&&( 1 == temp_scenc_infor[i].edit_sta)
			return i;
	}
	return 0xff;
}
// 获取场景信息选中序号
unsigned char get_scene_infor_select_sequence_number(void)
{

	// return get_selected_sequence_number(0x1060,8);
	return get_box_select_number(0x1900, 64);
}
// 设定场景信息选中序号
void set_scene_infor_select_sequence_number(void)
{
	//	set_selected_sequence_number(0x1070,0x1060,8);
	set_box_select(0x1070, 0x1900, 64, &scene_infor_key_nb_bak, &scene_infor_select_sta_bak);
}
// 获取场景名称选中序号
unsigned char get_scene_name_select_sequence_number(void)
{
	// return get_selected_sequence_number(0x1068,8);
	return get_box_select_number(SCENE_NAME_LIST_SELECT_ADR, 64);
}
// 设定场景名称选中序号
void set_scene_name_select_sequence_number(void)
{
	// set_selected_sequence_number(0x102f,0x1068,8);
	set_box_select(0x102f, SCENE_NAME_LIST_SELECT_ADR, 64, &scene_name_key_nb_bak, &scene_name_select_sta_bak);
}
// 判断指定场景是否存在
unsigned char if_scene_name_exists(unsigned char index)
{
	unsigned short temp_dat = 0;
	read_dgusii_vp(0x4100 + index * 32, (unsigned char *)&temp_dat, 1);
	;
	if (0 == temp_dat)
	{
		return BLANK;
	}
	else
	{
		return FULL;
	}
}
// 判断场景名字是否不存在
unsigned char if_all_scene_name_blank(void)
{
	unsigned char i = 0;
	for (i = 0; i < SCENE_NAME_NUB_LIMIT; i++)
	{
		if (FULL == if_scene_name_exists(i))
		{
			return i;
		}
	}
	return 0xff;
}
// 存储场景名称
void scene_name_save_flash(name_scene_t *p)
{
	//	p->data_sta = FULL;//设置数据
	//	p->scene_name_index = index;//设置场景序号
	//	read_dgusii_vp(0x4089,(unsigned char *)&p->scene_name,8);//读取设定的场景名称
	norflash_write(SCENE_NAME_FLASH_ADR_SATRT + (p->scene_name_index - 1) * NAME_SCENE_T_SIZE, (unsigned char *)p, NAME_SCENE_T_SIZE); // 存到flash
																																																																		 //	write_dgusii_vp(0x4100+(p->scene_name_index-1)*32,(unsigned char *)&p->scene_name,8);//屏幕显示场景名称
																																																																		 //	write_dgusii_vp(0x3320+(p->scene_name_index-1)*8,(unsigned char *)&p->scene_name,8);
}
// 写场景名称
void scene_name_write_dgusii(name_scene_t *p)
{
	p->data_sta = FULL;																																						// 设置数据
	read_dgusii_vp(0x4089, (unsigned char *)&p->scene_name, 8);																		// 读取设定的场景名称
	write_dgusii_vp(0x4100 + (p->scene_name_index - 1) * 32, (unsigned char *)&p->scene_name, 8); // 屏幕显示场景名称
	write_dgusii_vp(0x3320 + (p->scene_name_index - 1) * 8, (unsigned char *)&p->scene_name, 8);
}
void scene_name_save(name_scene_t *p)
{
	scene_name_write_dgusii(p);
	scene_name_save_flash(p);
}
//
// 读取场景名称
void scene_name_read_flash(name_scene_t *p, unsigned char index)
{
	//	p->scene_name_index = index;//设置场景序号

	norflash_read(SCENE_NAME_FLASH_ADR_SATRT + index * NAME_SCENE_T_SIZE, (unsigned char *)p, NAME_SCENE_T_SIZE); // 读场景name参数
	write_dgusii_vp(0x4089, (unsigned char *)&p->scene_name, 8);																									// 显示设定的场景名称
}
// 删除场景列表名称
void delete_scene_list_name(unsigned char index)
{

	write_dgusii_vp(0x3320 + index * 8, (unsigned char *)&name_space, 8); // 清除场景名字
}
// 清空场景列表名称
void clear_scene_list_name(void)
{
	unsigned char i = 0;
	for (i = 0; i < SINGLE_SCENE_NAME_INFOR_SIZE; i++)
	{
		delete_scene_list_name(i);
	}
}
// 删除选中的场景名称
void scene_name_delete_flash(unsigned char index)
{
	//  unsigned char name_space[16] = "";
	T5L_Flash(0xA5, 0Xe000, SCENE_NAME_FLASH_ADR_SATRT + (index)*NAME_SCENE_T_SIZE, NAME_SCENE_T_SIZE);												 // 写0FLASHname清除
	T5L_Flash(0x5A, 0X4100 + 32 * (index), SCENE_NAME_FLASH_ADR_SATRT + (index)*NAME_SCENE_T_SIZE, 32);												 // 读为0场景名称信息
	T5L_Flash(0xA5, 0Xe000, SCENE_INFOR_FLASH_ADR_SATRT + (index)*SINGLE_NAME_SCENC_INFOR_SIZE, SINGLE_NAME_SCENC_INFOR_SIZE); // 写0FLASHscencinfor清除

	//	write_dgusii_vp(0x3320+index*8,(unsigned char *)&name_space,8);//清除场景名字
	//	clear_all_module_var_en(SCENE_NAME_LIST_SELECT_ADR,SCENE_NAME_NUB_LIMIT);
}
void scene_edit_clear_infor(name_scene_t *p_module_scene)
{
	clear_scene_list_name();																																																																		// 清除场景信息列表
	T5L_Flash(0xA5, 0Xe000, SCENE_INFOR_FLASH_ADR_SATRT + (p_module_scene->scene_name_index - 1) * SINGLE_NAME_SCENC_INFOR_SIZE, SINGLE_NAME_SCENC_INFOR_SIZE); // 写0FLASHscencinfor清除
	USER_PRINTF("clear_single_scene_name_flash \n");
	memset((unsigned char *)module_scene_select_key_bak, 0, 8);
	memset((unsigned char *)p_module_scene->module_select, 0, 8);
	p_module_scene->edit_sta = 2;
}
// 清除所有场景flash
void scene_name_clear_flash(void)
{
	unsigned char index = 0;
	if (0xff == if_all_scene_name_blank())
	{
		return;
	}
	for (index = 0; index < 12; index++)
	{

		T5L_Flash(0xA5, 0Xe000, SCENE_INFOR_FLASH_ADR_SATRT + (index)*0x1000, 0x1000); // 写0FLASHscencinfor清除
	}
	T5L_Flash(0xA5, 0Xe000, SCENE_NAME_FLASH_ADR_SATRT, SCENE_NAME_NUB_MAX * NAME_SCENE_T_SIZE); // 写0FLASHname清除
	T5L_Flash(0x5A, 0X4100, SCENE_NAME_FLASH_ADR_SATRT, 32 * 64);																 // 清空场景管理场景名称
																																															 //	for(index = 0;index<SCENE_NAME_NUB_LIMIT;index++)
																																															 //	{
																																															 //		write_dgusii_vp(0x4100+(p->scene_name_index-1)*32,(unsigned char *)&p->scene_name,8);//屏幕显示场景名称
																																															 //
																																															 //	}
}
// 存储场景信息内容
void scene_infor_save_flash(name_scene_t *p_module_scene)
{

	//  p->index = index;//设置场景信息标号
	//	p->type  = p_module->type;
	//	p->adr   = p_module->adr;
	//  p->channel_nb = p_module->channel_nb;
	//	unsigned char index = 0;
	//	index = (p_module_scene->scene_name_index-1);
	printf_temp_scenc_infor(10);
	norflash_write(SCENE_INFOR_FLASH_ADR_SATRT + (p_module_scene->scene_name_index - 1) * SINGLE_NAME_SCENC_INFOR_SIZE, (unsigned char *)(temp_scenc_infor), SINGLE_NAME_SCENC_INFOR_SIZE);
	USER_PRINTF("-->scene_infor_save_flash_success \n");
}
// 清空一个场景所有信息内容
void scene_infor_clear_flash(scenc_infor_t *p)
{
	T5L_Flash(0xA5, 0Xe000, SCENE_INFOR_FLASH_ADR_SATRT + (p->scene_infor_index - 1) * SINGLE_NAME_SCENC_INFOR_SIZE, SINGLE_NAME_SCENC_INFOR_SIZE); // 写0FLASHscencinfor清除
	T5L_Flash(0x5A, 0X5000, SCENE_INFOR_FLASH_ADR_SATRT + (p->scene_infor_index - 1) * SINGLE_NAME_SCENC_INFOR_SIZE, SINGLE_NAME_SCENC_INFOR_SIZE); // 读为0场景信息
																																																																									// USER_PRINTF("-->clear %bd scene all flash ok\n", p->scene_infor_index);
}
// 删除一个场景单个信息flash
void scene_infor_delete_flash(scenc_infor_t *p)
{
	T5L_Flash(0xA5, 0Xe000, SCENE_INFOR_FLASH_ADR_SATRT + (p->scene_infor_index - 1) * SINGLE_NAME_SCENC_INFOR_SIZE + (p->index - 1) * SCENC_INFOR_T_SIZE, SCENC_INFOR_T_SIZE); // 写0FLASH清除
																																																																																							//	T5L_Flash(0x5A,0X5000, SCENE_INFOR_FLASH_ADR_SATRT+(p->scene_infor_index-1)*SINGLE_NAME_SCENC_INFOR_SIZE+(p->index-1)*SCENC_INFOR_T_SIZE,SCENC_INFOR_T_SIZE); //读为0场景信息
																																																																																							// USER_PRINTF("-->delete %bd scene %d infor flash ok\n", p->scene_infor_index, p->index);
}
// 模块场景信息显示
void module_scene_infor_dis(scenc_infor_t *p)
{
	unsigned char i = 0;
	unsigned char temp_tab[32] = {0};
	switch (p->type)
	{
	case SINGLE_RELAY_MODULE:
	case SINGLE_TIME_RELAY_MODULE:
	case DOUBLE_RELAY_MODULE:
	case DOUBLE_TIME_RELAY_MODULE:
		for (i = 0; i < 16; i++)
		{
			temp_tab[2 * i + 1] = (p->led_enable >> i) & 0x1;
			;
		}
		write_dgusii_vp(0x101c, (unsigned char *)&temp_tab, 16);
		for (i = 0; i < 16; i++)
		{
			temp_tab[2 * i + 1] = (p->led_sta >> i) & 0x1;
			;
		}
		write_dgusii_vp(0x1040, (unsigned char *)&temp_tab, 16);

		break;
	case VOL_DIM_MODULE:
		for (i = 0; i < 4; i++)
		{
			temp_tab[2 * i + 1] = p->diming_enable >> i & 0x1;
		}
		write_dgusii_vp(0x1050, (unsigned char *)&temp_tab, 4);
		write_dgusii_vp(0x1054, (unsigned char *)&(p->dimming_duty[0]), 4);

		break;
	case SCR_DIM_MODULE:
		for (i = 0; i < 4; i++)
		{
			temp_tab[2 * i + 1] = p->diming_enable >> i & 0x1;
		}
		write_dgusii_vp(0x1058, (unsigned char *)&temp_tab, 4);

		write_dgusii_vp(0x105c, (unsigned char *)&(p->dimming_duty[0]), 4);

		break;
	}
}
// 显示继电器场景界面
void display_relay_scene_interface(module_t *p)
{
	write_dgus(0x3f08, p->adr);
	write_dgus(0x3f09, p->channel_nb);
	write_dgusii_vp(0x3f00, (unsigned char *)&p->module_name, 8);
	write_dgusii_vp(0x3f0a, (unsigned char *)&p->channel_name, 8 * 16);
	// USER_PRINTF("display module index is %bd\n", p->index);
	switch (p->channel_nb)
	{
	case 4:

		pic_set(24);
		break;
	case 6:

		pic_set(25);
		break;
	case 8:

		pic_set(26);
		break;
	case 10:

		pic_set(27);
		break;
	case 12:

		pic_set(28);
		break;
	case 14:

		pic_set(29);
		break;
	case 16:

		pic_set(30);
		break;
	default:
		break;
	}
}
// 显示调光模块界面
void display_dim_scene_interface(module_t *p)
{
	write_dgus(0x3f08, p->adr);
	write_dgus(0x3f09, p->channel_nb);
	write_dgusii_vp(0x3f00, (unsigned char *)&p->module_name, 8);
	write_dgusii_vp(0x3f0a, (unsigned char *)&p->channel_name, 8 * 16);
	// USER_PRINTF("display module index is %bd\n", p->index);
	switch (p->type)
	{
	case VOL_DIM_MODULE:
		pic_set(SCENE_DIM_PAGE);
		break;
	case SCR_DIM_MODULE:
		pic_set(SCENE_SCR_PAGE);
		break;
	default:
		break;
	}
}
// 显示设置界面
void display_scene_interface(module_t *p)
{
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

		display_relay_scene_interface(p);
		USER_PRINTF("display_relay_scene_interface\n");
		//				relay_read(p);
		break;
	case VOL_DIM_MODULE:
	case SCR_DIM_MODULE:
		//				clear_touch_sta();
		display_dim_scene_interface(p);
		USER_PRINTF("display_dim_scene_interface\n");
		//				dim_read(p);
		break;
		break;
	default:

		break;
	}
}
// 选择模块进行场景设置
void module_scene_select(module_t *p, unsigned char key)
{
	norflash_read((key - 1) * 140, (unsigned char *)p, 140);
	//	USER_PRINTF("module_scene.scene_name_index -->%bd\n",module_scene.scene_name_index);
	display_scene_interface(p);
}
// 选择模块进行场景设置
void module_scene_select_ctrl(scenc_infor_t *p_scenc_infor)
{
	unsigned short key_nb = 0;
	unsigned short size_cal = 0;
	// static unsigned char i = 0;
	// if (0 == i)
	// {
	// 	i = 1;
	// 	size_cal = SCENC_INFOR_T_SIZE; //

	// 	USER_PRINTF("scenc_infor_t size is %d\n", size_cal);
	// 	size_cal = NAME_SCENE_T_SIZE;
	// 	USER_PRINTF("name_scene_t size is %d\n", size_cal);
	// 	size_cal = SINGLE_TIMING_SCENC_INFOR_T_SIZE;
	// 	USER_PRINTF("timing_scenc_infor_t size is %d\n", size_cal);
	// 	//		size_cal = SINGLE_TIMING_SCENC_INFOR_T_SIZE*2;

	// 	USER_PRINTF("MODULE_FLASH_ADR_END is 0x%hx\n", MODULE_FLASH_ADR_END);
	// 	USER_PRINTF("SCENE_NAME_FLASH_ADR_END is 0x%hx\n", SCENE_NAME_FLASH_ADR_END);
	// 	USER_PRINTF("TIMING_SCENE_INFOR_FLASH_ADR_SATRT is 0x%hx\n", TIMING_SCENE_INFOR_FLASH_ADR_SATRT);
	// }
	get_key_value(0x101b, &key_nb);
	if ((key_nb) && (0 == GET_BIT(module_scene_select_key_bak[((key_nb - 1) / 8)], (key_nb - 1) % 8)) && (0x31 == p_scenc_infor->edit_sta))
	{

		// USER_PRINTF("module_%d_select_ctrl_add\n", key_nb);
		//		printf_tab(512,module_scene_select_key_bak);
		temp_module_index = key_nb;
		//		SET_BIT(module_scene_select_key_bak[(p_module_scene->scene_name_index-1)+((key_nb-1)/8)],(key_nb-1)%8);
		//		clear_led_sta(led_sta);
		// USER_PRINTF("module_scene.scene_name_index -->%bd\n", module_scene.scene_name_index);
		module_scene_select(&st_module, key_nb);

		clear_key_value(0x101b, &key_nb);
		key_nb = 0;
	}
	else if ((key_nb) && (0 == GET_BIT(module_scene_select_key_bak[((key_nb - 1) / 8)], (key_nb - 1) % 8)) && (0x51 == p_scenc_infor->edit_sta))
	{
		// USER_PRINTF("module_scene.scene_name_index -->%bd\n", module_scene.scene_name_index);
		USER_PRINTF("module_scene_select_ctrl_modtify\n");
		//		printf_tab(512,module_scene_select_key_bak);
		temp_module_index = key_nb;
		//		SET_BIT(module_scene_select_key_bak[(p_module_scene->scene_name_index-1)+((key_nb-1)/8)],(key_nb-1)%8);
		//		clear_led_sta(led_sta);
		module_scene_select(&st_module, key_nb);
		clear_key_value(0x101b, &key_nb);
		key_nb = 0;
	}
}
// 设置场景继电器模块信息
void module_relay_scene_set(module_t *p, scenc_infor_t *p_scenc_infor, name_scene_t *p_module_scene, unsigned short index, unsigned char edit_sta)
{
	//	unsigned short scene_led_enable = 0,scene_led_sta = 0;
	//	unsigned short led_hr_index_start = 0;
	unsigned char temp_tab[32] = {0};
	if ((SINGLE_RELAY_MODULE == p->type) || (SINGLE_TIME_RELAY_MODULE == p->type))
	{
		p_scenc_infor->led_hr_index_start = 337;
	}
	else if ((DOUBLE_RELAY_MODULE == p->type) || (DOUBLE_TIME_RELAY_MODULE == p->type))
	{
		p_scenc_infor->led_hr_index_start = 400;
	}
	else
	{
		return;
	}
	p_scenc_infor->scene_infor_index = p_module_scene->scene_name_index;
	p_scenc_infor->type = p->type;
	p_scenc_infor->adr = p->adr;
	p_scenc_infor->channel_nb = p->channel_nb;
	p_scenc_infor->module_index = p->index;
	p_scenc_infor->data_sta = 1;
	p_scenc_infor->index = index + 1;
	p_scenc_infor->edit_sta = edit_sta & 0x0f;
	//	get_led_set_sta(&p_scenc_infor->led_enable,&p_scenc_infor->led_sta);
	switch (p_scenc_infor->channel_nb)
	{
	case 4:
		read_dgusii_vp(0x101c, (unsigned char *)temp_tab, 16);
		//	USER_PRINTF("function_module_relay_scene_send--->%x\n",scene_led_enable);
		p_scenc_infor->led_enable = temp_tab[1] | temp_tab[3] << 1 | temp_tab[5] << 2 | temp_tab[7] << 3;
		read_dgusii_vp(0x1040, (unsigned char *)temp_tab, 16);
		p_scenc_infor->led_sta = temp_tab[1] | temp_tab[3] << 1 | temp_tab[5] << 2 | temp_tab[7] << 3;
		break;
	case 6:
		read_dgusii_vp(0x101c, (unsigned char *)temp_tab, 16);
		//	USER_PRINTF("function_module_relay_scene_send--->%x\n",scene_led_enable);
		p_scenc_infor->led_enable = temp_tab[1] | temp_tab[3] << 1 | temp_tab[5] << 2 | temp_tab[7] << 3 | temp_tab[9] << 4 | temp_tab[11] << 5;
		read_dgusii_vp(0x1040, (unsigned char *)temp_tab, 16);
		p_scenc_infor->led_sta = temp_tab[1] | temp_tab[3] << 1 | temp_tab[5] << 2 | temp_tab[7] << 3 | temp_tab[9] << 4 | temp_tab[11] << 5;
		break;
	case 8:
		read_dgusii_vp(0x101c, (unsigned char *)temp_tab, 16);
		//	USER_PRINTF("function_module_relay_scene_send--->%x\n",scene_led_enable);
		p_scenc_infor->led_enable = temp_tab[1] | temp_tab[3] << 1 | temp_tab[5] << 2 | temp_tab[7] << 3 | temp_tab[9] << 4 | temp_tab[11] << 5 | temp_tab[13] << 6 | temp_tab[15] << 7;
		read_dgusii_vp(0x1040, (unsigned char *)temp_tab, 16);
		p_scenc_infor->led_sta = temp_tab[1] | temp_tab[3] << 1 | temp_tab[5] << 2 | temp_tab[7] << 3 | temp_tab[9] << 4 | temp_tab[11] << 5 | temp_tab[13] << 6 | temp_tab[15] << 7;
		break;
	case 10:
		read_dgusii_vp(0x101c, (unsigned char *)temp_tab, 16);
		//	USER_PRINTF("function_module_relay_scene_send--->%x\n",scene_led_enable);
		p_scenc_infor->led_enable = temp_tab[1] | temp_tab[3] << 1 | temp_tab[5] << 2 | temp_tab[7] << 3 | temp_tab[9] << 4 | temp_tab[11] << 5 | temp_tab[13] << 6 | temp_tab[15] << 7 |
																temp_tab[17] << 8 | temp_tab[19] << 9;
		read_dgusii_vp(0x1040, (unsigned char *)temp_tab, 16);
		p_scenc_infor->led_sta = temp_tab[1] | temp_tab[3] << 1 | temp_tab[5] << 2 | temp_tab[7] << 3 | temp_tab[9] << 4 | temp_tab[11] << 5 | temp_tab[13] << 6 | temp_tab[15] << 7 |
														 temp_tab[17] << 8 | temp_tab[19] << 9;
		break;
	case 12:
		read_dgusii_vp(0x101c, (unsigned char *)temp_tab, 16);
		//	USER_PRINTF("function_module_relay_scene_send--->%x\n",scene_led_enable);
		p_scenc_infor->led_enable = temp_tab[1] | temp_tab[3] << 1 | temp_tab[5] << 2 | temp_tab[7] << 3 | temp_tab[9] << 4 | temp_tab[11] << 5 | temp_tab[13] << 6 | temp_tab[15] << 7 |
																temp_tab[17] << 8 | temp_tab[19] << 9 | temp_tab[21] << 10 | temp_tab[23] << 11;
		read_dgusii_vp(0x1040, (unsigned char *)temp_tab, 16);
		p_scenc_infor->led_sta = temp_tab[1] | temp_tab[3] << 1 | temp_tab[5] << 2 | temp_tab[7] << 3 | temp_tab[9] << 4 | temp_tab[11] << 5 | temp_tab[13] << 6 | temp_tab[15] << 7 |
														 temp_tab[17] << 8 | temp_tab[19] << 9 | temp_tab[21] << 10 | temp_tab[23] << 11;
		break;
	case 14:
		read_dgusii_vp(0x101c, (unsigned char *)temp_tab, 16);
		//	USER_PRINTF("function_module_relay_scene_send--->%x\n",scene_led_enable);
		p_scenc_infor->led_enable = temp_tab[1] | temp_tab[3] << 1 | temp_tab[5] << 2 | temp_tab[7] << 3 | temp_tab[9] << 4 | temp_tab[11] << 5 | temp_tab[13] << 6 | temp_tab[15] << 7 |
																temp_tab[17] << 8 | temp_tab[19] << 9 | temp_tab[21] << 10 | temp_tab[23] << 11 | temp_tab[25] << 12 | temp_tab[27] << 13;
		read_dgusii_vp(0x1040, (unsigned char *)temp_tab, 16);
		p_scenc_infor->led_sta = temp_tab[1] | temp_tab[3] << 1 | temp_tab[5] << 2 | temp_tab[7] << 3 | temp_tab[9] << 4 | temp_tab[11] << 5 | temp_tab[13] << 6 | temp_tab[15] << 7 |
														 temp_tab[17] << 8 | temp_tab[19] << 9 | temp_tab[21] << 10 | temp_tab[23] << 11 | temp_tab[25] << 12 | temp_tab[27] << 13;
	case 16:
		read_dgusii_vp(0x101c, (unsigned char *)temp_tab, 16);
		//	USER_PRINTF("function_module_relay_scene_send--->%x\n",scene_led_enable);
		p_scenc_infor->led_enable = temp_tab[1] | temp_tab[3] << 1 | temp_tab[5] << 2 | temp_tab[7] << 3 | temp_tab[9] << 4 | temp_tab[11] << 5 | temp_tab[13] << 6 | temp_tab[15] << 7 |
																temp_tab[17] << 8 | temp_tab[19] << 9 | temp_tab[21] << 10 | temp_tab[23] << 11 | temp_tab[25] << 12 | temp_tab[27] << 13 | temp_tab[29] << 14 | temp_tab[31] << 15;
		read_dgusii_vp(0x1040, (unsigned char *)temp_tab, 16);
		p_scenc_infor->led_sta = temp_tab[1] | temp_tab[3] << 1 | temp_tab[5] << 2 | temp_tab[7] << 3 | temp_tab[9] << 4 | temp_tab[11] << 5 | temp_tab[13] << 6 | temp_tab[15] << 7 |
														 temp_tab[17] << 8 | temp_tab[19] << 9 | temp_tab[21] << 10 | temp_tab[23] << 11 | temp_tab[25] << 12 | temp_tab[27] << 13 | temp_tab[29] << 14 | temp_tab[31] << 15;
		break;
	}

	USER_PRINTF("-->module_relay_scene_set\n");
}

// 设置场景调光模块信息
void module_dim_scene_set(module_t *p, scenc_infor_t *p_scenc_infor, name_scene_t *p_module_scene, unsigned short index, unsigned char edit_sta)
{
	unsigned short scene_dim_enable = 0;
	unsigned short scene_dim_enable_adr = 0, scene_dim_gear_adr = 0;
	unsigned char temp_tab[8] = {0}, dim_tab[4] = {0};
	if ((VOL_DIM_MODULE == p->type))
	{
		scene_dim_enable_adr = 0x1050;
		scene_dim_gear_adr = 0x1054;
	}
	else if ((SCR_DIM_MODULE == p->type))
	{
		scene_dim_enable_adr = 0x1058;
		scene_dim_gear_adr = 0x105c;
	}
	else
	{
		return;
	}
	p_scenc_infor->type = p->type;
	p_scenc_infor->adr = p->adr;
	p_scenc_infor->channel_nb = p->channel_nb;
	p_scenc_infor->module_index = p->index;
	p_scenc_infor->data_sta = 1;
	p_scenc_infor->index = index + 1;
	p_scenc_infor->edit_sta = edit_sta & 0x0f;
	read_dgusii_vp(scene_dim_enable_adr, (unsigned char *)temp_tab, 4);
	p_scenc_infor->diming_enable = scene_dim_enable = temp_tab[1] | temp_tab[3] << 1 | temp_tab[5] << 2 | temp_tab[7] << 3;
	// USER_PRINTF("scene_dim_enable value--->%x\n", scene_dim_enable);
	read_dgusii_vp(scene_dim_gear_adr, (unsigned char *)temp_tab, 4);
	p_scenc_infor->dimming_duty[0] = dim_tab[0] = temp_tab[1];
	p_scenc_infor->dimming_duty[1] = dim_tab[1] = temp_tab[3];
	p_scenc_infor->dimming_duty[2] = dim_tab[2] = temp_tab[5];
	p_scenc_infor->dimming_duty[3] = dim_tab[3] = temp_tab[7];
	p_scenc_infor->scene_infor_index = p_module_scene->scene_name_index;
	printf_tab(4, dim_tab);
}
// 继电器模块场景设定信息modbus发送数据
void module_relay_scene_send(scenc_infor_t *p_scenc_infor)
{
	if ((SINGLE_RELAY_MODULE == p_scenc_infor->type) || (SINGLE_TIME_RELAY_MODULE == p_scenc_infor->type) || (DOUBLE_RELAY_MODULE == p_scenc_infor->type) || (DOUBLE_TIME_RELAY_MODULE == p_scenc_infor->type))
	{
		set_master_send_hr(master_send_hr, p_scenc_infor->led_hr_index_start, p_scenc_infor->scene_infor_index << 8 | p_scenc_infor->edit_sta);
		// USER_PRINTF("scene_led_enable value--->%x\n", p_scenc_infor->led_enable);
		set_master_send_hr(master_send_hr, p_scenc_infor->led_hr_index_start + 1, p_scenc_infor->led_enable);
		set_master_send_hr(master_send_hr, p_scenc_infor->led_hr_index_start + 2, p_scenc_infor->led_sta);
		// USER_PRINTF("scene_led_sta value--->%x\n", p_scenc_infor->led_sta);
		set_master_send_hr(master_send_hr, p_scenc_infor->led_hr_index_start + 3, ENTERPRISE);
		pack_data_send(&user_modbus, p_scenc_infor->adr, MD_FR_MHR, p_scenc_infor->led_hr_index_start, 4);
	}
	else
	{
		return;
	}
}
// 调光模块场景设定信息modbus发送数据
void module_dim_scene_send(scenc_infor_t *p_scenc_infor)
{
	if ((VOL_DIM_MODULE == p_scenc_infor->type) || (SCR_DIM_MODULE == p_scenc_infor->type))
	{
		set_master_send_hr(master_send_hr, 50, p_scenc_infor->scene_infor_index << 8 | p_scenc_infor->edit_sta);
		set_master_send_hr(master_send_hr, 51, p_scenc_infor->diming_enable);
		set_master_send_hr(master_send_hr, 52, p_scenc_infor->dimming_duty[0]);
		set_master_send_hr(master_send_hr, 53, p_scenc_infor->dimming_duty[1]);
		set_master_send_hr(master_send_hr, 54, p_scenc_infor->dimming_duty[2]);
		set_master_send_hr(master_send_hr, 55, p_scenc_infor->dimming_duty[3]);
		set_master_send_hr(master_send_hr, 56, ENTERPRISE);
		pack_data_send(&user_modbus, p_scenc_infor->adr, MD_FR_MHR, 50, 7);
	}
	else
	{
		return;
	}
}
void scene_delete_ctrl_send(name_scene_t *p)
{
	//	static unsigned char send_cnt
	//	unsigned char index = 0;
	if (2 == p->edit_sta) // 删除
	{

		if (0 == p->scene_name_index)
		{
			USER_PRINTF("-->no scene need to delete \n");
			//			mbHost.scene_send_cyc_time = 0;
			scene_data.delete_scenc_send_cyc_flag = 0;
			pop_menu_key_ctrl(0xf0); // 关闭操作弹窗
			p->edit_sta = 0;
			return;
		}
		scene_data.delete_scenc_send_cyc_flag = 1;
	}
	if (1 == scene_data.delete_scenc_send_cyc_flag)
	{
		if ((0 == scene_data.delete_scenc_send_cyc_time) && (mbh_getState() == MBH_STATE_IDLE))
		{
			scene_data.delete_scenc_send_cyc_time = 100;
			if (FULL == ((temp_scenc_infor + scene_data.delete_scenc_send_cnt)->data_sta))
			{
				if (1 == infor_clear_sta)
					(temp_scenc_infor + scene_data.delete_scenc_send_cnt)->edit_sta = 2;
				module_relay_scene_send(temp_scenc_infor + scene_data.delete_scenc_send_cnt);
				module_dim_scene_send(temp_scenc_infor + scene_data.delete_scenc_send_cnt);
				scene_data.delete_scenc_send_cnt++;
			}
			else
			{
				scene_data.delete_scenc_send_cnt++;
			}
		}
	}
	if (scene_data.delete_scenc_send_cnt > SINGLE_SCENE_NAME_INFOR_SIZE)
	{
		scene_data.delete_scenc_send_cnt = 0;
		scene_data.delete_scenc_send_cyc_time = 0;
		scene_data.delete_scenc_send_cyc_flag = 0;
		p->edit_sta = 0;
		infor_clear_sta = 0;
		// USER_PRINTF("-->%bd scene name delete ok!\n", p->scene_name_index);
		clear_temp_scenc_infor();
	}
}
// 场景名字列表操作modbus发送数据
void scene_name_ctrl_send(name_scene_t *p)
{
	static unsigned char i = 0;
	unsigned char j = 0;
	unsigned char index = 0;
	scenc_infor_t scenc_infor[4] = {
			{0, 0, 0, SINGLE_RELAY_MODULE, 0, 0, 0, 0, 0, 0, 337, 0, 0},
			{0, 0, 0, DOUBLE_RELAY_MODULE, 0, 0, 0, 0, 0, 0, 400, 0, 0},
			{0, 0, 0, VOL_DIM_MODULE, 0, 0, 0, 0, 0, 0, 0, 0, 0},
			{0, 0, 0, SCR_DIM_MODULE, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	};
	//  USER_PRINTF("module_scene.edit_sta is %bd\n",p->edit_sta);
	//	index = get_scene_name_select_sequence_number();
	//	if(index)
	//	{
	//		USER_PRINTF("-->index is %bd\n",index);
	//	}
	if ((1 == p->edit_sta) || (0 == p->edit_sta))
	{
		//		mbHost.scene_send_cyc_time = 0;
		scene_data.scenc_send_name_cyc_flag = 0;
		return;
	}
	else if (3 == p->edit_sta) // 清空
	{
		if (0xff == if_all_scene_name_blank())
		{
			scene_data.scenc_send_name_cyc_flag = 0;
			pop_menu_key_ctrl(0xf0); // 关闭操作弹窗
			p->edit_sta = 0;
			return;
		}
		scenc_infor[0].edit_sta = 3;
		scenc_infor[1].edit_sta = 3;
		scenc_infor[2].edit_sta = 3;
		scenc_infor[3].edit_sta = 3;
	}
	//	else if(2 == p->edit_sta)//删除
	//	{

	//		if(0 == p->scene_name_index)
	//		{
	//			USER_PRINTF("-->delete index is zero\n");
	////			mbHost.scene_send_cyc_time = 0;
	//			scene_data.scenc_send_name_cyc_flag = 0;
	//			pop_menu_key_ctrl(0xf0);//关闭操作弹窗
	//			p->edit_sta = 0;
	//			return;
	//		}

	//		for(j = 0;j<4;j++)
	//		{
	//			scenc_infor[j].edit_sta = 2;
	//			scenc_infor[j].scene_infor_index = p->scene_name_index;
	//		}
	////		USER_PRINTF("-->enter_delete_state\n");
	//	}

	if ((1 == scene_data.scenc_send_name_cyc_flag) && (0 == mbHost.scene_send_cyc_time))
	{
		mbHost.scene_send_cyc_time = MBH_CYC_TIME;
		module_relay_scene_send(scenc_infor + i);
		module_dim_scene_send(scenc_infor + i);
		USER_PRINTF("-->scene_name_ctrl_send\n");
		i++;
	}
	if (i > 2)
	{
		// USER_PRINTF("module_scene.edit_sta is %bd\n", p->edit_sta);
		//		write_dgus(0x107f,0xff);
		i = 0;
		scene_data.scenc_send_name_cyc_flag = 0;
		//		mbHost.scene_send_cyc_time = 0;
		//		pic_set(1);

		p->edit_sta = 0;
		//		p->scene_name_index = 0;
		USER_PRINTF("-->scene_name_ctrl_send_over\n");
		//		T5L_Flash(0x5A,0X4100, SCENE_NAME_FLASH_ADR_SATRT,SCENE_NAME_NUB*NAME_SCENE_T_SIZE); //读为0场景名称信息
		//		USER_PRINTF("-->scene_display_name_clear_ok\n");
		pop_menu_key_ctrl(0xf0); // 关闭操作弹窗
	}
}
// 场景信息发送所有模块
void module_scene_send(void)
{
	static unsigned char i = 0;
	if ((1 == scene_data.scenc_send_infor_cyc_flag) && (0 == mbHost.scene_send_cyc_time))
	{
		//		USER_PRINTF("-->module_scene_send_cyc\n");
		//				USER_PRINTF("-->i value is %bd\n",i);
		if ((FULL == temp_scenc_infor[i].data_sta) && (0 != temp_scenc_infor[i].edit_sta))
		{
			mbHost.scene_send_cyc_time = MBH_CYC_TIME;
			module_relay_scene_send(temp_scenc_infor + i);
			module_dim_scene_send(temp_scenc_infor + i);
			USER_PRINTF("-->module_scene_send\n");
			i++;
		}
		else if (BLANK == temp_scenc_infor[i++].data_sta)
		{
			//			USER_PRINTF("-->BLANK == temp_scenc_infor[i].data_sta\n");
			//			mbHost.scene_send_cyc_time = 200;
			;
		}
	}
	if (i > 63)
	{
		//		CLEAR_BIT(module_scene_select_key_bak[(module_scene.scene_name_index-1)*((temp_module_index-1)/8)],(temp_module_index-1)%8);
		memset((unsigned char *)module_scene_select_key_bak, 0, 8);
		temp_module_index = 0;
		i = 0;
		mbHost.scene_send_cyc_time = 0;
		scene_data.scenc_send_infor_cyc_flag = 0;
		clear_temp_scenc_infor(); // 清空场景信息临时变量
		clear_all_scene_infor();	// 清空场景信息显示
		pop_menu_key_ctrl(0xf0);
		// pic_set(22);
	}
}
// 显示具体场景执行信息
void display_scene_infor(scenc_infor_t *p_scenc_infor)
{
	unsigned char mod_infom_tab[141] = {0};
	unsigned char module_name_1[17] = {0};
	unsigned char led_on_tb[49] = {0};
	unsigned char led_off_tb[49] = {0};
	unsigned short on_sta = 0, off_sta = 0;
	unsigned char i = 0, j = 0, k = 0, l = 0;
	unsigned char dim_duty_tb[40] = {0};
	unsigned char dim_duty_str[10] = {0};
	unsigned char str_len_cal = 0;
	unsigned char str_adr = 0;
	char channel[48] = "01,02,03,04,05,06,07,08,09,10,11,12,13,14,15,16,";
	module_t module_read = {0};
	if ((p_scenc_infor->type < 2) || (4 == p_scenc_infor->type) || (5 == p_scenc_infor->type))
	{
		on_sta = p_scenc_infor->led_enable & (p_scenc_infor->led_sta);
		// USER_PRINTF("on_sta is 0x%x\n", on_sta);
		off_sta = p_scenc_infor->led_enable & (~p_scenc_infor->led_sta);
		// USER_PRINTF("off_sta is 0x%x\n", off_sta);
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
		// USER_PRINTF("led_on_tb is %s\n", led_on_tb);
		// USER_PRINTF("led_off_tb is %s\n", led_off_tb);
		//		strncpy(module_name_1,&module_name_tab[p_scenc_infor->type],8);
	}
	else
	{
		for (i = 0; i < 4; i++)
		{
			if (GET_BIT(p_scenc_infor->diming_enable, i))
			{

				sprintf(dim_duty_str, "%d%%;", p_scenc_infor->type == 2 ? p_scenc_infor->dimming_duty[i] * 10 : p_scenc_infor->dimming_duty[i]);

				str_len_cal = strlen(dim_duty_str);
				// USER_PRINTF("dim_duty_str str_len_cal is %bd\n", str_len_cal);
				strncpy(dim_duty_tb + str_adr, &channel[i * 3], 3);
				str_adr += 3;
				// USER_PRINTF("dim_duty_str is %s\n", dim_duty_str);
				strncpy(dim_duty_tb + str_adr, &dim_duty_str, str_len_cal);
				str_adr += str_len_cal;
				l++;
			}
		}
		// USER_PRINTF("dim_duty_infor is %s\n", dim_duty_tb);
		//		strncpy(module_name_1,&module_name_tab[p_scenc_infor->type],10);
	}

	// USER_PRINTF("-->display_scene_infor p_scenc_infor->module_index is %bd\n", p_scenc_infor->module_index);
	norflash_read((p_scenc_infor->module_index) * 140, (unsigned char *)&module_read, 140);

	strncpy(module_name_1, &module_read.module_name[0], 16);
	strrpl(module_name_1, " ", "");

	// if((on_sta)&&(off_sta))
	// 	sprintf(mod_infom_tab,"%s;地址:%bd;%bd路;%s开;%s关",module_name_1,p_scenc_infor->adr,p_scenc_infor->channel_nb,led_on_tb,led_off_tb);
	// else if((on_sta)&&( 0 == off_sta))
	// 	sprintf(mod_infom_tab,"%s;地址:%bd;%bd路;%s开",module_name_1,p_scenc_infor->adr,p_scenc_infor->channel_nb,led_on_tb);
	// else if(( 0 == on_sta)&&(off_sta))
	// 	sprintf(mod_infom_tab,"%s;地址:%bd;%bd路;%s关",module_name_1,p_scenc_infor->adr,p_scenc_infor->channel_nb,led_off_tb);
	// else if(p_scenc_infor->diming_enable)
	// 	sprintf(mod_infom_tab,"%s;地址:%bd;%bd路;%s",module_name_1,p_scenc_infor->adr,p_scenc_infor->channel_nb,dim_duty_tb);
	// else
	// 	sprintf(mod_infom_tab,"%s;地址:%bd;%bd路;未配置场景",module_name_1,p_scenc_infor->adr,p_scenc_infor->channel_nb);
	if ((on_sta) && (off_sta))
		sprintf(mod_infom_tab, "地址:%bd;%bd路;%s开;%s关;模块:%s", p_scenc_infor->adr, p_scenc_infor->channel_nb, led_on_tb, led_off_tb, module_name_1);
	else if ((on_sta) && (0 == off_sta))
		sprintf(mod_infom_tab, "地址:%bd;%bd路;%s开;模块:%s", p_scenc_infor->adr, p_scenc_infor->channel_nb, led_on_tb, module_name_1);
	else if ((0 == on_sta) && (off_sta))
		sprintf(mod_infom_tab, "地址:%bd;%bd路;%s关;模块:%s", p_scenc_infor->adr, p_scenc_infor->channel_nb, led_off_tb, module_name_1);
	else if (p_scenc_infor->diming_enable)
		sprintf(mod_infom_tab, "地址:%bd;%bd路;%s;模块:%s", p_scenc_infor->adr, p_scenc_infor->channel_nb, dim_duty_tb, module_name_1);
	else
		sprintf(mod_infom_tab, "地址:%bd;%bd路;未配置场景;%s", p_scenc_infor->adr, p_scenc_infor->channel_nb, module_name_1);

	// USER_PRINTF("-->module_message %s\n", mod_infom_tab);
	//	USER_PRINTF("-->module->index %bd\n",p_scenc_infor->index);
	write_dgusii_vp(0x5000 + (p_scenc_infor->index - 1) * 70, (unsigned char *)mod_infom_tab, 43);
	// USER_PRINTF("-->display_scene_infor_scenc_infor_index %d\n", p_scenc_infor->index);
}
// 场景列表添加模块信息
void add_module_scene_infor(module_t *p, scenc_infor_t *p_scenc_infor, name_scene_t *p_module_scene)
{
	unsigned short key_nb = 0;
	unsigned char i = 0;
	unsigned short j = 0;
	//	unsigned char temp_index = 0;
	get_key_value(0x102e, &key_nb);
	if ((1 == key_nb) && (0x31 == p_scenc_infor->edit_sta)) // 确认增加
	{
		// USER_PRINTF("p_scenc_infor->edit_sta -->%bx\n", p_scenc_infor->edit_sta);
		//		p_scenc_infor->edit_sta = 1;
		p_scenc_infor->scene_infor_index = p_module_scene->scene_name_index;
		//		USER_PRINTF("p_module_scene->scene_name_index -->%bd\n",p_module_scene->scene_name_index);
		// USER_PRINTF("p_scenc_infor->scene_infor_index -->%bd\n", p_scenc_infor->scene_infor_index);
		p_scenc_infor->index = get_blank_scene_infor_index();
		// USER_PRINTF("get_scene_infor_blank_index -->%d\n", p_scenc_infor->index);
		module_relay_scene_set(p, temp_scenc_infor + p_scenc_infor->index, p_module_scene, p_scenc_infor->index, p_scenc_infor->edit_sta);
		module_dim_scene_set(p, temp_scenc_infor + p_scenc_infor->index, p_module_scene, p_scenc_infor->index, p_scenc_infor->edit_sta);
		display_scene_infor(temp_scenc_infor + p_scenc_infor->index);
		clear_key_value(0x102e, &key_nb);
		//	unsigned short index;
		//	unsigned char  type;
		//	unsigned char  adr;
		//	unsigned char  channel_nb;
		//	unsigned char  edit_sta;
		// USER_PRINTF("module_t_index -->%bd\n", p->index);
		// printf_temp_scenc_infor(10);
		//		SET_BIT(module_scene_select_key_bak[(p_module_scene->scene_name_index-1)+((p->index)/8)],(p->index)%8);
		//		SET_BIT(p_module_scene->module_select[(p_module_scene->scene_name_index-1)+((p->index)/8)],(p->index)%8);
		SET_BIT(module_scene_select_key_bak[(p->index) / 8], (p->index) % 8);
		SET_BIT(p_module_scene->module_select[(p->index) / 8], (p->index) % 8);
		write_dgus(0X1200 + p->index, 1);
		//		j = (p_module_scene->scene_name_index-1)*8+((p->index)/8);
		j = ((p->index) / 8);
		if (p_scenc_infor->index >= 5)
		{
			scene_infor_page = SINGLE_SCENE_INFOR_LIST_SECOND_PAGE + p_scenc_infor->index / 5 - 1;
			pic_set(scene_infor_page);
		}
		else
		{
			scene_infor_page = SINGLE_SCENE_INFOR_LIST_FIRST_PAGE;
			pic_set(scene_infor_page);
		}
		// USER_PRINTF("module_scene_select_key_bak[%d] is %bd\n", j, module_scene_select_key_bak[j]);
		//		printf_tab(512,module_scene_select_key_bak);
	}
	else if ((2 == key_nb) && (0x31 == p_scenc_infor->edit_sta)) // 取消增加
	{
		// USER_PRINTF("p_scenc_infor->edit_sta -->%bx\n", p_scenc_infor->edit_sta);
		//		CLEAR_BIT(module_scene_select_key_bak[(p_module_scene->scene_name_index-1)+((p->index)/8)],(p->index)%8);
		//		CLEAR_BIT(p_module_scene->module_select[(p_module_scene->scene_name_index-1)+((p->index)/8)],(p->index)%8);
		//		CLEAR_BIT(module_scene_select_key_bak[((p->index)/8)],(p->index)%8);
		//		CLEAR_BIT(p_module_scene->module_select[((p->index)/8)],(p->index)%8);
		//		write_dgus(0X1200+p->index,0);
		clear_key_value(0x102e, &key_nb);
		temp_module_index = 0;
		pic_set(scene_infor_page);
		//		printf_tab(512,module_scene_select_key_bak);
	}
	else if ((1 == key_nb) && (0x51 == p_scenc_infor->edit_sta)) // 确认修改
	{
		// USER_PRINTF("p_scenc_infor->edit_sta -->%bx\n", p_scenc_infor->edit_sta);
		// USER_PRINTF("-->current infor index is %d\n", p_scenc_infor->index);
		// USER_PRINTF("module_t_index -->%bd\n", p->index);
		//		SET_BIT(module_scene_select_key_bak[(p_module_scene->scene_name_index-1)+((p->index)/8)],(p->index)%8);
		//		SET_BIT(p_module_scene->module_select[(p_module_scene->scene_name_index-1)+((p->index)/8)],(p->index)%8);
		SET_BIT(module_scene_select_key_bak[(p->index) / 8], (p->index) % 8);
		SET_BIT(p_module_scene->module_select[(p->index) / 8], (p->index) % 8);
		write_dgus(0X1200 + p->index, 1);

		module_relay_scene_set(p, temp_scenc_infor + p_scenc_infor->index, p_module_scene, p_scenc_infor->index, p_scenc_infor->edit_sta);
		module_dim_scene_set(p, temp_scenc_infor + p_scenc_infor->index, p_module_scene, p_scenc_infor->index, p_scenc_infor->edit_sta);
		display_scene_infor(temp_scenc_infor + p_scenc_infor->index);
		clear_key_value(0x102e, &key_nb);
		//		j = (p_module_scene->scene_name_index-1)*8+((p->index)/8);
		j = ((p->index) / 8);
		if (p_scenc_infor->index >= 5)
		{
			scene_infor_page = SINGLE_SCENE_INFOR_LIST_SECOND_PAGE + p_scenc_infor->index / 5 - 1;
			pic_set(scene_infor_page);
		}
		else
		{
			scene_infor_page = SINGLE_SCENE_INFOR_LIST_FIRST_PAGE;
			pic_set(scene_infor_page);
		}
		// USER_PRINTF("module_scene_select_key_bak[%d] is %bd\n", j, module_scene_select_key_bak[j]);
		//		printf_tab(512,module_scene_select_key_bak);
	}
	else if ((2 == key_nb) && (0x51 == p_scenc_infor->edit_sta)) // 取消修改
	{
		// USER_PRINTF("p_scenc_infor->edit_sta -->%bx\n", p_scenc_infor->edit_sta);
		//		CLEAR_BIT(module_scene_select_key_bak[(p_module_scene->scene_name_index-1)+((p->index)/8)],(p->index)%8);
		//		CLEAR_BIT(p_module_scene->module_select[(p_module_scene->scene_name_index-1)+((p->index)/8)],(p->index)%8);
		//		CLEAR_BIT(module_scene_select_key_bak[((p->index)/8)],(p->index)%8);
		//		CLEAR_BIT(p_module_scene->module_select[((p->index)/8)],(p->index)%8);
		//		if(temp_module_index_bak!=p->index)
		//		{
		////			write_dgus(0X1200+p->index,0);
		//			CLEAR_BIT(module_scene_select_key_bak[((temp_module_index_bak)/8)],(temp_module_index_bak)%8);
		//			CLEAR_BIT(p_module_scene->module_select[((temp_module_index_bak)/8)],(temp_module_index_bak)%8);
		//			write_dgus(0X1200+temp_module_index_bak,0);
		//			temp_module_index_bak = p->index;
		//
		//		}
		clear_key_value(0x102e, &key_nb);
		temp_module_index = 0;
		//		j = (p_module_scene->scene_name_index-1)*8+((p->index)/8);
		j = ((p->index) / 8);
		// USER_PRINTF("p_module_scene->scene_name_index -->%bd\n", p_module_scene->scene_name_index);
		// USER_PRINTF("p->index -->%bd\n", p->index);
		// USER_PRINTF("module_scene_select_key_bak[%d] is %bd\n", j, module_scene_select_key_bak[(p_module_scene->scene_name_index - 1) + ((p->index) / 8)]);
		pic_set(scene_infor_page);
	}
}
// 添加场景序号
void scene_name_add(name_scene_t *p_module_scene)
{
	unsigned char index = 0;
	index = get_blank_scene_name_index();
	if (OVERFLOW_SIZE == index)
	{
		USER_PRINTF("-->waring! too many scene \n");
		pop_menu_key_ctrl(OVERFLOW_WARING_CODE);
		return;
	}
	pic_set(SCENE_ADD_PAGE);
	p_module_scene->scene_name_index = index + 1;
	// USER_PRINTF("-->current_scene_name_index is %bd\n", p_module_scene->scene_name_index);
	clear_temp_scenc_infor();
	clear_all_scene_infor();
	memset((unsigned char *)module_scene_select_key_bak, 0, 8);
	clear_all_module_var_en(0x1200, SINGLE_SCENE_NAME_INFOR_SIZE);
	p_module_scene->edit_sta = 1;
	set_default_scene_name();
	// pic_page = SINGLE_SCENE_INFOR_LIST_FIRST_PAGE;
}
// 场景修改
void scene_name_modify(name_scene_t *p_module_scene)
{
	//	p_module_scene->scene_name_index = 0;
	unsigned char index = 0;
	unsigned char i = 0, j = 0;

	index = get_scene_name_select_sequence_number() - 1;
	//	USER_PRINTF("name_scene.edit_sta is %bd\n",module_scene.edit_sta);
	USER_PRINTF("scene_name_modify index is %bd\n", index);
	if (BLANK == if_scene_name_exists(index))
	{
		return;
	}
	// pic_page = SINGLE_SCENE_INFOR_LIST_FIRST_PAGE;
	pic_set(SINGLE_SCENE_INFOR_LIST_FIRST_PAGE);

	scene_name_read_flash(p_module_scene, index);

	printf_tab(8, &p_module_scene->module_select);
	norflash_read(SCENE_INFOR_FLASH_ADR_SATRT + (index)*SINGLE_NAME_SCENC_INFOR_SIZE, (unsigned char *)(temp_scenc_infor), SINGLE_NAME_SCENC_INFOR_SIZE);
	memcpy((unsigned char *)&module_scene_select_key_bak, (unsigned char *)&(p_module_scene->module_select), 8);
	printf_tab(8, &module_scene_select_key_bak);
	for (i = 0; i < 8; i++)
	{
		for (j = 0; j < 8; j++)
		{
			if (GET_BIT(module_scene_select_key_bak[i], j))
			{
				write_dgus(0X1200 + i * 8 + j, 1);
			}
			else
			{
				write_dgus(0X1200 + i * 8 + j, 0);
			}
		}
	}

	for (i = 0; i < SINGLE_SCENE_NAME_INFOR_SIZE; i++)
	{
		if (FULL == (temp_scenc_infor + i)->data_sta)
			display_scene_infor(temp_scenc_infor + i);
		else
			clear_single_scene(i);
	}
	p_module_scene->edit_sta = 4;
	//	USER_PRINTF("name_scene.edit_sta is %bd\n",module_scene.edit_sta);
}
// 场景清除
void scene_name_clear(name_scene_t *p_module_scene)
{

	scene_name_clear_flash();
	clear_scene_list_name();
	USER_PRINTF("clear_scene_name_flash \n");
	memset((unsigned char *)module_scene_select_key_bak, 0, 8);
	memset((unsigned char *)p_module_scene->module_select, 0, 8);
	mbHost.scene_send_cyc_time = 200;
	scene_data.scenc_send_name_cyc_flag = 1;
	p_module_scene->edit_sta = 3;
}
// 场景删除
void scene_name_delete(name_scene_t *p_module_scene)
{
	scene_name_delete_flash(p_module_scene->scene_name_index - 1);
	delete_scene_list_name(p_module_scene->scene_name_index - 1);
	USER_PRINTF("delete_single_scene_name_flash \n");
	memset((unsigned char *)module_scene_select_key_bak, 0, 8);
	memset((unsigned char *)p_module_scene->module_select, 0, 8);
	//	mbHost.scene_send_cyc_time  = 200;
	//	scene_data.scenc_send_name_cyc_flag  = 1;
	p_module_scene->edit_sta = 2;
}
// 场景名称列表控制操作
void scene_name_key_ctrl(name_scene_t *p_module_scene)
{
	unsigned short key_nb = 0;
	unsigned char edit_sta = 0;
	//	unsigned short index = 0;
	get_key_value(0x1072, &key_nb);
	if ((key_nb))
	{
		switch (key_nb)
		{
		case 1: // 添加
			scene_name_add(p_module_scene);

			break;
		case 3: // 修改
			//				edit_sta = 2;
			//				memcpy(get_module_scene_module_select());
			scene_name_modify(p_module_scene);
			break;
		}
		clear_key_value(0x1072, &key_nb);
	}
	get_key_value(0x1073, &key_nb); // 场景名称清空
	if ((1 == key_nb))
	{
		clear_key_value(0x1073, &key_nb);
		pop_menu_key_ctrl(1);

		scene_name_clear(p_module_scene);
		USER_PRINTF("scene name clear ok!\n");
		//		USER_PRINTF("clear_scene_name \n");

		//		pop_menu_0xb0(read_pic(),1,17);
	}
	get_key_value(0x1074, &key_nb); // 场景名称删除
	if ((1 == key_nb))
	{
		clear_key_value(0x1074, &key_nb);

		p_module_scene->scene_name_index = get_scene_name_select_sequence_number();
		if (BLANK == if_scene_name_exists(p_module_scene->scene_name_index - 1))
		{
			return;
		}
		if (0 == p_module_scene->scene_name_index)
			return;
		pop_menu_key_ctrl(1);
		norflash_read(SCENE_INFOR_FLASH_ADR_SATRT + (get_scene_name_select_sequence_number() - 1) * SINGLE_NAME_SCENC_INFOR_SIZE, (unsigned char *)(temp_scenc_infor), SINGLE_NAME_SCENC_INFOR_SIZE);
		scene_name_delete(p_module_scene);
		USER_PRINTF("scene name delete ok!\n");

		//		pop_menu_0xb0(read_pic(),1,17);
	}
}

// 场景信息列表确认保存
void scene_infor_confirm_ctrl(name_scene_t *p_module_scene)
{
	if (0xff == if_all_scene_infor_blank()) // 未添加场景模块信息
	{
		return;
	}
	mbHost.scene_send_cyc_time = 200;
	scene_data.scenc_send_infor_cyc_flag = 1;
	pop_menu_key_ctrl(1);
	USER_PRINTF("-->scene_infor_confirm_ctrl \n");
	scene_infor_save_flash(p_module_scene);
	scene_name_save(p_module_scene);
	if (p_module_scene->scene_name_index > 6)
	{

		pic_page = SINGLE_SCENE_NAME_MANGE_SECOND_PAGE + (p_module_scene->scene_name_index - 1) / 6;
		pic_set(pic_page);
	}
	else
	{

		pic_page = SINGLE_SCENE_NAME_MANGE_FIRST_PAGE;
		pic_set(pic_page);
	}
	// if()
}
// 场景信息列表取消保存
void scene_infor_cancel_ctrl(name_scene_t *p_module_scene, scenc_infor_t *p_scenc_infor)
{
	memset((unsigned char *)module_scene_select_key_bak, 0, 8);
	clear_temp_scenc_infor();																					// 清空临时场景信息变量
	memset((unsigned char *)p_scenc_infor, 0, sizeof(scenc_infor_t)); // 清空场景信息变量
	memset((unsigned char *)p_module_scene, 0, sizeof(name_scene_t)); // 清空场景名字变量
	clear_all_scene_infor();																					// 清空场景信息显示
	clear_all_module_var_en(0x1200, SINGLE_SCENE_NAME_INFOR_SIZE);
	pic_set(pic_page);
}
////场景信息列表清空发送删除场景指令
// void scene_infor_clear_send(name_scene_t *p_module_scene,scenc_infor_t *p_scenc_infor)
//{
////	if(2 == get_module_scene_edit_sta())//如果处于修改场景状态，需要下发指令删除所有场景
////	{
////		scene_name_delete(p_module_scene);
////    scene_infor_clear_flash(p_scenc_infor);
////	}
////	else
////	{
////
////	}
//}
// 场景信息列表清空
void scene_infor_clear_ctrl(name_scene_t *p_module_scene, scenc_infor_t *p_scenc_infor)
{
	//	if(2 == get_module_scene_edit_sta())//如果处于修改场景状态，需要下发指令删除所有场景
	//	{
	//		(temp_scenc_infor+p_scenc_infor->index)->edit_sta = 2;
	//		module_relay_scene_send(temp_scenc_infor+p_scenc_infor->index);
	//		module_dim_scene_send(temp_scenc_infor+p_scenc_infor->index);
	//	}
	memset((unsigned char *)module_scene_select_key_bak, 0, 8);
	clear_temp_scenc_infor();																					// 清空临时场景信息变量
	memset((unsigned char *)p_scenc_infor, 0, sizeof(scenc_infor_t)); // 清空场景信息变量
	memset((unsigned char *)p_module_scene, 0, sizeof(name_scene_t)); // 清空场景名字变量
	clear_all_scene_infor();																					// 清空场景信息显示
	clear_all_module_var_en(0x1200, SINGLE_SCENE_NAME_INFOR_SIZE);
}
// 场景信息列表修改
void scene_infor_modify_ctrl(scenc_infor_t *p_scenc_infor)
{
	unsigned char index = 0;
	//	module_t temp_module = {0};
	p_scenc_infor->edit_sta = 0x51; // 添加修改
	index = get_scene_infor_select_sequence_number();
	//	USER_PRINTF("-->get_scene_infor_select_sequence_number is bd\n",index);
	p_scenc_infor->index = get_scene_infor_select_sequence_number() - 1;
	// USER_PRINTF("-->current scene infor select index is d\n", p_scenc_infor->index);
	if (BLANK == (temp_scenc_infor + p_scenc_infor->index)->data_sta)
		return;
	p_scenc_infor->edit_sta = 0x51; // 添加修改
	// USER_PRINTF("-->current scene infor select index is %d\n", p_scenc_infor->index);

	norflash_read((temp_scenc_infor + p_scenc_infor->index)->module_index * 140, (unsigned char *)&st_module, 140);
	//	temp_module_index_bak = st_module.index;
	//	USER_PRINTF("scene_modify_module_index is %bd\n",temp_module_index_bak);
	//	CLEAR_BIT(module_scene_select_key_bak[(p_scenc_infor->scene_infor_index-1)*(((temp_scenc_infor+index)->module_index-1)/8)],((temp_scenc_infor+index)->module_index-1)%8);
	display_scene_interface(&st_module);
	module_scene_infor_dis(temp_scenc_infor + p_scenc_infor->index);
}
// 场景信息列表删除
void scene_infor_delete_ctrl(scenc_infor_t *p_scenc_infor)
{
	unsigned char index = 0;
	//	USER_PRINTF("-->delete_p_scenc_infor->index %bx\n",p_scenc_infor->index);
	//	if(BLANK == (temp_scenc_infor+p_scenc_infor->index)->data_sta)
	//		return;
	index = get_scene_infor_select_sequence_number();
	if (0 == index)
	{
		return;
	}
	p_scenc_infor->index = index - 1;
	// USER_PRINTF("-->select_scene_infor_index is %d\n", p_scenc_infor->index);
	// USER_PRINTF("-->select_p_scenc_infor->module_index is %bd\n", p_scenc_infor->module_index);
	clear_single_scene(p_scenc_infor->index); // 清除选中场景信息显示
	if (4 == get_module_scene_edit_sta())			// 如果处于修改场景状态，需要下发指令删除场景
	{
		(temp_scenc_infor + p_scenc_infor->index)->edit_sta = 2;
		module_relay_scene_send(temp_scenc_infor + p_scenc_infor->index);
		module_dim_scene_send(temp_scenc_infor + p_scenc_infor->index);
		scene_infor_delete_flash(temp_scenc_infor + p_scenc_infor->index);

		//		CLEAR_BIT(module_scene.module_select[p_scenc_infor->module_index/8],(p_scenc_infor->module_index)%8);

		CLEAR_BIT(module_scene_select_key_bak[((temp_scenc_infor + p_scenc_infor->index)->module_index) / 8], ((temp_scenc_infor + p_scenc_infor->index)->module_index) % 8);
		CLEAR_BIT(module_scene.module_select[((temp_scenc_infor + p_scenc_infor->index)->module_index) / 8], ((temp_scenc_infor + p_scenc_infor->index)->module_index) % 8);
		scene_name_save_flash(&module_scene);
	}
	else
	{
		CLEAR_BIT(module_scene_select_key_bak[(temp_module_index - 1) / 8], (temp_module_index - 1) % 8);
	}
	memset((unsigned char *)(temp_scenc_infor + p_scenc_infor->index), 0, sizeof(scenc_infor_t)); // 清除场景信息变量
	memset((unsigned char *)p_scenc_infor, 0, sizeof(scenc_infor_t));															// 清空场景信息变量

	clear_single_module_var_en(0x1200 + p_scenc_infor->index);
	//	clear_all_module_var_en(SCENE_INFOR_LIST_SELECT_ADR,SCENE_NAME_NUB);
	USER_PRINTF("-->scene_infor_delete_ctrl\n");
}
// 场景编辑状态清空信息
void scene_edit_sta_clear_infor(name_scene_t *p_module_scene, scenc_infor_t *p_scenc_infor)
{
	clear_all_module_var_en(0x1200, SINGLE_SCENE_NAME_INFOR_SIZE);
	// USER_PRINTF("clear scene_name_index is %bd\n", p_module_scene->scene_name_index);
	T5L_Flash(0xA5, 0Xe000, SCENE_INFOR_FLASH_ADR_SATRT + (p_module_scene->scene_name_index - 1) * SINGLE_NAME_SCENC_INFOR_SIZE, SINGLE_NAME_SCENC_INFOR_SIZE); // 写0FLASHscencinfor清除
	clear_all_scene_infor();																																																																		// 清空场景列表信息显示
	memset((unsigned char *)module_scene_select_key_bak, 0, 8);
	memset((unsigned char *)p_scenc_infor, 0, sizeof(scenc_infor_t)); // 清空场景信息变量
	memset((unsigned char *)p_module_scene->module_select, 0, 8);
	scene_name_save_flash(&module_scene);
	p_module_scene->edit_sta = 2;
	infor_clear_sta = 1;
	USER_PRINTF("-->clear single scene name information\n");

	//	memset((unsigned char *)module_scene_select_key_bak,0,8);
	//	clear_temp_scenc_infor();//清空临时场景信息变量
	//	memset((unsigned char *)p_scenc_infor,0,sizeof(scenc_infor_t));//清空场景信息变量
	//	memset((unsigned char *)p_module_scene,0,sizeof(name_scene_t));//清空场景名字变量
	//	clear_all_scene_infor();//清空场景信息显示
	//	clear_all_module_var_en(0x1200,SINGLE_SCENE_NAME_INFOR_SIZE);
}

// 场景信息列表按键控制
void scene_infor_key_ctrl(name_scene_t *p_module_scene, scenc_infor_t *p_scenc_infor)
{
	unsigned short key_nb = 0;
	get_key_value(0x1071, &key_nb);
	if ((key_nb))
	{
		//		set_scene_infor_edit_sta(p_scenc_infor,key_nb);
		switch (key_nb)
		{
		case CONFIRM:

			scene_infor_confirm_ctrl(p_module_scene);
			break;
		case CANCEL:
			scene_infor_cancel_ctrl(p_module_scene, p_scenc_infor);
			break;
		case ADD:
			p_scenc_infor->edit_sta = 0x31; // 添加修改
			scene_infor_page = SINGLE_SCENE_INFOR_LIST_FIRST_PAGE;
			break;
		case CLEAR:
			//				p_scenc_infor->edit_sta = 0x43;//清空

			break;
		case MODIFY:

			scene_infor_modify_ctrl(p_scenc_infor);
			break;
		case DELETE:
			//				p_scenc_infor->edit_sta = 0x62;//删除

			break;
		default:
			break;
		}
		clear_key_value(0x1071, &key_nb);
	}
	/***
	进入修改场景模式时，删除或者清空场景信息，需要下发对应的modbus 指令
	**/
	get_key_value(0x1075, &key_nb); // 场景信息清空
	if ((1 == key_nb))
	{
		if (4 == get_module_scene_edit_sta()) // 如果处于修改场景状态，需要下发指令删除所有场景
		{
			USER_PRINTF("-->scene edit sta need to clear scene information!\n");
			scene_edit_sta_clear_infor(p_module_scene, p_scenc_infor);
			//			scene_name_delete(p_module_scene);
			//			scene_infor_clear_flash(p_scenc_infor);
		}
		else
		{
			scene_infor_clear_ctrl(p_module_scene, p_scenc_infor);
		}
		//		scene_infor_clear_send(p_module_scene,p_scenc_infor);
		//
		clear_key_value(0x1075, &key_nb);
	}
	get_key_value(0x1076, &key_nb); // 场景信息删除
	if ((1 == key_nb))
	{
		scene_infor_delete_ctrl(p_scenc_infor);
		clear_key_value(0x1076, &key_nb);
	}
}
// 设置场景信息列表界面
void set_scene_infor_page(void)
{
	unsigned short key_nb = 0;
	get_key_value(0x1b25, &key_nb);
	if (key_nb)
	{
		scene_infor_page = key_nb;
		clear_key_value(0x1b25, &key_nb);
	}
}
void scene_touch_ctrl(unsigned char index)
{
	unsigned char action = 0;
	action = read_dgus(0x1b30);
	if (action)
	{
		set_master_send_hr(master_send_hr, 7, index);
		set_master_send_hr(master_send_hr, 8, action);
		pack_data_send(&user_modbus, 0, MD_FR_MHR, 7, 2);
		action = 0;
		write_dgus(0x1b30, action);
	}
}
// 场景配置
void scene_modify(void)
{
	//	unsigned short page = 0;
	//	page = read_dgus(0x14);
	//	USER_PRINTF("module_scene.scene_name_index -->%bd\n",module_scene.scene_name_index);

	scene_name_key_ctrl(&module_scene);
	add_module_scene_infor(&st_module, &scenc_infor, &module_scene);
	module_scene_select_ctrl(&scenc_infor);

	scene_infor_key_ctrl(&module_scene, &scenc_infor);
	set_scene_infor_page();
	scene_touch_ctrl(scene_index);
}
// void scene_key_run(void)
//{
//	scene_infor_key_ctrl((temp_scenc_infor+scenc_infor.index),&module_scene);
// }
// 触摸声音控制
void beep_ctrl(unsigned char enable)
{
	unsigned char config_sys_cmd_80[4] = {0};
	read_dgusii_vp(0x80, config_sys_cmd_80, 2);
	sys_delay_about_ms(5);
	if (enable)
		config_sys_cmd_80[3] |= (1 << 3);
	else
		config_sys_cmd_80[3] &= ~(1 << 3);
	config_sys_cmd_80[0] = 0x5a;
	write_dgusii_vp(0x80, config_sys_cmd_80, 2);
	while (config_sys_cmd_80[0])
	{
		read_dgusii_vp(0x80, config_sys_cmd_80, 2);
	}
}
// 屏幕自动待机休眠控制
void back_light_ctrl(unsigned char enable)
{
	unsigned char config_sys_cmd_80[4] = {0};
	read_dgusii_vp(0x80, config_sys_cmd_80, 2);
	sys_delay_about_ms(5);
	if (enable)
		config_sys_cmd_80[3] |= (1 << 2);
	else
		config_sys_cmd_80[3] &= ~(1 << 2);
	config_sys_cmd_80[0] = 0x5a;
	write_dgusii_vp(0x80, config_sys_cmd_80, 2);
	while (config_sys_cmd_80[0])
	{
		read_dgusii_vp(0x80, config_sys_cmd_80, 2);
	}
}
// 触摸屏背光亮度,自动休眠时间设置
// work_light:正常工作时的背光亮度,范围:[0,100]
// sleep_light:休眠时的背光亮度,范围:[0,100]
// sleep_interval:无任何操作,隔sleep_interval*10ms后会自动进入休眠状态
void sys_led_config(unsigned char work_light, unsigned char sleep_light, unsigned short sleep_interval)
{
#define LED_CONFIG_ADDR 0x82
	unsigned char led_cmd[4] = {0};
	led_cmd[0] = work_light > 1 ? work_light : 1;
	led_cmd[1] = sleep_light;
	led_cmd[2] = sleep_interval >> 8;
	led_cmd[3] = sleep_interval & 0xff;
	write_dgusii_vp(LED_CONFIG_ADDR, led_cmd, 2);
	//	while(led_cmd[0])
	//	{
	//		read_dgusii_vp(LED_CONFIG_ADDR,led_cmd,2);
	//	}
}
// 弹出菜单设置
void pop_menu_key_ctrl(unsigned char menu_key_value)
{
	unsigned char temp_tab[8] = {0};
	unsigned char beep_enable = 0;
	beep_ctrl(0);
	temp_tab[0] = 0x5a;
	temp_tab[1] = 0xa5;
	temp_tab[2] = 0x00;
	temp_tab[3] = 0x04;
	temp_tab[4] = 0xff;
	temp_tab[5] = menu_key_value;
	temp_tab[6] = 0x00;
	temp_tab[7] = 1;
	sys_delay_about_ms(2);
	write_dgusii_vp(0xd4, temp_tab, 4);
	sys_delay_about_ms(2);
	beep_enable = read_dgus(BEEP_ENABLE_DGUS_ADR);
	// USER_PRINTF("-->beep_enable is %bd\n", beep_enable);
	//	sys_delay_about_ms(2);
	beep_ctrl(beep_enable); // control蜂鸣器声音
	sys_delay_about_ms(2);
}
// 场景触摸执行
void scene_touch_run(void)
{
	unsigned short key_nb = 0;
	// static unsigned char action_sta[SCENE_NAME_NUB_LIMIT] = {1};
	name_scene_t module_scene_r = {0};
	get_key_value(0x102d, &key_nb);
	if ((key_nb))
	{
		//		nor_read_flash
		norflash_read(MODULE_FLASH_ADR_END + (key_nb - 1) * NAME_SCENE_T_SIZE, (unsigned char *)&module_scene_r, NAME_SCENE_T_SIZE);
		if (FULL == module_scene_r.data_sta)
		{
			pop_menu_key_ctrl(0x14);
			scene_index = key_nb;
			// action_sta[key_nb - 1] = 0;
			// set_master_send_hr(master_send_hr, 7, key_nb);
			// set_master_send_hr(master_send_hr, 8, action_sta[key_nb - 1] + 1);
			// pack_data_send(&user_modbus, 0, MD_FR_MHR, 7, 2);
		}
		clear_key_value(0x102d, &key_nb);
	}
}
