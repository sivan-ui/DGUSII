/*
 * @Author: xw.qu
 * @Date: 2023-09-28 09:14:31
 * @LastEditors: xw.qu
 * @LastEditTime: 2023-09-28 10:37:39
 * @FilePath: \USER\touch_panel.h
 * @Description: 
 * 
 * Copyright (c) 2023 by xw.qu, All Rights Reserved. 
 */


#ifndef __TOUCH_PANEL_H__
#define __TOUCH_PANEL_H__
typedef struct 
{
	/* data */
	unsigned char adr;
	unsigned char key_nb;

}touch_panel_t;


typedef struct 
{
  touch_panel_t touch_panel;
	unsigned char scene_execute_number;
}touch_panel_op_t;


#endif