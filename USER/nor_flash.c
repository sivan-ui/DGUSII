/*
 * @Author: xw.qu
 * @Date: 2023-08-31 09:22:42
 * @LastEditors: xw.qu
 * @LastEditTime: 2023-08-31 10:49:03
 * @FilePath: \USER\nor_flash.c
 * @Description: flash write read
 * 
 * Copyright (c) 2023 by xw.qu, All Rights Reserved. 
 */

#include "nor_flash.h"



xdata u8 norflash_cmd[8];


//��T5L�ڲ��Դ���nor flashд������
//addr:nor flash�ĵ�ַ,����Ϊż��,��ΧΪ:0x000000-0x027FFE,Ȼ��һ����ַ��Ӧ2���ֽ�,��������Ϊ320KB
//buff:��Ҫд��Ļ�����,�������ٴ���len*2
//len:д�������,����Ϊż��,����д�����󳤶�Ҳ������,�����CACHE_ADDR��ֵ�й�ϵ
void norflash_write(u32 addr,u8* buff,u16 len)
{
	EA = 0;//���ж�
	//1.�Ȱ�buff�е�����д�뵽DGUSII���ñ�����(CACHE_ADDR)����һ��
	write_dgusii_vp(CACHE_ADDR,buff,len);
	
	//2.Ȼ���ٰ�CACHE_ADDR��������д�뵽nor flash��
	norflash_cmd[0] = NOR_FLASH_WRITE;//д����
	norflash_cmd[1] = (addr>>16)&0xff;//nor flash��ַ
	norflash_cmd[2] = (addr>>8)&0xff;
	norflash_cmd[3] = addr&0xff;
	norflash_cmd[4] = (CACHE_ADDR>>8)&0xff;//dgusii���ݵ�ַ
	norflash_cmd[5] = CACHE_ADDR&0xff;
	norflash_cmd[6] = (len>>8)&0xff;//д������ݳ���
	norflash_cmd[7] = len&0xff;
	write_dgusii_vp(NOR_FLASH_ADDR,norflash_cmd,4);
	
	//3.�ȴ��������
	while(1)
	{
		read_dgusii_vp(NOR_FLASH_ADDR,norflash_cmd,2);//ֻ���ȡ0x0008������ǰ2����,Ȼ���ж�D7�Ƿ�Ϊ0
		if(norflash_cmd[0]==0)
			break;
		sys_delay_about_ms(1);//�����ʱ�����,���Է�ֹĪ������Ĵ���
	}
	EA = 1;//���ж�
}



//��T5L�ڲ��Դ���nor flash��ȡ����
//addr:nor flash�ĵ�ַ,����Ϊż��,��ΧΪ:0x000000-0x027FFE,Ȼ��һ����ַ��Ӧ2���ֽ�,��������Ϊ320KB
//buff:��Ҫ����Ļ�����,�������ٴ���len*2
//len:��ȡ������,����Ϊż��,����д�����󳤶�Ҳ������,�����CACHE_ADDR��ֵ�й�ϵ
void norflash_read(u32 addr,u8* buff,u16 len)
{
	//1.��nor flash�е����ݶ�ȡ��CACHE_ADDR
	norflash_cmd[0] = NOR_FLASH_READ;//������
	norflash_cmd[1] = (addr>>16)&0xff;//nor flash��ַ
	norflash_cmd[2] = (addr>>8)&0xff;
	norflash_cmd[3] = addr&0xff;
	norflash_cmd[4] = (CACHE_ADDR>>8)&0xff;//dgusii���ݵ�ַ
	norflash_cmd[5] = CACHE_ADDR&0xff;
	norflash_cmd[6] = (len>>8)&0xff;//д������ݳ���
	norflash_cmd[7] = len&0xff;
	write_dgusii_vp(NOR_FLASH_ADDR,norflash_cmd,4);
	
	//2.�ȴ��������
	while(1)
	{
		read_dgusii_vp(NOR_FLASH_ADDR,norflash_cmd,2);//ֻ���ȡ0x0008������ǰ4���ֽ�,Ȼ���ж�D7�Ƿ�Ϊ0
		if(norflash_cmd[0]==0)
			break;
		sys_delay_about_ms(1);//�����ʱ�����,���Է�ֹĪ������Ĵ���
	}
	
	//3.�ٰ�CACHE_ADDR�������ݶ�ȡ��buff��
	read_dgusii_vp(CACHE_ADDR,buff,len);
}



















