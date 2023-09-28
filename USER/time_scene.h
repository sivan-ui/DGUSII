#ifndef __TIMING_SCENE_H__
#define __TIMING_SCENE_H__
#include "module.h"
#include "scene.h"
#include "nor_flash.h"
#define TIMING_SCENE_NUB_SIGNLE  128
#define TIMING_SCENE_NUB_SIGNLE_LIMIT  50
#define TIMING_SCENE_DIM_PAGE 84
#define TIMING_SCENE_SCE_PAGE 85

#define TIMING_SCENE_MANGE_FIRST_PAGE 4
#define TIMING_SCENE_MANGE_SECOND_PAGE 108

#define TIMING_SCENE_ADD_PAGE 70
#define TIMING_SCENE_START_YEAR 0X2001
#define TIMING_SCENE_START_MONTH 0X2002
#define TIMING_SCENE_START_DAY 0X2003

#define TIMING_SCENE_END_YEAR 0X2004
#define TIMING_SCENE_END_MONTH 0X2005
#define TIMING_SCENE_END_DAY 0X2006
typedef struct
{
	unsigned char module_select_sta ;
	unsigned char all_light_select_sta ;
	unsigned char scene_select_sta ;
	unsigned char page_bak;
	unsigned char all_light_on_sta;
	unsigned char all_light_off_sta;
	unsigned char scene_send_sta[16];
	unsigned short cyc_send_time;
  unsigned short send_delay_time;
	
	unsigned long adr;
}time_scene_var_t;
extern time_scene_var_t time_scene_var;
typedef struct 
{
	unsigned short year;
	unsigned char month;
	unsigned char day;
	
}date_t;
typedef struct
{
	unsigned char hour;
	unsigned char min;
	unsigned char sec;
	
}time_t;
enum
{
	EMPTY_TIME,
	MODULE_TIME,
	
	ALL_ON_OFF_TIME,
	SCENE_TIME,
};
enum
{
	ALL_ON_OFF_NULL,
	ALL_ON,	
	ALL_OFF,

};
typedef struct
{
	unsigned char  type;
	unsigned char  adr;
	unsigned char  channel_nb;
	unsigned char  module_index;
	unsigned short led_enable;
  unsigned short led_sta;	
	unsigned short led_hr_index_start;
	unsigned char  diming_enable;
  unsigned short dimming_duty[4];		
}time_module_scene_set_t;

typedef struct
{
	unsigned char  data_sta;
	unsigned char  reserved;
	unsigned char  time_scene_exectue_type;
	unsigned char  index;	
	unsigned char  week_enable;
	unsigned char  date_enable;	
	unsigned char  scene_name_index;
	unsigned char  all_on_off_sta;
	time_module_scene_set_t  time_module_scene_set;
	
}time_scene_set_t;
typedef struct
{	
	date_t start_date;
	date_t end_date;
	time_t exectue_time;
  time_scene_set_t time_scene_set;
}timing_content_t;
extern timing_content_t timing_content[TIMING_SCENE_NUB_SIGNLE];
#define SINGLE_TIMING_SCENC_INFOR_T_SIZE            sizeof(timing_content_t)/2  //19
#define ALL_TIMING_SCENC_INFOR_T_SIZE            (TIMING_SCENE_NUB_SIGNLE*SINGLE_TIMING_SCENC_INFOR_T_SIZE)//19*128
#define TIMING_SCENE_INFOR_FLASH_ADR_SATRT   (0xe740)
#define TIMING_SCENE_INFOR_FLASH_ADR_END     (TIMING_SCENE_INFOR_FLASH_ADR_SATRT  + ALL_TIMING_SCENC_INFOR_T_SIZE)
void time_scene_init(void);
void set_timing_scene_name_select_sequence_number(void);
void operand(module_t* p);
void timing_scene_run(void);
void display_timing_scene_infor(timing_content_t *p_timing_content);
void printf_temp_timing_content(timing_content_t *p_timing_content);
void timed_auto_send(void);
void timing_auto_detect(void);
void printf_timing_content(unsigned char index);
unsigned char get_timing_week_enable(unsigned short week_enable_adr);
void set_the_right_number_of_days(unsigned short year_adr, unsigned short month_adr,unsigned short day_adr);
void right_number_of_days_ctrl(void);
//void printf_temp_timing_content(unsigned char index);
#endif