//======================================================================
//Copyright (C), 2014-2999, Tech. Co., Ltd.
//======================================================================
//FileName: User_Timer.h
//Author:
//Version: 1.0.0
//Date:
//Description:
//======================================================================

#include "stm32g0xx_hal.h"
#ifndef __USER_TIMER_H
#define __USER_TIMER_H


/*定时定义*/
#define  IDLE_TIMER			        0
#define  DATA_SHOW_TIMER			1
#define  DATA_SAMPLE_TIMER			2
#define  DATA_SAMPLE_TIMER_2			3
#define  SAMP_TIME         		        4
#define  SAMP_TIME_2         		    5
#define  DATA_SAMPLE_TIMER_3			6

#define	 TIMERTOTAL			        7


/*定时器结构体*/
typedef struct{
	uint8_t Enable;
	uint8_t Flag;
	uint16_t Count;
}Timer_DataTypeDef;


/*外部声明变量*/
extern Timer_DataTypeDef Timer[TIMERTOTAL];

/*函数声明*/
void TIM2_Init(void);
void Timer_IQR(void);
void Set_Timer(uint8_t Index, uint16_t Count);
void Reset_Timer(uint8_t Index);
void Timer_Init(void);

#endif
