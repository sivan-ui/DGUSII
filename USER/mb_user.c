/*
 * @Author: xw.qu
 * @Date: 2023-08-31 09:22:42
 * @LastEditors: xw.qu
 * @LastEditTime: 2023-09-26 10:16:04
 * @FilePath: \USER\mb_user.c
 * @Description: modbus data send
 * 
 * Copyright (c) 2023 by xw.qu, All Rights Reserved. 
 */
#include "mb_user.h"
#include "mb_include.h"
FUNC_MODBUS_MASTER_FUNC_T mb_master_func;
unsigned short master_send_hr[HR_NB];
unsigned char send_table[HR_NB * 2];
MODBUS_T user_modbus;
// 从源缓冲区拷贝n个short到目的缓冲区，b_xch=1,交换高低字节
void short_copy_xch(void *t, void *s, int n, unsigned char b_xch)
{
	int i;
	unsigned char *pt, *ps0;
	unsigned char ch;

	pt = (unsigned char *)t;
	ps0 = (unsigned char *)s;

	if (b_xch)
	{
		for (i = 0; i < n; i++)
		{
			ch = *ps0++;
			*pt++ = *ps0++;
			*pt++ = ch;
		}
	}
	else
	{
		n *= 2;
		for (i = 0; i < n; i++)
		{
			*pt++ = *ps0++;
		}
	}
}
int modbus_master_send(MODBUS_T *p)
{
	unsigned short send_n = 0;
	unsigned short *phr, da_n = 0, da_adr = 0, adr_end1 = 0;
	unsigned char b_ext = 0;
	unsigned char b_hr_ok = 0;

	b_ext = p->b_ext;

	phr = p->phr;
	da_adr = p->da_adr;
	da_n = p->da_n;

	p->send_dat[0] = da_adr >> 8;
	p->send_dat[1] = da_adr & 0xff;
	p->send_dat[2] = da_n >> 8;
	p->send_dat[3] = da_n & 0xff;

	adr_end1 = (da_adr + da_n);
	b_hr_ok = (adr_end1 <= p->hr_n);

	send_n = 0;
	switch (p->func)
	{
		//==========================================================================

#ifdef MODBUS_HR_EN
	case MD_RD_HR: // 读取HR
		if (b_hr_ok)
		{
			send_n = 6;
		}
		break;
#endif
	//==========================================================================
	case MD_FR_SHR: // 强制单个HR
		if (da_adr < p->hr_n)
		{
			da_n = *(phr + da_adr);
			p->send_dat[4] = da_n >> 8;
			p->send_dat[5] = da_n & 0xff;
			p->da_n = da_n;
			send_n = 6;
		}
		break;
	case MD_FR_MHR: // 强制多个HR
		if (b_hr_ok)
		{
			p->send_dat[4] = da_n * 2;
			send_n = da_n * 2 + 7;
			if (!b_ext)
			{
				short_copy_xch(&p->send_dat[5], (phr + da_adr), da_n, 0);
			}
		}
		break;
	default:
		// 命令格式错
		break;
	}
	send_n -= 2;
	return (send_n);
}
void mb_data_init(MODBUS_T *p)
{
	p->phr = master_send_hr;
	p->hr_n = HR_NB;
	p->send_dat = send_table;
}
void modtify_mb_infom(MODBUS_T *p, unsigned char slv, unsigned char fun, unsigned short adr, unsigned char da_n)
{
	p->slv = slv;
	p->func = fun;
	p->da_adr = adr;
	p->da_n = da_n;
	//	USER_PRINTF("-->p->da_n is %d\n",p->da_n);
	p->send_dat_nb = modbus_master_send(p);
	// USER_PRINTF("-->p->send_dat_nb is %d\n", p->send_dat_nb);
	USER_PRINTF("-->modtify_mb_infom_ok\n");
}
void set_master_send_hr(unsigned short *p, unsigned short index, unsigned short dat)
{
	*(p + index) = dat;
}
void modbus_send_data(MODBUS_T *p)
{
	mbh_send(p->slv, p->func, (unsigned char *)p->send_dat, p->send_dat_nb);
	USER_PRINTF("-->mb_send_data_ok\n");
}
// 数据打包串口发送
void pack_data_send(MODBUS_T *p, unsigned char slv, unsigned char fun, unsigned short dat_adr, unsigned char dat_nb)
{
	modtify_mb_infom(p, slv, fun, dat_adr, dat_nb); // 设置信息
	modbus_send_data(p);														// 发送
}