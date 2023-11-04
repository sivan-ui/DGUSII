/*
 * @Author: xw.qu
 * @Date: 2023-09-28 09:14:31
 * @LastEditors: xw.qu
 * @LastEditTime: 2023-11-04 11:18:26
 * @FilePath: \Binaryd:\七寸屏\NEW_UI\PROJ_V1.0-20231102 - 13\C51\template\USER\touch_panel.h
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
	void set_key_execute_scene_nb(touch_panel_t *p,unsigned char nb);
	unsigned char get_key_execute_scene_nb(touch_panel_t *p,unsigned char nb);

}touch_panel_fun_t;
typedef struct 
{
  touch_panel_t touch_panel;
	touch_panel_fun_t touch_panel_fun;
	unsigned char scene_execute_number;
}touch_panel_op_t;

touch_panel_op_t scene_touch;
#endif