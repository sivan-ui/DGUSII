/*
 * @Author: xw.qu
 * @Date: 2023-08-31 09:22:42
 * @LastEditors: xw.qu
 * @LastEditTime: 2023-09-26 10:15:37
 * @FilePath: \USER\main.c
 * @Description: main.c
 * 
 * Copyright (c) 2023 by xw.qu, All Rights Reserved. 
 */


#include "sys.h"
#include "scene.h"
#include "time_scene.h"
#include "stdio.h"

void main(void)

{
	unsigned short i = 0;
	static unsigned char time = 0;
	static unsigned char time_200ms = 0;
	module_t module_read = {0};
	name_scene_t module_scene_read = {0};
	timing_content_t timing_content_read = {0};
	Sys_Cpu_Init();
	uart_init();
	mbh_init(19200, 0);
	mb_data_init(&user_modbus);
	//	 scene_init();
	//	sys_init();//系统初始化
	rdtime(); // 更新硬件RTC时间
	//	beep_ctrl(0);//关闭蜂鸣器声音
	//  init_beep_enable();
	init_flash_parameters();


	for (i = 0; i < MODULE_NUB; i++)
	{
		norflash_read(i * 140, (unsigned char *)&module_read, 140);
		norflash_read(MODULE_FLASH_ADR_END + i * NAME_SCENE_T_SIZE, (unsigned char *)&module_scene_read, NAME_SCENE_T_SIZE);

		if (FULL == module_read.data_sta)
		{
			write_dgusii_vp(0x3120 + 8 * i, (unsigned char *)module_read.module_name, 8);
			// USER_PRINTF(" module_read index%bd  flash data is full\n", module_read.index);
			display_module_information(&module_read, i);
			add_arr_data(&g_var_module.module_adr[0],module_read.adr,module_read.index);
		}
		if (FULL == module_scene_read.data_sta)
		{
			write_dgusii_vp(0x4100 + i * 32, (unsigned char *)module_scene_read.scene_name, 8);
			write_dgusii_vp(0x3320 + i * 8, (unsigned char *)module_scene_read.scene_name, 8);
//			USER_PRINTF(" module_scene index%bd  flash data is full\n", module_scene_read.scene_name_index);
		}
	}
  printf_tab(20,&g_var_module.module_adr[0]);
	//	norflash_read(TIMING_SCENE_INFOR_FLASH_ADR_SATRT,(unsigned char *)&timing_content,ALL_TIMING_SCENC_INFOR_T_SIZE);
	norflash_read(TIMING_SCENE_INFOR_FLASH_ADR_SATRT, (unsigned char *)&timing_content, ALL_TIMING_SCENC_INFOR_T_SIZE);
	//	printf_temp_timing_content(&timing_content_read);
	for (i = 0; i < TIMING_SCENE_NUB_SIGNLE; i++)
	{
		//		norflash_read(TIMING_SCENE_INFOR_FLASH_ADR_SATRT+i*SINGLE_TIMING_SCENC_INFOR_T_SIZE,(unsigned char *)&timing_content_read,SINGLE_TIMING_SCENC_INFOR_T_SIZE);
		if (1 == timing_content[i].time_scene_set.data_sta)
		{
			display_timing_scene_infor(timing_content + i);
			//			USER_PRINTF(" timing_content index%d  flash data is full\n",i);
		}
		else
		{
			//			USER_PRINTF(" timing_content index%d  flash data is blank\n",i);
		}
	}
	//	 printf_timing_content(10);
	write_dgus(0x1001, 0xff);
	write_dgusii_vp(0x3110, (unsigned char *)&module_name_tab[0], 6);
	//	init_pwd_f();
	// time_scene_init();
	USER_PRINTF("SYS HAS BEEN INITING\n");	
	while (1)
	{
		Clock();					 // RTC时间更新
		uart_frame_deal(); // 串口数据处理
		read_dgus_time();	 // 设置时间时读取一下时间
		timing_auto_detect();
		if (time_tick > 100)
		{
			time_tick = 0;
			time++;
			module_modify();

		}
		if (time > 2)
		{
			time = 0;
			time_200ms++;
			
		
			relay_ctrl(&module);
			timing_module_parameter_settings(&module);
			
			all_module_on_off();
			all_dim_on_off();
			scene_modify();
			scene_touch_run();
			select_password_enable();
			lock_screen_enable_ctrl();

		}
		if(time_200ms>2)
		{
			time_200ms = 0;
			right_number_of_days_ctrl();			

		}
		dim_ctrl(&module);
		module_scene_send();
		scene_delete_ctrl_send(&module_scene);
		scene_name_ctrl_send(&module_scene);
		module_touch_ctrl();
		mbh_uartTxIsr();
		mbh_poll();
		factory_data_reset();
		set_scene_infor_select_sequence_number();
		set_scene_name_select_sequence_number();
		set_timing_scene_name_select_sequence_number();
		timing_scene_run();
		timed_auto_send();
		timing_module_parameter_batch_send();
		automatically_retrieve();
		auto_add_module();
		//		timing_module_parameter_week_enable_batch_send();
		//    operand();
	}
}
