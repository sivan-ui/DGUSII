#ifndef __MODULE_H__
#define __MODULE_H__
#define MAX_DEVICE_ADR 252
#define MODULE_NUB  64
#define MODULE_FLASH_SIZE 140
#define MODULE_FLASH_ADR_END  (MODULE_NUB*MODULE_FLASH_SIZE)
#define SUPER_PASSWORD   3258
#define PWD_SELECT_FRAME_ADR 0x1802
#define PWD_FLASH_ADR    0x020004
#define DIM_CTRL_PAGE  17
#define SCR_CTRL_PAGE  18
#define SINGLE_TIME_PARA_SET_PAGE   56
#define RELAY_MODULE_KEY_ALL_ON_ADR 0X1B02
#define RELAY_MODULE_KEY_ALL_OFF_ADR 0X1B03
#define DIM_MODULE_KEY_ALL_ON_ADR 0X1B00
#define DIM_MODULE_KEY_ALL_OFF_ADR 0X1B01
#define BEEP_ENABLE_DGUS_ADR 0x1800
#define BATCH_CHANNELS_TIME_FREAME_ADR 0X1B11
typedef struct 
{
	unsigned char batch_set_parameters_flag ;
	unsigned char batch_send_count ;
	unsigned char batch_send_cyc_time ;
	unsigned char batch_relay_send_cyc_time ;
	unsigned char parameter_set_channel_number;
	unsigned char parameter_set_sta;
	unsigned char parameter_set_fun_execute_nb;
	unsigned char auto_retrieve_flag;	
	unsigned char retrieve_device_adr;
	unsigned char slave_adr;
	unsigned char dim_light_bak[4];
	unsigned char module_adr[MAX_DEVICE_ADR];	
	unsigned char module_edit_sta;

	unsigned char pwd_enable_bak;
	unsigned char beep_enable_bak;
	unsigned char backlight_enable_bak;
	unsigned short auto_retrieve_cyc_time;
	unsigned short led_sta_bak;
	
	
}v_module_t;
extern v_module_t g_var_module;
//typedef struct
//{
//	void (*module_add)(unsigned char index);
//	void (*module_del)(unsigned char index);
//	void (*module_clear)(unsigned char index);	
//	void (*module_modify)(unsigned char index);
//}module_fun_t;
//typedef struct
//{
//  unsigned char index;
//	unsigned short led_enable;
//  unsigned short led_sta;	
////	module_fun_t relay_fun;
//}relay_module_t;
//extern relay_module_t relay_module;
//typedef struct
//{
//	unsigned char index;
//	unsigned char diming_enable;
//  unsigned short dimming_duty[4];
////	module_fun_t vol_dim_fun;
//}vol_dim_module_t;
//typedef struct
//{
//	unsigned char index;
//	unsigned char diming_enable;
//  unsigned char dimming_duty[4];
////	module_fun_t scr_dim_fun;
//}scr_dim_module_t;

typedef struct   this_module 
{	
	unsigned char data_sta;
	unsigned char index;
	unsigned char reserve;	
	unsigned char type;
	unsigned char adr;
	unsigned char channel_nb;
	unsigned char module_name[17];
	unsigned char channel_name[257];
}module_t;
enum 
{
  BLANK,
	FULL,
};
enum 
{
  SINGLE_CHANNEL_STATE,
	SINGLE_CHANNELS_STATE,
	BATCH_CHANNEL_STATE,
	BATCH_CHANNELS_STATE,
};
extern module_t module;
extern module_t st_module ;
extern module_t timing_module ;
enum 
{
	SINGLE_RELAY_MODULE,
	SINGLE_TIME_RELAY_MODULE,
	VOL_DIM_MODULE,
	SCR_DIM_MODULE,
	DOUBLE_RELAY_MODULE,
	DOUBLE_TIME_RELAY_MODULE
};
extern void  read_sys_time(unsigned char *p);
extern unsigned char xdata module_name_tab[6][13];
extern char get_module_value(void);
//extern module_t tab_module[MODULE_NUB]; 
extern void set_module_type(void );
extern	void module_modify(void);
extern void read_dgus_time(void);
void module_touch_ctrl(void);
void factory_data_reset(void);
void display_interface(module_t* p);
extern void relay_ctrl(module_t* p);
extern void all_module_on_off(void);
extern void all_dim_on_off(void);
extern void modifying_module_information(module_t *p);
extern void display_module_information(module_t *p,unsigned char index);
extern void dim_ctrl(module_t* p);
extern void dim_single_ctrl(module_t* p);
extern unsigned short find_index(unsigned int adr_start,unsigned char flash_size,unsigned short quantity);
extern void get_key_value(unsigned short adr,unsigned short* key_value);
extern void clear_key_value(unsigned short adr,unsigned short* key_value);
extern unsigned short read_pic(void);
extern void pic_set(unsigned int pic);//Page switching
extern void set_selected_sequence_number(unsigned short touch_key_adr,unsigned short select_key_adr,unsigned char page_nb);
void set_box_select(unsigned short touch_key_adr, unsigned short select_key_adr, unsigned char nb,unsigned short *p_key_nb_bak, unsigned char *p_select_sta);
extern unsigned char get_selected_sequence_number(unsigned short select_key_adr,unsigned char page);
extern unsigned char get_box_select_number(unsigned short select_key_adr, unsigned char nb);
void timing_module_parameter_settings(module_t* p);
void display_week_parameter(unsigned short week_enable_adr,unsigned char week_enable);
void init_pwd_f(void);
void select_password_enable(void);
void init_beep_enable(void);
void init_back_light_parameters(void);
void init_flash_parameters(void);
//void timing_module_parameter_week_enable_batch_send(void);
void timing_module_parameter_batch_send(void);
void automatically_retrieve(void);
void auto_add_module(void);
void lock_screen_enable_ctrl(void);
#endif