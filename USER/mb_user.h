#ifndef __MB_USER_H__
#define __MB_USER_H__
// #include "t5los8051.h"
#define HR_NB 512
#define ENTERPRISE 201
extern unsigned short master_send_hr[HR_NB];
extern unsigned char send_table[HR_NB * 2];
// #define MODBUS_RW_EN		//RW读写使能
#define MODBUS_HR_EN // HR读写使能
typedef enum
{
	MD_FUNC_NULL,
	MD_RD_COIL = 1,
	MD_RD_INPUT = 2,
	MD_RD_HR = 3,
	MD_FR_SCOIL = 5,
	MD_FR_SHR = 6,
	MD_FR_MCOIL = 15,
	MD_FR_MHR = 16,
	MD_FR_MHR_RDHR = 23
} MD_FUNC;
typedef struct
{
	unsigned char slv;
	unsigned char func;
	unsigned short da_adr;
	unsigned short da_n;
#ifdef MODBUS_RW_EN
	unsigned short rww_adr;
	unsigned short rww_n;
#endif
	// mf_func mff;
} FUNC_MODBUS_MASTER_FUNC_T;
// MODBUS 常量结构体
typedef struct
{
	unsigned char slv;					// 从机地址
	unsigned char func;					// 命令
	unsigned char rec_sta;			// 接收状态
	unsigned char b_ext;				// 状态
	unsigned char *send_dat;		// 发送缓冲
	unsigned short send_dat_nb; //
#ifdef MODBUS_COIL_OFS_EN
	unsigned short coil_ofs; // 线圈HR偏移量
#endif
#ifdef MODBUS_INPUT_OFS_EN
	unsigned short input_ofs; // 输入点HR偏移量
#endif
	unsigned short hr_n;		// HR的数量
	unsigned short *phr;		// HR地址
	unsigned short da_adr;	// 数据地址
	unsigned short da_n;		// 数据个数/数据内容
	unsigned short rww_adr; // RW命令的写入地址
	unsigned short rww_n;		// RW命令的写入数量

} MODBUS_T;
extern FUNC_MODBUS_MASTER_FUNC_T mb_master_func;
extern MODBUS_T user_modbus;
extern void mb_data_init(MODBUS_T *p);
extern void modtify_mb_infom(MODBUS_T *p, unsigned char slv, unsigned char fun, unsigned short adr, unsigned char da_n);
extern void set_master_send_hr(unsigned short *p, unsigned short index, unsigned short dat);
extern void short_copy_xch(void *t, void *s, int n, unsigned char b_xch);
// extern void realy_mb_send_single_ctrl(MODBUS_T *p);
// extern void realy_mb_send_all_ctrl(MODBUS_T *p);
extern void pack_data_send(MODBUS_T *p, unsigned char slv, unsigned char fun, unsigned short dat_adr, unsigned char dat_nb);
extern void modbus_send_data(MODBUS_T *p);
#endif