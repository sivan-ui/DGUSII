/*
 * @Author: xw.qu
 * @Date: 2023-08-31 09:22:42
 * @LastEditors: xw.qu
 * @LastEditTime: 2023-08-31 10:48:32
 * @FilePath: \USER\mb_port.c
 * @Description: 
 * 
 * Copyright (c) 2023 by xw.qu, All Rights Reserved. 
 */



#include "mb_include.h"
#include "t5los8051.h"

void mb_port_uartInit(unsigned long baud,unsigned char parity)
{

}

void mb_port_uartEnable(unsigned char txen,unsigned char rxen)
{
	if(txen)
	{

	}
	else
	{

	}
	
	if(rxen)
	{

	}
	else
	{

	}		
}
void mb_set_receive_state(void)
{
	TR4=0;
}

void mb_set_send_state(void)
{
	TR4=1;	
}
void mb_port_putchar(unsigned char ch)
{
	
	mb_set_send_state();
  while(Busy4);
  Busy4 = 1;
  SBUF4_TX = ch; 		


}

void mb_port_getchar(unsigned char *ch)
{
	 *ch=SBUF4_RX;
}

void mb_port_timerInit(unsigned long baud)
{
	unsigned long j,k;
	k=2;
	j=(unsigned long)1000*3*11;	//(1/1mS)*3T*11B/BaudRate


	j/=baud;		

	if(j==0)
	{
		j++;
	}
	j+=k;		
	mbhRecTimeout = j;	
}

void mb_port_timerEnable()
{
                               //使能定时器
}

void mb_port_timerDisable()
{

}

