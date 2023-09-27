/*
 * @Author: xw.qu
 * @Date: 2023-08-31 09:22:43
 * @LastEditors: xw.qu
 * @LastEditTime: 2023-08-31 10:53:58
 * @FilePath: \USER\Uart.c
 * @Description: uart function
 * 
 * Copyright (c) 2023 by xw.qu, All Rights Reserved. 
 */
#include "Uart.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "stdarg.h"

void flash_data_init();

//**********************************************************
#if UART2_ENABLE
u8 xdata R_u2[UART2_RX_LENTH]; // ����2��������
#else
u8 xdata R_u2[20]; // ����2��������
#endif
// u8          xdata         W_u2[135];	  //����2��������
u8 xdata R_OD2 = 0;	 // ����2�յ�����
u16 xdata R_CN2 = 0; // ��2���ȼ�����
u8 xdata T_O2 = 0;	 // ��2��ʱ������
bit Busy2 = 0;			 // ����2���ͱ�־
//**********************************************************
#if UART3_ENABLE
u8 xdata R_u3[UART3_RX_LENTH]; // ����3��������
#else
u8 xdata R_u3[20]; // ����3��������
#endif
u8 xdata R_OD3 = 0;	 // ����3�յ�����
u16 xdata R_CN3 = 0; // ��3���ȼ�����
u8 xdata T_O3 = 0;	 // ��3��ʱ������
bit Busy3 = 0;			 // ����3���ͱ�־
// ========================================================================
#if UART4_ENABLE
// u8          xdata        R_u4[UART4_RX_LENTH];	        //����4��������
#else
u8 xdata R_u4[20]; // ����4��������
#endif
// u8        xdata        W_u4[255];	        //����4��������
u8 xdata R_OD4 = 0;	 // ����4�յ�����
u16 xdata R_CN4 = 0; // ��4���ȼ�����
u8 xdata T_O4 = 0;	 // ��4��ʱ������
bit Busy4 = 0;			 // ����4���ͱ�־
//**********************************************************
#if UART5_ENABLE
u8 xdata R_u5[UART5_RX_LENTH]; // ����5��������
#else
u8 xdata R_u5[20]; // ����5��������
#endif
// u8          xdata         W_u5[135];	  //����5��������
u8 xdata R_OD5 = 0;	 // ����5�յ�����
u16 xdata R_CN5 = 0; // ��5���ȼ�����
u8 xdata T_O5 = 0;	 // ��5��ʱ������
bit Busy5 = 0;			 // ����5���ͱ�־
//**********************************************************

bit Response_flog = 0;		// Ӧ����
bit Auto_data_upload = 0; // �����Զ��ϴ���
bit Crc_check_flog = 0;		// CrcУ����
u8 xdata i_test;
/****************************************************************************/
unsigned short HR[HR_MAX_NUMBER];
#define msg_type_TYPE 1
#define enterprise_TYPE 2
#define timestamp_TYPE 3
#define relay_modules_TYPE 4
#define srelay_modules_TYPE 5
#define dimmer_modules_TYPE 6
#define panel_modules_TYPE 7
#define modules_scene_panels_TYPE 8

#define modules_name_address 0x2000
#define modules_name_offset 0x0020
#define switch_name_address 0x2500
#define switch_name_offset 0x0020

#define PRINTF_USART_NUM 5

const char *base64char = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
int num_strchr(const char *str, char c) //
{
	const char *pindex = strchr(str, c);
	if (NULL == pindex)
	{
		return -1;
	}
	return pindex - str;
}
int base64_decode(const char *base64, unsigned char *dedata)
{
	int i = 0, j = 0;
	int trans[4] = {0, 0, 0, 0};
	for (; base64[i] != '\0'; i += 4)
	{

		trans[0] = num_strchr(base64char, base64[i]);
		trans[1] = num_strchr(base64char, base64[i + 1]);

		dedata[j++] = ((trans[0] << 2) & 0xfc) | ((trans[1] >> 4) & 0x03);

		if (base64[i + 2] == '=')
		{
			continue;
		}
		else
		{
			trans[2] = num_strchr(base64char, base64[i + 2]);
		}

		dedata[j++] = ((trans[1] << 4) & 0xf0) | ((trans[2] >> 2) & 0x0f);

		if (base64[i + 3] == '=')
		{
			continue;
		}
		else
		{
			trans[3] = num_strchr(base64char, base64[i + 3]);
		}

		dedata[j++] = ((trans[2] << 6) & 0xc0) | (trans[3] & 0x3f);
	}

	dedata[j] = '\0';
	printf("hello word!");
	return 0;
}
#if UART2_ENABLE
void Uart2_Init(u32 baud_rate)
{
	u16 i = 0;
	i = 1024 - FOSC / 64 / baud_rate;
	SREL2H = (u8)(i >> 8); // ������ = FCLK/64*(1024-SREL)
	SREL2L = (u8)i;				 //
	MUX_SEL |= 0x40;			 // UART2����,UART3��������WDT�ر�
	ADCON = 0x80;					 // ѡ��SREL0H:L��Ϊ�����ʷ�����
	SCON2 = 0x50;					 // ����ʹ�ܺ�ģʽ����
	PCON &= 0x7F;					 // SMOD=0
	IEN0 |= 0X10;					 // ES0=1	 ����2 ����+�����ж�
	EA = 1;
}
#endif
/****************************************************************************/
#if UART3_ENABLE
void Uart3_Init(u32 baud_rate)
{
	u16 i = 0;
	i = 1024 - FOSC / 32 / baud_rate;
	SREL3H = (u8)(i >> 8); // ������ = FCLK/64*(1024-SREL)
	SREL3L = (u8)i;				 // S_2/115200=224=0X03C8 1024-206438400/(32*115200)=0X03C8
	MUX_SEL |= 0x20;			 // UART3����
	P0MDOUT |= 0x40;			 // P0.6 TXD ����
	SCON3 = 0x90;					 // ����ʹ�ܺ�ģʽ����
	IEN2 |= 0x01;					 // �ж�ʹ��
	EA = 1;
}
#endif
/****************************************************************************/
#if UART4_ENABLE
void Uart4_Init(u32 baud_rate)
{
	u16 i = 0;
	i = FOSC / 8 / baud_rate;
	BODE4_DIV_H = (u8)(i >> 8); //
	BODE4_DIV_L = (u8)i;				//
	SCON4T = 0x80;							// ����ʹ�ܺ�ģʽ����
	SCON4R = 0x80;							// ����ʹ�ܺ�ģʽ����
	ES4R = 1;										// �жϽ���ʹ��
	ES4T = 1;										// �жϷ���ʹ��
	TR4 = 0;
	EA = 1;
}
#endif
/****************************************************************************/
#if UART5_ENABLE
void Uart5_Init(u32 baud_rate)
{
	u16 i = 0;
	i = FOSC / 8 / baud_rate;
	BODE5_DIV_H = (u8)(i >> 8); //
	BODE5_DIV_L = (u8)i;				//
	SCON5T = 0x80;							// ����ʹ�ܺ�ģʽ����,8bitģʽ
	//		 SCON5T= 0xC0;//����ʹ�ܺ�ģʽ����,9bitģʽ
	SCON5R = 0x80; // ����ʹ�ܺ�ģʽ����
	ES5R = 1;			 // �жϽ���ʹ��
	ES5T = 1;			 // �жϷ���ʹ��
	TR5 = 0;
	EA = 1;
}
#endif
/*************************************************************************************************/
void uart_init()
{
#if UART2_ENABLE
	Uart2_Init(BAUD_UART2);
#endif
#if UART3_ENABLE
	Uart3_Init(BAUD_UART3);
#endif
#if UART4_ENABLE
	Uart4_Init(BAUD_UART4);
#endif
#if UART5_ENABLE
	Uart5_Init(BAUD_UART5);
#endif

	// #if DEBUG
	//
	// #else
	//			flash_data_init();
	// #endif
}

/*************************************************************
��������void  uart_send_byte(u8 Uart_number,u8 Dat)
������Uart_number=���ں�=2��3��4��5��DatΪ��Ҫ���͵�byte
���ߣ�cuijia
���ڣ�20220526
***********************************************************/
void uart_send_byte(u8 Uart_number, u8 Dat)
{
	if (Uart_number == 2)
	{
		while (Busy2)
			;
		Busy2 = 1;
		SBUF2 = Dat;
	}
	else if (Uart_number == 3)
	{
		while (Busy3)
			;
		Busy3 = 1;
		SBUF3 = Dat;
	}
	else if (Uart_number == 4)
	{
		TR4 = 1;
		while (Busy4)
			;
		Busy4 = 1;
		SBUF4_TX = Dat;
		while (Busy4)
			;
		TR4 = 0;
	}
	else if (Uart_number == 5)
	{
		while (Busy5)
			;
		Busy5 = 1;
		SBUF5_TX = Dat;
	}
}
/****************************�����Զ��ϴ�************************************************/
void uart_data_send(u8 *Arr, u8 Uartnum, bit Outo_send, bit Crc_ck)
{
	u8 i = 0;
	u16 V1 = read_dgus(0x0f01);
	Auto_data_upload = Outo_send;
	if (Auto_data_upload)
	{
		Crc_check_flog = Crc_ck;
		if (Crc_check_flog) // ��CrcУ��
		{
			u16 Crc = 0;
			Arr[2] = (((u8)V1) << 1) + 6;
			Crc = crc16table(Arr + 3, Arr[2] - 2);
			Arr[Arr[2] + 1] = Crc & 0x00FF;
			Arr[Arr[2] + 2] = Crc >> 8;
			if (Uartnum == 4)
				TR4 = 1;
			else if (Uartnum == 5)
				TR5 = 1;
			for (i = 0; i < Arr[2] + 3; i++)
				uart_send_byte(Uartnum, Arr[i]);
			if (Uartnum == 4)
			{
				while (Busy4)
					;
				TR4 = 0;
			}
			else if (Uartnum == 5)
			{
				while (Busy5)
					;
				TR5 = 0;
			}
			Arr[2] = (((u8)V1) << 1) + 4;
		}
		else
		{
			if (Uartnum == 4)
				TR4 = 1;
			else if (Uartnum == 5)
				TR5 = 1;
			for (i = 0; i < Arr[2] + 3; i++)
				uart_send_byte(Uartnum, Arr[i]);
			if (Uartnum == 4)
			{
				while (Busy4)
					;
				TR4 = 0;
			}
			else if (Uartnum == 5)
			{
				while (Busy5)
					;
				TR5 = 0;
			}
		}
	}
}
/****************************�����Զ��ϴ�************************************************/
void Sw_Data_Send()
{
	u16 Va = read_dgus(0x0f00);
	u16 V1 = read_dgus(0x0f01);
	if (((u8)(Va >> 8)) == 0x5A)
	{
		u8 i = 0;
		u16 Temp = 0;
		u8 Val[100] = {0}; // 5A A5 06 83 ADDR Len XX XX
		Val[0] = 0x5A;
		Val[1] = 0xA5;
		Val[2] = (((u8)V1) << 1) + 4;
		Val[3] = 0x83;
		Val[4] = (u8)Va;
		Val[5] = (u8)(V1 >> 8);
		Val[6] = (u8)V1;
		for (i = 0; i < (u8)V1; i++)
		{
			Temp = read_dgus(((Val[4] << 8) + Val[5] + i));
			Val[7 + 2 * i] = (u8)(Temp >> 8);
			Val[8 + 2 * i] = (u8)(Temp);
		}
#if UART2_ENABLE
		uart_data_send(Val, 2, DATA_UPLOAD_UART2, CRC_CHECK_UART2);
#endif
#if UART3_ENABLE
		uart_data_send(Val, 3, DATA_UPLOAD_UART3, CRC_CHECK_UART3);
#endif
#if UART4_ENABLE
		uart_data_send(Val, 4, DATA_UPLOAD_UART4, CRC_CHECK_UART4);
#endif
#if UART5_ENABLE
		uart_data_send(Val, 5, DATA_UPLOAD_UART5, CRC_CHECK_UART5);
#endif
		write_dgus(0x0f00, 0);
		write_dgus(0x0f01, 0);
	}
}

/****************************************************************************/
void deal_82_cmd(u8 Uart, u8 *arr)
{
	u8 i = 0;
	if (Crc_check_flog == 0) // ����CRC����
	{
		write_dgusii_vp((arr[4] << 8) + arr[5], arr + 6, arr[2] - 3);
		if (Response_flog)
		{
			u8 Temp_arr[] = {DTHD1, DTHD2, 0X03, 0X82, 0X4F, 0X4B};
			if (Uart == 4)
				TR4 = 1;
			else if (Uart == 5)
				TR5 = 1;
			for (i = 0; i < 6; i++)
				uart_send_byte(Uart, Temp_arr[i]); // 5A A5 03 82 4F 4B
			if (Uart == 4)
			{
				while (Busy4)
					;
				TR4 = 0;
			}
			else if (Uart == 5)
			{
				while (Busy5)
					;
				TR5 = 0;
			}
		}
	}
	else // ��CRCУ��
	{
		u16 Crc = 0, Crc_check = 0;
		Crc = crc16table((u8 *)(&arr[3]), arr[2] - 2);
		Crc_check = (u16)(arr[3 + arr[2] - 1] << 8) + (u16)(arr[3 + arr[2] - 2]);
		if (Crc == Crc_check)
		{
			write_dgusii_vp((arr[4] << 8) + arr[5], arr + 6, arr[2] - 5);
			if (Response_flog)
			{
				u8 Temp_arr[] = {DTHD1, DTHD2, 0X05, 0X82, 0X4F, 0X4B, 0XA5, 0XEF};
				if (Uart == 4)
					TR4 = 1;
				else if (Uart == 5)
					TR5 = 1;
				for (i = 0; i < 8; i++)
					uart_send_byte(Uart, Temp_arr[i]); // 5A A5 03 82 4F 4B
				if (Uart == 4)
				{
					while (Busy4)
						;
					TR4 = 0;
				}
				else if (Uart == 5)
				{
					while (Busy5)
						;
					TR5 = 0;
				}
			}
		}
	}
}
/****************************************************************************/
void deal_83_cmd(u8 Uart, u8 *arr, u8 *arr1)
{
	u8 i = 0;
	if (Crc_check_flog == 0) // ����CRC����
	{
		for (i = 0; i < 7; i++)
			arr[i] = arr1[i];
		read_dgusii_vp((arr[4] << 8) + arr[5], (u8 *)&arr[7], arr[6]);
		arr[2] = (2 * arr[6]) + 4;
		if (Uart == 4)
			TR4 = 1;
		else if (Uart == 5)
			TR5 = 1;
		for (i = 0; i < (arr[2] + 3); i++)
			uart_send_byte(Uart, arr[i]);
		if (Uart == 4)
		{
			while (Busy4)
				;
			TR4 = 0;
		}
		else if (Uart == 5)
		{
			while (Busy5)
				;
			TR5 = 0;
		}
	}
	else // ��Crc����
	{
		u16 Crc = 0, Crc_check = 0;
		for (i = 0; i < 9; i++)
			arr[i] = arr1[i];
		Crc = crc16table((u8 *)(&arr[3]), arr[2] - 2);
		Crc_check = (u16)(arr[3 + arr[2] - 1] << 8) + (u16)(arr[3 + arr[2] - 2]);
		if (Crc == Crc_check)
		{
			read_dgusii_vp((arr[4] << 8) + arr[5], (u8 *)&arr[7], arr[6]);
			arr[2] = (2 * arr[6]) + 4 + 2;
			Crc = crc16table(arr + 3, arr[2] - 2);
			arr[arr[2] + 1] = Crc & 0x00FF;
			arr[arr[2] + 2] = Crc >> 8;
			if (Uart == 4)
				TR4 = 1;
			else if (Uart == 5)
				TR5 = 1;
			for (i = 0; i < (arr[2] + 3); i++)
				uart_send_byte(Uart, arr[i]);
			if (Uart == 4)
			{
				while (Busy4)
					;
				TR4 = 0;
			}
			else if (Uart == 5)
			{
				while (Busy5)
					;
				TR5 = 0;
			}
		}
	}
}
/*************************************************************************************************************
��������deal_uart_data(u8* Arr_rece,u16* Data_len,u8 Uart_num,bit Response,bit Crc_ck)
������Arr_receΪ���ڽ������飬Data_len�������ݳ��ȵ�ַ��Uart_num���ںţ�Response�Ƿ��Ӧ��Crc_ck�Ƿ��У��
���ߣ�cuijia
���ڣ�20220526
*************************************************************************************************************/
void deal_uart_data(u8 *Arr_rece, u16 *Data_len, u8 Uart_num, bit Response, bit Crc_ck)
{
	u16 N = 0;
	bit Flog = 1;
	while (Flog)
	{
		if ((Arr_rece[N] == DTHD1) && (Arr_rece[N + 1] == DTHD2)) // 5A A5 07 82 1000 0001 0002
		{
			if (Arr_rece[N + 3] == 0x82)
			{
				Response_flog = Response;						 // Ӧ����
				Crc_check_flog = Crc_ck;						 // Crc���
				deal_82_cmd(Uart_num, Arr_rece + N); // ����82ָ��
				N = N + Arr_rece[N + 2] + 2;
			}																	// 0	 1	2  3  4	5  6  7	8  9 10
			else if (Arr_rece[N + 3] == 0x83) // 5A A5 08 83 1000 02 0001 0002
			{

				u8 Val[FRAME_LEN] = {0};
				Crc_check_flog = Crc_ck;									// Crc���
				deal_83_cmd(Uart_num, Val, Arr_rece + N); // ����83ָ��
				N = N + Arr_rece[N + 2] + 2;
			}
			else
				N++;
		}
		if (N < (*Data_len - 4))
			N++;
		else
			Flog = 0;
	}
}

void uart_frame_deal()
{

	/*test LKC*/
#if DEBUG
	if (0 == R_OD4_flag)
	{
		R_OD4 = 1;
		R_OD4_flag = 1;
	}
	else
	{
	}
#endif
	if ((1 == R_OD2) && (T_O2 == 0))
	{
		deal_uart_data(R_u2, (u16 *)(&R_CN2), 2, RESPONSE_UART2, CRC_CHECK_UART2);
		//							   USER_PRINTF("USART2_GET_DATA\n");
		R_OD2 = 0;
		R_CN2 = 0;
	} // ����2�����շ�
	if ((1 == R_OD3) && (T_O3 == 0))
	{
		deal_uart_data(R_u3, (u16 *)(&R_CN3), 3, RESPONSE_UART3, CRC_CHECK_UART3);
		R_OD3 = 0;
		R_CN3 = 0;
	} // ����2�����շ�
	//            if((1==R_OD4)&&(T_O4==0)){
	////							   deal_uart4_data(R_u4,(u16*)(&R_CN4),4,RESPONSE_UART4,CRC_CHECK_UART4);
	//							   R_OD4=0;R_CN4=0;
	//						} //����4�����շ�
	if ((1 == R_OD5) && (T_O5 == 0))
	{
		deal_uart_data(R_u5, (u16 *)(&R_CN5), 5, RESPONSE_UART5, CRC_CHECK_UART5);
		R_OD5 = 0;
		R_CN5 = 0;
		//								USER_PRINTF("USART5_GET_DATA\n");
	} // ����5�����շ�
}
//=========================//
//=========�жϺ���========//
//=========================//

//========================
void uart4_Risr() interrupt 11 // ����4�жϽ���
{
	//            R_u4[R_CN4]=SBUF4_RX;
	mbh_uartRxIsr();
	SCON4R &= 0xFE;
	R_OD4 = 1;
	if (R_CN4 < UART4_RX_LENTH - 1)
		R_CN4++;
	T_O4 = 5;
}
//========================
void uart4_Tisr() interrupt 10
{

	SCON4T &= 0xFE;
	Busy4 = 0;
}
//=========================
void uart5_Risr() interrupt 13 // ����5�жϽ���
{
	R_u5[R_CN5] = SBUF5_RX;
	SCON5R &= 0xFE;
	R_OD5 = 1;
	if (R_CN5 < UART5_RX_LENTH - 1)
		R_CN5++;
	T_O5 = 5;
}
//========================
void uart5_Tisr() interrupt 12
{
	SCON5T &= 0xFE;
	Busy5 = 0;
}
//========================
//*********************************************************
void uart3_Risr() interrupt 16 // ����3�շ��ж�
{
	if (SCON3 & 0x01)
	{
		R_u3[R_CN3] = SBUF3;
		SCON3 &= 0xFE;
		//				        SCON3&=0xFE;
		R_OD3 = 1;
		if (R_CN3 < UART3_RX_LENTH - 1)
			R_CN3++;
		T_O3 = 5;
	}
	if (SCON3 & 0x02)
	{

		SCON3 &= 0xFD;
		SCON3 &= 0xFD;
		Busy3 = 0;
	}
}
//*********************************************************
void uart2_Risr() interrupt 4 // ����2�շ��ж�
{
	if (RI2 == 1)
	{
		R_u2[R_CN2] = SBUF2;
		SCON2 &= 0xFE;
		R_OD2 = 1;
		if (R_CN2 < UART2_RX_LENTH - 1)
			R_CN2++;
		T_O2 = 5;
	}
	if (TI2 == 1)
	{
		SCON2 &= 0xFD;
		Busy2 = 0;
	}
}

void user_uart_send(char c)
{
	uart_send_byte(PRINTF_USART_NUM, c);
}

void UART5_SendString(char *s)
{
	while (*s) // ����ַ���������
	{
		user_uart_send(*s++); // ���͵�ǰ�ַ�
	}
}
char putchar(char c)
{
	uart_send_byte(PRINTF_USART_NUM, c);
	return c;
}
