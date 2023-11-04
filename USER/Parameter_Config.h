/*
 * @Author: xw.qu
 * @Date: 2023-08-31 09:22:42
 * @LastEditors: xw.qu
 * @LastEditTime: 2023-11-04 11:13:34
 * @FilePath: \Binaryd:\七寸屏\NEW_UI\PROJ_V1.0-20231102 - 13\C51\template\USER\Parameter_Config.h
 * @Description: 
 * 
 * Copyright (c) 2023 by xw.qu, All Rights Reserved. 
 */
#ifndef __PARAMETER_CONFIG_H__
#define __PARAMETER_CONFIG_H__
#include "t5los8051.h"
#include "Uart.h"
#include "nor_flash.h"
#include "mb_include.h"
#include "mb_user.h"
#define SELECT_RTC_TYPE 2 // RX8130=1;SD2058=2 ;无RTC=0

#define UART2_ENABLE 1 // T5L  
#define UART3_ENABLE 0 // T5L  
#define UART4_ENABLE 1 // T5L  
#define UART5_ENABLE 1 // T5L  

#define UART2_RX_LENTH 2048 // T5L  
#define UART3_RX_LENTH 2048 // T5L  
#define UART4_RX_LENTH 2048 // T5L  
#define UART5_RX_LENTH 2048 // T5L  

#define BAUD_UART2 115200 // T5L  
#define BAUD_UART3 115200 // T5L  
#define BAUD_UART4 19200  // 115200  485通讯
#define BAUD_UART5 115200 // T5L  

#define RESPONSE_UART2 1 // 
#define RESPONSE_UART3 0 // 
#define RESPONSE_UART4 0 // 
#define RESPONSE_UART5 0 // 

#define DATA_UPLOAD_UART2 1 // 
#define DATA_UPLOAD_UART3 1 // 
#define DATA_UPLOAD_UART4 1 // 
#define DATA_UPLOAD_UART5 0 // 

#define CRC_CHECK_UART2 0 // 
#define CRC_CHECK_UART3 0 // 
#define CRC_CHECK_UART4 0 // 
#define CRC_CHECK_UART5 0 // 

#define DUBG_PRINTF
#ifdef DUBG_PRINTF
#define USER_PRINTF printf
#else
#define USER_PRINTF / ## /
#endif
#endif