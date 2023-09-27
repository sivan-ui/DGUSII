/*
 * @Author: xw.qu
 * @Date: 2023-08-31 09:22:42
 * @LastEditors: xw.qu
 * @LastEditTime: 2023-08-31 10:48:52
 * @FilePath: \USER\mb_port.h
 * @Description: 
 * 
 * Copyright (c) 2023 by xw.qu, All Rights Reserved. 
 */

#ifndef __MB_PORT_H
#define __MB_PORT_H



typedef enum
{
	MB_PARITY_NONE=0X00,	//无奇偶校验，两个停止位
	MB_PARITY_ODD, 			//奇校验
	MB_PARITY_EVEN			//偶校验
}mbParity;



/**
 * 	@brief  MODBUS串口初始化接口
 * 	@param	baud:串口波特率
 * 	@param 	parity:奇偶校验位设置	
 * 	@return	NONE
 * 	@note	需要根据使用MCU进行移植
 */
void mb_port_uartInit(unsigned long baud,unsigned char parity);
/**
 * 	@brief  串口TX\RX使能接口
 * 	@param	txen:0-关闭tx中断	1-打开tx中断
 * 	@param 	rxen:0-关闭rx中断	1-打开rx中断	
 * 	@return	NONE
 * 	@note	需要根据使用MCU进行移植
 */
void mb_port_uartEnable(unsigned char txen,unsigned char rxen);
/**
 * 	@brief  串口发送一个byte
 * 	@param	ch:要发送的byte	
 * 	@return	NONE
 * 	@note	需要根据使用MCU进行移植
 */
void mb_port_putchar(unsigned char ch);
/**
 * 	@brief  串口读取一个byte
 * 	@param	ch:存放读取一个byte的指针	
 * 	@return	NONE
 * 	@note	需要根据使用MCU进行移植
 */
void mb_port_getchar(unsigned char *ch);
/**
 * 	@brief  定时器初始化接口
 * 	@param	baud:串口波特率,根据波特率生成3.5T的定时
 * 	@return	NONE
 * 	@note	需要根据使用MCU进行移植
 */
void mb_port_timerInit(unsigned long baud);
/**
 * 	@brief  定时器使能
 * 	@return	NONE
 * 	@note	定时器要清0重新计数
 */
void mb_port_timerEnable(void);
/**
 * 	@brief  定时器关闭
 * 	@return	NONE
 * 	@note	定时器要清0重新计数
 */
void mb_port_timerDisable(void);
void mb_set_receive_state(void);
void mb_set_send_state(void);
#endif

