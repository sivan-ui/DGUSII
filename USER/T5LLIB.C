#include "T5LLIB.h"
#include "t5los8051.h"

// ���ָ��DGUS������ַ��DATA3����Ļ����˳�
void wait_ok(unsigned int addr)
{
	ADR_H = 0x00;
	ADR_M = (unsigned char)(addr >> 8);
	ADR_L = (unsigned char)(addr);
	ADR_INC = 0x00;
	do
	{
		for (addr = 0; addr < 1000; addr++) // �ͷű����ռ�һ��ʱ��
		{
			DATA2 = DATA1;
		}
		RAMMODE = 0xAF;
		while (APP_ACK == 0)
			;
		APP_EN = 1;
		while (APP_EN == 1)
			;
		RAMMODE = 0x00;
	} while (DATA3 != 0);
}

// ��дT5LƬ��256KW Flash��mod=0x5A read from flash��ȡ��mod=0xA5Ϊwrite to flash
// addr=DGUS������ַ��������ż����addr_flash=flash��ȡ��ַ��������ż����len=��ȡ�ֳ��ȣ�������ż����
void T5L_Flash(unsigned char mod, unsigned int addr, long addr_flash, unsigned int len)
{
	ADR_H = 0x00;
	ADR_M = 0x00;
	ADR_L = 0x04;
	ADR_INC = 0x01;
	RAMMODE = 0x8F; // ������Flash
	while (APP_ACK == 0)
		;
	DATA3 = mod;
	DATA2 = (unsigned char)(addr_flash >> 16);
	DATA1 = (unsigned char)(addr_flash >> 8);
	DATA0 = (unsigned char)(addr_flash & 0xFE);
	APP_EN = 1;
	while (APP_EN == 1)
		;
	DATA3 = (unsigned char)(addr >> 8);
	DATA2 = (unsigned char)(addr & 0xFE);
	DATA1 = (unsigned char)(len >> 8);
	DATA0 = (unsigned char)(len & 0xFE);
	APP_EN = 1;
	while (APP_EN == 1)
		;
	RAMMODE = 0x00;
	wait_ok(0x0004);
} // �ȴ����ݶ�ȡOK

// ��ȡDGUS�������� addr=DGUS������ַ  *buf=���ݷ���ָ��  len=��ȡ�����ֳ���
void read_dgusii_vp(unsigned int addr, unsigned char *buf, unsigned int len)
{
	unsigned char i;
	i = (unsigned char)(addr & 0x01);
	addr = addr / 2;
	ADR_H = 0x00;
	ADR_M = (unsigned char)(addr >> 8);
	ADR_L = (unsigned char)(addr);
	ADR_INC = 0x01;
	RAMMODE = 0xAF;
	while (APP_ACK == 0)
		;
	while (len > 0)
	{
		APP_EN = 1;
		while (APP_EN == 1)
			;
		if ((i == 0) && (len > 0))
		{
			*buf++ = DATA3;
			*buf++ = DATA2;
			i = 1;
			len--;
		}
		if ((i == 1) && (len > 0))
		{
			*buf++ = DATA1;
			*buf++ = DATA0;
			i = 0;
			len--;
		}
	}
	RAMMODE = 0x00;
}

// дDGUS�������� addr=DGUS������ַ  *buf=д������ָ��  len=д�������ֳ���
void write_dgusii_vp(unsigned int addr, unsigned char *buf, unsigned int len)
{
	unsigned char i;
	i = (unsigned char)(addr & 0x01);
	addr = addr / 2;
	ADR_H = 0x00;
	ADR_M = (unsigned char)(addr >> 8);
	ADR_L = (unsigned char)(addr);
	ADR_INC = 0x01;
	RAMMODE = 0x8F;
	while (APP_ACK == 0)
		;
	if (i && len > 0)
	{
		RAMMODE = 0x83;
		DATA1 = *buf++;
		DATA0 = *buf++;
		APP_EN = 1;
		while (APP_EN == 1)
			;
		len--;
	}
	RAMMODE = 0x8F;
	while (len >= 2)
	{
		DATA3 = *buf++;
		DATA2 = *buf++;
		DATA1 = *buf++;
		DATA0 = *buf++;
		APP_EN = 1;
		while (APP_EN == 1)
			;
		len = len - 2;
	}
	if (len)
	{
		RAMMODE = 0x8C;
		DATA3 = *buf++;
		DATA2 = *buf++;
		APP_EN = 1;
		while (APP_EN == 1)
			;
	}
	RAMMODE = 0x00;
}
