//======================================================================
//======================================================================
//FileName:  Key_Scan.c
//Author:
//Version: 1.0.0
//Date:
//Description:
//======================================================================

#include "Key_Scan.h"

#include "Data_Init.h"
#include "Mcu_Init.h"
#include "User_Conf.h"
#include "LCD_12864.h"

uint8_t Key_Value = 0; //KEY按键值
uint8_t Key_Flag = 0;

/*******************************************************************************
* Function:	Key_Scan()
* Description:  按键扫描
*        BIT0  BIT1  BIT2  BIT3  BIT4
*        MENU  ENTER UP    DOWN  ESC
*******************************************************************************/
void Key_Scan(void) {
    static uint8_t Key_Press_Down = 0;
    static uint32_t key_press_start_time = 0;
    static uint8_t long_press_triggered = 0;
    Key_Value = 0;

    if (Key_Press_Down) {
        // 检查是否发生长按 (5000ms)
        if (long_press_triggered == 0) {
            if (HAL_GetTick() - key_press_start_time >= 5000) {
                Key_Value = Key_Press_Down | KEY_LONG_PRESS_FLAG;
                long_press_triggered = 1;
                Set_Timer(IDLE_TIMER, 2400);
                if (Sleep_Active) {
                    Wake_Up();
                    Key_Flag = 0;
                }
                return;
            }
        }

        //if (KEY_MENU && KEY_ENTER && KEY_UP && KEY_DOWN && KEY_ESC)
        if (KEY_MENU && KEY_ENTER && KEY_DOWN && KEY_ESC) //4个按键全部释放
        {
            if (long_press_triggered) {
                long_press_triggered = 0;
            } else {
                Key_Value = Key_Press_Down;
            }
            Key_Press_Down = 0;
            Set_Timer(IDLE_TIMER, 2400);//无操作计时休眠 28s(1100)
            if (Sleep_Active) {
                Wake_Up();
                Key_Flag = 0;
                if (1 == Key_Value) Key_Value = 0; //MENU按键点亮背光后，清除按键值，防止直接进入菜单
            }
        }
        return;
    }

    //if (KEY_MENU && KEY_ENTER && KEY_UP && KEY_DOWN && KEY_ESC)return;
    if (KEY_MENU && KEY_ENTER && KEY_DOWN && KEY_ESC) return; //4按键
    Delay_ms(10);
    if (!KEY_MENU) {
        Key_Flag = 1;
        Key_Press_Down = BIT0;
        key_press_start_time = HAL_GetTick();
    } else if (!KEY_ENTER) {
        Key_Flag = 1;
        Key_Press_Down = BIT1;
        key_press_start_time = HAL_GetTick();
    }
    /*else if (!KEY_UP)
    {
        Key_Flag = 1;
        Key_Press_Down = BIT2;
    }*/
    else if (!KEY_DOWN) {
        Key_Flag = 1;
        Key_Press_Down = BIT3;
        key_press_start_time = HAL_GetTick();
    } else if (!KEY_ESC) {
        Key_Flag = 1;
        Key_Press_Down = BIT4;
        key_press_start_time = HAL_GetTick();
    } else {
        Key_Press_Down = 0;
    }
}

//======================================================================
//FEMFILE
//======================================================================
