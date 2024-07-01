//======================================================================
//Copyright (C), 2014-2999, Tech. Co., Ltd.
//======================================================================
//FileName: Data_Init.h
//Author:
//Version: 1.0.0
//Date:
//Description:
//======================================================================
#ifndef __DATA_INIT_H
#define __DATA_INIT_H
#include "stm32g0xx_hal.h"

typedef struct {
    uint8_t BatNum;   //电压
    uint16_t Vol[16];

    uint8_t TempNum;  //温度
    int16_t Temp[10];

    int16_t Current; //电流
    uint16_t Vsum;   //总压
    uint16_t Rm;     //剩余容量
    uint16_t Fcc;       //满充容量
    uint16_t Cycle;     //循环
    uint16_t DesignCap; //设置容量

    uint16_t Status1;
    uint16_t Status2;
    uint16_t Status5;
    uint16_t Warning1; //告警
    uint16_t Warning2; //告警

    uint16_t BAT_TEMP;//电芯平均温度
    int16_t s16MosTemp;   //MOS温度
    int16_t s16EnvirTemp; //环境温度

    uint8_t Project_code_[10];

    union {
        uint16_t u16Data;
        struct {
            uint16_t bCellVoltOV : 1;
            uint16_t bCellVoltUL : 1;
            uint16_t bTempOV : 1;
            uint16_t bTempUL : 1;
            uint16_t bSocUL : 1;
            uint16_t bVDiffOV : 1;
            uint16_t bCurrOV : 1;
        } BitName;
    } Alarm_State;

    union {
        uint16_t u16Data;
        struct {
            uint16_t bCellVoltOV : 1;
            uint16_t bCellVoltUL : 1;
            uint16_t bTempOV : 1;
            uint16_t bTempUL : 1;
            uint16_t bCurrOV : 1;
            uint16_t bShortFault : 1;
            uint16_t bNeg : 1;
            uint16_t bSmpErr;
            uint16_t bChgMosErr;
            uint16_t bDisMosErr;
            uint16_t bChipSmpErr;
        } BitName;
    } Prp_State;
} PACK_DataTypeDef;

typedef struct {
    uint16_t Short_Count;     //短路次数
    uint16_t Over_Temp_Count; //过温
    uint16_t Over_Curr_Count; //过流次数
    uint16_t Over_Dchg_Count; //过压
    uint16_t Over_Chg_Count;  //欠压

} Record_DataTypeDef;

/*外部声明变量*/
extern volatile uint8_t Sleep_Active;
extern PACK_DataTypeDef PackData;
extern Record_DataTypeDef Record;
extern char BMS_Version[];
extern uint16_t Soc;
extern uint16_t SOH;

/*函数说明*/
void PackData_Init(void);
void Sleep_Ctrl(void);
extern void Wake_Up(void);

#endif
