/*******************************************************************************
********************************************************************************
* FileName: Show_Ctrl.c
* Author:
* Version:  1.00
* Date:
* Description: Progress all of the LCD screens
*******************************************************************************/
#include "Show_Ctrl.h"

#include "Data_Init.h"
#include "Font_Library.h"
#include "Key_Scan.h"
#include "LCD_12864.h"
#include "Uart_Comm.h"
#include "User_Conf.h"
#include "paras.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

//--------------------------全局变量------------------------------------
uint8_t New_Page_Status = 0;     //需要切换的新页面
uint8_t Old_Page_Status = 0;     //当前页面
uint8_t SelectCH_Line_Index = 0; //选择字符所在位置

extern uint8_t getValidData;
uint8_t Time_Buffer[12] = {
    '2',
    '0',
    '9',
    '9',
    '0',
    '0',
    '0',
    '0',
    '0',
    '0',
    '0',
    '0',
};
int16_t GetHighTemp();
uint16_t GetHighBatVol();
uint16_t GetLowBatVol();
/*******************************************************************************
 * Function:	Show_SelectCh()
 * Description: 显示选择项，最前面两个字符显示，选中则显示"->";
 *                                                未选中则显示"--".
 *******************************************************************************/
void Show_SelectCh(uint8_t Line_Num) {
    Lcd_showStringEN(0, 0 * 8, "--", 0);
    Lcd_showStringEN(2, 0 * 8, "--", 0);
    Lcd_showStringEN(4, 0 * 8, "--", 0);
    Lcd_showStringEN(6, 0 * 8, "--", 0);
    switch (Line_Num) {
        case 0: Lcd_showStringEN(0, 0 * 8, "->", 1); break;

        case 1: Lcd_showStringEN(2, 0 * 8, "->", 1); break;

        case 2: Lcd_showStringEN(4, 0 * 8, "->", 1); break;

        case 3: Lcd_showStringEN(6, 0 * 8, "->", 1); break;

        default: break;
    }

    if (Old_Page_Status == PAGE_SYSTEMSET) {
        Lcd_showStringEN(4, 0 * 8, "  ", 0);
        Lcd_showStringEN(6, 0 * 8, "  ", 0);
    }
    if (Old_Page_Status == PAGE_PARASET) {
        Lcd_showStringEN(4, 0 * 8, "  ", 0);
        Lcd_showStringEN(6, 0 * 8, "  ", 0);
    }
}

//======================================================================
// Function:	Page_Welcome_1()
// Description:
//======================================================================
void Page_Welcome_1(void) {
//    Page_Welcome();
    Page_Welcome_new();
    New_Page_Status = PAGE_WELCOME;
}
//======================================================================
// Function:	Page_Welcome()
// Description: 首页面
//======================================================================
void Show_TemperatureNull(uint8_t Row) { Lcd_showStringEN(Row, 9 * 8, "  -- ", 0); }

#define enlen_1 (0)
void Page_Welcome_new(void) {

    //第一种大图
# if enlen_1 == 1
//电池
//    Lcd_showStringENG(2, 2 * 6, "  ", 0);
    Lcd_showStringENG(2, 1 * 8, "1", 0);//电池—+
    Lcd_showStringENG(2, 2 * 7, "2", 0);//电池—+

    Lcd_showStringENG(2, 4 * 7, "1", 0);
    Lcd_showStringENG(2, 5 * 7, "2", 0);
//    Lcd_showStringENG(2, 2 * 8, "2", 0);
//    Lcd_showChar(3, 8 * 8, '#', 0); //show "电芯温度"
    Lcd_showChar(3, 0 * 8, '~', 0);//空白8*16右半边
    Lcd_showChar(3, 1 * 8, '}', 0);//空白8*16
    Lcd_showChar(3, 2 * 8, '}', 0);//空白8*16
    Lcd_showChar(3, 3 * 8, '}', 0);//空白8*16
    Lcd_showChar(3, 4 * 8, '}', 0);//空白8*16
    Lcd_showChar(3, 5 * 8, '}', 0);//空白8*16

    Lcd_showChar(4, 0 * 8, '~', 0);//空白8*16
    Lcd_showChar(4, 1 * 8, '}', 0);//空白8*16右半边
    Lcd_showChar(4, 2 * 8, '}', 0);//空白8*16
    Lcd_showChar(4, 3 * 8, '}', 0);//空白8*16
    Lcd_showChar(4, 4 * 8, '}', 0);//空白8*16
    Lcd_showChar(4, 5 * 8, '}', 0);//空白8*16



//逆变器上
    Lcd_showChar(3, 15 * 8, '[', 0);//逆变器右边
//    Lcd_showChar(3, 10 * 8, '}', 0);//空白8*16
    Lcd_showChar(3, 11 * 8, ';', 1);//空白8*16
    Lcd_showChar(3, 12 * 8, ';', 1);//空白8*16
    Lcd_showChar(3, 13 * 8, ';', 1);//空白8*16
    Lcd_showChar(3, 14 * 8, ';', 1);//空白8*16

    //下
    Lcd_showChar(4, 15 * 8, '{', 0);//空白8*16半边
    Lcd_showChar(4, 11 * 8, ':', 1);//空白8*16右半边
    Lcd_showChar(4, 12 * 8, ':', 1);//空白8*16
    Lcd_showChar(4, 13 * 8, '?', 1);//逆变器框框
    Lcd_showChar(4, 14 * 8, '@', 1);//逆变器框框
//    Lcd_showChar(4, 5 * 8, '}', 0);//空白8*16

    Lcd_showStringENG(2, 14 * 6, "5", 0);
    Lcd_showStringENG(2, 15 * 6, "3", 0);
    Lcd_showStringENG(2, 16 * 6, "3", 0);
    Lcd_showStringENG(2, 17 * 6, "3", 0);
    Lcd_showStringENG(2, 18 * 6, "3", 0);
    Lcd_showStringENG(2, 19 * 6, "3", 0);
    Lcd_showStringENG(2, 20 * 6, "4", 0);

    Lcd_showStringENG(3, 8 * 6, ":", 0);//箭头线
    Lcd_showStringENG(3, 9 * 6, ":", 0);//箭头线
    Lcd_showStringENG(3, 10 * 6, ":", 0);//箭头线
    Lcd_showStringENG(3, 11 * 6, ":", 0);//箭头线
    Lcd_showStringENG(3, 12 * 6, ";", 0);//箭头
    Lcd_showStringENG(3, 13 * 6, ":", 0);//箭头线
 Show_Current1(4, 4 * 8, PackData.Current); // 72

#else
    Lcd_showStringENG(2, 1 * 3, "(", 0);//电池—+左
    Lcd_showStringENG(2, 2 * 4, ")", 0);//电池—+

    Lcd_showStringENG(2, 26 * 1, "(", 0);
    Lcd_showStringENG(2, 8 * 4, ")", 0);
//    Lcd_showStringENG(2, 2 * 8, "2", 0);
//    Lcd_showChar(3, 8 * 8, '#', 0); //show "电芯温度"
    Lcd_showChar(3, 0 * 8, '(', 0);//空白8*16
    Lcd_showChar(3, 1 * 8, ')', 0);//空白8*16
    Lcd_showChar(3, 2 * 8, '}', 0);//空白8*16
    Lcd_showChar(3, 3 * 8, '+', 0);//空白8*16
    Lcd_showChar(3, 4 * 8, '*', 0);//空白8*16右半边
//    Lcd_showChar(3, 5 * 8, '}', 0);//空白8*16

    Lcd_showChar(4, 0 * 8, '<', 0);//空白8*16右半边
    Lcd_showChar(4, 1 * 8, '<', 0);//空白8*16
    Lcd_showChar(4, 2 * 8, '<', 0);//空白8*16
    Lcd_showChar(4, 3 * 8, '<', 0);//空白8*16
    Lcd_showChar(4, 4 * 8, '<', 0);//空白8*16
//    Lcd_showChar(4, 5 * 8, '}', 0);//空白8*16

//逆变器上
//    Lcd_showChar(3, 10 * 8, '}', 0);//空白8*16
//    Lcd_showChar(3, 11 * 8, ';', 1);//空白8*16
    Lcd_showChar(3, 12 * 8, '!', 0);//空白8*16
    Lcd_showChar(3, 13 * 8, ';', 1);//空白8*16
    Lcd_showChar(3, 14 * 8, ';', 1);//空白8*16
    Lcd_showChar(3, 15 * 8, '[', 0);//逆变器右边

    //下

//    Lcd_showChar(4, 11 * 8, ':', 1);//空白8*16右半边
    Lcd_showChar(4, 12 * 8, '"', 0);//空白8*16
    Lcd_showChar(4, 13 * 8, ':', 1);//空白8*16
    Lcd_showChar(4, 14 * 8, '?', 1);//逆变器框框
    Lcd_showChar(4, 15 * 8, '@', 1);//逆变器框框
//    Lcd_showChar(4, 15 * 8, '{', 0);//空白8*16半边



    Lcd_showStringENG(2, 16 * 6, "0", 0);
    Lcd_showStringENG(2, 17 * 6, "3", 0);
    Lcd_showStringENG(2, 18 * 6, "3", 0);
    Lcd_showStringENG(2, 19 * 6, "3", 0);
    Lcd_showStringENG(2, 20 * 6, "3", 0);
    Lcd_showStringENG(2, 21 * 6, "3", 0);


    if(PackData.Current == 0){
        Lcd_showStringENG(3, 11 * 6, ":", 0);//箭头线
        Lcd_showChar(4, 5*8, ' ', 0);
        Lcd_showChar(4, 6*8, ' ', 0);
        Lcd_showChar(4, 7*8, ' ', 0);
        Lcd_showChar(4, 8*8, '-', 0);
        Lcd_showChar(4, 9*8, '-', 0);
        Lcd_showChar(4, 10*8, '-', 0);

    } else if ((PackData.Current > 0)){//300A
        Show_Current1(4, 5 * 8, PackData.Current); // 72
        Lcd_showStringENG(3, 11 * 6, "<", 0);//充电箭头
    } else if (PackData.Current < 0){
        Show_Current1(4, 5 * 8, PackData.Current); // 72
        Lcd_showStringENG(3, 11 * 6, ";", 0);//放电箭头
    }
    Lcd_showChar(4, 11 * 8, 'A', 0); //show "A"
    Lcd_showStringENG(3, 7 * 6, ":", 0);//箭头线
    Lcd_showStringENG(3, 8 * 6, ":", 0);//箭头线
    Lcd_showStringENG(3, 9 * 6, ":", 0);//箭头线
    Lcd_showStringENG(3, 10 * 6, ":", 0);//箭头线
    Lcd_showStringENG(3, 12 * 6, ":", 0);//
    Lcd_showStringENG(3, 13 * 6, ":", 0);//箭头线
    Lcd_showStringENG(3, 14 * 6, ":", 0);//箭头线
    Lcd_showStringENG(3, 15 * 6, ":", 0);//箭头线

#endif

//#if 0
//总压
    Show_TotalVoltage1(0, 0 * 8, (PackData.Vsum));
    Lcd_showChar(0, 4 * 8, 'V', 0); //show "A"


    //SOC 最后一行前面
    Lcd_showChar(0, 15 * 8, '%', 0);

    if (((Soc / 100) % 10) == 0)
        Lcd_showChar6_8(0, 12 * 8, ' ', 0); // SOC
    else
        Lcd_showChar(0, 12 * 8, (uint8_t)((Soc / 100) % 10) + '0', 0);

    if ((((Soc / 10) % 10) == 0) && (((Soc / 100) % 10) == 0))
        Lcd_showChar6_8(0, 13 * 8, ' ', 0);
    else
    Lcd_showChar(0, 13 * 8, (uint8_t)((Soc / 10) % 10) + '0', 0);
    Lcd_showChar(0, 14 * 8, (uint8_t)(Soc % 10) + '0', 0);


    Lcd_showChar(6, 3 * 8, 'A', 0); //show "A"
    Lcd_showChar(6, 4 * 8, 'H', 0); //show "A"
    if (PackData.Rm >= 10000) {     // 剩余容量
    Lcd_showChar(6, 0 * 8, (uint8_t)(PackData.Rm / 10000) + '0', 0);
    Lcd_showChar(6, 1 * 8, (uint8_t)((PackData.Rm / 1000) % 10) + '0', 0);
    //        Lcd_showChar(6, 2 * 8, (uint8_t)((PackData.Fcc / 100) % 10) + '0',
    //        0);
    } else if (PackData.Rm >= 1000) {
    Lcd_showChar(6, 0 * 8, ' ', 0);
    Lcd_showChar(6, 1 * 8, (uint8_t)(PackData.Rm / 1000) + '0', 0);
    //        Lcd_showChar(6, 2 * 8, (uint8_t)((PackData.Fcc / 10) % 10) + '0',
    //        0);
    } else {
    Lcd_showChar(6, 0 * 8, ' ', 0);
    Lcd_showChar(6, 1 * 8, ' ', 0);
    }
    Lcd_showChar(6, 2 * 8, (uint8_t)((PackData.Rm / 100) % 10) + '0', 0);

    //保护状态
    if (PackData.Prp_State.BitName.bCellVoltOV){
        Lcd_showChar(6, 13 * 8, 'O', 0);
        Lcd_showChar(6, 14 * 8, 'V', 0);
    } else if (PackData.Prp_State.BitName.bCellVoltUL){
        Lcd_showChar(6, 13 * 8, 'L', 0);
        Lcd_showChar(6, 14 * 8, 'V', 0);
    }else if (PackData.Prp_State.BitName.bTempOV){
        Lcd_showChar(6, 13 * 8, 'O', 0);
        Lcd_showChar(6, 14 * 8, 'T', 0);
    }else if (PackData.Prp_State.BitName.bTempUL){
        Lcd_showChar(6, 13 * 8, 'L', 0);
        Lcd_showChar(6, 14 * 8, 'T', 0);
    }else if (PackData.Prp_State.BitName.bCurrOV){
        Lcd_showChar(6, 13 * 8, 'O', 0);
        Lcd_showChar(6, 14 * 8, 'C', 0);
    }
    else if (PackData.Prp_State.BitName.bShortFault){
        Lcd_showChar(6, 13 * 8, 'S', 0);
        Lcd_showChar(6, 14 * 8, 'C', 0);
    }else {
        Lcd_showChar(6, 13 * 8, '-', 0);
        Lcd_showChar(6, 14 * 8, '-', 0);
    }
}

void Page_Welcome(void) {
    //  OLED_ShowBMP(0, 0, 128, 64, 0, 0); //图片

    /*首页6*7日期*/
    Lcd_showCha6_7(0, 0 * 6, Time_Buffer[0], 1);
    Lcd_showCha6_7(0, 1 * 6, Time_Buffer[1], 1);
    Lcd_showCha6_7(0, 2 * 6, Time_Buffer[2], 1);
    Lcd_showCha6_7(0, 3 * 6, Time_Buffer[3], 1);
    Lcd_showCha6_7(0, 4 * 6, '+', 0); //:
    Lcd_showCha6_7(0, 5 * 6, Time_Buffer[4], 1);
    Lcd_showCha6_7(0, 6 * 6, Time_Buffer[5], 1);
    Lcd_showCha6_7(0, 7 * 6, '+', 0); //:
    Lcd_showCha6_7(0, 8 * 6, Time_Buffer[6], 1);
    Lcd_showCha6_7(0, 9 * 6, Time_Buffer[7], 1);

    Lcd_showStringENG(0, 10 * 6, "   ", 0);
    Lcd_showStringENG(0, 13 * 6, "SOH", 0);
    Lcd_showCha6_7(0, 16 * 6, '#', 0); //:

    /*首页6*7单体最高最低电压*/
    Lcd_showStringENG(7, 0 * 6, "ZAX", 0);
    Lcd_showCha6_7(7, 3 * 6, '#', 0); //:
    Lcd_showStringENG(7, 8 * 6, "MV", 0);
    Lcd_showStringENG(7, 10 * 6, " ", 0);
    Lcd_showStringENG(7, 11 * 6, "ZIN", 0);
    Lcd_showCha6_7(7, 14 * 6, '#', 0); //:
    Lcd_showStringENG(7, 0 * 6, "", 0);
    Lcd_showStringENG(7, 18 * 6, "", 0);
    Lcd_showStringENG(7, 19 * 6, "MV", 0);
    Show_Data_4Bit1(7, 4 * 6, GetHighBatVol());
    Show_Data_4Bit1(7, 15 * 6, GetLowBatVol());

    /*SOH*/
    if (((SOH / 100) % 10) == 0) Lcd_showCha6_7(0, 17 * 6, ' ', 0);
    else
        Lcd_showCha6_7(0, 17 * 6, (uint8_t)((SOH / 100) % 10) + '0', 1);

    if ((((SOH / 10) % 10) == 0) && (((SOH / 100) % 10) == 0)) Lcd_showCha6_7(0, 18 * 6, ' ', 0);
    else
    Lcd_showCha6_7(0, 18 * 6, (uint8_t)((SOH / 10) % 10) + '0', 1);
    Lcd_showCha6_7(0, 19 * 6, (uint8_t)(SOH % 10) + '0', 1);
    Lcd_showStringENG(0, 20 * 6, " ", 0); //反白

    /*首页图片*/
    Lcd_showChar(3, 8 * 8, '#', 0); //show "电芯温度"
    Lcd_showChar(5, 8 * 8, '!', 0); //show SOC图片
    Lcd_showChar(5, 9 * 8, '&', 0); //show "SOC图片"
    Lcd_showChar(1, 8 * 8, '+', 0); //show "环境温度图片"

    /*首页总压*/
    Lcd_showChar(3, 8 * 6, 'V', 0); //show "V"

    if(PackData.Vsum==0){
        PackData.Vsum=0;
    }

    Show_TotalVoltage1(3, 0 * 8, (PackData.Vsum));

    /*充放电电流*/
    Lcd_showChar(1, 8 * 6, 'A', 0); //show "A"

    Show_Current1(1, 0 * 8, (PackData.Current));

    /*剩余容量*/
    Lcd_showChar(5, 5 * 8, 'A', 0); //show "A"
    Lcd_showChar(5, 6 * 8, 'H', 0); //show "H"

    /*电芯温度*/
    Show_Temperature1(3, 9 * 8, GetHighTemp());
    Lcd_showChar6_16(3, 13 * 8, '#', 0); //摄氏度

    /*环境温度*/
    Show_Temperature1(1, 9 * 8, PackData.s16EnvirTemp);
    Lcd_showChar6_16(1, 13 * 8, '#', 0); //摄氏度
}

//======================================================================
// Function:	Page_Menu()
// Description:菜单页面
//======================================================================
void Page_Menu(void) {
    Show_SelectCh(SelectCH_Line_Index);
    Lcd_showStringEN(0, 2 * 8, "BMS Parameter   ", 0);
    Lcd_showChar6_8(0, 15 * 8, '<', 0); // show "》"

    Lcd_showStringEN(2, 2 * 8, "Battery Status  ", 0);
    Lcd_showChar6_8(2, 15 * 8, '<', 0); // show "》"

    Lcd_showStringEN(4, 2 * 8, "GYRO Status     ", 0);
    Lcd_showChar6_8(4, 15 * 8, '<', 0); // show "》"

    Lcd_showStringEN(6, 2 * 8, "Version Number  ", 0);
    Lcd_showChar6_8(6, 15 * 8, '<', 0); // show "》"
}

//======================================================================
// Function:	Page_Analog()
// Description:
//======================================================================
void Page_Analog_1(void) {
    Show_SelectCh(SelectCH_Line_Index);

    Lcd_showStringEN(0, 2 * 8, "Voltage:         V", 0);
    Lcd_showStringEN(2, 2 * 8, "Current:         A", 0);
    Lcd_showStringEN(4, 2 * 8, "CellTemp      ", 0);
    Lcd_showStringEN(6, 2 * 8, "CellVolt      ", 0);
    Lcd_showChar6_8(4, 11 * 8, '<', 0); // show "》"
    Lcd_showChar6_8(6, 11 * 8, '<', 0); // show "》"
}

//======================================================================
// Function:	Page_Status()
// Description:
//======================================================================
void Page_Status(void) {
    Show_SelectCh(SelectCH_Line_Index);

    Lcd_showStringEN(0, 2 * 8, "Status:       ", 0);
    Lcd_showStringEN(2, 2 * 8, "Alarm Status ", 0);
    Lcd_showStringEN(4, 2 * 8, "Protect Status", 0);
    Lcd_showStringEN(6, 2 * 8, "Failure Alarm", 0);

    Lcd_showChar6_8(2, 15 * 8, '<', 0); // show "》"
    Lcd_showChar6_8(4, 15 * 8, '<', 0); // show "》"
    Lcd_showChar6_8(6, 15 * 8, '<', 0); // show "》"
}

/*******************************************************************************
 * Function:	Page_ParaSet()
 * Description:
 *陀螺仪设置（选配）默认关闭
 *
 *******************************************************************************/
void Page_ParaSet(void) {
    Show_SelectCh(SelectCH_Line_Index);

    Lcd_showStringEN(0, 2 * 8, "Set X axis:   ", 0);
    Lcd_showStringEN(2, 2 * 8, "Place Option: ", 0);
}

//======================================================================
// Function:	Page_SystemSet()
// Description:
//======================================================================
void Page_SystemSet(void) {
    Show_SelectCh(SelectCH_Line_Index);

    Lcd_showStringEN(0, 2 * 8, "BMS Version  ", 0);
    Lcd_showStringEN(2, 2 * 8, "LCD Version  ", 0);
    Lcd_showChar6_8(0, 15 * 8, '<', 0); // show "��"
    Lcd_showChar6_8(2, 15 * 8, '<', 0); // show "��"
}

//======================================================================
// Function:	Page_Temperature_1()
// Description:
//======================================================================
void Page_Temperature_1(void) {
    Lcd_showStringEN(0, 0 * 6, "--Temp01           C", 0);
    Lcd_showStringEN(2, 0 * 6, "--Temp02           C", 0);
    Lcd_showStringEN(4, 0 * 6, "--Temp03           C", 0);
    Lcd_showStringEN(6, 0 * 6, "--Temp04           C", 0);
    Lcd_showChar6_8(0, 18 * 6, '_', 0); // show "摄氏度"
    Lcd_showChar6_8(2, 18 * 6, '_', 0);
    Lcd_showChar6_8(4, 18 * 6, '_', 0);
    Lcd_showChar6_8(6, 18 * 6, '_', 0);
}
//======================================================================
// Function:	Page_Temperature_2()
// Description: 用8路NTC时可打开
//======================================================================
void Page_Temperature_2(void) {
    Lcd_showStringEN(0, 0 * 6, "--Temp05:      C", 0);
    Lcd_showStringEN(2, 0 * 6, "--Temp06:      C", 0);
    Lcd_showStringEN(4, 0 * 6, "--Temp07:      C", 0);
    Lcd_showStringEN(6, 0 * 6, "--Temp08:      C", 0);
    Lcd_showChar6_8(0, 18 * 6, '_', 0); // show "��C"
    Lcd_showChar6_8(2, 18 * 6, '_', 0); // show "��C"
    Lcd_showChar6_8(4, 18 * 6, '_', 0); // show "��C"
    Lcd_showChar6_8(6, 18 * 6, '_', 0); // show "��C"
}
//======================================================================
// Function:	Page_Temperature_3()
// Description:
//======================================================================
void Page_Temperature_3(void) {
    Lcd_showStringEN(0, 0 * 6, "MOS Temp           C", 0); // PCB--> MOS
    Lcd_showStringEN(2, 0 * 6, "Env Temp           C", 0);
    Lcd_showChar6_8(0, 18 * 6, '_', 0); // show "��C"
    Lcd_showChar6_8(2, 18 * 6, '_', 0); // show "��C"
}

//======================================================================
// Function:	Page_Voltage_1()
// Description:
//======================================================================
void Page_Voltage_1(void) {
    Lcd_showStringEN(0, 0 * 8, "--Cell01          mV", 0);
    Lcd_showStringEN(2, 0 * 8, "--Cell02          mV", 0);
    Lcd_showStringEN(4, 0 * 8, "--Cell03          mV", 0);
    Lcd_showStringEN(6, 0 * 8, "--Cell04          mV", 0);
}

//======================================================================
// Function:	Page_Voltage_2()
// Description:
//======================================================================
void Page_Voltage_2(void) {
    Lcd_showStringEN(0, 0 * 8, "--Cell05          mV", 0);
    Lcd_showStringEN(2, 0 * 8, "--Cell06          mV", 0);
    Lcd_showStringEN(4, 0 * 8, "--Cell07          mV", 0);
    Lcd_showStringEN(6, 0 * 8, "--Cell08          mV", 0);
}

//======================================================================
// Function:	Page_Voltage_3()
// Description:
//======================================================================
void Page_Voltage_3(void) {
    Lcd_showStringEN(0, 0 * 8, "--Cell09          mV", 0);
    Lcd_showStringEN(2, 0 * 8, "--Cell10          mV", 0);
    Lcd_showStringEN(4, 0 * 8, "--Cell11          mV", 0);
    Lcd_showStringEN(6, 0 * 8, "--Cell12          mV", 0);
}

//======================================================================
// Function:	Page_Voltage_4()
// Description:
//======================================================================
void Page_Voltage_4(void) {
    Lcd_showStringEN(0, 0 * 8, "--Cell13          mV", 0);
    Lcd_showStringEN(2, 0 * 8, "--Cell14          mV", 0);
    Lcd_showStringEN(4, 0 * 8, "--Cell15          mV", 0);
    Lcd_showStringEN(6, 0 * 8, "--Cell16          mV", 0);
}

//======================================================================
// Function:	Page_Capacity()
// Description:
//======================================================================
void Page_Capacity(void) {
    Lcd_showStringEN(0, 0 * 8, "   SOC    :        %", 0);
    Lcd_showStringEN(2, 0 * 8, "Nominal   :        AH", 0);
    Lcd_showStringEN(4, 0 * 8, "Remain    :        AH", 0);
    Lcd_showStringEN(6, 0 * 8, "BMS Cycles:         0", 0);
}

//======================================================================
// Function:	Page_Warning_1()
// Description:
//======================================================================
void Page_Warning_1(void) {
    Lcd_showStringEN(0, 0 * 8, "--Over Volt :   ", 0);
    Lcd_showStringEN(2, 0 * 8, "--Low Volt  :   ", 0);
    Lcd_showStringEN(4, 0 * 8, "--Over Temp :   ", 0);
    Lcd_showStringEN(6, 0 * 8, "--Low Temp  :   ", 0);
}

//======================================================================
// Function:	Page_Warning_2()
// Description:
//======================================================================
void Page_Warning_2(void) {
    Lcd_showStringEN(0, 0 * 8, "--Low SOC   :   ", 0);
    Lcd_showStringEN(2, 0 * 8, "--Diff Volt :   ", 0);
    Lcd_showStringEN(4, 0 * 8, "--Over Curr :   ", 0);
    Lcd_showStringEN(6, 0 * 8, "--Reverse   :   ", 0);
}

//======================================================================
// Function:	Page_Protection_1()
// Description:
//======================================================================
void Page_Protection_1(void) {
    Lcd_showStringEN(0, 0 * 8, "--Over Volt :   ", 0);
    Lcd_showStringEN(2, 0 * 8, "--Low Volt  :   ", 0);
    Lcd_showStringEN(4, 0 * 8, "--Over Temp :   ", 0);
    Lcd_showStringEN(6, 0 * 8, "--Low Temp  :   ", 0);
}

//======================================================================
// Function:	Page_Protection_2()
// Description:
//======================================================================
void Page_Protection_2(void) {
    Lcd_showStringEN(0, 0 * 8, "--Over Curr :   ", 0);
    Lcd_showStringEN(2, 0 * 8, "--Short Curr:   ", 0);
}

/*******************************************************************************
 * Function:	Page_FailAlarm_1()
 * Description: show the Fail Alarm info in 3 pages
 *******************************************************************************/
void Page_FailAlarm_1(void) {
    Lcd_showStringEN(0, 0 * 8, "--Sample Line:  ", 0);
    Lcd_showStringEN(2, 0 * 8, "--Charge MOS :  ", 0);
    Lcd_showStringEN(4, 0 * 8, "--DisCHG MOS :  ", 0);
    Lcd_showStringEN(6, 0 * 8, "--Sample Chip:  ", 0);
}

/*******************************************************************************
 * Function:	Page_FailAlarm_2()
 * Description: show the Fail Alarm info in 3 pages
 *******************************************************************************/
void Page_FailAlarm_2(void) {
    Lcd_showStringEN(0, 0 * 8, "SC Times   :    ", 0);
    Lcd_showStringEN(2, 0 * 8, "OverTempCNT:    ", 0);
    Lcd_showStringEN(4, 0 * 8, "OverCurrCNT:    ", 0);
    Lcd_showStringEN(6, 0 * 8, "OverChgCNT :    ", 0);
}

/*******************************************************************************
 * Function:	Page_FailAlarm_3()
 * Description: show the Fail Alarm info in 3 pages
 *******************************************************************************/
void Page_FailAlarm_3(void) { Lcd_showStringEN(0, 0 * 8, "OverDchgCNT:    ", 0); }

/*******************************************************************************
 * Function:	Page_BMS_Version()
 * Description: BMS软件版本号
 *******************************************************************************/
void Page_BMS_Version(void) {
    uint8_t i, j;
    uint8_t _buf[10] = {0};
    Lcd_showStringEN(0, 0 * 8, "BMS SW Version:  ", 0);
    Lcd_showStringEN(4, 0 * 8, "BMS HW Version: ", 0);

    memset(_buf, 0, 10);
    _buf[0] = g_sm_info.Software_Ver[0] + 0x30;
    _buf[1] = '.';
    _buf[2] = g_sm_info.Software_Ver[1] + 0x30;
    _buf[3] = '.';
    _buf[4] = g_sm_info.Software_Ver[2] + 0x30;
    Lcd_showStringEN(2, 0 * 8, _buf, 0);

    memset(_buf, 0, 10);
    memcpy(_buf, g_sm_info.Hardware_type, 10);
    Lcd_showStringEN(2, 4 * 8, _buf, 0);


    memset(_buf, 0, 10);
    memcpy(_buf, g_sm_info.Product_code, 10);
    Lcd_showStringEN(6, 0 * 8, _buf, 0);

    memset(_buf, 0, 10);
    memcpy(_buf, g_sm_info.Project_code, 10);
    Lcd_showStringEN(6, 10 * 8, _buf, 0);



    j = 2;
    while ('-' != BMS_Version[j]) {
    j++;
    if (j > 29) //规定长度
        return;
    }
    j++;
    for (i = 0; i < 8; i++) {
    if ('^' == BMS_Version[i + j]) break;
    Lcd_showChar6_8(2, (i + 9) * 8, BMS_Version[i + j], 0);
    }
}

/*******************************************************************************
 * Function:	Page_LCD_Version()
 * Description: show the LCD software and hardware information
 * KS-V2.0.2:满充容量
 * KS-V2.0.4:剩余容量
 * KS-V2.1.1:更改首页电流显示三位后面不清空问题
 *******************************************************************************/
void Page_LCD_Version(void) {
    Lcd_showStringEN(0, 0 * 8, "LCD SW Version: ", 0);//主板协议回复不在判断cid1
    Lcd_showStringEN(2, 0 * 8, " KS-V2.1.1", 0);//更改首界面满充容量为剩余容量
    Lcd_showStringEN(4, 0 * 8, "LCD HW Version: ", 0);
    Lcd_showStringEN(6, 0 * 8, " KS-LCD-02-V1", 0);
}

/*******************************************************************************
 * Function:	GyroSet()
 * Description: Gyro setting process 设置过程
 *******************************************************************************/
void GyroSet(void) {
    static uint8_t ParaSetFlag = 0;
    static uint8_t PlaceFlag = 0;
    static uint8_t X_axisFlag = 0;

    if (0 == ParaSetFlag) //菜单切换
    {
        if ((Key_Value & BIT2)) {
            if (SelectCH_Line_Index == 0) { SelectCH_Line_Index = 2; }
            SelectCH_Line_Index--;
            Show_SelectCh(SelectCH_Line_Index);
        } else if ((Key_Value & BIT3)) {
            if (++SelectCH_Line_Index >= 2) { SelectCH_Line_Index = 0; }
            Show_SelectCh(SelectCH_Line_Index);
        }
    }
    /*陀螺仪设置说明*/
    /*else if(1 == ParaSetFlag)       //陀螺仪上下按键设置方向
          {
              if((Key_Value & BIT2) || (Key_Value & BIT3))
              {
                  if(0 == PlaceFlag)
                  {
                      PlaceFlag = 1;
                      Lcd_showStringEN(4, 6 * 8, "Horizontal", 1);
                  }
                  else if(1 == PlaceFlag)
                  {
                      PlaceFlag = 0;
                      Lcd_showStringEN(4, 6 * 8, "Vertical  ", 1);
                  }
              }
          }
          else if(2 == ParaSetFlag)       //陀螺仪上下按键设置X,Y
          {
              if((Key_Value & BIT2) || (Key_Value & BIT3))
              {
                  if(0 == X_axisFlag)
                  {
                      X_axisFlag = 1;
                      Lcd_showStringEN(0, 13*8, "180", 1);
                  }
                  else if(1 == X_axisFlag)
                  {
                      X_axisFlag = 0;
                      Lcd_showStringEN(0, 13*8, "  0", 1);
                  }
              }
          }*/

    if ((Key_Value & BIT4)) {
        if (ParaSetFlag) {
            ParaSetFlag = 0; //返回页面初始值
        } else {
            New_Page_Status = PAGE_MENU; //回主菜单
            SelectCH_Line_Index = 2;
        }
    }
    /*陀螺仪设置说明*/
    /*else if((Key_Value & BIT1))
              {

                  ParaSetFlag = 0;


                  if(0 == SelectCH_Line_Index)
                  {
                      if(0 == ParaSetFlag)
                      {
                          ParaSetFlag = 2;
                        //  X_axisFlag = 2;
                          X_axisFlag = X_axisFlag_COM;
                      }
                      else if(2 == ParaSetFlag)
                      {
                          SetCommondFlag = 1;   //Uart_SetParaCMD_1(X_axisFlag);  //发送X轴指令
                          ParaSetFlag = 0;
                          X_axisFlag_COM = X_axisFlag;
                      }
                      if(1 == X_axisFlag)
                      {
                          Lcd_showStringEN(0, 13*8, "180", 0);
                      }
                      else if(0 == X_axisFlag)
                      {
                          Lcd_showStringEN(0, 13*8, "  0", 0);
                      }
                  }

                  if(1 == SelectCH_Line_Index)
                  {
                      if(0 == ParaSetFlag)
                      {
                          ParaSetFlag = 1;
                       //   PlaceFlag = 2;
                          PlaceFlag = PlaceFlag_COM;
                      }
                      else if(1 == ParaSetFlag)
                      {
                          SetCommondFlag = 2;   //Uart_SetParaCMD_2(PlaceFlag); //陀螺仪垂直、水平指令
                          ParaSetFlag = 0; PlaceFlag_COM = PlaceFlag;
                      }
                      if(1 == PlaceFlag)
                      {
                          Lcd_showStringEN(4, 6 * 8, "Horizontal", 0);
                      }
                      else if(0 == PlaceFlag)
                      {
                          Lcd_showStringEN(4, 6 * 8, "Vertical  ", 0);
                      }
                  }

              }*/
}

//======================================================================
// Function:	Page_Select_Ctrl()
// Description:  页面的按键选择和控制
//======================================================================
void Page_Select_Ctrl(void) {
    if (!Key_Value) return;
    if ((Key_Value & BIT0)) {
        New_Page_Status = PAGE_MENU;
        if (Old_Page_Status != PAGE_MENU) { SelectCH_Line_Index = 0; }
        return;
    }

    switch (Old_Page_Status) {
        case PAGE_MENU:
            if ((Key_Value & BIT1)) {
                if (SelectCH_Line_Index == 0) {
                    New_Page_Status = PAGE_ANALOG_1;
                } else if (SelectCH_Line_Index == 1) {
                    New_Page_Status = PAGE_STATUS;
                } else if (SelectCH_Line_Index == 2) {
                    New_Page_Status = PAGE_PARASET;
                    SetCommondFlag = 3;
                    Uart_GetParaCMD_1(); //读X轴指令
                    Delay_ms(10);
                    Uart_GetParaCMD_2(); //读水平、垂直指令
                    Delay_ms(10);
                    if (1 == X_axisFlag_COM) {
                        Lcd_showStringEN(0, 13 * 8, "180", 0);
                    } else if (0 == X_axisFlag_COM) {
                        Lcd_showStringEN(0, 13 * 8, "  0", 0);
                    }

                    if (1 == PlaceFlag_COM) {
                        Lcd_showStringEN(4, 6 * 8, "Horizontal", 0);
                    } else if (0 == PlaceFlag_COM) {
                        Lcd_showStringEN(4, 6 * 8, "Vertical  ", 0);
                    }
                } else {
                    New_Page_Status = PAGE_SYSTEMSET;
                }

                SelectCH_Line_Index = 0;
            } else if ((Key_Value & BIT2)) {
                if (SelectCH_Line_Index == 0) { SelectCH_Line_Index = 4; }
                SelectCH_Line_Index--;
                Show_SelectCh(SelectCH_Line_Index);
            } else if ((Key_Value & BIT3)) {
                if (++SelectCH_Line_Index >= 4) { SelectCH_Line_Index = 0; }
                Show_SelectCh(SelectCH_Line_Index);
            }

            else if ((Key_Value & BIT4)) //按ESC回到首页面
            {
                Key_Flag = 0;
                New_Page_Status = PAGE_WELCOME;
            }
            break;

        case PAGE_ANALOG_1:
            if ((Key_Value & BIT1)) {
                if (SelectCH_Line_Index == 2) {
                    New_Page_Status = PAGE_TEMPERATURE_1;
                    SelectCH_Line_Index = 0;
                } else if (SelectCH_Line_Index == 3) {
                    New_Page_Status = PAGE_VOLTAGE_1;
                    SelectCH_Line_Index = 0;
                }
            } else if ((Key_Value & BIT2)) {
                if (SelectCH_Line_Index == 0) {
                    SelectCH_Line_Index = 3;
                    Show_SelectCh(SelectCH_Line_Index);
                } else {
                    SelectCH_Line_Index--;
                    Show_SelectCh(SelectCH_Line_Index);
                }
            } else if ((Key_Value & BIT3)) {
                if (SelectCH_Line_Index >= 3) {
                    SelectCH_Line_Index = 0;
                    Show_SelectCh(SelectCH_Line_Index);
                } else {
                    SelectCH_Line_Index++;
                    Show_SelectCh(SelectCH_Line_Index);
                }
            } else if ((Key_Value & BIT4)) {
                New_Page_Status = PAGE_MENU;
                SelectCH_Line_Index = 0;
            }
            break;

        case PAGE_STATUS:
            if ((Key_Value & BIT1)) {
                if (SelectCH_Line_Index == 1) {
                    New_Page_Status = PAGE_WARNING_1;
                    SelectCH_Line_Index = 0;
                } else if (SelectCH_Line_Index == 2) {
                    New_Page_Status = PAGE_PROTECTION_1;
                    SelectCH_Line_Index = 0;
                } else if (SelectCH_Line_Index == 3) {
                    New_Page_Status = PAGE_FAILALARM_1;
                    SelectCH_Line_Index = 0;
                }
            } else if ((Key_Value & BIT2)) {
                if (SelectCH_Line_Index == 0) { SelectCH_Line_Index = 4; }
                SelectCH_Line_Index--;
                Show_SelectCh(SelectCH_Line_Index);
            } else if ((Key_Value & BIT3)) {
                if (++SelectCH_Line_Index >= 4) { SelectCH_Line_Index = 0; }
                Show_SelectCh(SelectCH_Line_Index);
            } else if ((Key_Value & BIT4)) {
                New_Page_Status = PAGE_MENU;
                SelectCH_Line_Index = 1;
            }
            break;
        case PAGE_PARASET:
            GyroSet();
            break; //陀螺仪设置参数

            /*case PAGE_PARASET:
        if((Key_Value & BIT4))
        {
            New_Page_Status = PAGE_MENU;
            SelectCH_Line_Index = 2;
        }
        break;*/

            /*case PAGE_SYSTEMSET:
        if((Key_Value & BIT4))
        {
            New_Page_Status = PAGE_MENU;
            SelectCH_Line_Index = 3;
        }
        break;*/

        case PAGE_TEMPERATURE_1: //温度菜单控制
            if ((Key_Value & BIT2)) {
                New_Page_Status = PAGE_TEMPERATURE_2;
            } else if ((Key_Value & BIT3)) {
                if (PackData.TempNum > 6) {
                    New_Page_Status = PAGE_TEMPERATURE_3;
                } else {
                    New_Page_Status = PAGE_TEMPERATURE_1;
                }
            } else if ((Key_Value & BIT4)) {
                New_Page_Status = PAGE_ANALOG_1;
                SelectCH_Line_Index = 2;
            }
            break;

            //        case PAGE_TEMPERATURE_2:

        case PAGE_TEMPERATURE_3:
            if ((Key_Value & BIT2)) {
                if (PackData.TempNum > 6) {
                    New_Page_Status = PAGE_TEMPERATURE_2;
                } else {
                    New_Page_Status = PAGE_TEMPERATURE_1;
                }
            } else if ((Key_Value & BIT3)) {
                New_Page_Status = PAGE_TEMPERATURE_1;
            } else if ((Key_Value & BIT4)) {
                New_Page_Status = PAGE_ANALOG_1;
                SelectCH_Line_Index = 2;
            }
            break;

        case PAGE_VOLTAGE_1:
            if ((Key_Value & BIT2)) {
                New_Page_Status = PAGE_CAPACITY;
            } else if ((Key_Value & BIT3)) {
                New_Page_Status = PAGE_VOLTAGE_2;
            } else if ((Key_Value & BIT4)) {
                New_Page_Status = PAGE_ANALOG_1;
                SelectCH_Line_Index = 3;
            }
            break;

        case PAGE_VOLTAGE_2:
            if ((Key_Value & BIT2)) {
                New_Page_Status = PAGE_VOLTAGE_1;
            } else if ((Key_Value & BIT3)) {
                New_Page_Status = PAGE_VOLTAGE_3;
            } else if ((Key_Value & BIT4)) {
                New_Page_Status = PAGE_ANALOG_1;
                SelectCH_Line_Index = 3;
            }
            break;

        case PAGE_VOLTAGE_3:
            if ((Key_Value & BIT2)) {
                New_Page_Status = PAGE_VOLTAGE_2;
            } else if ((Key_Value & BIT3)) {
                New_Page_Status = PAGE_VOLTAGE_4;
            } else if ((Key_Value & BIT4)) {
                New_Page_Status = PAGE_ANALOG_1;
                SelectCH_Line_Index = 3;
            }
            break;

        case PAGE_VOLTAGE_4:
            if ((Key_Value & BIT2)) {
                New_Page_Status = PAGE_VOLTAGE_3;
            } else if ((Key_Value & BIT3)) {
                //  New_Page_Status = PAGE_VOLTAGE_1;
                New_Page_Status = PAGE_CAPACITY;
            } else if ((Key_Value & BIT4)) {
                New_Page_Status = PAGE_ANALOG_1;
                SelectCH_Line_Index = 3;
            }
            break;

        case PAGE_CAPACITY:
            if ((Key_Value & BIT2)) {
                New_Page_Status = PAGE_VOLTAGE_4;
            } else if ((Key_Value & BIT3)) {
                New_Page_Status = PAGE_VOLTAGE_1;
            } else if ((Key_Value & BIT4)) {
                New_Page_Status = PAGE_ANALOG_1;
                SelectCH_Line_Index = 3;
            }
            break;

        case PAGE_WARNING_1: //告警状态
            if ((Key_Value & (BIT2 | BIT3))) {
                New_Page_Status = PAGE_WARNING_2;
            } else if ((Key_Value & BIT4)) {
                New_Page_Status = PAGE_STATUS;
                SelectCH_Line_Index = 1;
            }
            break;

        case PAGE_WARNING_2:
            if ((Key_Value & (BIT2 | BIT3))) {
                New_Page_Status = PAGE_WARNING_1;
            } else if ((Key_Value & BIT4)) {
                New_Page_Status = PAGE_STATUS;
                SelectCH_Line_Index = 1;
            }
            break;

        case PAGE_PROTECTION_1: //保护状态
            if ((Key_Value & BIT2)) {
                New_Page_Status = PAGE_PROTECTION_2;
            } else if ((Key_Value & BIT3)) {
                New_Page_Status = PAGE_PROTECTION_2;
            } else if ((Key_Value & BIT4)) {
                New_Page_Status = PAGE_STATUS;
                SelectCH_Line_Index = 2;
            }
            break;

        case PAGE_PROTECTION_2:
            if ((Key_Value & BIT2)) {
                New_Page_Status = PAGE_PROTECTION_1;
            } else if ((Key_Value & BIT3)) {
                New_Page_Status = PAGE_PROTECTION_1;
            } else if ((Key_Value & BIT4)) {
                New_Page_Status = PAGE_STATUS;
                SelectCH_Line_Index = 2;
            }
            break;

        case PAGE_FAILALARM_1: //故障状态
            if ((Key_Value & BIT2)) {
                New_Page_Status = PAGE_FAILALARM_3;
            } else if ((Key_Value & BIT3)) {
                New_Page_Status = PAGE_FAILALARM_2;
            } else if ((Key_Value & BIT4)) {
                New_Page_Status = PAGE_STATUS;
                SelectCH_Line_Index = 3;
            }
            break;

        case PAGE_FAILALARM_2:
            if ((Key_Value & BIT2)) {
                New_Page_Status = PAGE_FAILALARM_1;
            } else if ((Key_Value & BIT3)) {
                New_Page_Status = PAGE_FAILALARM_3;
            } else if ((Key_Value & BIT4)) {
                New_Page_Status = PAGE_STATUS;
                SelectCH_Line_Index = 3;
            }
            break;

        case PAGE_FAILALARM_3:
            if ((Key_Value & BIT2)) {
                New_Page_Status = PAGE_FAILALARM_2;
            } else if ((Key_Value & BIT3)) {
                New_Page_Status = PAGE_FAILALARM_1;
            } else if ((Key_Value & BIT4)) {
                New_Page_Status = PAGE_STATUS;
                SelectCH_Line_Index = 3;
            }
            break;

        case PAGE_SYSTEMSET: //版本信息
            if ((Key_Value & BIT1)) {
                if (SelectCH_Line_Index == 0) {
                    New_Page_Status = PAGE_BMS_VERSION;
                } else if (SelectCH_Line_Index == 1) {
                    New_Page_Status = PAGE_LCD_VERSION;
                }

                SelectCH_Line_Index = 0;
            } else if ((Key_Value & BIT2)) {
                if (SelectCH_Line_Index == 0) { SelectCH_Line_Index = 3; }
                SelectCH_Line_Index--;
                Show_SelectCh(SelectCH_Line_Index);
            } else if ((Key_Value & BIT3)) {
                if (++SelectCH_Line_Index >= 2) { SelectCH_Line_Index = 0; }
                Show_SelectCh(SelectCH_Line_Index);
            } else if ((Key_Value & BIT4)) {
                New_Page_Status = PAGE_MENU;
                SelectCH_Line_Index = 3;
            }
            break;

        case PAGE_BMS_VERSION: //按键返回上一界面
            if ((Key_Value & BIT4)) {
                New_Page_Status = PAGE_SYSTEMSET;
                SelectCH_Line_Index = 0;
            }
            break;
        case PAGE_LCD_VERSION:
            if ((Key_Value & BIT4)) {
                New_Page_Status = PAGE_SYSTEMSET;
                SelectCH_Line_Index = 1;
            }
            break;

        default: break;
    }
}

//======================================================================
// Function:	Show_Temperature()
// Description:温度计算显示
//======================================================================
void Show_Temperature(int8_t Row, int8_t Col, int16_t Data) {
    int16_t Tmp;

    if (Data >= 0) {
        Tmp = Data;
        Tmp = Tmp * 10 / 10;
        Lcd_showChar6_8(Row, Col + 0 * 8, ' ', 0);
        Lcd_showChar6_8(Row, Col + 4 * 8, ' ', 0);
        if (Tmp >= 1000) {
            Lcd_showChar6_8(Row, Col + 1 * 8, (uint8_t)(Tmp / 1000) + '0', 0);
            Lcd_showChar6_8(Row, Col + 2 * 8, (uint8_t)((Tmp / 100) % 10) + '0', 0);
        } else {
            Lcd_showChar6_8(Row, Col + 1 * 8, ' ', 0);
            if (Tmp >= 100) {
                Lcd_showChar6_8(Row, Col + 2 * 8, (uint8_t)((Tmp / 100) % 10) + '0', 0);
            } else {
                Lcd_showChar6_8(Row, Col + 2 * 8, ' ', 0);
            }
        }
        Lcd_showChar6_8(Row, Col + 3 * 8, (uint8_t)((Tmp / 10) % 10) + '0', 0);
    } else {
        Tmp = 0 - Data;
        if (Tmp > 999) { Tmp = 999; }
        Tmp = Tmp * 10 / 10;
        Lcd_showChar6_8(Row, Col + 0 * 8, ' ', 0);
        Lcd_showChar6_8(Row, Col + 4 * 8, ' ', 0);
        if (Tmp >= 100) {
            Lcd_showChar6_8(Row, Col + 1 * 8, '-', 0);
            Lcd_showChar6_8(Row, Col + 2 * 8, (uint8_t)(Tmp / 100) + '0', 0);
        } else {
            Lcd_showChar6_8(Row, Col + 1 * 8, ' ', 0);
            Lcd_showChar6_8(Row, Col + 2 * 8, '-', 0);
        }
        Lcd_showChar6_8(Row, Col + 3 * 8, (uint8_t)((Tmp / 10) % 10) + '0', 0);
    }
}
/*显示首页6*16电芯温度*/
void Show_Temperature1(int8_t Row, int8_t Col, int16_t Data) {
    int16_t Tmp;

    if (Data >= 0) {
        Tmp = Data;
        Tmp = Tmp * 10 / 10;
        Lcd_showChar6_16(Row, Col + 0 * 8, ' ', 0);
        Lcd_showChar6_16(Row, Col + 4 * 8, ' ', 0);
        if (Tmp >= 1000) {
            Lcd_showChar6_16(Row, Col + 1 * 8, (uint8_t)(Tmp / 1000)%10 + '0', 0);
            Lcd_showChar6_16(Row, Col + 2 * 8, (uint8_t)((Tmp / 100) % 10) + '0', 0);
        } else {
            Lcd_showChar6_16(Row, Col + 1 * 8, ' ', 0);
            if (Tmp >= 100) {
                Lcd_showChar6_16(Row, Col + 2 * 8, (uint8_t)((Tmp / 100) % 10) + '0', 0);
            } else {
                Lcd_showChar6_16(Row, Col + 2 * 8, ' ', 0);
            }
        }
        Lcd_showChar6_16(Row, Col + 3 * 8, (uint8_t)((Tmp / 10) % 10) + '0', 0);
    } else {
        Tmp = 0 - Data;
        if (Tmp > 999) { Tmp = 999; }
        Tmp = Tmp * 10 / 10;
        Lcd_showChar6_16(Row, Col + 0 * 8, ' ', 0);
        Lcd_showChar6_16(Row, Col + 4 * 8, ' ', 0);

        if (Tmp >= 100) {
            Lcd_showChar6_16(Row, Col + 1 * 8, '-', 0);
            Lcd_showChar6_16(Row, Col + 2 * 8, (uint8_t)(Tmp / 100) %10+ '0', 0);
        } else {
            Lcd_showChar6_16(Row, Col + 1 * 8, ' ', 0);
            Lcd_showChar6_16(Row, Col + 2 * 8, '-', 0);
        }
        Lcd_showChar6_16(Row, Col + 3 * 8, (uint8_t)((Tmp / 10) % 10) + '0', 0);
    }
}

/*******************************************************************************
 *Function:      Show_TemperatureNull()
 *Description:   没有温度数据显示时，显示"   -- "
 *
 *******************************************************************************/
//void Show_TemperatureNull(uint8_t Row) { Lcd_showStringEN(Row, 9 * 8, "  -- ", 0); }

//======================================================================
// Function:	Show_Cell_Voltage()
// Description:子菜单单体电压
//======================================================================
void Show_Data_4Bit(uint8_t Row, uint8_t Col, uint16_t Data) {
    if (Data >= 1000) {
        if (Data > 9999) { Data = 9999; }
        Lcd_showChar6_8(Row, Col + 0 * 8, (uint8_t)(Data / 1000) + '0', 0);
        Lcd_showChar6_8(Row, Col + 1 * 8, (uint8_t)((Data / 100) % 10) + '0', 0);
        Lcd_showChar6_8(Row, Col + 2 * 8, (uint8_t)((Data / 10) % 10) + '0', 0);
        Lcd_showChar6_8(Row, Col + 3 * 8, (uint8_t)(Data % 10) + '0', 0);
    } else if (Data >= 100) {
        Lcd_showChar6_8(Row, Col + 0 * 8, ' ', 0);
        Lcd_showChar6_8(Row, Col + 1 * 8, (uint8_t)(Data / 100) + '0', 0);
        Lcd_showChar6_8(Row, Col + 2 * 8, (uint8_t)((Data / 10) % 10) + '0', 0);
        Lcd_showChar6_8(Row, Col + 3 * 8, (uint8_t)(Data % 10) + '0', 0);
    } else if (Data >= 10) {
        Lcd_showChar6_8(Row, Col + 0 * 8, ' ', 0);
        Lcd_showChar6_8(Row, Col + 1 * 8, (uint8_t)(Data / 10) + '0', 0);
        Lcd_showChar6_8(Row, Col + 2 * 8, (uint8_t)(Data % 10) + '0', 0);
        Lcd_showChar6_8(Row, Col + 3 * 8, ' ', 0);
    } else {
        Lcd_showChar6_8(Row, Col + 0 * 8, ' ', 0);
        Lcd_showChar6_8(Row, Col + 1 * 8, ' ', 0);
        Lcd_showChar6_8(Row, Col + 2 * 8, (uint8_t) Data + '0', 0);
        Lcd_showChar6_8(Row, Col + 3 * 8, ' ', 0);
    }
}

//首页单体电压显示6*7字体
void Show_Data_4Bit1(uint8_t Row, uint8_t Col, uint16_t Data) {
    if (Data >= 1000) {
        if (Data > 9999) { Data = 9999; }
        Lcd_showCha6_7(Row, Col + 0 * 6, (uint8_t)(Data / 1000) + '0', 1);
        Lcd_showCha6_7(Row, Col + 1 * 6, (uint8_t)((Data / 100) % 10) + '0', 1);
        Lcd_showCha6_7(Row, Col + 2 * 6, (uint8_t)((Data / 10) % 10) + '0', 1);
        Lcd_showCha6_7(Row, Col + 3 * 6, (uint8_t)(Data % 10) + '0', 1);
    } else if (Data >= 100) {
        Lcd_showCha6_7(Row, Col + 0 * 6, ' ', 0);
        Lcd_showCha6_7(Row, Col + 1 * 6, (uint8_t)(Data / 100) + '0', 1);
        Lcd_showCha6_7(Row, Col + 2 * 6, (uint8_t)((Data / 10) % 10) + '0', 1);
        Lcd_showCha6_7(Row, Col + 3 * 6, (uint8_t)(Data % 10) + '0', 1);
    } else if (Data >= 10) {
        Lcd_showCha6_7(Row, Col + 0 * 6, ' ', 0);
        Lcd_showCha6_7(Row, Col + 1 * 6, (uint8_t)(Data / 10) + '0', 1);
        Lcd_showCha6_7(Row, Col + 2 * 6, (uint8_t)(Data % 10) + '0', 1);
        Lcd_showCha6_7(Row, Col + 3 * 6, ' ', 0);
    } else {
        Lcd_showCha6_7(Row, Col + 0 * 6, ' ', 0);
        Lcd_showCha6_7(Row, Col + 1 * 6, ' ', 0);
        Lcd_showCha6_7(Row, Col + 2 * 6, (uint8_t) Data + '0', 1);
        Lcd_showCha6_7(Row, Col + 3 * 6, ' ', 0);
    }
}

//======================================================================
// Function:	Show_Current()
// Description:  子菜单显示电流
//======================================================================
void Show_Current(uint8_t Row, uint8_t Col, int16_t Data) {
    uint16_t Tmp;
    uint8_t BaiWei, ShiWei, GeWei, Fen_10, Fen_100;

    Tmp = abs(Data);
    Lcd_showChar6_8(Row, Col, ' ', 0);
    BaiWei = 0;
    ShiWei = 0;
    GeWei = 0;
    Fen_10 = 0;
    Fen_100 = 0;
    while (Tmp >= 10000) {
        Tmp -= 10000;
        BaiWei++;
    }
    while (Tmp >= 1000) {
        Tmp -= 1000;
        ShiWei++;
    }
    while (Tmp >= 100) {
        Tmp -= 100;
        GeWei++;
    }
    while (Tmp >= 10) {
        Tmp -= 10;
        Fen_10++;
    }
    while (Tmp >= 1) {
        Tmp -= 1;
        Fen_100++;
    }

    if (0 == BaiWei) //最高位为0，显示空格
    {
        BaiWei = ' ';
        if (0 == ShiWei) ShiWei = ' ';
        else {
            if (Data < 0) { BaiWei = '-'; }
            ShiWei += '0';
        }
    } else {
        if (Data < 0) { Lcd_showChar6_8(Row, Col, '-', 0); }
        BaiWei += '0';
        ShiWei += '0';
    }

    if ((Data < 0) && (BaiWei == ' ') && (ShiWei == ' ')) { ShiWei = '-'; }
    GeWei += '0';
    Fen_10 += '0';
    Fen_100 += '0';

    Lcd_showChar6_8(Row, Col + 8, BaiWei, 0);
    Lcd_showChar6_8(Row, Col + 16, ShiWei, 0);
    Lcd_showChar6_8(Row, Col + 24, GeWei, 0);
    Lcd_showChar6_8(Row, Col + 32, '.', 0);
    Lcd_showChar6_8(Row, Col + 40, Fen_10, 0);
}

/*首页显示电流*/
void  Show_Current1(uint8_t Row, uint8_t Col, int16_t Data) {
    uint16_t Tmp;
    uint8_t BaiWei, ShiWei, GeWei, Fen_10, Fen_100;

    Tmp = abs(Data);
    Lcd_showChar(Row, Col, ' ', 0);
    BaiWei = 0;
    ShiWei = 0;
    GeWei = 0;
    Fen_10 = 0;
    Fen_100 = 0;
    while (Tmp >= 10000) {
        Tmp -= 10000;
        BaiWei++;
    }
    while (Tmp >= 1000) {
        Tmp -= 1000;
        ShiWei++;
    }
    while (Tmp >= 100) {
        Tmp -= 100;
        GeWei++;
    }
    while (Tmp >= 10) {
        Tmp -= 10;
        Fen_10++;
    }
//    while (Tmp >= 1) {
//        Tmp -= 1;
//        Fen_100++;
//    }

    if (0 == BaiWei) {//最高位为0，显示空格
        BaiWei = ' ';
        if (0 == ShiWei) ShiWei = ' ';
        else {
            if (Data < 0) { BaiWei = '-'; }
            ShiWei += '0';
        }
    } else {
        if (Data < 0) { Lcd_showChar(Row, Col, '-', 0); }
        BaiWei += '0';
        ShiWei += '0';
    }

    if ((Data < 0) && (BaiWei == ' ') && (ShiWei == ' ')) { ShiWei = '-'; }
    GeWei += '0';
    Fen_10 += '0';
//    Fen_100 += '0';

    Lcd_showChar(Row, Col + 8,  BaiWei,  0);
    Lcd_showChar(Row, Col + 16,  ShiWei, 0);
    Lcd_showChar(Row, Col + 24,  GeWei ,  0);
    Lcd_showChar(4, Col + 32, '.', 0);
    Lcd_showChar(Row, Col + 40, Fen_10, 0);
}

//======================================================================
// Function:	Show_TotalVoltage()
// Description:  子菜单显示总压
//======================================================================
void Show_TotalVoltage(uint8_t Row, uint8_t Col, uint16_t Data) {
    if (Data >= 10000) {
        Lcd_showChar6_8(Row, Col + 0 * 8, (uint8_t)(Data / 10000) + '0', 0);
        Lcd_showChar6_8(Row, Col + 1 * 8, (uint8_t)((Data / 1000) % 10) + '0', 0);
    } else {
        Lcd_showChar6_8(Row, Col + 0 * 8, ' ', 0);
        Lcd_showChar6_8(Row, Col + 1 * 8, (uint8_t)(Data / 1000) + '0', 0);
    }
    Lcd_showChar6_8(Row, Col + 2 * 8, '.', 0);
    Lcd_showChar6_8(Row, Col + 3 * 8, (uint8_t)((Data / 100) % 10) + '0', 0);
    Lcd_showChar6_8(Row, Col + 4 * 8, (uint8_t)((Data / 10) % 10) + '0', 0);
}

//首页10*12显示电压
void Show_TotalVoltage1(uint8_t Row, uint8_t Col, uint16_t Data) {
    if (Data >= 10000) {
        Lcd_showChar(Row, Col + 0 * 8, (uint8_t)(Data / 10000) + '0', 0);
        Lcd_showChar(Row, Col + 1 * 8, (uint8_t)((Data / 1000) % 10) + '0', 0);
    } else {
        Lcd_showChar(Row, Col + 0 * 8, ' ', 0);
        Lcd_showChar(Row, Col + 1 * 8, (uint8_t)(Data / 1000) + '0', 0);
    }
    Lcd_showCha6_7(1, Col + 2 * 8, '.', 0);
    Lcd_showChar(Row, Col + 3 * 8, (uint8_t)((Data / 100) % 10) + '0', 0);
//    Lcd_showChar(Row, Col + 4 * 8, (uint8_t)((Data / 10) % 10) + '0', 0);//后两位小数点
}

//======================================================================
// Function:	Page_Digital_Show()
// Description:电池状态
//======================================================================
void Page_Digital_Show(void) {
    switch (Old_Page_Status) {
        case PAGE_ANALOG_1:
            Show_TotalVoltage(0, 80, (PackData.Vsum));
            Show_Current(2, 9 * 8, PackData.Current); // 72
            break;

        case PAGE_STATUS:
            if (Dis_Charge_Flag == 1) {
                Lcd_showStringEN(0, 9 * 8, "CHARGE", 0);
            } else if (Dis_Charge_Flag == 2) {
                Lcd_showStringEN(0, 9 * 8, "DISCHG", 0);
            } else {
                if (PackData.Prp_State.BitName.bCellVoltOV) {
                    Lcd_showStringEN(0, 9 * 8, "  FULL", 0);
                } else {
                    Lcd_showStringEN(0, 9 * 8, "  IDLE", 0);
                }
            }
            break;

        case PAGE_TEMPERATURE_1:
            if (PackData.TempNum > 2) {
                Show_Temperature(0, 9 * 8, PackData.Temp[0]);
            } else {
                Show_TemperatureNull(0);
            }

            if (PackData.TempNum > 3) {
                Show_Temperature(2, 9 * 8, PackData.Temp[1]);
            } else {
                Show_TemperatureNull(2);
            }

            if (PackData.TempNum > 4) {
                Show_Temperature(4, 9 * 8, PackData.Temp[2]);
            } else {
                Show_TemperatureNull(4);
            }

            if (PackData.TempNum > 5) {
                Show_Temperature(6, 9 * 8, PackData.Temp[3]);
            } else {
                Show_TemperatureNull(6);
            }
            break;

            //        case PAGE_TEMPERATURE_2:

        case PAGE_TEMPERATURE_3:
            Show_Temperature(0, 9 * 8, PackData.s16MosTemp);
            Show_Temperature(2, 9 * 8, PackData.s16EnvirTemp);
            break;

        case PAGE_VOLTAGE_1:
            if (PackData.BatNum > 0) {
                Show_Data_4Bit(0, 76, PackData.Vol[0]);
            } else {
                Lcd_showStringEN(0, 10 * 8, " -- ", 0);
            }

            if (PackData.BatNum > 1) {
                Show_Data_4Bit(2, 76, PackData.Vol[1]);
            } else {
                Lcd_showStringEN(2, 10 * 8, " -- ", 0);
            }

            if (PackData.BatNum > 2) {
                Show_Data_4Bit(4, 76, PackData.Vol[2]);
            } else {
                Lcd_showStringEN(4, 10 * 8, " -- ", 0);
            }

            if (PackData.BatNum > 3) {
                Show_Data_4Bit(6, 76, PackData.Vol[3]);
            } else {
                Lcd_showStringEN(6, 10 * 8, " -- ", 0);
            }
            break;

        case PAGE_VOLTAGE_2:
            if (PackData.BatNum > 4) {
                Show_Data_4Bit(0, 76, PackData.Vol[4]);
            } else {
                Lcd_showStringEN(0, 10 * 8, " -- ", 0);
            }

            if (PackData.BatNum > 5) {
                Show_Data_4Bit(2, 76, PackData.Vol[5]);
            } else {
                Lcd_showStringEN(2, 10 * 8, " -- ", 0);
            }

            if (PackData.BatNum > 6) {
                Show_Data_4Bit(4, 76, PackData.Vol[6]);
            } else {
                Lcd_showStringEN(4, 10 * 8, " -- ", 0);
            }

            if (PackData.BatNum > 7) {
                Show_Data_4Bit(6, 76, PackData.Vol[7]);
            } else {
                Lcd_showStringEN(6, 10 * 8, " -- ", 0);
            }
            break;

        case PAGE_VOLTAGE_3:
            if (PackData.BatNum > 8) {
                Show_Data_4Bit(0, 76, PackData.Vol[8]);
            } else {
                Lcd_showStringEN(0, 10 * 8, " -- ", 0);
            }

            if (PackData.BatNum > 9) {
                Show_Data_4Bit(2, 76, PackData.Vol[9]);
            } else {
                Lcd_showStringEN(2, 10 * 8, " -- ", 0);
            }

            if (PackData.BatNum > 10) {
                Show_Data_4Bit(4, 76, PackData.Vol[10]);
            } else {
                Lcd_showStringEN(4, 10 * 8, " -- ", 0);
            }

            if (PackData.BatNum > 11) {
                Show_Data_4Bit(6, 76, PackData.Vol[11]);
            } else {
                Lcd_showStringEN(6, 10 * 8, " -- ", 0);
            }
            break;

        case PAGE_VOLTAGE_4:
            if (PackData.BatNum > 12) {
                Show_Data_4Bit(0, 76, PackData.Vol[12]);
            } else {
                Lcd_showStringEN(0, 10 * 8, " -- ", 0);
            }

            if (PackData.BatNum > 13) {
                Show_Data_4Bit(2, 76, PackData.Vol[13]);
            } else {
                Lcd_showStringEN(2, 10 * 8, " -- ", 0);
            }

            if (PackData.BatNum > 14) {
                Show_Data_4Bit(4, 76, PackData.Vol[14]);
            } else {
                Lcd_showStringEN(4, 10 * 8, " -- ", 0);
            }

            if (PackData.BatNum > 15) {
                Show_Data_4Bit(6, 76, PackData.Vol[15]);
            } else {
                Lcd_showStringEN(6, 10 * 8, " -- ", 0);
            }
            break;

        case PAGE_CAPACITY:
            if (((Soc / 100) % 10) == 0) Lcd_showChar6_8(0, 11 * 8, ' ', 0); //SOC
            else
                Lcd_showChar6_8(0, 11 * 8, (uint8_t)((Soc / 100) % 10) + '0', 0);

            if ((((Soc / 10) % 10) == 0) && (((Soc / 100) % 10) == 0)) Lcd_showChar6_8(0, 12 * 8, ' ', 0);
            else
                Lcd_showChar6_8(0, 12 * 8, (uint8_t)((Soc / 10) % 10) + '0', 0);
            Lcd_showChar6_8(0, 13 * 8, (uint8_t)(Soc % 10) + '0', 0);

            if (PackData.Fcc >= 10000) { //总容量
                Lcd_showChar6_8(2, 9 * 8, (uint8_t)(PackData.Fcc / 10000) + '0', 0);
                Lcd_showChar6_8(2, 10 * 8, (uint8_t)((PackData.Fcc / 1000) % 10) + '0', 0);
            } else if (PackData.Fcc >= 1000) {
                Lcd_showChar6_8(2, 9 * 8, ' ', 0);
                Lcd_showChar6_8(2, 10 * 8, (uint8_t)(PackData.Fcc / 1000) + '0', 0);
            } else {
                Lcd_showChar6_8(2, 9 * 8, ' ', 0);
                Lcd_showChar6_8(2, 10 * 8, ' ', 0);
            }
            Lcd_showChar6_8(2, 11 * 8, (uint8_t)((PackData.Fcc / 100) % 10) + '0', 0);
            Lcd_showChar6_8(2, 12 * 8, '.', 0);
            Lcd_showChar6_8(2, 13 * 8, (uint8_t)((PackData.Fcc / 10) % 10) + '0', 0);

            if (PackData.Rm >= 10000) { //剩余容量
                Lcd_showChar6_8(4, 9 * 8, (uint8_t)(PackData.Rm / 10000) + '0', 0);
                Lcd_showChar6_8(4, 10 * 8, (uint8_t)((PackData.Rm / 1000) % 10) + '0', 0);
            } else if (PackData.Rm >= 1000) {
                Lcd_showChar6_8(4, 9 * 8, ' ', 0);
                Lcd_showChar6_8(4, 10 * 8, (uint8_t)(PackData.Rm / 1000) + '0', 0);
            } else {
                Lcd_showChar6_8(4, 9 * 8, ' ', 0);
                Lcd_showChar6_8(4, 10 * 8, ' ', 0);
            }
            Lcd_showChar6_8(4, 11 * 8, (uint8_t)((PackData.Rm / 100) % 10) + '0', 0);
            Lcd_showChar6_8(4, 12 * 8, '.', 0);
            Lcd_showChar6_8(4, 13 * 8, (uint8_t)((PackData.Rm / 10) % 10) + '0', 0);

            Show_Data_4Bit(6, 12 * 8, PackData.Cycle); //循环次数
            break;

            /*告警状态*/
        case PAGE_WARNING_1:
            if (PackData.Alarm_State.BitName.bCellVoltOV) Lcd_showStringEN(0, 13 * 8, "YES", 0);
            else
                Lcd_showStringEN(0, 13 * 8, " NO", 0);

            if (PackData.Alarm_State.BitName.bCellVoltUL) Lcd_showStringEN(2, 13 * 8, "YES", 0);
            else
                Lcd_showStringEN(2, 13 * 8, " NO", 0);

            if (PackData.Alarm_State.BitName.bTempOV) Lcd_showStringEN(4, 13 * 8, "YES", 0);
            else
                Lcd_showStringEN(4, 13 * 8, " NO", 0);

            if (PackData.Alarm_State.BitName.bTempUL) Lcd_showStringEN(6, 13 * 8, "YES", 0);
            else
                Lcd_showStringEN(6, 13 * 8, " NO", 0);
            break;

        case PAGE_WARNING_2:
            if (PackData.Alarm_State.BitName.bSocUL) Lcd_showStringEN(0, 13 * 8, "YES", 0);
            else
                Lcd_showStringEN(0, 13 * 8, " NO", 0);

            if (PackData.Alarm_State.BitName.bVDiffOV) Lcd_showStringEN(2, 13 * 8, "YES", 0);
            else
                Lcd_showStringEN(2, 13 * 8, " NO", 0);

            if (PackData.Alarm_State.BitName.bCurrOV) Lcd_showStringEN(4, 13 * 8, "YES", 0);
            else
                Lcd_showStringEN(4, 13 * 8, " NO", 0);

            if (PackData.Prp_State.BitName.bNeg) Lcd_showStringEN(6, 13 * 8, "YES", 0);
            else
                Lcd_showStringEN(6, 13 * 8, " NO", 0);
            break;

            /*保护状态*/
        case PAGE_PROTECTION_1:
            if (PackData.Prp_State.BitName.bCellVoltOV) Lcd_showStringEN(0, 13 * 8, "YES", 0);
            else
                Lcd_showStringEN(0, 13 * 8, " NO", 0);

            if (PackData.Prp_State.BitName.bCellVoltUL) Lcd_showStringEN(2, 13 * 8, "YES", 0);
            else
                Lcd_showStringEN(2, 13 * 8, " NO", 0);

            if (PackData.Prp_State.BitName.bTempOV) Lcd_showStringEN(4, 13 * 8, "YES", 0);
            else
                Lcd_showStringEN(4, 13 * 8, " NO", 0);

            if (PackData.Prp_State.BitName.bTempUL) Lcd_showStringEN(6, 13 * 8, "YES", 0);
            else
                Lcd_showStringEN(6, 13 * 8, " NO", 0);
            break;

        case PAGE_PROTECTION_2:
            if (PackData.Prp_State.BitName.bCurrOV) Lcd_showStringEN(0, 13 * 8, "YES", 0);
            else
                Lcd_showStringEN(0, 13 * 8, " NO", 0);

            if (PackData.Prp_State.BitName.bShortFault) Lcd_showStringEN(2, 13 * 8, "YES", 0);
            else
                Lcd_showStringEN(2, 13 * 8, " NO", 0);
            break;

            /*故障状态*/
        case PAGE_FAILALARM_1:
            if (PackData.Prp_State.BitName.bSmpErr) Lcd_showChar6_8(0, 14 * 8, 'Y', 0); // NOK
            else
                Lcd_showChar6_8(0, 14 * 8, 'N', 0); // OK

            if (PackData.Prp_State.BitName.bChgMosErr) Lcd_showChar6_8(2, 14 * 8, 'Y', 0); // NOK
            else
                Lcd_showChar6_8(2, 14 * 8, 'N', 0); // OK

            if (PackData.Prp_State.BitName.bDisMosErr) Lcd_showChar6_8(4, 14 * 8, 'Y', 0); // NOK
            else
                Lcd_showChar6_8(4, 14 * 8, 'N', 0); // OK

            if (PackData.Prp_State.BitName.bChipSmpErr) Lcd_showChar6_8(6, 14 * 8, 'Y', 0); // NOK
            else
                Lcd_showChar6_8(6, 14 * 8, 'N', 0); // OK
            break;

        case PAGE_FAILALARM_2:
            Show_Data_4Bit(0, 12 * 8, Record.Short_Count);
            Show_Data_4Bit(2, 12 * 8, Record.Over_Temp_Count);
            Show_Data_4Bit(4, 12 * 8, Record.Over_Curr_Count);
            Show_Data_4Bit(6, 12 * 8, Record.Over_Chg_Count);
            break;

        case PAGE_FAILALARM_3:
            Show_Data_4Bit(0, 12 * 8, Record.Over_Dchg_Count);
            break;

        case PAGE_BMS_VERSION://版本号
//            Show_Data_4Bit(6, 7 * 8, PackData.Project_code_[9]);
//            Show_Data_4Bit(6, 7 * 8, PackData.Project_code[0]);
//            Show_Data_4Bit(6, 7 * 8, 6);
            break;
#if 0
            /*首页剩余容量*/
        case PAGE_WELCOME://
            if (PackData.Rm >= 10000) {
                Lcd_showChar(6, 0 * 8, (uint8_t)(PackData.Rm / 10000) + '0', 0);
                Lcd_showChar(6, 1 * 8, (uint8_t)((PackData.Rm / 1000) % 10) + '0', 0);
            } else if (PackData.Rm >= 1000) {
                Lcd_showChar(6, 0 * 8, ' ', 0);
                Lcd_showChar(6, 1 * 8, (uint8_t)(PackData.Rm / 1000) + '0', 0);
            } else {
                Lcd_showChar(6, 0 * 8, ' ', 0);
                Lcd_showChar(6, 1 * 8, ' ', 0);
            }
            Lcd_showChar(6, 2 * 8, (uint8_t)((PackData.Rm / 100) % 10) + '0', 0);
            Lcd_showCha6_7(6, 3 * 8, '.', 0);
            Lcd_showChar(6, 4 * 8, (uint8_t)((PackData.Rm / 10) % 10) + '0', 0);

            /*首页保护状态6*7显示*/
            if (PackData.Prp_State.BitName.bCellVoltOV) Lcd_showStringENG(1, 19 * 6, "OV", 0);
            else
                Lcd_showStringENG(1, 19 * 6, "  ", 0);

            if (PackData.Prp_State.BitName.bCellVoltUL) Lcd_showStringENG(2, 19 * 6, "LV", 0);
            else
                Lcd_showStringENG(2, 19 * 6, "  ", 0);

            if (PackData.Prp_State.BitName.bTempOV) Lcd_showStringENG(5, 19 * 6, "OT", 0);
            else
                Lcd_showStringENG(5, 19 * 6, "  ", 0);

            if (PackData.Prp_State.BitName.bTempUL) Lcd_showStringENG(6, 19 * 6, "LT", 0);
            else
                Lcd_showStringENG(6, 19 * 6, "  ", 0);

            if (PackData.Prp_State.BitName.bCurrOV) Lcd_showStringENG(3, 19 * 6, "OC", 0);
            else
                Lcd_showStringENG(3, 19 * 6, "  ", 0);

            if (PackData.Prp_State.BitName.bShortFault) Lcd_showStringENG(4, 19 * 6, "SC", 0);
            else
                Lcd_showStringENG(4, 19 * 6, "  ", 0);

            /*首页显示SOC*/
            if (((Soc / 100) % 10) == 0) Lcd_showChar6_16(5, 10 * 8, ' ', 0);
            else
                Lcd_showChar6_16(5, 10 * 8, (uint8_t)((Soc / 100) % 10) + '0', 0);

            if ((((Soc / 10) % 10) == 0) && (((Soc / 100) % 10) == 0)) Lcd_showChar6_16(5, 11 * 8, ' ', 0);
            else
                Lcd_showChar6_16(5, 11 * 8, (uint8_t)((Soc / 10) % 10) + '0', 0);
            Lcd_showChar6_16(5, 12 * 8, (uint8_t)(Soc % 10) + '0', 0);
            Lcd_showChar6_16(5, 13 * 8, '%', 0);
#endif
            /*首页电池状态*/
            //            if (Dis_Charge_Flag == 1) //"CHG "
            //            {
            //                Lcd_showStringENG(7, 0 * 6, "CHARGE", 0);
            //            } else if (Dis_Charge_Flag == 2) //"DGS "
            //            {
            //                Lcd_showStringENG(7, 0 * 6, "DISCHG", 0);
            //            } else {
            //                if (PackData.Prp_State.BitName.bCellVoltOV)
            //                {
            //                    Lcd_showStringENG(7, 2 * 6, "FULL", 0);
            //                } else {
            //                    Lcd_showStringENG(7, 0 * 6, "  IDLE", 0);
            //                }
            //            }
            break;
    }
}

/*******************************************************************************
 * Function:   Page_Change_Ctrl()
 * Description:
 *******************************************************************************/
void Page_Change_Ctrl(void) {
    if (Old_Page_Status == New_Page_Status) return;

    Old_Page_Status = New_Page_Status;
    Clear_Screen(0x00, 0x00);
//    Clear_Screen(0xFF,0xFF);		//界面切换时有阴影问题

    switch (New_Page_Status) {
//        case PAGE_WELCOME: Page_Welcome(); break; //新增按键操作，数据更新
        case PAGE_WELCOME: Page_Welcome_new(); break; //新增按键操作，数据更新

        case PAGE_MENU: Page_Menu(); break;

        case PAGE_ANALOG_1: Page_Analog_1(); break;
//        case PAGE_ANALOG_2: Page_Analog_2(); break;

        case PAGE_STATUS: Page_Status(); break;

        case PAGE_PARASET: Page_ParaSet(); break;

        case PAGE_SYSTEMSET: Page_SystemSet(); break;

        case PAGE_TEMPERATURE_1: Page_Temperature_1();break;

//        case PAGE_TEMPERATURE_2: Page_Temperature_2(); break; //8路NTC

        case PAGE_TEMPERATURE_3: Page_Temperature_3(); break;

        case PAGE_VOLTAGE_1: Page_Voltage_1(); break;

        case PAGE_VOLTAGE_2: Page_Voltage_2(); break;

        case PAGE_VOLTAGE_3: Page_Voltage_3(); break;

        case PAGE_VOLTAGE_4: Page_Voltage_4(); break;

        case PAGE_CAPACITY: Page_Capacity(); break;

        case PAGE_WARNING_1: Page_Warning_1(); break;

        case PAGE_WARNING_2: Page_Warning_2(); break;

        case PAGE_PROTECTION_1: Page_Protection_1(); break;

        case PAGE_PROTECTION_2: Page_Protection_2(); break;

//        case PAGE_PROTECTION_3:Page_Protection_3();break;

        case PAGE_FAILALARM_1: Page_FailAlarm_1(); break;

        case PAGE_FAILALARM_2: Page_FailAlarm_2(); break;

        case PAGE_FAILALARM_3: Page_FailAlarm_3(); break;

        case PAGE_BMS_VERSION: Page_BMS_Version(); break;

        case PAGE_LCD_VERSION: Page_LCD_Version(); break;

        default: break;
    }
    Page_Digital_Show();
}

//======================================================================
// Function:	Show_Ctrl()
// Description:
//======================================================================
void Show_Ctrl(void) {
    Page_Select_Ctrl();
    Page_Change_Ctrl();

    if (!Timer[DATA_SHOW_TIMER].Flag) return;
    Set_Timer(DATA_SHOW_TIMER, 20);
    Page_Digital_Show();
}

//======================================================================
// ENDFILE    1600
//======================================================================
