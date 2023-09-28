#ifndef __SYS_H__
#define __SYS_H__
#include "t5los8051.h"
#include "Uart.h"
#include "i2c.h"
#include "module.h"
#include "scene.h"
#include "mb_include.h"
#include "time_scene.h"

extern u16 xdata time_tick;

extern u16 xdata re_send_time;
extern unsigned char pic_page ;
/// 对位节操作
#define GET_BIT(x, g_bit) ((x & (1 << g_bit)) >> g_bit) /* 获取第bit位 */
#define CLEAR_BIT(x, c_bit) (x &= ~(1 << c_bit))        /* 清零第bit位 */
#define SET_BIT(x, s_bit) (x |= (1 << s_bit))           /* 置位第bit位 */

#define OVERFLOW_SIZE 0xFF
#define MAX_INDEX   48
#define CONFIRM_POP_KEY_ADR 0X15
// 函数申明
void Sys_Cpu_Init();
void sys_init(void);
void sys_delay_about_ms(u16 ms);
void sys_delay_about_us(u8 us);
void sys_delay_ms(u16 ms);
void sys_read_vp(u16 addr, u8 *buf, u16 len);
void sys_write_vp(u16 addr, u8 *buf, u16 len);
void user_delay(unsigned int time);
extern void printf_tab(unsigned short nb, unsigned char *p);
extern char *strrpl(char *s, const char *old_str, const char *new_str);
void removeWhitespace(char *str) ;
unsigned char add_arr_data(unsigned char *t_arr, unsigned char element, unsigned char pos);
unsigned char delete_arr_data(unsigned char *t_arr, unsigned char pos);
void clear_arr_data(unsigned char *t_arr,unsigned char size);
unsigned char find_nub_index(unsigned char *t_arr,unsigned char num_to_find);
unsigned char get_days_in_month(unsigned short year, unsigned char month);
unsigned char check_constant(unsigned short var,unsigned char *pre_var);
#endif
