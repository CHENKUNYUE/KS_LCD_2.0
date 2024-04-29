//======================================================================
//Copyright (C), 2014-2999, Tech. Co., Ltd.
//======================================================================
//FileName: User_Usart.h
//Author:
//Version: 1.0.0
//Date:
//Description:
//======================================================================

#ifndef __USER_USART_H
#define __USER_USART_H
#include "stm32g0xx_hal.h"


/*串口1接收状态*/
#define  SOI 			        1
#define  ADDR/*VER*/			2
#define  CMD/*ADR*/			3
#define  VER/*CID1*/			4
#define  LENGTH/*CID2*/			5
#define  DATA/*LENGTH*/			6
#define  CHECKSUM/*INFO*/		7
#define  EOI/*CHKSUM*/			8
//#define  EOI			9


/*串口1通用命令码*/
#define  ANALOG			0x42
#define  WARNING		0x44
#define  SYSTEM			0x47
#define  VERSION		0x4F
#define  MANUFACTRUER	        0x51
#define  PACKNUM		0x90
#define  PACKNUMSD		0x40
#define  BAUDRATE		0x91
#define  CONTROL		0x99


/*串口1自定义命令码*/
#define  GETRECORD      0xA1
#define  GETPARA        0xA2
#define  SETPARA        0xA3
#define  REVPARA        0xA4

#define  DCHG_CURR0     0xA5
#define  CHG_CURR0      0xA6

#define  SETSOC			0xA7
#define  SETSLEEP		0xA8
#define  SETVREF        0xA9

#define  DCHG_GAIN_HIGH 0xAA
#define  DCHG_GAIN_LOW	0xAB
#define  CHG_GAIN		0xAC
#define  SETVSUM        0xAD
#define  SET_FULL_CHG   0xAE

#define  SETCFET	    0x9A
#define  SETDFET  	    0x9B
#define  GOTOSLEEP      0x9C

#define  GETDATE	    0xB1
#define  SETDATE  	    0xB2
#define  STORE_ADDR     0xB3
#define  SETBATCYCLE	0xBD

#define  SOFTWARE_VER   0xC1
#define  PRODUCT_CODE   0xC2
#define  WRITE_CODE     0xC3


/*串口1响应码*/
#define  NORMAL			0x00
#define  VERERROR		0x01
#define  CHKSUMERROR	0x02
#define  LCHKSUMERROR	0x03
#define  CID2ERROR		0x04
#define  FORMERROR		0x05
#define  DATAERROR		0x06
#define  ADRERROR		0x07
#define  COMERROR		0x08
#define  SETERROR		0x09


/*外部变量声明*/
extern uint8_t Temp[2];


/*函数声明*/
void Usart1_Init(uint32_t BaudRate);
void Usart2_Init(uint32_t BaudRate);
void Usart3_Init(uint32_t BaudRate);
uint8_t TwoAsciiToHex(uint8_t Ch1, uint8_t Ch2);
uint8_t Check_LCHKSUM(uint8_t *Pchar);
uint8_t Check_CHKSUM(uint8_t *Pchar);
void HextoTwoAscii(uint8_t Ch, uint8_t *Pchar);
void Length_Calculate(uint8_t *Pchar, uint16_t Len);
void Rtn_Create(uint8_t *Pchar, uint8_t Rtn);

#endif
