//======================================================================
//Copyright (C), 2014-2999, Tech. Co., Ltd.
//======================================================================
//FileName: Data_Init.h
//Author:
//Version: 1.0.0
//Date:
//Description:
//======================================================================
#ifndef __UART_COMM_H
#define __UART_COMM_H
#include "stm32g0xx_hal.h"

#define Uart_RX_BuffLen 1000
extern uint8_t Dis_Charge_Flag;
extern uint8_t Uart_Buffer[];
extern uint8_t PlaceFlag_COM;
extern uint8_t X_axisFlag_COM;
extern uint8_t SetCommondFlag;
extern uint8_t PlaceFlag;
extern uint8_t X_axisFlag;
extern uint8_t Comm_Flag_1, Comm_Flag_2, Comm_Flag_3;
extern uint8_t sleep_flag;
extern uint16_t uart_error_flag;
/*函数声明*/
void Uart_ReceiveIQR(uint8_t Buffer);
void Uart_TansmitIQR(void);
void Uart_RequestData(void);
void Uart_DataRespond(void);

void Uart_RequestData_2(void);
void Uart_DataRespond_2(void);

//void Uart_RequestData_3(void);
//void Uart_DataRespond_3(void);

void Uart_DataRespond_4(void);
void Uart_DataRespond_5(void);

void Uart_ConvertData_2(void);
void Uart_ConvertData_3(void);

void Uart_SetParaCMD_1(uint8_t Data);
void Uart_SetParaCMD_2(uint8_t Data);
void Uart_GetParaCMD_1(void);
void Uart_GetParaCMD_2(void);
void UartInit(void);
extern void UartInit(void);

#endif
