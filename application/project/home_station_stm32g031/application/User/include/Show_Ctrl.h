//======================================================================
//Copyright (C), 2014-2999, Tech. Co., Ltd.
//======================================================================
//FileName: Data_Init.h
//Author:
//Version: 1.0.0
//Date:
//Description:
//======================================================================

#ifndef __SHOW_CTRL_H
#define __SHOW_CTRL_H
#include "stm32g0xx_hal.h"


#define  PAGE_WELCOME         0
#define  PAGE_MENU            1

#define  PAGE_ANALOG_1        2
#define  PAGE_ANALOG_2        3
#define  PAGE_STATUS          4
#define  PAGE_PARASET         5
#define  PAGE_SYSTEMSET       6

#define  PAGE_TEMPERATURE_1   7
#define  PAGE_TEMPERATURE_2   8
#define  PAGE_TEMPERATURE_3   9

#define  PAGE_VOLTAGE_1       10
#define  PAGE_VOLTAGE_2       11
#define  PAGE_VOLTAGE_3       12
#define  PAGE_VOLTAGE_4       13

#define  PAGE_CAPACITY        14

#define  PAGE_WARNING_1       15
#define  PAGE_WARNING_2       16

#define  PAGE_PROTECTION_1    17
#define  PAGE_PROTECTION_2    18
#define  PAGE_PROTECTION_3    19

#define  PAGE_FAILALARM_1     20
#define  PAGE_FAILALARM_2     21
#define  PAGE_FAILALARM_3     22

#define  PAGE_BMS_VERSION     23
#define  PAGE_LCD_VERSION     24
#define  PAGE_SERIAL_NUMBER   25


/*外部变量声明*/
extern uint8_t New_Page_Status;
extern uint8_t Old_Page_Status;
extern uint8_t Key_Flag; 
extern uint8_t Time_Buffer[12];
extern uint8_t BMS_HW_V[10];


/*函数声明*/
void Page_Welcome(void);
void Page_Welcome_new(void);
void Page_Menu(void);
void Page_Welcome_1(void);
void Show_Ctrl(void);
void Show_Data_4Bit(uint8_t Row, uint8_t Col, uint16_t Data);
void Show_Data_4Bit1(uint8_t Row, uint8_t Col, uint16_t Data);
void Show_Temperature(int8_t Row, int8_t Col, int16_t Data);
void Show_Current(uint8_t Row, uint8_t Col, int16_t Data);
void Show_Current1(uint8_t Row, uint8_t Col, int16_t Data);
void Show_TotalVoltage(uint8_t Row, uint8_t Col, uint16_t Data);
void Show_TotalVoltage1(uint8_t Row, uint8_t Col, uint16_t Data);
void Show_Temperature1(int8_t Row, int8_t Col, int16_t Data);
#endif
