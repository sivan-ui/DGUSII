/*
 * @Author: xw.qu
 * @Date: 2023-08-31 09:22:42
 * @LastEditors: xw.qu
 * @LastEditTime: 2023-09-26 17:01:27
 * @FilePath: \USER\mb_host.c
 * @Description:
 *
 * Copyright (c) 2023 by xw.qu, All Rights Reserved.
 */
#include "mb_include.h"
#include "string.h"
#include "T5lOS8051.h"
unsigned char mbhRecTimeout;
mbHost_T mbHost;
// unsigned char retransmission_time;

// modbus初始化
void mbh_init(unsigned long baud, unsigned char parity)
{
	mb_port_uartInit(baud, parity);
	mb_port_timerInit(baud);
}
unsigned char mbh_getState(void)
{
	return mbHost.state;
}

// 发送一帧命令
char mbh_send(unsigned char add, unsigned char cmd, unsigned char *dat, unsigned short data_len)
{
	unsigned short crc = 0;
	//	USER_PRINTF("%bd\n",mbHost.state);
	//	if(mbHost.state!=MBH_STATE_IDLE)return -1; //busy state

	mbHost.txCounter = 0;
	mbHost.rxCounter = 0;
	mbHost.errTimes = 0;
	mbHost.txBuf[0] = add;
	mbHost.txBuf[1] = cmd;
	memcpy((mbHost.txBuf + 2), dat, data_len);
	mbHost.txLen = data_len + 2; // data(n)+add(1)+cmd(1)
	USER_PRINTF("TX LEN %d\n",mbHost.txLen);
	crc = mb_crc16(mbHost.txBuf, mbHost.txLen);
	mbHost.txBuf[mbHost.txLen++] = (unsigned char)(crc & 0xff);
	mbHost.txBuf[mbHost.txLen++] = (unsigned char)(crc >> 8);

	mbHost.state = MBH_STATE_TX;
	mb_port_uartEnable(1, 0); // enable tx,disable rx

	// printf("%d ", mbHost.txLen);
	/*当打开TXE中断以后，立马就会触发一次，所以这里不用先发送一个byte*/
	//	mb_port_putchar(mbHost.txBuf[mbHost.txCounter++]); //send first char,then enter tx isr
	return 0;
}
// 接收正确,进行解析处理
void mbh_exec(unsigned char *pframe, unsigned *len)
{
	unsigned short datalen = *len - 2;
	switch (pframe[1]) // cmd
	{
	case 1:
		mbh_hook_rec01(pframe[0], (pframe + 2), &datalen);
		break;
	case 2:
		mbh_hook_rec02(pframe[0], (pframe + 2), &datalen);
		break;
	case 3:
		mbh_hook_rec03(pframe[0], (pframe + 2), &datalen);
		break;
	case 4:
		mbh_hook_rec04(pframe[0], (pframe + 2), &datalen);
		break;
	case 5:
		mbh_hook_rec05(pframe[0], (pframe + 2), &datalen);
		break;
	case 6:
		mbh_hook_rec06(pframe[0], (pframe + 2), &datalen);
		break;
	case 15:
		mbh_hook_rec15(pframe[0], (pframe + 2), &datalen);
		break;
	case 16:
		mbh_hook_rec16(pframe[0], (pframe + 2), &datalen);
		break;
	}
}
void mbh_poll()
{

	switch (mbHost.state)
	{
	/*接收完一帧数据,开始进行校验*/
	case MBH_STATE_RX_CHECK: // 接收完成，对一帧数据进行检查
		//			USER_PRINTF("STATE--MBH_STATE_RX_CHECK\n");
		if ((mbHost.rxCounter >= MBH_RTU_MIN_SIZE) && (mb_crc16(mbHost.rxBuf, mbHost.rxCounter) == 0)) // 接收的一帧数据正确
		{
			//				USER_PRINTF("%bd\n",mbHost.state);
			if (((mbHost.txBuf[0] == mbHost.rxBuf[0]) && (mbHost.txBuf[1] == mbHost.rxBuf[1]))) // 发送帧数据和接收到的帧数据地址和功能码一样
			{

				mbHost.state = MBH_STATE_EXEC;
			}
			else
			{
				mbHost.state = MBH_STATE_REC_ERR;
				mbHost.re_send_time = MBH_RESEND_TIME;
				//					USER_PRINTF("-->check_error\n");
			}
		}
		else
		{
			mbHost.state = MBH_STATE_REC_ERR;
			USER_PRINTF("-->check_error\n");
			;
		}
		break;
	/*接收一帧数据出错*/
	case MBH_STATE_REC_ERR:

		//			retransmission_time++;
		//			if(retransmission_time>50000)
		//			{
		//				retransmission_time = 0;

		//			}
		mbHost.scene_send_cyc_time = MBH_CYC_TIME;
		if (0 == mbHost.re_send_time)
		{
			mbHost.errTimes++;
			if (mbHost.errTimes >= MBH_ERR_MAX_TIMES)
			{
				mbHost.state = MBH_STATE_TIMES_ERR;
			}
			else // 重新再启动一次传输
			{
				mbHost.txCounter = 0;
				mbHost.rxCounter = 0;
				mbHost.state = MBH_STATE_TX;

				mb_port_uartEnable(1, 0); // enable tx,disable rx
			}
			USER_PRINTF("STATE--MBH_STATE_REC_ERR\n");
		}
		//			USER_PRINTF("mbHost.errTimes-->>%bd\n",mbHost.errTimes);
		break;
	/*超过最大错误传输次数*/
	case MBH_STATE_TIMES_ERR:
		USER_PRINTF("STATE--MBH_STATE_TIMES_ERR\n");
		mbh_hook_timesErr(mbHost.txBuf[0], mbHost.txBuf[1]);
		mbHost.txCounter = 0;
		mbHost.rxCounter = 0;
		//			mbHost.re_send_time = MBH_RESEND_TIME;
		//		  USER_PRINTF("%bd\n",mbHost.state);
		break;
	/*确定接收正确执行回调*/
	case MBH_STATE_EXEC: // 主机发送接收完成，执行回调
		USER_PRINTF("STATE--MBH_STATE_EXEC\n");
		mbh_exec(mbHost.rxBuf, &mbHost.rxCounter);
		mbHost.state = MBH_STATE_IDLE;
		mbHost.scene_send_cyc_time = 200;
		//		  USER_PRINTF("%bd\n",mbHost.state);
		break;
	}
}

void mbh_timer3T5Isr()
{
	switch (mbHost.state)
	{
	/*发送完但没有接收到数据*/
	case MBH_STATE_TX_END:
		//			USER_PRINTF("--->RxISR_MBH_STATE_TX_END\n");
		//			if(1 == mbHost.recDataFlag)
		//			{
		mbHost.rxTimeOut--;
		//			}
		if (mbHost.rxTimeOut == 0) // 接收超时
		{
			mbHost.rxTimeOut = MBH_REC_TIMEOUT;
			mbHost.state = MBH_STATE_REC_ERR;
			mbHost.re_send_time = MBH_RESEND_TIME;
			//				USER_PRINTF("-->rec time out\n");
			//				mb_port_timerDisable();		//关闭定时器
			//				mbHost.recDataFlag = 0;
			//				mb_port_uartEnable(0,0); 	//串口tx、rx都关闭
		}
		break;
	case MBH_STATE_RX: // 3.5T到,接收一帧完成
		if (mbHost.rxTimeOut != 0 && --mbHost.rxTimeOut == 0)
		{
			mbHost.state = MBH_STATE_RX_CHECK;
			mbHost.re_send_time = 0;
			//				mbHost.recDataFlag = 0;
			//				mbHost.rxTimeOut = 0;
			//				mb_port_timerDisable();		//关闭定时器
			//				mb_port_uartEnable(0,0); 	//串口tx、rx都关闭
		}
		break;
	}
}
void mbh_uartRxIsr()
{
	unsigned char ch;
	mb_port_getchar(&ch);
	switch (mbHost.state)
	{
	case MBH_STATE_TX_END:
		mbHost.rxCounter = 0;
		mbHost.rxBuf[mbHost.rxCounter++] = ch;
		mbHost.state = MBH_STATE_RX;
		mb_port_timerEnable();
		break;
	case MBH_STATE_RX:
		if (mbHost.rxCounter < MBH_RTU_MAX_SIZE)
		{
			mbHost.rxBuf[mbHost.rxCounter++] = ch;
		}
		mb_port_timerEnable();
		mbHost.rxTimeOut = mbhRecTimeout;
		break;
	default:
		if (mbHost.rxCounter < MBH_RTU_MAX_SIZE)
		{
			mbHost.rxBuf[mbHost.rxCounter++] = ch;
		}
		mb_port_timerEnable();
		break;
	}
}
void mbh_uartTxIsr()
{
	switch (mbHost.state)
	{
	case MBH_STATE_TX:
		if (mbHost.txCounter == mbHost.txLen) // 全部发送完
		{
			mbHost.state = MBH_STATE_TX_END;
			mb_port_uartEnable(0, 1); // disable tx,enable rx
			mbHost.rxTimeOut = 0;			// 清除接收超时计数
			mb_port_timerEnable();		// open timer
			mb_set_receive_state();
		}
		else
		{
			if (0 == time_scene_var.send_delay_time)
			{
				if (mbHost.rxCounter)
				{
					mbHost.rxCounter = 0;
					time_scene_var.send_delay_time = 50;
				}
				mb_port_putchar(mbHost.txBuf[mbHost.txCounter++]);
			}
		}

		break;
	case MBH_STATE_TX_END:
		//			USER_PRINTF("--->TxISR_MBH_STATE_TX_END\n");
		//		  mbHost.re_send_time = MBH_RESEND_TIME;
		mb_port_uartEnable(0, 1); // disable tx,enable rx
		break;
	}
}
void clear_mbh_rec_one_frame_sta(void)
{
	mbHost.receive_frame_state = 0;
}
unsigned char get_mbh_rec_one_frame_sta(void)
{
	if ((mbHost.rxCounter >= MBH_RTU_MIN_SIZE) && (mb_crc16(mbHost.rxBuf, mbHost.rxCounter) == 0))
	{
		mbHost.receive_frame_state = 1;

		mbHost.rxCounter = 0;
	}
	return mbHost.receive_frame_state;
}
