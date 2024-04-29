
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


#endif
