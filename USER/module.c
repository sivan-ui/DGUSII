/*
 * @Author: xw.qu
 * @Date: 2023-08-31 09:22:42
 * @LastEditors: xw.qu
 * @LastEditTime: 2023-09-27 15:35:35
 * @FilePath: \USER\module.c
 * @Description: relay module configuration
 *
 * Copyright (c) 2023 by xw.qu, All Rights Reserved.
 */
#include "module.h"
#include "Uart.h"
#include "T5LLIB.h"
#include "T5lOS8051.h"
v_module_t g_var_module;
#define DEVICE_NB 10
#define DEVICE_INFOR_INDEX 14
u16 xdata num4 = 4;
unsigned char modfity_index = 0;
unsigned char var_previous_page = 0;

unsigned char channel_number = 0;
unsigned short module_key_nb_bak = 0;
unsigned char module_select_sta_bak = 0;
unsigned short init_pwd = 8888; // default password
module_t module = {
		{1},
		{2},
		{0},
		{0},
		{1},
		{4},
		{"����1"}, // ��ͨģ��
		{"ͨ��1           ͨ��2           ͨ��3           ͨ��4           ͨ��5           ͨ��6           ͨ��7           ͨ��8           ͨ��9           ͨ��10          ͨ��11          ͨ��12          ͨ��13          ͨ��14          ͨ��15          ͨ��16          "},
};
module_t auto_module = {
		{1},
		{2},
		{0},
		{0},
		{1},
		{4},
		{0}, // ��ͨģ��
		{"ͨ��1           ͨ��2           ͨ��3           ͨ��4           ͨ��5           ͨ��6           ͨ��7           ͨ��8           ͨ��9           ͨ��10          ͨ��11          ͨ��12          ͨ��13          ͨ��14          ͨ��15          ͨ��16          "},
};
module_t st_module = {
		{1},
		{2},
		{0},
		{0},
		{1},
		{4},
		{"����1"}, // ��ͨģ��
		{"ͨ��1           ͨ��2           ͨ��3           ͨ��4           ͨ��5           ͨ��6           ͨ��7           ͨ��8           ͨ��9           ͨ��10          ͨ��11          ͨ��12          ͨ��13          ͨ��14          ͨ��15          ͨ��16          "},
};
module_t timing_module = {
		{0},
		{0},
		{0},
		{0},
		{0},
		{0},
		{"����1"}, // ��ͨģ��
		{"ͨ��1           ͨ��2           ͨ��3           ͨ��4           ͨ��5           ͨ��6           ͨ��7           ͨ��8           ͨ��9           ͨ��10          ͨ��11          ͨ��12          ͨ��13          ͨ��14          ͨ��15          ͨ��16          "},
};
module_t temp_module = {
		{0},
		{0},
		{0},
		{0},
		{0},
		{0},
		{"����1"}, // ��ͨģ��
		{"ͨ��1           ͨ��2           ͨ��3           ͨ��4           ͨ��5           ͨ��6           ͨ��7           ͨ��8           ͨ��9           ͨ��10          ͨ��11          ͨ��12          ͨ��13          ͨ��14          ͨ��15          ͨ��16          "},
};
unsigned char module_tab[10] = {
		1, 2, 3, 4, 5, 6, 7, 8, 9, 10
		//	{2},
		//	{0},
		//	{0},
		//	{1},
		//	{4},
		//	{"����1"},         //��ͨģ��
		//	{"ͨ��1","ͨ��2","ͨ��3","ͨ��4","ͨ��5","ͨ��6","ͨ��7","ͨ��8","ͨ��9","ͨ��10","ͨ��11","ͨ��12","ͨ��13","ͨ��14","ͨ��15","ͨ��16"},

};
// bit key_flag = 0;
unsigned short xdata key, num_return;
unsigned short xdata channel = 0;
unsigned short module_type = 0, module_type_bak = 0;
unsigned short adr = 0;
void timing_module_parameter_week_enable_channel_send(struct this_module *p, unsigned char channel, unsigned char state);
void timing_module_parameter_solar_channel_send(struct this_module *p, unsigned char channel, unsigned char state);
void timing_module_parameter_time_frame_channel_send(struct this_module *p, unsigned char channel, unsigned char state);
void timing_module_parameter_ongitude_latitude_send(struct this_module *p);
void timing_module_parameter_time_send(struct this_module *p);
unsigned char check_or_not_frame(unsigned short adr);
// void (*timing_module_parameter_set_send)(struct   this_module* xdata p,unsigned char channel,unsigned char state);
// void (*fun)(module_t* p,unsigned char channel,unsigned char state);

// code  void (*p_fun_tab[5])( struct   this_module xdata * p,unsigned char xdata channel,unsigned char xdata state) =
//{
//	timing_module_parameter_ongitude_latitude_send,
//	timing_module_parameter_time_send,
//	timing_module_parameter_week_enable_channel_send,
//	timing_module_parameter_solar_channel_send,
//	timing_module_parameter_time_frame_channel_send,

//};
// unsigned char *p_module_name;
unsigned char xdata module_default_name[17] = {
		" "};
unsigned char xdata module_name_tab[6][13] = {
		"��ͨģ��    ", // ��ͨģ��
		"ʱ��ģ��    ", // ʱ��ģ��
		"����ģ��    ", // ����ģ��
		"�ɿع�ģ��  ", // �ɿع�ģ��
		"˫����ͨģ��", // ˫����ͨģ��
		"˫��ʱ��ģ��", // ˫��ʱ��ģ��

};
unsigned char default_module_channel_name[257] = {
		"ͨ��1           ͨ��2           ͨ��3           ͨ��4           ͨ��5           ͨ��6           ͨ��7           ͨ��8           ͨ��9           ͨ��10          ͨ��11          ͨ��12          ͨ��13          ͨ��14          ͨ��15          ͨ��16          "};


// ��ȡ��ֵ
void get_key_value(unsigned short adr, unsigned short *key_value)
{
	read_dgusii_vp(adr, (unsigned char *)key_value, 1);
}
// ��ֵ��0
void clear_key_value(unsigned short adr, unsigned short *key_value)
{
	unsigned short key_zero = 0;
	USER_PRINTF("<--0x%x  key_value is---%d\n", adr, *key_value);
	write_dgusii_vp(adr, (unsigned char *)&key_zero, 1);
	//	key_flag = 1;
}
// ����״̬��0
void clear_touch_sta(void)
{
	unsigned char tab_zero[8] = {0};
	write_dgusii_vp(0x16, (unsigned char *)&tab_zero, 4);
	//	key_flag = 1;
}
// ���������Ƿ����
unsigned char check_whether_data_exists(unsigned int adr_start, unsigned char index, unsigned char flash_block)
{
	unsigned char ch_data = 0;
	unsigned char tab_data[4] = {0};
	norflash_read(index * flash_block + adr_start, tab_data, 2);
	//	printf_tab(4,tab_data);
	if (0 == tab_data[0])
	{
		ch_data = BLANK;
		// USER_PRINTF("<--index %bd is blank \n", index);
	}
	else
		ch_data = FULL;

	return ch_data;
}

// ���ؿհ��������
unsigned short find_index(unsigned int adr_start, unsigned char flash_size, unsigned short quantity)
{
	unsigned short index = 0;
	for (index = 0; index < quantity; index++)
	{
		if (BLANK == check_whether_data_exists(adr_start, index, flash_size))
		{
			return index;
		}
	}
	return 0xffff;
}
// void return_to_previous_page(unsigned char page)
//{
//	pic_set(page);
// }
// ����ģ����Ϣ
void set_module_information(module_t *p, unsigned char index, unsigned short module_type)
{
	unsigned short temp = 0;
	//	p->data_sta = 1;
	//	unsigned char data_sta;
	//	unsigned char index;
	//	unsigned char reserve;
	//	unsigned char type;
	//	unsigned char adr;
	//	unsigned char channel_nb;
	//	unsigned char module_name[16];
	//	unsigned char channel_name[16][16];
	//	unsigned char index = 0 ;
	//	for(index = 0;index<MODULE_NUB;index++)
	//	{
	//		if( BLANK == check_whether_data_exists(index))
	//		{
	//			save_module_information(p,index);
	// memset(p,0,sizeof(module_t));
	p->index = index;
	read_dgusii_vp(0x2000, (unsigned char *)&temp, 1);
	p->adr = (unsigned char)temp;
	read_dgusii_vp(0x1002, (unsigned char *)&temp, 1);
	p->channel_nb = (unsigned char)temp;
	// USER_PRINTF("-->module_name is %s;\n",p->module_name);
	read_dgusii_vp(0x3000, (unsigned char *)&p->module_name, 8);
	// printf_tab(16,(unsigned char *)&p->module_name);
	// USER_PRINTF("-->module_name is %s;\n",p->module_name);

	// removeWhitespace(p->module_name);
	// USER_PRINTF("-->module_name is %s;\n",p->module_name);
	read_dgusii_vp(0x3008, (unsigned char *)&p->channel_name, 128);
	p->data_sta = FULL;
	p->type = module_type;
	//	USER_PRINTF("P->TYEP is %d\n",module_type);
	//			break;
	//		}
	//	}
}
// ��ʾģ����Ϣ
void display_module_information(module_t *p, unsigned char index)
{
	unsigned char mod_infom_tab[64] = {0};
	unsigned char module_name_1[13] = {0};
	//	USER_PRINTF("-->module type is %bd\n",p->type);
	if (p->type <= 2)
	{
		strncpy(module_name_1, &module_name_tab[p->type], 8);
	}
	else if (p->type == 3)
	{
		strncpy(module_name_1, &module_name_tab[p->type], 10);
	}
	else
	{
		strncpy(module_name_1, &module_name_tab[p->type], 12);
	}
	//	USER_PRINTF("-->module_name is %s;\n", p->module_name);
	strrpl(p->module_name, " ", "");
	// strrpl(p->module_name, "  ", "");

	// sprintf(mod_infom_tab, "����:%s,ģ��:%.16s,��ַ:%bd,ͨ��:%bd", module_name_1, p->module_name, p->adr, p->channel_nb);
	sprintf(mod_infom_tab, "����:%s,��ַ:%bd,ͨ��:%bd,ģ��:%s", module_name_1, p->adr, p->channel_nb, p->module_name);
	//	USER_PRINTF("-->mod_infom_tab is %s\n", mod_infom_tab);
	//	USER_PRINTF("-->p->index %bd\n", p->index);
	write_dgusii_vp(0x3520 + index * 32, (unsigned char *)mod_infom_tab, 32);
}
//�Ƿ�����ظ���ַ
// unsigned char if_adr_repeat(unsigned char edit_sta,unsigned char adr,unsigned char *p)
// {
// 	switch (edit_sta)
// 	{
// 		case ADD:
// 		if(find_nub_index(p,adr))
// 		{
// 			USER_PRINTF("--> adr already exist\n!");
// 			USER_PRINTF("--> save module information fail!\n");
// 			return 1;
// 		}		
// 		break;
// 		case MODIFY:
// 		if(find_nub_index(p,adr))
// 		{
// 			USER_PRINTF("--> adr already exist\n!");
// 			USER_PRINTF("--> save module information fail!\n");
// 			return 1;
// 		}			
// 		break;
// 		default:
// 		break;
// 	}
// 	return 0;

	
// }
// �洢ģ����Ϣ
void save_module_information(module_t *p)
{
	unsigned short key_nb1 = 0;
	unsigned short key_nb2 = 0;
	unsigned short temp_adr = 0;
	//	static unsigned char temp_index = 0;
	read_dgusii_vp(0x1009, (unsigned char *)&key_nb1, 1);
	if (1 == key_nb1)
	{
		key_nb1 = 0;
		write_dgusii_vp(0x1009, (unsigned char *)&key_nb1, 1);
		USER_PRINTF("-->ctrl_mgmt_sta\n");
		modfity_index = find_index(0, MODULE_FLASH_SIZE, MODULE_NUB);
		//		USER_PRINTF("<--get_blank_index -->%bd\n", modfity_index);
		write_dgusii_vp(0x3110, (unsigned char *)&module_name_tab[0], 6);
		write_dgus(0X1000, 4);
	}
	sys_delay_about_ms(5);
	read_dgusii_vp(0x1006, (unsigned char *)&key_nb2, 1);
	// if (key_nb2)
	// {
	// 	USER_PRINTF("-->key_value ---%d\n", key_nb2);
	// }
	if ((4 == key_nb2) || (6 == key_nb2) || (8 == key_nb2) || (10 == key_nb2) || (12 == key_nb2) || (14 == key_nb2) || (16 == key_nb2) || (0x14 == key_nb2) || (0x24 == key_nb2))
	{
		key_nb2 = 0;
		sys_delay_about_ms(1);
		write_dgusii_vp(0x1006, (unsigned char *)&key_nb2, 1);
		printf_tab(20,&g_var_module.module_adr[0]);
		read_dgusii_vp(0x2000, (unsigned char *)&temp_adr, 1);
		if(find_nub_index(&g_var_module.module_adr[0],temp_adr))
		{
			pop_menu_key_ctrl(0x1f);
			USER_PRINTF("--> adr already exist!\n");
			USER_PRINTF("--> save module information fail!\n");
			return ;
		}
		set_module_information(p, modfity_index, (unsigned char)module_type_bak);
		//		USER_PRINTF("-->module_type_bak is %d\n",module_type_bak);
		
		add_arr_data(&g_var_module.module_adr[0],p->adr,modfity_index);

		norflash_write(modfity_index * MODULE_FLASH_SIZE, (unsigned char *)p, MODULE_FLASH_SIZE);
		write_dgusii_vp(0x3120 + 8 * modfity_index, (unsigned char *)p->module_name, 8);
		display_module_information(p, modfity_index);
		USER_PRINTF("-->save_module_information_ok\n");
		if (modfity_index >= 5)
		{
			pic_page = 88 + modfity_index / 5 - 1;
			pic_set(pic_page);
			
		}
		else
		{
			pic_page = 2;
			pic_set(pic_page);
		}
		module_type_bak = 0;
	}
	else if (0xff == key_nb2)
	{
		key_nb2 = 0;
		sys_delay_about_ms(1);
		write_dgusii_vp(0x1006, (unsigned char *)&key_nb2, 1);	
		pic_set(pic_page); 	
		// if (modfity_index >= 5)
		// {
		// 	pic_set(88 + modfity_index / 5 - 1);
		// }
		// else
		// {
		// 	pic_set(2);
		// }
	}

	//	sys_delay_about_ms(10);//�����ʱ�����,���Է�ֹĪ������Ĵ���
}
// ����ģ��ѡ����� touch_key_adr-0x110c; select_key_adr-0x1104
void set_selected_sequence_number(unsigned short touch_key_adr, unsigned short select_key_adr, unsigned char page_nb)
{
	unsigned short key_nb = 0;
	static unsigned char select_sta = 0;
	static unsigned char page = 0;
	unsigned char i = 0;
	get_key_value(touch_key_adr, &key_nb);
	if ((key_nb))
	{
		for (i = 0; i < page_nb; i++)
		{
			write_dgus(select_key_adr + i, 0);
		}
		if (page != ((key_nb - 1) / 8))
		{
			page = (key_nb - 1) / 8;
			select_sta = 0;
		}
		select_sta = select_sta == (key_nb - (page * 8)) ? ((select_sta > 0) ? 0 : (key_nb - (page * 8))) : (key_nb - (page * 8));
		// USER_PRINTF("-->set_select_sta is--%bd\n", select_sta);
		write_dgus(select_key_adr + (key_nb - 1) / 8, select_sta);
		clear_key_value(touch_key_adr, &key_nb);
	}
}
/**
 * @brief ����ѡ���
 * @param {unsigned short} touch_key_adr ������ַ
 * @param {unsigned short} select_key_adr ���ַ
 * @param {unsigned char} nb ����
 * @param {unsigned short} *p_key_nb_bak ����ֵ����
 * @param {unsigned char} *p_select_sta  ��ѡ����
 * @return {*}
 */
void set_box_select(unsigned short touch_key_adr, unsigned short select_key_adr, unsigned char nb,unsigned short *p_key_nb_bak, unsigned char *p_select_sta)
{
	unsigned short key_nb = 0;
	// static unsigned short key_nb_bak = 0;
	// static unsigned char select_sta = 0;
	unsigned char i = 0;
	get_key_value(touch_key_adr, &key_nb);

	if ((key_nb)) // ��������
	{		
		for (i = 0; i < nb; i++)
		{
			if (i == key_nb - 1)
			{
				continue;
			}
			write_dgus(select_key_adr + i, 0); // ���˴����Ŀ�������д0
		}
		if (*p_key_nb_bak != key_nb)
		{
			*p_key_nb_bak = key_nb;
			*p_select_sta = 0;
			
		}
		// USER_PRINTF("-->set_select_sta before is--%bd\n", select_sta);
		*p_select_sta ^= 1;
		// USER_PRINTF("-->set_select_sta after is--%bd\n", select_sta);
		write_dgus(select_key_adr + (key_nb - 1), *p_select_sta);
		clear_key_value(touch_key_adr, &key_nb);
	}
}

// ��ȡģ��ѡ����� 0x1104

unsigned char get_box_select_number(unsigned short select_key_adr, unsigned char nb)
{
	unsigned char i = 0;
	unsigned char seq_nb = 0;
	for (i = 0; i < nb; i++)
	{
		seq_nb = read_dgus(select_key_adr + i);
		if (seq_nb)
		{
			seq_nb = seq_nb + i;
			// USER_PRINTF("-->get_select_sta is--%bd\n", seq_nb);
			return (seq_nb);
		}
	}
	return 0;
}
unsigned char get_selected_sequence_number(unsigned short select_key_adr, unsigned char page)
{
	unsigned char i = 0;
	unsigned char seq_nb = 0;
	for (i = 0; i < page; i++)
	{
		seq_nb = read_dgus(select_key_adr + i);
		if (seq_nb)
		{
			seq_nb = seq_nb + i * 8;
			// USER_PRINTF("-->get_select_sta is--%bd\n", seq_nb);
			return (seq_nb);
		}
	}
	return 0;
}
void delay_us(unsigned short t)
{
	char i;
	while (t)
	{
		for (i = 0; i < 50; i++)
		{
			i = i;
		}
		t--;
	}
}
// ������ҳ��ֵ
unsigned short read_pic(void)
{
	unsigned short pic = 0;
	pic = read_dgus(0x0014);
	sys_delay_about_ms(2);
	return pic;
}

// ҳ���л�
void pic_set(unsigned int pic) // picΪ��ת����ҳ��
{
	unsigned short run_num = 0;
	run_num = 0x5A01;
	write_dgusii_vp(0x0085, (unsigned char *)&pic, 1);
	write_dgusii_vp(0x0084, (unsigned char *)&run_num, 1);
	while (run_num >> 8) // ���߰�λ5A�Ƿ�����
	{
		read_dgusii_vp(0x0084, (unsigned char *)&run_num, 1);
		delay_us(3000); //**********
	}
}
// ����ͨ������
void set_channel_nb(void)
{
	read_dgusii_vp(0x1000, (unsigned char *)&channel, 1);

	//	USER_PRINTF("channel value ---- %d\n",channel);
	{
		switch (channel)
		{
		case 0:
			//				write_dgusii_vp(0x1002,(unsigned char *)&num4,1);
			//			display_icon();
			break;
		case 4:
			//				channel_bak = 0;
			write_dgusii_vp(0x1002, (unsigned char *)&channel, 1);
			//			  write_dgus(17,54);
			//			  channel_bak = channel;
			break;
		case 6:
			//				channel_bak = 0;
			write_dgusii_vp(0x1002, (unsigned char *)&channel, 1);
			//			  write_dgus(17,54);
			//			  channel_bak = channel;
			break;
		case 8:
			//				channel_bak = 1;
			write_dgusii_vp(0x1002, (unsigned char *)&channel, 1);
			//			  write_dgus(17,55);
			//			  channel_bak = channel;
			break;
		case 10:
			//				channel_bak = 1;
			write_dgusii_vp(0x1002, (unsigned char *)&channel, 1);
			//			  write_dgus(17,55);
			//			  channel_bak = channel;
			break;
		case 12:
			//				channel_bak = 2;
			write_dgusii_vp(0x1002, (unsigned char *)&channel, 1);
			//			  write_dgusii_vp(0x1003,(unsigned char*)&channel_bak,1);
			//			  channel_bak = channel;
			break;
		case 14:
			//				channel_bak = 2;
			write_dgusii_vp(0x1002, (unsigned char *)&channel, 1);
			//			  write_dgusii_vp(0x1003,(unsigned char*)&channel_bak,1);
			//			  channel_bak = channel;
			break;
		case 16:
			//				channel_bak = 3;
			write_dgusii_vp(0x1002, (unsigned char *)&channel, 1);
			//			  write_dgus(17,53);
			//			  channel_bak = channel;
			break;
		default:
			//				write_dgusii_vp(0x1000,(unsigned char*)&channel_bak,1);
			break;
		}
	}
}

// ����ģ������
void set_module_type(void)
{

	//	data_t* p;
	//	static char i = 0;
	//		//void write_dgusii_vp(unsigned int addr,unsigned char *buf,unsigned int len)
	//	read_dgusii_vp(0x3010,(unsigned char *)&num_test,8);
	//	read_dgusii_vp(0x1000,(unsigned char *)&num_return,1);

	//	num_test = 0X03;
	//	static unsigned short module_type = 0;
	unsigned short default_channel = 4;
	//	USER_PRINTF("touch key value ---- %d\n",key);
	read_dgusii_vp(0x1001, (unsigned char *)&module_type, 1);
	// if (module_type < 0xff)
	// 	USER_PRINTF("-->MODULE_TYPE = %d\n", module_type);
	switch (module_type)
	{
	case 0:
		module_type_bak = 0;
		write_dgusii_vp(0x3110, (unsigned char *)&module_name_tab[module_type_bak], 6);

		//      set_channel_nb();
		//		  write_dgusii_vp(0x1000,(unsigned char *)&default_channel,1);
		module_type = 0xff;
		write_dgusii_vp(0x1001, (unsigned char *)&module_type, 1);
		// USER_PRINTF("-->module_type_bak = %d\n", module_type_bak);
		break;
	case 1:
		module_type_bak = 0;
		write_dgusii_vp(0x3110, (unsigned char *)&module_name_tab[module_type_bak], 6);
		//      set_channel_nb();
		write_dgusii_vp(0x1000, (unsigned char *)&default_channel, 1);
		module_type = 0xff;
		write_dgusii_vp(0x1001, (unsigned char *)&module_type, 1);
		// USER_PRINTF("-->module_type_bak = %d\n", module_type_bak);
		break;
	case 2:
		module_type_bak = 1;
		write_dgusii_vp(0x3110, (unsigned char *)&module_name_tab[module_type_bak], 6);
		//      set_channel_nb();
		write_dgusii_vp(0x1000, (unsigned char *)&default_channel, 1);
		module_type = 0xff;
		write_dgusii_vp(0x1001, (unsigned char *)&module_type, 1);
		// USER_PRINTF("-->module_type_bak = %d\n", module_type_bak);
		break;

	case 3:
		module_type_bak = 2;
		write_dgusii_vp(0x3110, (unsigned char *)&module_name_tab[module_type_bak], 6);
		write_dgusii_vp(0x1000, (unsigned char *)&default_channel, 1);
		module_type = 0xff;
		write_dgusii_vp(0x1001, (unsigned char *)&module_type, 1);
		//      write_dgusii_vp(0x1002,(unsigned char *)&num4,1);
		// USER_PRINTF("-->module_type_bak = %d\n", module_type_bak);
		break;
	case 4:
		module_type_bak = 3;
		write_dgusii_vp(0x3110, (unsigned char *)&module_name_tab[module_type_bak], 6);
		write_dgusii_vp(0x1000, (unsigned char *)&default_channel, 1);
		module_type = 0xff;
		write_dgusii_vp(0x1001, (unsigned char *)&module_type, 1);
		//		  USER_PRINTF("MODULE_TYPE = %bd\n",module_type);
		//		  write_dgusii_vp(0x1002,(unsigned char *)&num4,1);
		//		  write_dgusii_vp(0x1000,(unsigned char *)&num,1);

		//      enable_touch_set(72,9,0);
		// USER_PRINTF("-->module_type_bak = %d\n", module_type_bak);
		break;
	case 5:
		module_type_bak = 4;
		write_dgusii_vp(0x3110, (unsigned char *)&module_name_tab[module_type_bak], 6);
		write_dgusii_vp(0x1000, (unsigned char *)&default_channel, 1);
		module_type = 0xff;
		write_dgusii_vp(0x1001, (unsigned char *)&module_type, 1);
		//      write_dgusii_vp(0x1002,(unsigned char *)&num4,1);
		// USER_PRINTF("-->module_type_bak = %d\n", module_type_bak);
		//		pic_set(84);
		break;
	case 6:
		module_type_bak = 5;
		write_dgusii_vp(0x3110, (unsigned char *)&module_name_tab[module_type_bak], 6);
		write_dgusii_vp(0x1000, (unsigned char *)&default_channel, 1);
		module_type = 0xff;
		write_dgusii_vp(0x1001, (unsigned char *)&module_type, 1);
		//      write_dgusii_vp(0x1002,(unsigned char *)&num4,1);
		// USER_PRINTF("-->module_type_bak = %d\n", module_type_bak);

		//		pic_set(84);
		break;
	default:

		set_channel_nb();
		module_type = 0xff;
		write_dgusii_vp(0x1001, (unsigned char *)&module_type, 1);
		//		module_type_bak = 0;
		//		USER_PRINTF("-->module_type_bak = %d\n", module_type_bak);
		//		  USER_PRINTF("module_type_default_run\n");
		//			USER_PRINTF("module_type_bak = %d\n",module_type_bak);
		//			write_dgusii_vp(0x3010,&module_p[0],8);
		break;
	}
	//	return 1;
}
// D7=��(0-0x63) D6=��(0-0x0C) D5=��(0-0x1F) D4=���ڣ�0-0x6�� D3=Сʱ(0-0x17) D2=����(0-0x3B) D1=��(0-0x3B) D0 δ����
void read_sys_time(unsigned char *p)
{
	read_dgusii_vp(0x10, p, 4);
}
// ����ʱ��ǰ��ȡһ��ʱ��
void read_dgus_time(void)
{
	unsigned short key_nb = 0;
	unsigned char rtc_parm[8] = {0};
	//	read_dgusii_vp(0x0010,(u8*)rtcdata,4);	//д��DGUS�����ռ�

	//	read_dgusii_vp(0x1008,(unsigned char *)&key_nb,1);
	get_key_value(0x1008, &key_nb);
	if (5 == key_nb)
	{
		//		USER_PRINTF(" key_value --->%d\n", key_nb);
		read_sys_time(rtc_parm);
		//		read_dgusii_vp(0x10,rtc_parm,4);
		// ��������rtc_parm[3] = week
		rtc_parm[3] = rtc_parm[4];
		rtc_parm[4] = rtc_parm[5];
		rtc_parm[5] = rtc_parm[6];
		rtc_parm[6] = rtc_parm[7];
		rtc_parm[7] = 0;
		printf_tab(8, rtc_parm);
		//		key_nb = 0;
		USER_PRINTF("-->read_dgus_time\n");
		read_dgus(0x10);
		//	  write_dgusii_vp(0x1008,(unsigned char *)&key_nb,1);
		clear_key_value(0x1008, &key_nb);
		write_dgusii_vp(0x9d, rtc_parm, 4);
	}
}
/**
 * @brief �л��̵���ģ����ʾ����
 * @param {module_t} *pģ��ָ��
 * @param {unsigned char} relay_page�̵���ҳ��
 * @param {unsigned char} time_relay_pageʱ�ؼ̵���ҳ��
 * @param {unsigned char} *p_var_page��¼��ǰҳ��
 * @return {*}
 */
void relay_interface_trigger(module_t *p, unsigned char relay_page, unsigned char time_relay_page, unsigned char *p_var_page)
{
	if ((DOUBLE_RELAY_MODULE == p->type) || (SINGLE_RELAY_MODULE == p->type))
	{
		pic_set(relay_page);
		*p_var_page = relay_page;
	}
	else
	{
		pic_set(time_relay_page);
		*p_var_page = time_relay_page;
	}
}
// ��ʾ�̵���ģ�����
void display_relay_interface(module_t *p)
{
	write_dgus(0x4088, p->adr);
	//			write_dgusii_vp(0x4088,(unsigned char *)&p->adr,1);
	write_dgusii_vp(0x4000, (unsigned char *)&p->module_name, 8);
	write_dgusii_vp(0x4008, (unsigned char *)&p->channel_name, 8 * 16);
	switch (p->channel_nb)
	{
	case 4:
		relay_interface_trigger(p, 38, 31, &var_previous_page);
		break;
	case 6:
		relay_interface_trigger(p, 39, 32, &var_previous_page);
		break;
	case 8:
		relay_interface_trigger(p, 40, 33, &var_previous_page);

		break;
	case 10:
		relay_interface_trigger(p, 41, 34, &var_previous_page);
		break;
	case 12:
		relay_interface_trigger(p, 42, 35, &var_previous_page);
		break;
	case 14:
		relay_interface_trigger(p, 43, 36, &var_previous_page);
		break;
	case 16:
		relay_interface_trigger(p, 44, 37, &var_previous_page);
		break;
	default:
		break;
	}
}
// ��ʾ����ģ�����
void display_dim_interface(module_t *p)
{
	write_dgus(0x4088, p->adr);
	//	write_dgusii_vp(0x4088,(unsigned char *)&p->adr,1);
	write_dgusii_vp(0x4000, (unsigned char *)&p->module_name, 8);
	write_dgusii_vp(0x4008, (unsigned char *)&p->channel_name, 8 * 16);
	switch (p->type)
	{
	case VOL_DIM_MODULE: // ��ͨ����
		pic_set(DIM_CTRL_PAGE);
		break;
	case SCR_DIM_MODULE: // �ɿع����
		pic_set(SCR_CTRL_PAGE);
		break;
	default:
		break;
	}
}
void return_to_previous_page(void)
{
	pic_set(var_previous_page);
}
void read_timing_module_parameter(module_t *p)
{
	pack_data_send(&user_modbus, p->adr, MD_RD_HR, 35, 146);
}
void timing_module_parameter_settings(module_t *p)
{
	unsigned short key_nb = 0;
	get_key_value(0x10dd, &key_nb);
	if (key_nb)
	{
		channel_number = key_nb;
		// USER_PRINTF("-->channel number is %bd\n", channel_number);
		if (0x12 != key_nb)
			pack_data_send(&user_modbus, p->adr, MD_RD_HR, 35, 200); // ��35��ʼ��200��
		clear_key_value(0x10dd, &key_nb);
	}
}
// ��ʾģ����ղ���
void display_week_parameter(unsigned short week_enable_adr, unsigned char week_enable)
{
	unsigned char i = 0;
	static unsigned char week_enable_bak = 0;
	if (week_enable_bak != week_enable)
	{
		week_enable_bak = week_enable;
		USER_PRINTF("-->week enable is %bd\n", week_enable);
		for (i = 0; i < 7; i++)
		{
			write_dgus(week_enable_adr + i, (week_enable >> i) & 0x01);
		}
	}
}
// ��ʾģ����նȲ���
void display_solar_parameter(unsigned short solar_adr, unsigned short solar_en_adr, unsigned char solar_enable, unsigned char solar_up, unsigned char solar_down)
{
	static unsigned char solar_down_bak = 15, solar_up_bak = 25, solar_en_bak = 0;
	if (solar_en_bak != solar_enable)
	{
		solar_en_bak = solar_enable;
		write_dgus(solar_en_adr, solar_enable);
	}
	if (solar_up_bak != solar_up)
	{
		solar_up_bak = solar_up;
		write_dgus(solar_adr + 1, solar_up);
	}
	if (solar_down_bak != solar_down)
	{
		solar_down_bak = solar_down;
		write_dgus(solar_adr, solar_down);
	}
}

// ��ʾģ��ʱ�β���
void display_time_frame_parameter(unsigned short time_frame_adr, unsigned char *p_data)
{
	static unsigned char time_frame_bak[16] = {18, 30, 6, 30, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	unsigned char i = 0;
	for (i = 0; i < 16; i++)
	{
		if (time_frame_bak[i] != p_data[i])
		{
			time_frame_bak[i] = p_data[i];
			write_dgus(time_frame_adr + i, p_data[i]);
		}
	}
}
// ��ʾģ�����
void display_timing_module_parameter(module_t *p, unsigned char channel)
{
	unsigned short week_enable_adr = 0;
	unsigned char week_enable = 0;
	unsigned short solar_adr = 0;
	unsigned short solar_en_adr = 0;
	unsigned char solar_enable = 0;
	unsigned char solar_up = 0;
	unsigned char solar_down = 0;
	unsigned short time_frame_adr = 0;
	unsigned char p_data[16] = {0};
	if (SINGLE_TIME_PARA_SET_PAGE != read_pic())
		return;
	//	USER_PRINTF("-->display_timing_module_parameter_page\n");
	if ((DOUBLE_TIME_RELAY_MODULE == p->type) && (mbHost.errTimes < MBH_ERR_MAX_TIMES))
	{
		solar_enable = HR[200 + channel] >> 15 & 0x01;
		solar_up = HR[200 + channel] >> 8 & 0x7f;
		solar_down = HR[200 + channel] & 0xff;
		memcpy(p_data, (unsigned char *)(HR + 53 + (channel - 1) * 8), 16);
	}
	else if ((SINGLE_TIME_RELAY_MODULE == p->type) && (mbHost.errTimes < MBH_ERR_MAX_TIMES))
	{

		solar_enable = (HR[160 + channel] >> 15) & 0x01;
		solar_up = (HR[160 + channel] >> 8) & 0x7f;
		solar_down = HR[160 + channel] & 0xff;
		memcpy(p_data, (unsigned char *)(HR + 49 + (channel - 1) * 8), 16);
	}
	else
	{
	}
	week_enable = ~HR[34 + channel];
	week_enable_adr = 0x10de;
	solar_en_adr = 0x10ed;
	solar_adr = 0x201d;
	time_frame_adr = 0x201f;
	write_dgus(0x201c, channel);
	//	USER_PRINTF("-->solar_enable is %bd\n",solar_enable);
	//	USER_PRINTF("-->solar_up is %bd\n",solar_up);
	//	USER_PRINTF("-->solar_down is %bd\n",solar_down);
	//	printf_tab(16,p_data);
	//	USER_PRINTF("-->solar_enable is %bd\n",solar_enable);
	display_week_parameter(week_enable_adr, week_enable);
	display_solar_parameter(solar_adr, solar_en_adr, solar_enable, solar_up, solar_down);
	display_time_frame_parameter(time_frame_adr, p_data);
}
// ͨ������
unsigned short get_channel_set(struct this_module *p, unsigned char channel, unsigned char state)
{
	unsigned short channel_enable = 0;
	switch (state)
	{
	case SINGLE_CHANNEL_STATE:
	case BATCH_CHANNEL_STATE:
		if ((DOUBLE_TIME_RELAY_MODULE == p->type))
		{
			channel_enable = (1 << channel) & 0xffff;
		}
		else
		{
			channel_enable = ((1 << (channel + 3)) | 0X08) & 0xffff;
		}
		break;
	case SINGLE_CHANNELS_STATE:
	case BATCH_CHANNELS_STATE:
		channel_enable = 0xffff;
		break;
	}
	return channel_enable;
}
// ��������
void timing_module_parameter_week_enable_channel_send(struct this_module *p, unsigned char channel, unsigned char state)
{
	unsigned char week_adr_start = 0;
	unsigned char week_adr_end = 0;
	unsigned char data_len = 0;
	unsigned char week_enable = 0, i = 0;
	switch (state)
	{
	case SINGLE_CHANNEL_STATE:
		week_enable = (~get_timing_week_enable(0x10DE)) & 0xffff;
		break;
	case SINGLE_CHANNELS_STATE:
		week_enable = (~get_timing_week_enable(0x10e6)) & 0xffff;
		break;
	case BATCH_CHANNEL_STATE:
	case BATCH_CHANNELS_STATE:
		week_enable = (~get_timing_week_enable(0x1b05)) & 0xffff;
		break;
	default:
		break;
	}
	if ((DOUBLE_TIME_RELAY_MODULE == p->type))
	{
		week_adr_start = 34;
		data_len = 18;
		week_adr_end = 51;
	}
	else
	{
		week_adr_start = 34;
		data_len = 14;
		week_adr_end = 47;
	}

	// USER_PRINTF("-->channel_enable is %x\n", get_channel_set(p, channel, state));
	// USER_PRINTF("-->week_enable is %bx\n", week_enable);
	set_master_send_hr(master_send_hr, week_adr_start, get_channel_set(p, channel, state));
	for (i = 0; i < p->channel_nb; i++)
	{
		set_master_send_hr(master_send_hr, week_adr_start + 1 + i, week_enable);
	}
	set_master_send_hr(master_send_hr, week_adr_end, ENTERPRISE);
	pack_data_send(&user_modbus, p->adr, MD_FR_MHR, week_adr_start, data_len);
}
// ��������
void timing_module_parameter_solar_channel_send(struct this_module *p, unsigned char channel, unsigned char state)
{
	unsigned char solar_adr_start = 0;
	unsigned char solar_adr_end = 0;
	unsigned char data_len = 0;
	//	unsigned short channel_enable = 0;
	unsigned char solar_enable = 0, i = 0;
	unsigned char solar_up = 0, solar_down = 0;
	unsigned short solar_parameter = 0;
	switch (state)
	{
	case SINGLE_CHANNEL_STATE:
		solar_enable = read_dgus(0x10ed);
		solar_down = read_dgus(0x201d);
		solar_up = read_dgus(0x201e);

		break;
	case SINGLE_CHANNELS_STATE:
		//		channel_enable = 0xffff;
		solar_enable = read_dgus(0x10ee);
		solar_down = read_dgus(0x202f);
		solar_up = read_dgus(0x2030);
		break;
	case BATCH_CHANNEL_STATE:
	case BATCH_CHANNELS_STATE:
		solar_enable = read_dgus(0x1B20);
		solar_down = read_dgus(0x1B0D);
		solar_up = read_dgus(0x1B0E);
		break;
	default:
		break;
	}
	if ((DOUBLE_TIME_RELAY_MODULE == p->type))
	{
		solar_adr_start = 200;
		data_len = 18;
		solar_adr_end = 217;
	}
	else
	{
		solar_adr_start = 160;
		data_len = 14;
		solar_adr_end = 173;
	}

	solar_parameter = solar_enable << 15 | solar_up << 8 | solar_down;
	set_master_send_hr(master_send_hr, solar_adr_start, get_channel_set(p, channel, state));
	for (i = 0; i < p->channel_nb; i++)
	{
		set_master_send_hr(master_send_hr, solar_adr_start + 1 + i, solar_parameter);
	}
	set_master_send_hr(master_send_hr, solar_adr_end, ENTERPRISE);
	pack_data_send(&user_modbus, p->adr, MD_FR_MHR, solar_adr_start, data_len);
}
// ʱ������
void timing_module_parameter_time_frame_channel_send(struct this_module *p, unsigned char channel, unsigned char state)
{
	unsigned char time_frame_adr_start = 0;
	unsigned char time_frame_adr_end = 0;
	unsigned char data_len = 0;
	unsigned short time_frame_set_adr = 0;
	//	unsigned short channel_enable = 0;
	unsigned char i = 0;
	unsigned short time_frame_parameter[8] = 0;
	switch (state)
	{
	case SINGLE_CHANNEL_STATE:
		time_frame_set_adr = 0x201f;
		//		time_frame_parameter[0] = read_dgus(0x201f) << 8 | read_dgus(0x2020);
		//		time_frame_parameter[1] = read_dgus(0x2021) << 8 | read_dgus(0x2022);
		//		time_frame_parameter[2] = read_dgus(0x2023) << 8 | read_dgus(0x2024);
		//		time_frame_parameter[3] = read_dgus(0x2025) << 8 | read_dgus(0x2026);
		//		time_frame_parameter[4] = read_dgus(0x2027) << 8 | read_dgus(0x2028);
		//		time_frame_parameter[5] = read_dgus(0x2029) << 8 | read_dgus(0x202a);
		//		time_frame_parameter[6] = read_dgus(0x202b) << 8 | read_dgus(0x202c);
		//		time_frame_parameter[7] = read_dgus(0x202d) << 8 | read_dgus(0x202e);

		break;
	case SINGLE_CHANNELS_STATE:
		time_frame_set_adr = 0x2031;
		//		time_frame_parameter[0] = read_dgus(0x2031) << 8 | read_dgus(0x2032);
		//		time_frame_parameter[1] = read_dgus(0x2033) << 8 | read_dgus(0x2034);
		//		time_frame_parameter[2] = read_dgus(0x2035) << 8 | read_dgus(0x2036);
		//		time_frame_parameter[3] = read_dgus(0x2037) << 8 | read_dgus(0x2038);
		//		time_frame_parameter[4] = read_dgus(0x2039) << 8 | read_dgus(0x203a);
		//		time_frame_parameter[5] = read_dgus(0x203b) << 8 | read_dgus(0x203c);
		//		time_frame_parameter[6] = read_dgus(0x203d) << 8 | read_dgus(0x203e);
		//		time_frame_parameter[7] = read_dgus(0x203f) << 8 | read_dgus(0x2040);
		break;
	case BATCH_CHANNEL_STATE:
	case BATCH_CHANNELS_STATE:
		time_frame_set_adr = 0X1B10;

		break;
	default:
		break;
	}
	time_frame_parameter[0] = read_dgus(time_frame_set_adr) << 8 | read_dgus(++time_frame_set_adr);
	time_frame_parameter[1] = read_dgus(++time_frame_set_adr) << 8 | read_dgus(++time_frame_set_adr);
	time_frame_parameter[2] = read_dgus(++time_frame_set_adr) << 8 | read_dgus(++time_frame_set_adr);
	time_frame_parameter[3] = read_dgus(++time_frame_set_adr) << 8 | read_dgus(++time_frame_set_adr);
	time_frame_parameter[4] = read_dgus(++time_frame_set_adr) << 8 | read_dgus(++time_frame_set_adr);
	time_frame_parameter[5] = read_dgus(++time_frame_set_adr) << 8 | read_dgus(++time_frame_set_adr);
	time_frame_parameter[6] = read_dgus(++time_frame_set_adr) << 8 | read_dgus(++time_frame_set_adr);
	time_frame_parameter[7] = read_dgus(++time_frame_set_adr) << 8 | read_dgus(++time_frame_set_adr);
	if ((DOUBLE_TIME_RELAY_MODULE == p->type))
	{
		time_frame_adr_start = 52;
		data_len = 130;
		time_frame_adr_end = 181;
	}
	else
	{
		time_frame_adr_start = 48;
		data_len = 98;
		time_frame_adr_end = 145;
	}
	set_master_send_hr(master_send_hr, time_frame_adr_start, get_channel_set(p, channel, state));
	// USER_PRINTF("-->p->channel_nb is %bd\n", p->channel_nb);
	for (i = 0; i < p->channel_nb; i++)
	{
		set_master_send_hr(master_send_hr, time_frame_adr_start + 1 + i * 8, time_frame_parameter[0]);
		set_master_send_hr(master_send_hr, time_frame_adr_start + 2 + i * 8, time_frame_parameter[1]);
		set_master_send_hr(master_send_hr, time_frame_adr_start + 3 + i * 8, time_frame_parameter[2]);
		set_master_send_hr(master_send_hr, time_frame_adr_start + 4 + i * 8, time_frame_parameter[3]);
		set_master_send_hr(master_send_hr, time_frame_adr_start + 5 + i * 8, time_frame_parameter[4]);
		set_master_send_hr(master_send_hr, time_frame_adr_start + 6 + i * 8, time_frame_parameter[5]);
		set_master_send_hr(master_send_hr, time_frame_adr_start + 7 + i * 8, time_frame_parameter[6]);
		set_master_send_hr(master_send_hr, time_frame_adr_start + 8 + i * 8, time_frame_parameter[7]);
	}
	set_master_send_hr(master_send_hr, time_frame_adr_end, ENTERPRISE);
	pack_data_send(&user_modbus, p->adr, MD_FR_MHR, time_frame_adr_start, data_len);
}
// ��ѡһ
unsigned char select_either_or_single(unsigned short key_adr, unsigned short adr1, unsigned short adr2)
{
	unsigned short key_nb = 0;
	get_key_value(key_adr, &key_nb);
	if (key_nb)
	{
		switch (key_nb)
		{
		case 1:
			if ((1 == read_dgus(adr1)))
			{
				write_dgus(adr1, 0);
				write_dgus(adr2, 1);
			}
			else if (0 == read_dgus(adr1))
			{
				write_dgus(adr1, 1);
				write_dgus(adr2, 0);
			}
			break;
		case 2:
			if ((1 == read_dgus(adr2)))
			{
				write_dgus(adr1, 1);
				write_dgus(adr2, 0);
			}
			else if (0 == read_dgus(adr2))
			{
				write_dgus(adr1, 0);
				write_dgus(adr2, 1);
			}
			break;
		default:
			break;
		}
		clear_key_value(key_adr, &key_nb);
		return 1;
	}
	return 0;
}
// ��ѡһѡ������
void select_either_or_all(void)
{
	select_either_or_single(0x1117, 0x110d, 0x110e);
	select_either_or_single(0x1118, 0x110f, 0x1110);
	select_either_or_single(0x1119, 0x1111, 0x1112);
	select_either_or_single(0x111a, 0x1113, 0x1114);
	select_either_or_single(0x111b, 0x1115, 0x1116);
}
// ��γ������
void timing_module_parameter_ongitude_latitude_send(struct this_module *p)
{

	set_master_send_hr(master_send_hr, 23, 0X88 << 8 | read_dgus(0x1111) << 7 | read_dgus(0x1110) << 6 | read_dgus(0x110E) << 4 | read_dgus(0x2041));
	set_master_send_hr(master_send_hr, 24, read_dgus(0x2042)); // ����L
	set_master_send_hr(master_send_hr, 25, read_dgus(0x2043)); // ����H
	set_master_send_hr(master_send_hr, 26, read_dgus(0x2044)); // γ��L
	set_master_send_hr(master_send_hr, 27, read_dgus(0x2045)); // γ��H
	set_master_send_hr(master_send_hr, 28, ENTERPRISE);
	// ����Ӽ�1 ƫ��ʱ��7�߰�  �ճ��Ӽ�1 ƫ��ʱ��7�Ͱ�
	set_master_send_hr(master_send_hr, 29, read_dgus(0x1701) << 15 | read_dgus(0x2047) << 8 | read_dgus(0x1700) << 7 | read_dgus(0x2046));
	// �ճ�����ͨ��ʹ��λ
	set_master_send_hr(master_send_hr, 30, read_dgus(0x10FF) << 15 | read_dgus(0x10FE) << 14 | read_dgus(0x10FD) << 13 | read_dgus(0x10FC) << 12 | read_dgus(0x10FB) << 11 | read_dgus(0x10FA) << 10 | read_dgus(0x10F9) << 9 | read_dgus(0x10F8) << 8 | read_dgus(0x10F7) << 7 | read_dgus(0x10F6) << 6 | read_dgus(0x10F5) << 5 | read_dgus(0x10F4) << 4 | read_dgus(0x10F3) << 3 | read_dgus(0x10F2) << 2 | read_dgus(0x10F1) << 1 | read_dgus(0x10F0));

	pack_data_send(&user_modbus, p->adr, MD_FR_MHR, 23, 8);
}
// ʱ������
void timing_module_parameter_time_send(struct this_module *p)
{
	set_master_send_hr(master_send_hr, 241, 0x88);
	set_master_send_hr(master_send_hr, 242, read_dgus(0x2048));
	set_master_send_hr(master_send_hr, 243, read_dgus(0x2049));
	set_master_send_hr(master_send_hr, 244, read_dgus(0x204a));
	set_master_send_hr(master_send_hr, 245, read_dgus(0x204b));
	set_master_send_hr(master_send_hr, 246, read_dgus(0x204c));
	set_master_send_hr(master_send_hr, 247, read_dgus(0x204d));
	set_master_send_hr(master_send_hr, 248, read_dgus(0x204e));
	set_master_send_hr(master_send_hr, 249, ENTERPRISE);
	// USER_PRINTF("-->p->adr is %bd\n", p->adr);
	pack_data_send(&user_modbus, p->adr, MD_FR_MHR, 241, 9);
}
// �������Ϳ���
void timing_module_parameter_key_ctrl(struct this_module *p, unsigned char channel)
{
	unsigned short key_nb = 0;
	get_key_value(0x10E5, &key_nb);
	if (key_nb)
	{
		switch (key_nb)
		{
		case 1:
			timing_module_parameter_week_enable_channel_send(p, channel, SINGLE_CHANNEL_STATE);
			break;
		case 2:
			timing_module_parameter_solar_channel_send(p, channel, SINGLE_CHANNEL_STATE);
			break;
		case 3:
			timing_module_parameter_time_frame_channel_send(p, channel, SINGLE_CHANNEL_STATE);
			break;
		case 4:
			return_to_previous_page();
			break;
		case 0xa:
			timing_module_parameter_ongitude_latitude_send(p);
			break;
		case 0xb:
			timing_module_parameter_time_send(p);
			break;
		case 0x11:
			timing_module_parameter_week_enable_channel_send(p, channel, SINGLE_CHANNELS_STATE);
			break;
		case 0x12:
			timing_module_parameter_solar_channel_send(p, channel, SINGLE_CHANNELS_STATE);
			break;
		case 0x13:
			timing_module_parameter_time_frame_channel_send(p, channel, SINGLE_CHANNELS_STATE);
			break;
		case 0x14:
			return_to_previous_page();
			break;
		default:
			break;
		}
		//		pack_data_send(&user_modbus,p->adr,MD_RD_HR,35,146);//��35��ʼ��146��
		clear_key_value(0x10E5, &key_nb);
	}
}
unsigned short led_sta[16] = {0};
// ���led״̬
void clear_led_sta(unsigned short *p)
{
	unsigned char i = 0;
	for (i = 0; i < 16; i++)
	{
		*(p + i) = 0;
	}
}
// ���̵���״̬
void relay_read(module_t *p)
{
	pack_data_send(&user_modbus, p->adr, MD_RD_HR, 32, 1);
}
// �̵�������
void relay_single_ctrl(module_t *p)
{

	unsigned short key_nb = 0;
	unsigned char i = 0;
	unsigned char single_key_tab[5][12] =
			{
					{2, 4, 11, 9},													 // 4·
					{2, 3, 4, 11, 10, 9},										 // 6·
					{2, 3, 4, 6, 7, 11, 10, 9},							 // 8·
					{1, 2, 3, 4, 5, 12, 11, 10, 9, 8},			 // 10·
					{1, 2, 3, 4, 5, 6, 7, 12, 11, 10, 9, 8}, // 12·
			};
	unsigned char double_key_tab[16] = {0x01, 0x02, 0x04, 0x05, 12, 11, 9, 8, 0x81, 0x82, 0x84, 0x85, 0x8c, 0x8b, 0x89, 0x88};
	get_key_value(0x1010, &key_nb);
	if (HR[32] != g_var_module.led_sta_bak) // ״̬�任

	{
		USER_PRINTF("-->relay state has been changed!\n");
		g_var_module.led_sta_bak = HR[32];
		// USER_PRINTF("-->g_var_module.led_sta_bak is %d!\n", g_var_module.led_sta_bak);
		for (i = 0; i < 16; i++)
		{
			led_sta[i] = (HR[32] >> i) & 0x01;
			write_dgusii_vp(0x1030 + i, (unsigned char *)&led_sta[i], 1);
			//			sys_delay_about_ms(1);
		}
	}
	if (key_nb)
	{

		led_sta[key_nb - 1] ^= 1;
		printf_tab(32, (unsigned char *)&led_sta[0]);
		write_dgusii_vp(0x1030 + key_nb - 1, (unsigned char *)&led_sta[key_nb - 1], 1);
		if ((SINGLE_RELAY_MODULE == p->type) || (SINGLE_TIME_RELAY_MODULE == p->type))
		{
			if (4 == p->channel_nb)
				set_master_send_hr(master_send_hr, 0, single_key_tab[0][key_nb - 1]);
			else if (6 == p->channel_nb)
				set_master_send_hr(master_send_hr, 0, single_key_tab[1][key_nb - 1]);
			else if (8 == p->channel_nb)
				set_master_send_hr(master_send_hr, 0, single_key_tab[2][key_nb - 1]);
			else if (10 == p->channel_nb)
				set_master_send_hr(master_send_hr, 0, single_key_tab[3][key_nb - 1]);
			else if (12 == p->channel_nb)
				set_master_send_hr(master_send_hr, 0, single_key_tab[4][key_nb - 1]);
		}
		else if ((DOUBLE_RELAY_MODULE == p->type) || (DOUBLE_TIME_RELAY_MODULE == p->type))
		{
			set_master_send_hr(master_send_hr, 0, double_key_tab[key_nb - 1]);
		}
		set_master_send_hr(master_send_hr, 1, ENTERPRISE);
		pack_data_send(&user_modbus, p->adr, MD_FR_MHR, 0, 2);
		HR[32] = g_var_module.led_sta_bak = master_send_hr[32];
		clear_key_value(0x1010, &key_nb);
	}
}
// �̵���ȫ��ȫ��
void relay_all_ctrl(module_t *p)
{
	unsigned char i = 0;
	unsigned short key_nb = 0;
	get_key_value(0x100c, &key_nb);
	if ((1 == key_nb) || (2 == key_nb))
	{
		for (i = 0; i < 16; i++)
		{
			led_sta[i] = key_nb > 1 ? 0 : 1;
			write_dgusii_vp(0x1030 + i, (unsigned char *)&led_sta[i], 1);
		}
		set_master_send_hr(master_send_hr, 31, 0xffff);
		set_master_send_hr(master_send_hr, 32, key_nb > 1 ? 0 : 0xffff);
		set_master_send_hr(master_send_hr, 33, ENTERPRISE);
		pack_data_send(&user_modbus, p->adr, MD_FR_MHR, 31, 3);
		clear_key_value(0x100c, &key_nb);
		HR[32] = g_var_module.led_sta_bak = master_send_hr[32];
	}
}
// ���ͼ̵������ع㲥����
void all_module_on_off(void)
{
	unsigned short key_nb = 0;
	get_key_value(RELAY_MODULE_KEY_ALL_ON_ADR, &key_nb);
	if ((1 == key_nb))
	{
		set_master_send_hr(master_send_hr, 31, 0xffff);
		set_master_send_hr(master_send_hr, 32, 0xffff);
		set_master_send_hr(master_send_hr, 33, ENTERPRISE);
		pack_data_send(&user_modbus, 0, MD_FR_MHR, 31, 3);
		clear_key_value(RELAY_MODULE_KEY_ALL_ON_ADR, &key_nb);
		HR[32] = g_var_module.led_sta_bak = 0xffff;
	}
	get_key_value(RELAY_MODULE_KEY_ALL_OFF_ADR, &key_nb);
	if ((1 == key_nb))
	{
		set_master_send_hr(master_send_hr, 31, 0xffff);
		set_master_send_hr(master_send_hr, 32, 0);
		set_master_send_hr(master_send_hr, 33, ENTERPRISE);
		pack_data_send(&user_modbus, 0, MD_FR_MHR, 31, 3);
		clear_key_value(RELAY_MODULE_KEY_ALL_OFF_ADR, &key_nb);
		HR[32] = g_var_module.led_sta_bak = 0;
	}
}
// �̵���״̬ˢ��
void relay_refesh(module_t *p)
{
	unsigned short key_nb = 0;
	get_key_value(0x100c, &key_nb);
	if (3 == key_nb)
	{
		pack_data_send(&user_modbus, p->adr, MD_RD_HR, 32, 1);

		clear_key_value(0x100c, &key_nb);
	}
}
// �̵����������
void relay_ctrl(module_t *p)
{
	relay_single_ctrl(p);
	relay_all_ctrl(p);
	relay_refesh(p);
}
// ������
void dim_read(module_t *p)
{
	delay_us(1000);
	pack_data_send(&user_modbus, p->adr, MD_RD_HR, 23, 4);
}
// �������
void dim_single_ctrl(module_t *p)
{
	unsigned char i = 0;
	static unsigned char dim_send_flag = 0, key_down_flag = 0, key_up_flag = 0;
	unsigned short key_nb = 0;
	unsigned short y_coordinate = 0;
	unsigned short x_coordinate = 0;
	unsigned char dim_gear[8] = {0};
	unsigned char touch_sta[8] = {0};
	for (i = 0; i < 4; i++)
	{
		if (g_var_module.dim_light_bak[i] != HR[23 + i])
		{
			USER_PRINTF("-->dim state has been changed!\n");
			g_var_module.dim_light_bak[i] = HR[23 + i];
			if (VOL_DIM_MODULE == p->type)
			{

				write_dgus(0x1100 + i, g_var_module.dim_light_bak[i]);
			}
			else
			{
				write_dgus(0x1013 + i, g_var_module.dim_light_bak[i]);
			}
		}
		//		sys_delay_about_ms(1);
	}
	if ((DIM_CTRL_PAGE == read_pic()) || (SCR_CTRL_PAGE == read_pic()))
	{

		//		USER_PRINTF("display_pic_dim_edit \n");
		read_dgusii_vp(0x16, touch_sta, 4);
		if (touch_sta[0])
		{
			printf_tab(8, touch_sta);

			y_coordinate = touch_sta[4] * 256 + touch_sta[5];
			x_coordinate = touch_sta[2] * 256 + touch_sta[3];
			if ((3 == touch_sta[1]) && (0 == key_down_flag))
			{
				key_down_flag = 1;
				key_up_flag = 0;
				USER_PRINTF("-->key_down_flag = 1 \n");
			}
			else if ((2 == touch_sta[1]) && (1 == key_down_flag))
			{
				key_down_flag = 3;
				key_up_flag = 0;
				USER_PRINTF("-->key_down_flag = 3 \n");
			}
			else if ((2 == touch_sta[1]) && (0 == key_down_flag) && (0 == key_up_flag))
			{
				key_up_flag = 1;
				USER_PRINTF("-->key_up_flag = 1 \n");
			}
			touch_sta[0] = 0;
			touch_sta[1] = 0;
			touch_sta[2] = 0;
			touch_sta[3] = 0;
			touch_sta[4] = 0;
			touch_sta[5] = 0;
			write_dgusii_vp(0x16, touch_sta, 4);
			USER_PRINTF("-->x_coordinate = %d,y_coordinate = %d \n", x_coordinate, y_coordinate);
		}

		set_master_send_hr(master_send_hr, 27, ENTERPRISE);
		if (VOL_DIM_MODULE == p->type)
		{
			read_dgusii_vp(0x1100, dim_gear, 4);
			//			set_master_send_hr(master_send_hr,22,0xffff);
			//			set_master_send_hr(master_send_hr,23,dim_gear[1]);
			//			set_master_send_hr(master_send_hr,24,dim_gear[3]);
			//			set_master_send_hr(master_send_hr,25,dim_gear[5]);
			//			set_master_send_hr(master_send_hr,26,dim_gear[7]);
		}
		else
		{
			read_dgusii_vp(0x1013, dim_gear, 4);
		}
		set_master_send_hr(master_send_hr, 22, 0xffff);
		set_master_send_hr(master_send_hr, 23, dim_gear[1]);
		set_master_send_hr(master_send_hr, 24, dim_gear[3]);
		set_master_send_hr(master_send_hr, 25, dim_gear[5]);
		set_master_send_hr(master_send_hr, 26, dim_gear[7]);
		HR[23] = g_var_module.dim_light_bak[0] = master_send_hr[23];
		HR[24] = g_var_module.dim_light_bak[1] = master_send_hr[24];
		HR[25] = g_var_module.dim_light_bak[2] = master_send_hr[25];
		HR[26] = g_var_module.dim_light_bak[3] = master_send_hr[26];
		if (((y_coordinate > 262) && (y_coordinate < 550) && ((3 == key_down_flag) || (1 == key_up_flag))))

		{
			USER_PRINTF("-->send--OK \n");
			dim_send_flag = 1;
			key_down_flag = 0;
			key_up_flag = 0;
		}

		//		USER_PRINTF("g_var_module.dim_light_bak-->");
		//		printf_tab(8,g_var_module.dim_light_bak);
		if (1 == dim_send_flag)
		{
			//			USER_PRINTF("dim_send_flag --> 1 \n");
			dim_send_flag = 0;
			pack_data_send(&user_modbus, p->adr, MD_FR_MHR, 22, 6);

			USER_PRINTF("-->DIM_MODULE dim_send_ok  \n");
		}
	}
	else
	{
		key_down_flag = 0;
		key_up_flag = 0;
	}
}
// ����ȫ��ȫ��
void dim_all_ctrl(module_t *p)
{
	//	unsigned char i = 0;
	unsigned short key_nb = 0;
	//	unsigned char dim_gear[8] = {0};
	get_key_value(0x100c, &key_nb);
	if ((0x11 == key_nb) || (0x12 == key_nb))
	{

		set_master_send_hr(master_send_hr, 22, 0xffff);
		set_master_send_hr(master_send_hr, 23, p->type == VOL_DIM_MODULE ? (key_nb > 0x11 ? 0 : 10) : (key_nb > 0x11 ? 0 : 100));
		set_master_send_hr(master_send_hr, 24, p->type == VOL_DIM_MODULE ? (key_nb > 0x11 ? 0 : 10) : (key_nb > 0x11 ? 0 : 100));
		set_master_send_hr(master_send_hr, 25, p->type == VOL_DIM_MODULE ? (key_nb > 0x11 ? 0 : 10) : (key_nb > 0x11 ? 0 : 100));
		set_master_send_hr(master_send_hr, 26, p->type == VOL_DIM_MODULE ? (key_nb > 0x11 ? 0 : 10) : (key_nb > 0x11 ? 0 : 100));
		set_master_send_hr(master_send_hr, 27, ENTERPRISE);
		pack_data_send(&user_modbus, p->adr, MD_FR_MHR, 22, 6);
		clear_key_value(0x100c, &key_nb);
		write_dgusii_vp(VOL_DIM_MODULE == p->type ? 0x1100 : 0x1013, (unsigned char *)(master_send_hr + 23), 4);
		HR[23] = g_var_module.dim_light_bak[0] = master_send_hr[23];
		HR[24] = g_var_module.dim_light_bak[1] = master_send_hr[24];
		HR[25] = g_var_module.dim_light_bak[2] = master_send_hr[25];
		HR[26] = g_var_module.dim_light_bak[3] = master_send_hr[26];
	}
}
// ���Ϳ��ع㲥����
void all_dim_on_off(void)
{
	//	unsigned char i = 0;
	unsigned short key_nb = 0;
	//	unsigned char dim_gear[8] = {0};
	get_key_value(DIM_MODULE_KEY_ALL_ON_ADR, &key_nb);
	if (1 == key_nb)
	{

		set_master_send_hr(master_send_hr, 22, 0xffff);
		set_master_send_hr(master_send_hr, 23, 100);
		set_master_send_hr(master_send_hr, 24, 100);
		set_master_send_hr(master_send_hr, 25, 100);
		set_master_send_hr(master_send_hr, 26, 100);
		set_master_send_hr(master_send_hr, 27, ENTERPRISE);
		pack_data_send(&user_modbus, 0, MD_FR_MHR, 22, 6);
		clear_key_value(DIM_MODULE_KEY_ALL_ON_ADR, &key_nb);
		HR[23] = g_var_module.dim_light_bak[0] = master_send_hr[23];
		HR[24] = g_var_module.dim_light_bak[1] = master_send_hr[24];
		HR[25] = g_var_module.dim_light_bak[2] = master_send_hr[25];
		HR[26] = g_var_module.dim_light_bak[3] = master_send_hr[26];
	}
	get_key_value(DIM_MODULE_KEY_ALL_OFF_ADR, &key_nb);
	if (1 == key_nb)
	{

		set_master_send_hr(master_send_hr, 22, 0xffff);
		set_master_send_hr(master_send_hr, 23, 0);
		set_master_send_hr(master_send_hr, 24, 0);
		set_master_send_hr(master_send_hr, 25, 0);
		set_master_send_hr(master_send_hr, 26, 0);
		set_master_send_hr(master_send_hr, 27, ENTERPRISE);
		pack_data_send(&user_modbus, 0, MD_FR_MHR, 22, 6);
		clear_key_value(DIM_MODULE_KEY_ALL_OFF_ADR, &key_nb);
		HR[23] = g_var_module.dim_light_bak[0] = master_send_hr[23];
		HR[24] = g_var_module.dim_light_bak[1] = master_send_hr[24];
		HR[25] = g_var_module.dim_light_bak[2] = master_send_hr[25];
		HR[26] = g_var_module.dim_light_bak[3] = master_send_hr[26];
	}
}
// ����״̬ˢ��
void dim_refesh(module_t *p)
{
	unsigned short key_nb = 0;
	get_key_value(0x100c, &key_nb);
	if (0x13 == key_nb)
	{
		pack_data_send(&user_modbus, p->adr, MD_RD_HR, 23, 4);

		clear_key_value(0x100c, &key_nb);
	}
}
// ����������
void dim_ctrl(module_t *p)
{
	dim_single_ctrl(p);
	dim_all_ctrl(p);
	dim_refesh(p);
}
// ��ʾ���ý���
void display_interface(module_t *p)
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

		display_relay_interface(p);
		USER_PRINTF("-->display_relay_interface\n");
		relay_read(p);
		break;
	case VOL_DIM_MODULE:
	case SCR_DIM_MODULE:
		clear_touch_sta();
		display_dim_interface(p);
		USER_PRINTF("-->display_dim_interface\n");
		dim_read(p);
		break;
	default:

		break;
	}
}
// ��ģ��

void module_touch(module_t *p, unsigned char key)
{
	norflash_read((key - 1) * MODULE_FLASH_SIZE, (unsigned char *)p, MODULE_FLASH_SIZE);
	display_interface(p);
}
void module_touch_ctrl(void)
{
	unsigned short key_nb = 0;
	get_key_value(0x100a, &key_nb);
	if (key_nb)
	{
		//		clear_led_sta(led_sta);
		module_touch(&module, key_nb);
		clear_key_value(0x100a, &key_nb);
		key_nb = 0;
	}
}
// �ָ���������
void factory_data_reset(void)
{

	unsigned char ret_tab[4] = {0x55, 0xaa, 0x5a, 0xa5};
	unsigned short key_nb = 0;
	unsigned char i = 0;
	get_key_value(0x100b, &key_nb);
	if (1 == key_nb)
	{
		// ��дT5LƬ��256KW Flash��mod=0x5A Ϊ��ȡ��mod=0xA5Ϊд��
		// addr=DGUS������ַ��������ż����addr_flash=flash��ȡ��ַ��������ż����len=��ȡ�ֳ��ȣ�������ż����
		// ����Ļ�ı�����ַ������һ��������ȫ��0�ģ�Ȼ������д�뵽���ݿ���и���
		for (i = 0; i < 60; i++)//�嵽F000 һЩ�������ò����
		{
			T5L_Flash(0xA5, 0Xe000, 1024 * i, 1024); // д0FLASH���
		}
		//		T5L_Flash(0x5A,0X3110, 0,1024); //��Ϊ0
		clear_key_value(0x100a, &key_nb);
		key_nb = 0;
		write_dgusii_vp(0x04, (unsigned char *)&ret_tab, 2); // MCU��λ
	}
}

/**
 * @brief �޸�ģ��-��ʾ�༭ҳ��
 * @param {module_t} *p ģ��ָ��
 * @return {*}
 */
void display_edit_interface(module_t *p)
{

	USER_PRINTF("display_edit_interface\n");
	// USER_PRINTF("current module type is %bd\n", p->type);
	write_dgus(0x2000, p->adr);
	module_type_bak = p->type;
	write_dgusii_vp(0x3110, (unsigned char *)&module_name_tab[p->type], 6);
	write_dgus(0x1000, p->channel_nb);
	write_dgusii_vp(0x3000, (unsigned char *)&p->module_name, 8);
	//	sprintf(p->channel_name,"%.256s",p->channel_name);
	// USER_PRINTF("-->module name is %s\n", p->module_name);
	// USER_PRINTF("-->module channel name is %s\n", p->channel_name);
	write_dgusii_vp(0x3008, (unsigned char *)&p->channel_name, 8 * 16);
	switch (p->channel_nb)
	{
	case 4:
		if ((DOUBLE_RELAY_MODULE == p->type) || (DOUBLE_TIME_RELAY_MODULE == p->type))
			pic_set(10);
		else if ((SINGLE_RELAY_MODULE == p->type) || (SINGLE_TIME_RELAY_MODULE == p->type))
			pic_set(5);
		else if ((VOL_DIM_MODULE == p->type))
			pic_set(63);
		else if ((SCR_DIM_MODULE == p->type))
			pic_set(64);
		break;
	case 6:
		if ((DOUBLE_RELAY_MODULE == p->type) || (DOUBLE_TIME_RELAY_MODULE == p->type))
			pic_set(11);
		else if ((SINGLE_RELAY_MODULE == p->type) || (SINGLE_TIME_RELAY_MODULE == p->type))
			pic_set(6);

		break;
	case 8:
		if ((DOUBLE_RELAY_MODULE == p->type) || (DOUBLE_TIME_RELAY_MODULE == p->type))
			pic_set(12);
		else if ((SINGLE_RELAY_MODULE == p->type) || (SINGLE_TIME_RELAY_MODULE == p->type))
			pic_set(7);

		break;
	case 10:
		if ((DOUBLE_RELAY_MODULE == p->type) || (DOUBLE_TIME_RELAY_MODULE == p->type))
			pic_set(13);
		else if ((SINGLE_RELAY_MODULE == p->type) || (SINGLE_TIME_RELAY_MODULE == p->type));
			pic_set(8);

		break;
	case 12:
		if ((DOUBLE_RELAY_MODULE == p->type) || (DOUBLE_TIME_RELAY_MODULE == p->type))
			pic_set(14);
		else if ((SINGLE_RELAY_MODULE == p->type) || (SINGLE_TIME_RELAY_MODULE == p->type))
			pic_set(9);

		break;
	case 14:
		if ((DOUBLE_RELAY_MODULE == p->type) || (DOUBLE_TIME_RELAY_MODULE == p->type))
			pic_set(15);

		break;
	case 16:
		if ((DOUBLE_RELAY_MODULE == p->type) || (DOUBLE_TIME_RELAY_MODULE == p->type))
			pic_set(16);
		break;
	default:
		break;
	}

}

// �޸�ģ����Ϣ
void modifying_module_information(module_t *p)

{
	unsigned short key_nb = 0;
	unsigned char select_index = 0;
	get_key_value(0x1009, &key_nb);
	if ((3 == key_nb))
	{
		clear_key_value(0x1009, &key_nb);
		// select_index = get_selected_sequence_number(0x1104, 8);
		select_index = get_box_select_number(0x1400, 64);
		// USER_PRINTF("select_index-->%bd\n", select_index);
		if ((select_index > 0) && (FULL == check_whether_data_exists(0, select_index - 1, MODULE_FLASH_SIZE)))
		{
			modfity_index = select_index - 1;
			norflash_read((select_index - 1) * MODULE_FLASH_SIZE, (unsigned char *)p, MODULE_FLASH_SIZE);
			//			display_interface(p);
			display_edit_interface(p);
			USER_PRINTF("-->modify module successfully!\n");
			delete_arr_data(&g_var_module.module_adr[0],modfity_index);
			// g_var_module.module_edit_sta = MODIFY;
		}
	}
}

// ɾ��ѡ��ģ����Ϣ
void clear_select_module_information(void)

{
	unsigned short key_nb = 0;
	unsigned char select_index = 0;
	//	unsigned char i = 0;
	get_key_value(0x1012, &key_nb);
	if ((1 == key_nb))
	{
		//		select_index = get_selected_sequence_number(0x1104, 8);
		select_index = get_box_select_number(0x1400, 64);
		// USER_PRINTF("select_index-->%bd\n", select_index);
		if (select_index)
		{
			//			if(FULL == p->data_sta)
			//			{
			T5L_Flash(0xA5, 0Xe000, (select_index - 1) * MODULE_FLASH_SIZE, MODULE_FLASH_SIZE);						 // д0FLASH���
			T5L_Flash(0x5A, 0X3520 + (select_index - 1) * 32, (select_index - 1) * MODULE_FLASH_SIZE, 32); // ��Ϊ0��������Ϣ
			//				T5L_Flash(0x5A,0X3120+(select_index-1)*32, TIMING_SCENE_INFOR_FLASH_ADR_END,32); //��Ϊ0��������Ϣ
			T5L_Flash(0x5A, 0x3120 + (select_index - 1) * 8, (select_index - 1) * MODULE_FLASH_SIZE, 8); // ��Ϊ0
			write_dgusii_vp(0x3120 + 8 * (select_index - 1), (unsigned char *)module_default_name, 8);
			delete_arr_data(&g_var_module.module_adr[0],select_index - 1);
			printf_tab(20,&g_var_module.module_adr[0]);
			//			}
		}
		clear_all_module_var_en(0x1400, 64);
		clear_key_value(0x1012, &key_nb);
	}
}
// �������ģ����Ϣ
void clear_all_module_information(void)

{
	unsigned short key_nb = 0;
	unsigned char i = 0;
	//	unsigned char select_index = 0;
	get_key_value(0x1011, &key_nb);
	if ((1 == key_nb))
	{
		USER_PRINTF("clear_all_module_information\n");
		//		T5L_Flash(0xA5,0Xe000, 0,1024);	    //д0FLASH���
		//		T5L_Flash(0xA5,0Xe000, 1024,1024);	//д0FLASH���
		//		T5L_Flash(0xA5,0Xe000, 2048,1024);	//д0FLASH���
		//		T5L_Flash(0xA5,0Xe000, 3072,1024);	//д0FLASH���
		//		T5L_Flash(0xA5,0Xe000, 4096,1024);	//д0FLASH���
		//		T5L_Flash(0xA5,0Xe000, 8192,768);	  //д0FLASH���
		for (i = 0; i < 8; i++)
		{
			T5L_Flash(0xA5, 0Xe000, 1024 * i, 1024); // д0FLASH���
		}
		T5L_Flash(0xA5, 0Xe000, 8192, 768);	 // д0FLASH���
		T5L_Flash(0x5A, 0X3520, 0, 32 * 64); // ��Ϊ0
		T5L_Flash(0x5A, 0x3120, 0, 8 * 64);	 // ��Ϊ0
		for (i = 0; i < 64; i++)
		{
			write_dgusii_vp(0x3120 + 8 * i, (unsigned char *)module_default_name, 8);
		}
		//		T5L_Flash(0x5A,0X3120, 0,1024); //��Ϊ0
		clear_key_value(0x1011, &key_nb);
	}
}
// ��ʼ������
void init_pwd_f(void)
{
	unsigned char mask[4] = {0};
	unsigned char pwd_enable = 0;
	norflash_read(0x020002, (unsigned char *)&mask, 2);

	pwd_enable = read_dgus(PWD_SELECT_FRAME_ADR);
	if (0xff != mask[0])
	{
		init_pwd = read_dgus(0x2050);
	}
	else
	{
		T5L_Flash(0x5a, PWD_SELECT_FRAME_ADR, PWD_FLASH_ADR, 2); // ������ʹ��
		T5L_Flash(0x5A, 0x2050, 0x020000, 2);										 // ��FLASH������
		init_pwd = read_dgus(0x2050);
	}
	// printf_tab(2, mask);
	// USER_PRINTF("-->read pwd enable is %bd\n", pwd_enable);
	// USER_PRINTF("-->read_pwd var is %d\n", init_pwd);
}
// �޸�����
void change_pwd(unsigned short *pwd_init)
{
	unsigned short key_nb = 0;
	unsigned char mask[4] = {0xff, 0xff, 0xff, 0xff};
	get_key_value(0x111e, &key_nb);
	if (1 == key_nb)
	{
		clear_key_value(0x111e, &key_nb);
		if (*pwd_init != read_dgus(0x2050))
		{
			*pwd_init = read_dgus(0x2050);
			T5L_Flash(0xA5, 0x2050, 0x020000, 2); // д���뵽FLASH
			norflash_write(0x020002, (unsigned char *)&mask, 2);
			USER_PRINTF("-->change password success!\n");
			printf_tab(2, mask);
		}
	}
}
// ѡ������ʹ��
void select_password_enable(void)
{
	//	static unsigned char var_enable = 0;
	unsigned short key_nb = 0;
	unsigned char pwd_enable = 0xff;
	get_key_value(0x1242, &key_nb);
	if (1 == key_nb)
	{

		pwd_enable = check_or_not_frame(PWD_SELECT_FRAME_ADR);
		if (1 == check_constant(pwd_enable,&g_var_module.pwd_enable_bak))
		{
			T5L_Flash(0xA5, PWD_SELECT_FRAME_ADR, PWD_FLASH_ADR, 2); // д����ʹ�ܵ�FLASH
			// USER_PRINTF("-->set password enable is %bd\n", pwd_enable);
		}

		clear_key_value(0x1242, &key_nb);
	}
}
// ��ʼ������������
void init_beep_enable(void)
{
	unsigned char mask[4] = {0};
	unsigned char beep_enable = 0;
	norflash_read(0x020008, (unsigned char *)&mask, 2);

	if (0xff != mask[0])
	{

		USER_PRINTF("-->init_beep_error\n");
	}
	else
	{

		T5L_Flash(0x5A, BEEP_ENABLE_DGUS_ADR, 0x020006, 2); // ��FLASH��ʹ��
	}
	beep_enable = read_dgus(BEEP_ENABLE_DGUS_ADR);
	sys_delay_about_ms(5);
	beep_ctrl(beep_enable); // control����������
	USER_PRINTF("-->read beep enable is %bd\n", beep_enable);
}
// ѡ�����������
void select_beep_enable(void)
{
	unsigned char mask[4] = {0xff, 0xff, 0xff, 0xff};
	unsigned short key_nb = 0;
	unsigned char beep_enable = 0xff;
	get_key_value(0x1241, &key_nb);
	if ((1 == key_nb))
	{
		beep_enable = check_or_not_frame(0x1800);

		if (1 == check_constant(beep_enable,&g_var_module.beep_enable_bak))
		{
			T5L_Flash(0xA5, BEEP_ENABLE_DGUS_ADR, 0x020006, 2); // д������ʹ�ܵ�FLASH
			norflash_write(0x020008, (unsigned char *)&mask, 2);
			beep_ctrl(beep_enable); // control����������
			// USER_PRINTF("-->set beep enable is %bd,\n!", beep_enable);
		}
		clear_key_value(0x1241, &key_nb);
	}
}
// �������
void clear_pwd(unsigned short adr)
{
	write_dgus(adr, 0xffff);
}
// �����Ƿ�һ��
void check_whether_the_passwords_are_consistent(unsigned char password_enable, unsigned short pwd_input, unsigned short pwd_init)
{
	if ((password_enable))
	{
		if ((pwd_init == pwd_input) || (SUPER_PASSWORD == pwd_input))
		{
			pic_set(1);
			clear_pwd(0x204f);
		}
		else
		{
		}
	}
}
// ��ҳ�ж�����
void starup_page_pwd(unsigned char password_enable)
{
	unsigned short key_nb = 0;
	get_key_value(0x111c, &key_nb);
	if ((1 == key_nb))
	{
		if (1 == password_enable)
		{
			pop_menu_key_ctrl(0x20);
		}
		else
		{
			pic_set(1);
			pic_page = 1;
		}
		clear_key_value(0x111c, &key_nb);
	}
	check_whether_the_passwords_are_consistent(password_enable, read_dgus(0x204f), init_pwd);
}
// ��ʼ��������� ��flash
void init_back_light_parameters(void)
{
	unsigned char mask[4] = {0};
	unsigned char backlight_enable = 0;
	unsigned char work_light = 0;
	unsigned short sleep_time = 0;
	norflash_read(0x02000e, (unsigned char *)&mask, 2);

	if (0xff != mask[0])
	{
	}
	else
	{
		USER_PRINTF("-->read_back_light_parameters_successfully!\n");
		T5L_Flash(0x5A, 0x1704, 0x02000a, 2); // ��FLASH������
		T5L_Flash(0x5A, 0x2052, 0x02000c, 2); // ��FLASH������
	}

	backlight_enable = read_dgus(0x1704);
	// USER_PRINTF("-->read backlight enable is %bd\n", backlight_enable);
	sys_delay_about_ms(5);
	work_light = read_dgus(0x2052);
	// USER_PRINTF("-->read work_light is %bd\n", work_light);
	sys_delay_about_ms(5);
	sleep_time = read_dgus(0x2053) * 100;
	// USER_PRINTF("-->read sleep_time is %ds\n", sleep_time / 100);
	sys_delay_about_ms(5);
	back_light_ctrl(backlight_enable);
	sys_delay_about_ms(2);
	sys_led_config(work_light, 0, sleep_time);
}
// ���汳�������flash
void save_back_light_parameters(void)
{
	unsigned char mask[4] = {0xff, 0xff, 0xff, 0xff};
	T5L_Flash(0xA5, 0x1704, 0x02000a, 2); // д�Զ�����������FLASH
	T5L_Flash(0xA5, 0x2052, 0x02000c, 2); // д���������FLASH
	norflash_write(0x02000e, (unsigned char *)&mask, 2);
	USER_PRINTF("-->save_back_light_parameters_successfully!\n");
}
/**
 * @brief ��ѡ����
 * @param {unsigned short} adr ��ѡ��ַ
 * @return {*}��ѡ�Ƿ�ѡ�� 1ѡ��  0δѡ��
 */
unsigned char check_or_not_frame(unsigned short adr)
{
	unsigned char check_var = 0;
	check_var = read_dgus(adr);

	check_var ^= 1;
	sys_delay_about_ms(3);
	write_dgus(adr, check_var);
	// USER_PRINTF("--> 0x%x check_var is %bd\n", adr, check_var);
	return check_var;
}
/**
 * @brief ��������
 * @return {*}
 */
void lock_screen_enable_ctrl(void)
{
	unsigned char backlight_enable = 0;
	unsigned short key_nb = 0;
	get_key_value(0x1240, &key_nb);
	if ((1 == key_nb))
	{
		backlight_enable = check_or_not_frame(0x1704);

		if (1 == check_constant(backlight_enable,&g_var_module.backlight_enable_bak))
		{
			back_light_ctrl(backlight_enable);
			USER_PRINTF("-->backlight_enable is %bd\n", backlight_enable);
			save_back_light_parameters();
		}

		clear_key_value(0x1240, &key_nb);
	}
}
// ��������
void back_light_set(void)
{
	unsigned short key_nb = 0;
	unsigned char back_enable = 0;
	unsigned char work_light = 0;
	unsigned short sleep_time = 0;
	get_key_value(0x1126, &key_nb);
	if ((1 == key_nb))
	{
		// back_enable = read_dgus(0x1128);
		// sys_delay_about_ms(5);
		work_light = read_dgus(0x2052);
		sys_delay_about_ms(5);
		sleep_time = read_dgus(0x2053) * 100;
		// USER_PRINTF("-->sleep_time is %d\n", sleep_time);
		sys_delay_about_ms(5);
		sys_led_config(work_light, 0, sleep_time);
		// back_light_ctrl(back_enable);
		// sys_delay_about_ms(5);
		// sys_led_config(work_light, 0, sleep_time);
		clear_key_value(0x1126, &key_nb);
		USER_PRINTF("-->back_light_set_ok!\n");
		save_back_light_parameters();
		pop_menu_key_ctrl(0xdd);
	}
}
// �����л�����ҳ
void black_screen_switches_to_home_page(void)
{
	static unsigned char s_sta = 0;
	unsigned char read_data[2] = {0};
	read_dgusii_vp(0x31, read_data, 1);
	if (0 == s_sta)
	{
		//		s_sta = 1;
		if (0 == read_data[1]) // ��Ļ����Ϊ0
		{
			s_sta = 1;
			pic_set(0);
		}
	}
	if (0 != read_data[1])
	{
		s_sta = 0;
	}
	//	printf_tab(2,read_data);
}
// ���ʱ�ز������ñ���
void clear_batch_set_var(void)
{
	if (g_var_module.batch_send_count > MODULE_NUB)
	{
		g_var_module.batch_send_count = 0;
		g_var_module.batch_set_parameters_flag = 0;
		g_var_module.batch_send_cyc_time = 0;
		pop_menu_key_ctrl(0xf0); // �������
		// pic_set(57);
		USER_PRINTF("-->time module batch set parameter successfully!\n");
	}
}
// ִ�ж�Ӧ�Ķ�������
void run_timing_module_parameter_set(module_t *p, unsigned char fun_execute_nb, unsigned char channel, unsigned char parameter_set_sta)
{
	switch (fun_execute_nb)
	{
	case 0:
		timing_module_parameter_ongitude_latitude_send(p);
		break;
	case 1:
		timing_module_parameter_time_send(p);
		break;
	case 2:
		timing_module_parameter_week_enable_channel_send(p, channel, parameter_set_sta);
		break;
	case 3:
		timing_module_parameter_solar_channel_send(p, channel, parameter_set_sta);
		break;
	case 4:
		timing_module_parameter_time_frame_channel_send(p, channel, parameter_set_sta);
		break;
	default:
		break;
	}
}
// ʱ�ز�����������
void timing_module_parameter_batch_send(void)
{
	//  static unsigned char state = 0;
	//	unsigned char channel = 0;
	if (((1 <= g_var_module.batch_set_parameters_flag) && (g_var_module.batch_set_parameters_flag <= 5)) || (0x13 == g_var_module.batch_set_parameters_flag) || (0x14 == g_var_module.batch_set_parameters_flag) || (0x15 == g_var_module.batch_set_parameters_flag)) // ��֪Ϊ�λ����쳣ֵ���֣��ų��쳣��ֵ
	{
		//		USER_PRINTF("-->batch_set_parameters_flag is %bd\n",g_var_module.batch_set_parameters_flag);
		if ((0 == g_var_module.batch_send_cyc_time) && (mbh_getState() == MBH_STATE_IDLE))
		{
			g_var_module.batch_send_cyc_time = 300;
			norflash_read(g_var_module.batch_send_count * 140, (unsigned char *)&temp_module, 140);
			if (FULL == temp_module.data_sta)
			{
				if ((temp_module.type == SINGLE_TIME_RELAY_MODULE) || (temp_module.type == DOUBLE_TIME_RELAY_MODULE))
				{
					//					USER_PRINTF("-->temp_module.adr is %bd\n",temp_module.adr);
					run_timing_module_parameter_set(&temp_module, g_var_module.parameter_set_fun_execute_nb, g_var_module.parameter_set_channel_number, g_var_module.parameter_set_sta);
					g_var_module.batch_send_count++;
				}
				else
				{
					g_var_module.batch_send_count++;
				}
			}
			else
			{
				g_var_module.batch_send_count++;
			}
		}
	}
	clear_batch_set_var();
}
/**
 * @brief ��������ʱ�ز���
 * @return {*}
 */
void batch_set_time_parameters(void)
{
	unsigned short key_nb = 0;
	get_key_value(0x112a, &key_nb);
	if ((key_nb))
	{
		switch (key_nb)
		{
		case 1: // ��γ������
			g_var_module.parameter_set_fun_execute_nb = 0;
			g_var_module.parameter_set_channel_number = 0;
			g_var_module.parameter_set_sta = SINGLE_CHANNEL_STATE;
			break;
		case 2: // ʱ������
			g_var_module.parameter_set_fun_execute_nb = 1;
			g_var_module.parameter_set_channel_number = 0;
			g_var_module.parameter_set_sta = SINGLE_CHANNEL_STATE;
			break;
		case 3: // ��������
			g_var_module.parameter_set_fun_execute_nb = 2;
			g_var_module.parameter_set_channel_number = read_dgus(0x1B04);
			g_var_module.parameter_set_sta = BATCH_CHANNEL_STATE;
			break;
		case 4: // ���ն�����
			g_var_module.parameter_set_fun_execute_nb = 3;
			g_var_module.parameter_set_channel_number = read_dgus(0x1B0C);
			g_var_module.parameter_set_sta = BATCH_CHANNEL_STATE;
			break;
		case 5: // ʱ������
			g_var_module.parameter_set_fun_execute_nb = 4;
			g_var_module.parameter_set_channel_number = read_dgus(0x1B0F);
			g_var_module.parameter_set_sta = BATCH_CHANNEL_STATE;
			break;
		case 0x13: // ������������
			g_var_module.parameter_set_fun_execute_nb = 2;
			g_var_module.parameter_set_channel_number = 0;
			g_var_module.parameter_set_sta = BATCH_CHANNELS_STATE;
			break;
		case 0x14: // �������ն�����
			g_var_module.parameter_set_fun_execute_nb = 3;
			g_var_module.parameter_set_channel_number = 0;
			g_var_module.parameter_set_sta = BATCH_CHANNELS_STATE;
			break;
		case 0x15: // ����ʱ������
			g_var_module.parameter_set_fun_execute_nb = 4;
			g_var_module.parameter_set_channel_number = 0;
			g_var_module.parameter_set_sta = BATCH_CHANNELS_STATE;
			break;
		default:
			break;
		}
		pop_menu_key_ctrl(0x31); // ����
		g_var_module.batch_set_parameters_flag = key_nb;
		// USER_PRINTF("-->batch_set_parameters_flag is %bd\n", g_var_module.batch_set_parameters_flag);
		clear_key_value(0x112a, &key_nb);
	}
}
// �Զ������豸����
void automatically_retrieve_cmd(void)
{

	unsigned short key_nb = 0;
	get_key_value(0x112c, &key_nb);
	if ((1 == key_nb))
	{
		g_var_module.auto_retrieve_flag = 1;
		pop_menu_key_ctrl(0x0d); // ����������
		clear_key_value(0x112c, &key_nb);
	}
}
// ���豸��Ϣ
void read_device_information(unsigned char adr)
{
	pack_data_send(&user_modbus, adr, MD_RD_HR, 14, 1);
}
// �Զ������豸
void automatically_retrieve(void)
{
	static unsigned char send_flag = 0;
	if (g_var_module.auto_retrieve_flag == 1)
	{
		//		g_var_module.retrieve_device_adr
		//		if( 0 == send_flag)
		//		{
		//			send_flag = 1;
		//			g_var_module.retrieve_device_adr = 1;
		//		}
		if ((0 == g_var_module.auto_retrieve_cyc_time) && (mbh_getState() == MBH_STATE_IDLE))
		{
			g_var_module.auto_retrieve_cyc_time = 500;
			read_device_information(g_var_module.retrieve_device_adr);
			g_var_module.retrieve_device_adr++;
			// USER_PRINTF("-->retrieve_device_adr is %bd\n", g_var_module.retrieve_device_adr);
		}
		//		if(mbh_getState() == MBH_STATE_REC_ERR)
		//		{
		//			g_var_module.auto_retrieve_cyc_time = 500;
		//		}
	}

	if ((g_var_module.retrieve_device_adr > DEVICE_NB)) //&&(0 == g_var_module.batch_send_cyc_time)
	{
		g_var_module.auto_retrieve_flag = 0;
		//		g_var_module.retrieve_device_adr = 1;

		send_flag = 1;
	}
	if ((1 == send_flag) && (0 == g_var_module.auto_retrieve_cyc_time))
	{
		send_flag = 0;
		g_var_module.retrieve_device_adr = 1;
		pop_menu_key_ctrl(0xf0); // �������
		USER_PRINTF("-->auto add module over!\n");
	}
}
// �Զ�����豸��Ϣ
void auto_add_module(void)
{

	unsigned char device_channel = 0;
	if (HR[DEVICE_INFOR_INDEX])
	{
		auto_module.index = find_index(0, MODULE_FLASH_SIZE, MODULE_NUB);
		device_channel = HR[DEVICE_INFOR_INDEX] & 0x0f;
		device_channel = device_channel > 14 ? 16 : device_channel;
		auto_module.adr = g_var_module.slave_adr;
		auto_module.channel_nb = device_channel;
		auto_module.data_sta = FULL;
		auto_module.type = (HR[DEVICE_INFOR_INDEX] >> 4) & 0x0f;
		sprintf(auto_module.module_name, "����%bd", auto_module.adr);
		//		sprintf(auto_module.channel_name,"%s",default_module_channel_name);
		//		auto_module.module_name[strlen(auto_module.module_name)] = '\0';

		read_dgusii_vp(0xa008, (unsigned char *)&auto_module.channel_name, 8 * 16);
		// USER_PRINTF("-->auto_module.adr is %bd\n", auto_module.adr);
		// USER_PRINTF("-->auto_module.channel_nb is %bd\n", auto_module.channel_nb);

		//		snprintf(auto_module.channel_name, 256, auto_module.channel_name);
		//		USER_PRINTF("-->auto_module.adr is %s\n",auto_module.adr );
		//		USER_PRINTF("-->auto_module.adr is %bd\n",auto_module.adr );
		//		USER_PRINTF("-->auto_module.adr is %bd\n",auto_module.adr );

		norflash_write(auto_module.index * MODULE_FLASH_SIZE, (unsigned char *)&auto_module, MODULE_FLASH_SIZE); // �洢flash
		write_dgusii_vp(0x3120 + 8 * auto_module.index, (unsigned char *)&auto_module.module_name, 8);					 //
		display_module_information(&auto_module, auto_module.index);																						 // ��ʾģ����Ϣ
		USER_PRINTF("-->auto adding module successfully!\n");
		HR[DEVICE_INFOR_INDEX] = 0;
	}
}
#define BOARDCAST_ALL_ON_ADR 0X1B21
#define BOARDCAST_ALL_OFF_ADR 0X1B22
/**
 * @brief ģ����ƹ㲥ȫ��ȫ��
 * @return {*}
 */
void module_all_ctrl(void)
{
	unsigned short key_nb = 0;
	get_key_value(BOARDCAST_ALL_ON_ADR, &key_nb);
	if ((1 == key_nb))
	{
		set_master_send_hr(master_send_hr, 22, 0xffff);
		set_master_send_hr(master_send_hr, 23, (100));
		set_master_send_hr(master_send_hr, 24, (100));
		set_master_send_hr(master_send_hr, 25, (100));
		set_master_send_hr(master_send_hr, 26, (100));
		set_master_send_hr(master_send_hr, 27, ENTERPRISE);
		set_master_send_hr(master_send_hr, 28, 0);
		set_master_send_hr(master_send_hr, 29, 0);
		set_master_send_hr(master_send_hr, 30, 0);
		set_master_send_hr(master_send_hr, 31, 0xffff);
		set_master_send_hr(master_send_hr, 32, 0xffff);
		set_master_send_hr(master_send_hr, 33, ENTERPRISE);
		pack_data_send(&user_modbus, 0, MD_FR_MHR, 22, 13);
		clear_key_value(BOARDCAST_ALL_ON_ADR, &key_nb);
	}
	get_key_value(BOARDCAST_ALL_OFF_ADR, &key_nb);
	if ((1 == key_nb))
	{
		set_master_send_hr(master_send_hr, 22, 0xffff);
		set_master_send_hr(master_send_hr, 23, (0));
		set_master_send_hr(master_send_hr, 24, (0));
		set_master_send_hr(master_send_hr, 25, (0));
		set_master_send_hr(master_send_hr, 26, (0));
		set_master_send_hr(master_send_hr, 27, ENTERPRISE);
		set_master_send_hr(master_send_hr, 28, 0);
		set_master_send_hr(master_send_hr, 29, 0);
		set_master_send_hr(master_send_hr, 30, 0);
		set_master_send_hr(master_send_hr, 31, 0xffff);
		set_master_send_hr(master_send_hr, 32, 0);
		set_master_send_hr(master_send_hr, 33, ENTERPRISE);
		pack_data_send(&user_modbus, 0, MD_FR_MHR, 22, 13);
		clear_key_value(BOARDCAST_ALL_OFF_ADR, &key_nb);

	}	
}	
/**
 * @brief ����ҳ��
 * @return {*}
 */
void return_page(void)
{
	unsigned short key_nb = 0;
	get_key_value(0x1b23, &key_nb);
	if (key_nb)
	{
		pic_page = key_nb;
		clear_key_value(0x1b23, &key_nb);
	}
	get_key_value(0x1b24, &key_nb);
	if (1 == key_nb)
	{
		pic_set(pic_page);
		clear_key_value(0x1b24, &key_nb);
	}	


}
// ģ������
void module_modify(void)
{
	unsigned short page = 0;
	unsigned short size_struct = 0;
	page = read_pic();
	size_struct = sizeof(module_t);
	/*module edit page*/
	if (((page <= 16) && (page >= 5)) || (page == 63) || (page == 64))
	{
		//		USER_PRINTF("-->page is %d\n", page);
		set_module_type();
		save_module_information(&module);
	}
	// set_selected_sequence_number(0x110c, 0x1104, 8);
	set_box_select(0x110c, 0x1400, 64, &module_key_nb_bak, &module_select_sta_bak);
	modifying_module_information(&module);
	clear_select_module_information();
	clear_all_module_information();
	timing_module_parameter_key_ctrl(&module, channel_number);
	display_timing_module_parameter(&module, channel_number);
	select_either_or_all();
	starup_page_pwd(read_dgus(PWD_SELECT_FRAME_ADR));
	change_pwd(&init_pwd);
	select_beep_enable();
	back_light_set();

	black_screen_switches_to_home_page();
	batch_set_time_parameters();
	automatically_retrieve_cmd();
	module_all_ctrl();
	return_page();
}


void init_flash_parameters(void)
{
	init_pwd_f();
	init_beep_enable();
	init_back_light_parameters();
	USER_PRINTF("-->read_flash_successfully\n");
	g_var_module.batch_set_parameters_flag = 0;
	g_var_module.retrieve_device_adr = 1;
	g_var_module.beep_enable_bak = 0xff;
	g_var_module.backlight_enable_bak = 0xff;
	g_var_module.pwd_enable_bak = 0xff;
}
