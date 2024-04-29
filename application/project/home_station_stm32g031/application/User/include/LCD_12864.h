//======================================================================
//Copyright (C), 2014-2999, Tech. Co., Ltd.
//======================================================================
//FileName: Data_Init.h
//Author:
//Version: 1.0.0
//Date:
//Description:
//======================================================================

#ifndef __LCD_12864_H
#define __LCD_12864_H
#include "stm32g0xx_hal.h"

#define A0_PIN                  GPIO_PIN_5                 
#define A0_GPIO_PORT            GPIOB  
#define  A0                     HAL_GPIO_WritePin(A0_GPIO_PORT, A0_PIN , GPIO_PIN_SET)
#define  _A0                    HAL_GPIO_WritePin(A0_GPIO_PORT, A0_PIN , GPIO_PIN_RESET)

#define SCK_PIN                 GPIO_PIN_11                 
#define SCK_GPIO_PORT           GPIOA  
#define SCK                     HAL_GPIO_WritePin(SCK_GPIO_PORT, SCK_PIN , GPIO_PIN_SET)
#define _SCK                    HAL_GPIO_WritePin(SCK_GPIO_PORT, SCK_PIN , GPIO_PIN_RESET)

#define SDO_PIN                 GPIO_PIN_12                 
#define SDO_GPIO_PORT           GPIOA  
#define SDO                     HAL_GPIO_WritePin(SDO_GPIO_PORT, SDO_PIN , GPIO_PIN_SET)
#define _SDO                    HAL_GPIO_WritePin(SDO_GPIO_PORT, SDO_PIN , GPIO_PIN_RESET)


#define CS_PIN                 GPIO_PIN_1                 
#define CS_GPIO_PORT           GPIOA  
#define CS                     HAL_GPIO_WritePin(CS_GPIO_PORT, CS_PIN , GPIO_PIN_SET)
#define _CS                    HAL_GPIO_WritePin(CS_GPIO_PORT, CS_PIN , GPIO_PIN_RESET)


#define  RESET_PIN             GPIO_PIN_8
#define  RESET_GPIO_PORT       GPIOB  
#define  RESET                 HAL_GPIO_WritePin(RESET_GPIO_PORT, RESET_PIN , GPIO_PIN_SET)
#define  _RESET                HAL_GPIO_WritePin(RESET_GPIO_PORT, RESET_PIN , GPIO_PIN_RESET)

#define  BL_EN_PIN             GPIO_PIN_4
#define  BL_EN_GPIO_PORT       GPIOA  
#define  BL_EN                 HAL_GPIO_WritePin(BL_EN_GPIO_PORT, BL_EN_PIN , GPIO_PIN_SET)
#define  _BL_EN                HAL_GPIO_WritePin(BL_EN_GPIO_PORT, BL_EN_PIN , GPIO_PIN_RESET)

#define MAXROWCNT       8
#define MAXCOLUMNCNT 	128


/*º¯ÊýÉùÃ÷*/
void Clear_Screen(unsigned char data1,unsigned char data2);
void Lcd_Close_Display(void);
void Display_HanZi_16x16(uint8_t Row, uint8_t Col, uint8_t Reverse, uint8_t *dp, uint8_t Index);
void Display_ZiFu_8x16(uint8_t Row, uint8_t Col, uint8_t *dp, uint8_t fb);
void Display_ZiFu1_16x16(uint8_t Row, uint8_t Col, uint8_t uchar, uint8_t fb);


void Lcd12864_Init(void);
void Lcd_showChar(uint8_t rowaddr,uint8_t columnaddr,uint8_t uchar,uint8_t fb);
void Lcd_showStringEN(uint8_t rowaddr, uint8_t columnaddr, uint8_t *str, uint8_t fb);
void Lcd_showStringENG(uint8_t rowaddr, uint8_t columnaddr, uint8_t *str, uint8_t fb);
void Lcd_showStringENGB(uint8_t rowaddr, uint8_t columnaddr, uint8_t *str, uint8_t fb);

void Lcd_showChar6_8(uint8_t rowaddr,uint8_t columnaddr,uint8_t uchar,uint8_t fb);
void Lcd_showCha6_7(uint8_t rowaddr,uint8_t columnaddr,uint8_t uchar,uint8_t fb);
void Lcd_showChar8_8(uint8_t rowaddr,uint8_t columnaddr,uint8_t uchar,uint8_t fb);
void OLED_ShowBMP(uint8_t x, uint8_t y, uint8_t px, uint8_t py, uint8_t index, uint8_t mode);
void Lcd_showChar6_16(uint8_t rowaddr,uint8_t columnaddr,uint8_t uchar,uint8_t fb);


extern const uint8_t ASCIITAB[];
uint16_t Lcd_searchFontCode(uint16_t code2);



void Lcd_showChax_x(uint8_t Row, uint8_t Col, uint8_t uchar, uint8_t fb);
#endif
