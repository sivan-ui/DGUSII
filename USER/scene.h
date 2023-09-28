/*
 * @Author: xw.qu
 * @Date: 2023-08-31 09:22:42
 * @LastEditors: xw.qu
 * @LastEditTime: 2023-10-28 15:28:18
 * @FilePath: \USER\scene.h
 * @Description: 
 * 
 * Copyright (c) 2023 by xw.qu, All Rights Reserved. 
 */

#ifndef __SCENE_H__
#define __SCENE_H__
#include "module.h"
#define SINGLE_SCENE_NAME_INFOR_SIZE 64

#define SCENE_NAME_NUB_MAX 64
#define SCENE_INFOR_NUB_MAX 64

#define ALL_SCENE_SCENE_NAME_INFOR_SIZE  SCENE_NAME_NUB_MAX*SCENE_INFOR_NUB_MAX

#define SCENE_NAME_NUB_LIMIT 36

#define SCENE_ADD_PAGE 23
#define SCENE_DIM_PAGE 82
#define SCENE_SCR_PAGE 83
#define SINGLE_SCENE_NAME_MANGE_FIRST_PAGE 22
#define SINGLE_SCENE_NAME_MANGE_SECOND_PAGE 97
#define SINGLE_SCENE_INFOR_LIST_FIRST_PAGE 23
#define SINGLE_SCENE_INFOR_LIST_SECOND_PAGE 122
#define SCENE_NAME_LIST_SELECT_ADR 0x1600
#define SCENE_INFOR_LIST_SELECT_ADR 0x1900
//typedef struct
//{
//	void (*module_add)(unsigned char index);
//	void (*module_del)(unsigned char index);
//	void (*module_clear)(unsigned char index);	
//	void (*module_modify)(unsigned char index);
//}module_fun_t;
enum
{
	EMPTY,
	CONFIRM,
	CANCEL,
	ADD,
	CLEAR,
	MODIFY,
	DELETE
};
typedef struct
{
	unsigned char scenc_send_infor_cyc_flag;
	unsigned char scenc_send_name_cyc_flag;	
	unsigned char delete_scenc_send_cyc_flag;
	unsigned char delete_scenc_send_cyc_time;
	unsigned char delete_scenc_send_cnt;
}scene_data_t;
extern scene_data_t scene_data;
typedef struct
{
  unsigned char index;
	unsigned short led_enable;
  unsigned short led_sta;	
//	module_fun_t relay_fun;
}relay_module_t;
//extern relay_module_t relay_module;
typedef struct
{
	unsigned char index;
	unsigned char diming_enable;
  unsigned short dimming_duty[4];
//	module_fun_t vol_dim_fun;
}vol_dim_module_t;


typedef struct
{
	unsigned char index;
	unsigned char diming_enable;
  unsigned char dimming_duty[4];
//	module_fun_t scr_dim_fun;
}scr_dim_module_t;
//typedef struct
//{	
//	unsigned char data_sta;
//	unsigned char type;
//	unsigned char adr;
//	unsigned char channel_nb;
//}module_infor_t;
typedef struct
{
	unsigned char  data_sta;
	unsigned char  module_index;
	unsigned short index;	
	unsigned char  type;
	unsigned char  adr;
	unsigned char  channel_nb;
	unsigned char  edit_sta;
	unsigned char  scene_infor_index; 	
	unsigned short led_enable;
  unsigned short led_sta;	
	unsigned short led_hr_index_start;
	unsigned char  diming_enable;
  unsigned short dimming_duty[4];	
//	module_fun_t relay_fun;
}scenc_infor_t;// sizeof 20

extern scenc_infor_t scenc_infor;
typedef struct
{	
	unsigned char  data_sta;
	unsigned char  scene_name_index;
	unsigned char  edit_sta;
	unsigned char  reserve2;
	unsigned char  scene_name[22];
	unsigned char  module_select[8];

}name_scene_t; // sizeof 34
extern name_scene_t module_scene;
#define SCENC_INFOR_T_SIZE            sizeof(scenc_infor_t)/2 //12
#define	NAME_SCENE_T_SIZE             sizeof(name_scene_t)/2  //17
#define SINGLE_NAME_SCENC_INFOR_SIZE  (SINGLE_SCENE_NAME_INFOR_SIZE*SCENC_INFOR_T_SIZE)

#define SCENE_NAME_FLASH_ADR_SATRT    0x2300
#define SCENE_NAME_FLASH_ADR_END      (SCENE_NAME_FLASH_ADR_SATRT + SCENE_NAME_NUB*NAME_SCENE_T_SIZE)


#define SCENE_INFOR_FLASH_ADR_SATRT   0x2740
#define SCENE_INFOR_FLASH_ADR_END     (SCENE_INFOR_FLASH_ADR_SATRT  + ALL_SCENE_SCENE_NAME_INFOR_SIZE*SCENC_INFOR_T_SIZE)

//#define MODULE_SELECT_FLASH_ADR_SATRT   SCENE_INFOR_FLASH_ADR_END
//#define MODULE_SELECT_FLASH_ADR_END     MODULE_SELECT_FLASH_ADR_SATRT  + SCENE_NUB
//extern unsigned short scene_send_cyc_time;
extern unsigned char scene_vaild[8];
extern void scene_init(scenc_infor_t *p_scenc_infor,name_scene_t *p);
extern void save_scene_infor(name_scene_t *p);
extern void add_module_scene_infor(module_t* p,scenc_infor_t *p_scenc_infor,name_scene_t *p_module_scene);
void module_scene_select_ctrl(scenc_infor_t *p_scenc_infor);
extern void beep_ctrl(unsigned char enable);
void scene_touch_run(void);
void scene_modify(void);
void module_scene_send(void);
void scene_key_run(void);
void scene_name_ctrl_send(name_scene_t *p);
void set_scene_infor_select_sequence_number(void);
void set_scene_name_select_sequence_number(void);
void pop_menu_key_ctrl(unsigned char menu_key_value);
void back_light_ctrl(unsigned char enable);
void sys_led_config(unsigned char work_light,unsigned char sleep_light,unsigned short sleep_interval);
//������е�ѡ��
void clear_all_module_var_en(unsigned short adr,unsigned char len);
void scene_delete_ctrl_send(name_scene_t *p);
#endif