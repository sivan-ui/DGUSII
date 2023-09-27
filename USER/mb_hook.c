/*
 * @Author: xw.qu
 * @Date: 2023-08-31 09:22:42
 * @LastEditors: xw.qu
 * @LastEditTime: 2023-09-26 10:15:50
 * @FilePath: \USER\mb_hook.c
 * @Description: modbus callback function
 * 
 * Copyright (c) 2023 by xw.qu, All Rights Reserved. 
 */


#include "mb_include.h"

void mbh_hook_rec01(unsigned char add,unsigned char *dat,unsigned short *datalen)
{

}
void mbh_hook_rec02(unsigned char add,unsigned char *dat,unsigned short *datalen)
{

}
void mbh_hook_rec03(unsigned char add,unsigned char *dat,unsigned short *datalen)
{
//	USER_PRINTF("03 receive datalen = %d \n",*datalen);
	if(*datalen)
	{

		short_copy_xch(HR+user_modbus.da_adr,(dat+1),*datalen-3,0);//copy数据
//		USER_PRINTF("03 receive datalen = %d \n",*datalen);
//		USER_PRINTF("03 receive datalen = %d \n",*datalen);
		
    // USER_PRINTF("HR[32] = 0x%x \n",*(HR+32));
	}	
	// USER_PRINTF("HR[32] = 0x%x \n",*(HR+32));	
	g_var_module.slave_adr = add;
//	printf_tab(80,(unsigned char *)HR);
//		USER_PRINTF("HR[49] = 0x%x \n",*(HR+49));
//	USER_PRINTF("HR[161] = 0x%x \n",*(HR+161));

}
void mbh_hook_rec04(unsigned char add,unsigned char *dat,unsigned short *datalen)
{

}
void mbh_hook_rec05(unsigned char add,unsigned char *dat,unsigned short *datalen)
{

}
void mbh_hook_rec06(unsigned char add,unsigned char *dat,unsigned short *datalen)
{

}
void mbh_hook_rec15(unsigned char add,unsigned char *dat,unsigned short *datalen)
{

}
void mbh_hook_rec16(unsigned char add,unsigned char *dat,unsigned short *datalen)
{
	
}


void mbh_hook_timesErr(unsigned char add,unsigned char cmd)
{
	mbHost.state=MBH_STATE_IDLE;
	mbHost.re_send_time = 0;
}
void mbh_resend_time_run(void)
{
	if(mbHost.re_send_time>0)mbHost.re_send_time--;
}

