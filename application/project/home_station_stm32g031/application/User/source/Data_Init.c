//======================================================================
//FileName: Data_Init.c
//
//Author:
//
//Version: 1.0.0
//
//Date:
//
//Description:
//
//======================================================================
#include "Data_Init.h"

#include "Key_Scan.h"
#include "LCD_12864.h"
#include "Mcu_Init.h"
#include "Show_Ctrl.h"
#include "Uart_Comm.h"
#include "User_Conf.h"
#include "stm32g0xx_hal.h"

//---------------------------全局变量-----------------------------------
volatile uint8_t Sleep_Active = 0;

PACK_DataTypeDef PackData;
Record_DataTypeDef Record;
Record_DataTypeDef Record;

char BMS_Version[40] = {
    ' ', ' ', ' ', 0, 0, 0,
};
uint16_t Soc = 10000;
uint16_t SOH = 10000;
uint8_t getValidData;

//======================================================================
//Function:	PackData_Init()
//Description:
//======================================================================
void PackData_Init(void) {
    uint8_t i;
    PackData.TempNum = 8; //6
    for (i = 0; i < 10; i++) {
        PackData.Temp[i] = 0;
    }
    Soc = 0;
    SOH = 0;

    PackData.Current  = 0;
    PackData.Vsum     = 0;
    PackData.BAT_TEMP = 0;
    PackData.Rm       = 0;

    PackData.Fcc       = 0;
    PackData.Cycle     = 0;
    PackData.DesignCap = 0;

    PackData.Status1 = 0;
    PackData.Status2 = 0;
    PackData.Status5 = 0;

    PackData.Warning1 = 0;
    PackData.Warning2 = 0;

    Record.Short_Count     = 0;
    Record.Over_Temp_Count = 0;
    Record.Over_Curr_Count = 0;
    Record.Over_Dchg_Count = 0;
    Record.Over_Chg_Count  = 0;
}

//======================================================================
//Function:	Sleep_Ready()
//Description:
//======================================================================
void Sleep_Ready(void) {
    //    USART_Cmd(USART1, DISABLE);
    Lcd_Close_Display();
    Delay_ms(100);

    _CS;
    _SCK;
    _SDO;
    //_C86;
    _A0;
    //_PS;
    Delay_ms(10);

    EN_KEY1_INT;
    EN_KEY2_INT;
    EN_KEY3_INT;
    EN_KEY4_INT;
    //EN_KEY5_INT;
    EN_INT;
    Page_Welcome_1();
    Clear_Screen(0x00, 0x00);
}

//======================================================================
//Function:	Wake_Ready()
//Description:  程序人口
//======================================================================
void Wake_Ready(void) {
    IWDG_ReloadCounter();
    DIS_KEY1_INT;
    DIS_KEY2_INT;
    DIS_KEY3_INT;
    DIS_KEY4_INT;
    //DIS_KEY5_INT;

    Lcd12864_Init();
    PackData_Init();
    UartInit();
    Page_Welcome_1();
    New_Page_Status = 0;
    Old_Page_Status = 0;

    //    USART_Cmd(USART1, ENABLE);
    Set_Timer(IDLE_TIMER, 2400); //
    Set_Timer(DATA_SAMPLE_TIMER, 20);
}

void Wake_Up(void) {
    BL_EN; //
    sleep_flag = 0;
    PackData_Init();
    // Page_Welcome();
    Page_Welcome_new();
    New_Page_Status = 0;
    Old_Page_Status = 0;
    Sleep_Active    = 0;
    HAL_NVIC_SystemReset();

    Set_Timer(IDLE_TIMER, 2400); //
}
//======================================================================
//Function:	Sleep_Ctrl()
//Description:  休眠
//======================================================================
void Sleep_Ctrl(void) {
    sleep_flag = 0;
    uart_error_flag = 0;

    if (!Timer[IDLE_TIMER].Flag && !Sleep_Active) return;
    Reset_Timer(IDLE_TIMER);
    Sleep_Active = 1;
    Sleep_Ready();
    Page_Welcome_1(); //休眠后返回到首页
    // Clear_Screen(0, 0);
    BL_EN; //背光控制
}

//======================================================================
//ENDFILE
//======================================================================
