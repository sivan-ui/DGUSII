/*
 * @Author: xw.qu
 * @Date: 2023-08-31 09:22:42
 * @LastEditors: xw.qu
 * @LastEditTime: 2023-08-31 10:48:01
 * @FilePath: \USER\mb_hook.h
 * @Description: 
 * 
 * Copyright (c) 2023 by xw.qu, All Rights Reserved. 
 */


#ifndef __MB_HOOK_H
#define __MB_HOOK_H

/**
 * 	@brief  MODBUS主机模式下接收到从机回复不同功能码的回调处理
 * 	@param	add:从机的地址
 * 	@param 	dat:接收到的从机发来的数据指针
 *  @param 	datalen:接收到的从机发来的数据长度
 * 	@return	NONE
 * 	@note	rec01\02\03……等数字代表功能码
 */
void mbh_hook_rec01(unsigned char add,unsigned char *dat,unsigned short *datalen);
void mbh_hook_rec02(unsigned char add,unsigned char *dat,unsigned short *datalen);
void mbh_hook_rec03(unsigned char add,unsigned char *dat,unsigned short *datalen);
void mbh_hook_rec04(unsigned char add,unsigned char *dat,unsigned short *datalen);
void mbh_hook_rec05(unsigned char add,unsigned char *dat,unsigned short *datalen);
void mbh_hook_rec06(unsigned char add,unsigned char *dat,unsigned short *datalen);
void mbh_hook_rec15(unsigned char add,unsigned char *dat,unsigned short *datalen);
void mbh_hook_rec16(unsigned char add,unsigned char *dat,unsigned short *datalen);
/**
 * 	@brief  MODBUS主机读写从机超过最大错误次数回调
 *	@param	add:从机的地址
 *	@param  cmd:功能码
 * 	@return	NONE
 * 	@note	
 */
void mbh_hook_timesErr(unsigned char add,unsigned char cmd);

extern void mbh_resend_time_run(void);

#endif

