//======================================================================
//======================================================================
//FileName:  User_TempTable.c
//Author:
//Version: 1.0.0
//Date:
//Description:
//======================================================================
#include "User_Timer.h"

#include "stm32g0xx_hal.h"
/*全局变量*/
Timer_DataTypeDef Timer[TIMERTOTAL];

//=====================================================================
//Function:	TIM2_Init()
//Description:	TIM2软件定时器基基准，25mS
//
//=====================================================================
void TIM2_Init(void) {
    //    CLK_PeripheralClockConfig(CLK_Peripheral_TIM2, ENABLE);
    //
    //    TIM2_DeInit();
    //    TIM2_PrescalerConfig(TIM2_Prescaler_64, TIM2_PSCReloadMode_Immediate);
    //
    //    TIM2_CounterModeConfig(TIM2_CounterMode_Up);
    //    TIM2_SetCounter(0);
    //    TIM2_SetAutoreload(3124);
    //
    //    TIM2_ITConfig(TIM2_IT_Update,ENABLE);
    //    TIM2_Cmd(ENABLE);
}

//=====================================================================
//Function:	Timer_IQR()
//Description:
//=====================================================================
uint32_t cnt = 0;
extern uint8_t get_protocol_time_flag;
void Timer_IQR(void) {
    uint8_t i;
    //  if(cnt++>20)
    if (cnt++ > 40) {
        //TODO flag= 1
        get_protocol_time_flag = 1;
        cnt                    = 0;
    }
    for (i = 0; i < TIMERTOTAL; i++) {
        if (Timer[i].Enable) {
            if (Timer[i].Count > 1) {
                Timer[i].Count--;
            } else {
                Timer[i].Enable = 0;
                Timer[i].Flag   = 1;
            }
        }
    }
}

//======================================================================
//Function:	  Set_Timer()
//Description:	启动软件定时器
//======================================================================

void Set_Timer(uint8_t Index, uint16_t Count) {
    Timer[Index].Flag   = 0;
    Timer[Index].Count  = Count;
    Timer[Index].Enable = 1;
}

//======================================================================
//Function:		Reset_Timer()
//Description:	复位软件定时器
//======================================================================
void Reset_Timer(uint8_t Index) {
    Timer[Index].Enable = 0;
    Timer[Index].Flag   = 0;
}

//=====================================================================
//Function:   Timer_Init()
//Description:	初始化软件定时器
//=====================================================================

void Timer_Init(void) {
    Set_Timer(IDLE_TIMER, 2400); //
    Set_Timer(DATA_SHOW_TIMER, 1);
    Set_Timer(DATA_SAMPLE_TIMER, 40);
    Set_Timer(DATA_SAMPLE_TIMER_2, 50);
    Set_Timer(DATA_SAMPLE_TIMER_3, 60);
}

//====================================================================
//ENDFILE
//====================================================================
