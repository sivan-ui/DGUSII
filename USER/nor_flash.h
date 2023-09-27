/*
 * @Author: xw.qu
 * @Date: 2023-08-31 09:22:42
 * @LastEditors: xw.qu
 * @LastEditTime: 2023-09-06 14:23:07
 * @FilePath: \USER\nor_flash.h
 * @Description: 
 * 
 * Copyright (c) 2023 by xw.qu, All Rights Reserved. 
 */

#ifndef __NOR_FLASH_H__
#define __NOR_FLASH_H_
#include "sys.h"


#define NOR_FLASH_ADDR			0x0008	//系统变量地址
#define NOR_FLASH_WRITE			0xA5		//进行写操作
#define NOR_FLASH_READ			0x5A		//进行读操作

#define CACHE_ADDR					0xF000	//0XF000-0XFFFF作为缓冲区,大概有4096个字,4096*2个字节,注意,GUI开发时不要使用此区域





void norflash_write(u32 addr,u8* buff,u16 len);
void norflash_read(u32 addr,u8* buff,u16 len);
#endif

