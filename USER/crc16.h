/*
 * @Author: xw.qu
 * @Date: 2023-08-31 09:22:42
 * @LastEditors: xw.qu
 * @LastEditTime: 2023-08-31 14:11:06
 * @FilePath: \USER\crc16.h
 * @Description: 
 * 
 * Copyright (c) 2023 by xw.qu, All Rights Reserved. 
 */


#ifndef _CRC16_H_
#define _CRC16_H_


#include "t5los8051.h"


u16 crc16table(u8 *ptr, u16 len);

#endif