
//======================================================================
//Copyright (C), 2014-2999, Tech. Co., Ltd.
//======================================================================
//FileName: Data_Init.h
//Author:
//Version: 1.0.0
//Date:
//Description:
//======================================================================

#ifndef __FONT_LIBRARY_H
#define __FONT_LIBRARY_H
#include "stm32g031xx.h"

typedef struct 
{ 
 uint16_t code; 
 uint16_t Index; 
}dotData_t; 

extern void Display_Custom_Bitmap_8x24(uint8_t Row, uint8_t Col, uint8_t WidthBytes, const uint8_t *bitmap_data);

/*外部声明变量*/
extern const uint8_t HanZi16x16[];
extern const uint8_t ZiFu16x16[];
extern const uint8_t YinWen8x16[];
extern const uint8_t ShuZi6x8[][6];
extern const uint8_t date6x7[][6];
extern const uint8_t show8x8[][8];

extern const uint8_t ASCIITAB[];
extern const dotData_t dotData[];
extern uint8_t fontnum (void);
extern const uint8_t Image[][1024];
extern const uint8_t ShuZi6x16[];

/* 新增图标字模声明 */
extern const uint8_t Lightning_Icon[8];   // 闪电图标 8x8
extern const uint8_t Battery_Empty[16];   // 空电池图标 8x16
extern const uint8_t Battery_Full[16];    // 满电池图标 8x16
extern const uint8_t Right_Frame_Lightning_64x16[128]; // 右侧边框+闪电图标 64x16，右侧预留总压显示区
extern const uint8_t Battery_Icon_24x16[48]; // 左侧SOC电池图标（默认）24x16
extern const uint8_t Battery_Icon_SOC_100_24x16[48];

extern const uint8_t Battery_Icon_SOC_0_30x16[60];
extern const uint8_t Battery_Icon_SOC_25_30x16[60];
extern const uint8_t Battery_Icon_SOC_50_30x16[60];
extern const uint8_t Battery_Icon_SOC_75_30x16[60];
extern const uint8_t Battery_Icon_SOC_100_30x16[60];
extern const uint8_t Right_Frame_Current_64x16[128]; // 右下电流框 64x16
extern const uint8_t Right_Frame_Lightning_64x24[192]; // 右上总压框 64x24
extern const uint8_t Right_Frame_Current_64x24[192]; // 右下电流框 64x24
extern const uint8_t Right_Frame_Lightning_64x16_New[128]; // 新 16px 总压框
extern const uint8_t Right_Frame_Current_64x16_New[128];   // 新 16px 电流框

extern const uint8_t Icon_Shield_16x16[32];
extern const uint8_t Icon_Thermometer_16x16[32];
extern const uint8_t Icon_Gear_16x16[32];

#endif
