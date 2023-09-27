#ifndef __I2C_H__
#define __I2C_H__

#include "t5los8051.h"
extern u16    xdata        SysTick_RTC;
extern bit                 RTC_Flog;
extern u16    xdata        Count_num1;
extern u16    xdata        TimVal;
extern u16    xdata        Count_for_tx;
extern u32    xdata        enter_code_time_cnt;

void    Clock();

void rdtime(void);







#endif