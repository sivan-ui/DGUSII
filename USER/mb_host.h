/*
 * @Author: xw.qu
 * @Date: 2023-08-31 09:22:42
 * @LastEditors: xw.qu
 * @LastEditTime: 2023-11-02 16:29:32
 * @FilePath: \USER\mb_host.h
 * @Description: 
 * 
 * Copyright (c) 2023 by xw.qu, All Rights Reserved. 
 */



#ifndef __MB_HOST_H
#define __MB_HOST_H

#define MBH_RTU_MIN_SIZE	4
#define MBH_RTU_MAX_SIZE	1024	//
#define MBH_ERR_MAX_TIMES	5
#define MBH_REC_TIMEOUT		100  //单位3.5T
#define MBH_RESEND_TIME		100  //单位MS
#define MBH_CYC_TIME 1000
//extern unsigned short retransmission_time;
extern  unsigned char mbhRecTimeout;
typedef struct
{
	unsigned char receive_frame_state;//收到一帧的数据状态
	unsigned char state;						//modbus host状态
	unsigned char rec_hook_state;						//modbus 接收指令
	unsigned char errTimes;  					//失败次数计数
	unsigned short txLen;     					//需要发送的帧长度
	unsigned short txCounter;					//已发送bytes计数
	unsigned char txBuf[MBH_RTU_MAX_SIZE];	//发送缓冲区
	unsigned short rxCounter;					//接收计数
	unsigned char rxBuf[MBH_RTU_MAX_SIZE];	//接收缓冲区
	unsigned char rxTimeOut;					//接收时的超时计数
	unsigned char rxTimeOutInit;					//接收时的超时计数
	unsigned char recDataFlag;					//接收时的超时计数
	unsigned short re_send_time;         //重发周期
	unsigned short scene_send_cyc_time;  //循环周期
}mbHost_T;
extern mbHost_T mbHost;
typedef enum
{
	MBH_STATE_IDLE=0X00,
	MBH_STATE_TX,
	MBH_STATE_TX_END,
	MBH_STATE_RX,
	MBH_STATE_RX_CHECK,
	MBH_STATE_EXEC,
	MBH_STATE_REC_ERR,		//接收错误状态
	MBH_STATE_TIMES_ERR,	//传输
	
}mb_host_state;

/**
 * 	@brief  MODBUS主机初始化
 * 	@param	baud:串口波特率
 * 	@param 	parity:奇偶校验位设置	
 * 	@return	NONE
 * 	@note	使用modbus之前先调用该函数进行初始化
 */
void mbh_init(unsigned long baud,unsigned char parity);
/**
 * 	@brief  MODBUS主机给从机发送一条命令
 * 	@param	add:从机地址
 * 	@param 	cmd:功能码
 *	@param	data:要发送的数据
 *	@param	len:发送的数据长度
 * 	@return	-1:发送失败	0:发送成功
 * 	@note	该函数为非阻塞式，调用后立即返回
 */
char mbh_send(unsigned char add,unsigned char cmd,unsigned char *dat,unsigned short data_len);
/**
 * 	@brief  获取MODBUS主机运行状态
 * 	@return	mb_host_state中状态
 * 	@note	
 */
unsigned char mbh_getState(void);
/**
 * 	@brief  MODBUS状态轮训
 * 	@return	none
 * 	@note	该函数必须不断轮询，用来底层核心状态进行切换
 *			可在操作系统任务中运行，但应该尽可能短的延时间隔
 */
void mbh_poll(void);

/**
* 	@brief  modbus主机定时器中断处理
 * 	@return	none
 * 	@note	放在mcu的中断服务中调用
 *			
 */
extern void mbh_timer3T5Isr(void);
/**
 * 	@brief  modbus主机串口接收中断处理
 * 	@return	none
 * 	@note	放在mcu的rx中断中调用
 *			
 */
void mbh_uartRxIsr(void);
/**
 * 	@brief  modbus主机串口接收中断处理
 * 	@return	none
 * 	@note	放在mcu的tx中断中调用
 *			
 */
void mbh_uartTxIsr(void);
unsigned char mbh_getState(void);
unsigned char mbh_getRecHookState(void);
void mbh_clearRecHookState(void);
#endif

