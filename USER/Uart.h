#ifndef __UART_H__
#define __UART_H__

#include "Sys.h"
#include "crc16.h"
#include "T5LLIB.h"
#include <stdlib.h>
#include <string.h>
#include "stdio.h"
#define DEBUG_ON 1
#if DEBUG_ON

#define DEBUG_PRINTF printf
//                            printf("\n");
#else
#define DEBUG_PRINTF //
#endif
// struct module_data
//{
//	u8 address;
//	u8 load_num;
//	u8 module_name[10];
//	u8 switch_name[16][10];
//	u8 switch_status[16];
//	u8 new_index_for_conditon[12];
// };
#define DEBUG 0
#define HR_MAX_NUMBER 500
extern unsigned short HR[HR_MAX_NUMBER];
extern u8 xdata T_O2; // 串口2超时计数器
extern u8 xdata T_O3; // 串口3超时计数器
extern u8 xdata T_O4; // 串口4超时计数器
extern u8 xdata T_O5; // 串口5超时计数器

extern bit Response_flog;		 // 应答用
extern bit Auto_data_upload; // 数据自动上传用
extern bit Crc_check_flog;	 // CRC校验标记
extern bit Busy4;						 // 串口4发送标记
// extern struct module_data relay_modules_data[16];
// extern struct module_data srelay_modules_data[16];
// extern struct module_data dimmer_modules_data[16];
// extern struct module_data panel_modules_data[16];
// extern u8 modules_data_num[4];
// extern u8 array_for_timestamp_flash[8];

void uart_init();
void Sw_Data_Send();
void uart_frame_deal();
void uart_data_2_DGUS();
extern u16 get_enterprise();
extern u32 get_timestamp();
void uart_send_byte(u8 Uart_number, u8 Dat);

extern u8 important_address;
extern u8 important_status[4];

void flash_data_init();

extern void uart_data_send(u8 *Arr, u8 Uartnum, bit Outo_send, bit Crc_ck);
extern void UART5_SendString(char *s);
#endif