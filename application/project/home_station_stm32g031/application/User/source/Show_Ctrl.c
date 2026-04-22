/*******************************************************************************
********************************************************************************
* FileName: Show_Ctrl.c
* Author:
* Version:  1.00
* Date:
* Description: Progress all of the LCD screens
*******************************************************************************/
#include "Show_Ctrl.h"

#include <stdbool.h>

#include "Data_Init.h"
#include "Font_Library.h"
#include "Key_Scan.h"
#include "LCD_12864.h"
#include "Uart_Comm.h"
#include "User_Conf.h"
#include "bms_rt_info.h"
#include "common_utils.h"
#include "p_1363_B0_frame.h"
#include "paras.h"
#include "protocol_common.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define PROTOCOL_ARR_LENGTH 35
#define ROW_FIRST 0
#define ROW_SECOND 2
#define ROW_THREE 4
#define ROW_LAST 6
#define PROTOCOL_EN_ALL 0 // 是否显示全部协议 1：是，0：否
//--------------------------全局变量------------------------------------
uint8_t New_Page_Status     = 0; // 需要切换的新页面
uint8_t Old_Page_Status     = 0; // 当前页面
uint8_t SelectCH_Line_Index = 0; // 选择字符所在位置

static PAGE_PROTOCOL_T page_protocol_t = {0};

PAGE_PROTOCOL_T *get_page_protocol_t(void) { return &page_protocol_t; };

#if PROTOCOL_EN_ALL == 1
static protocol_struct_t protocol_arr[PROTOCOL_ARR_LENGTH] = {
    {E_CAN_VICTORN, PAGE_CAN_PAGE_1, ROW_FIRST},
    {E_CAN_PYLON, PAGE_CAN_PAGE_1, ROW_SECOND},
    {E_CAN_GOODWE, PAGE_CAN_PAGE_1, ROW_THREE},
    {E_CAN_GROWATT, PAGE_CAN_PAGE_1, ROW_LAST},
    {E_CAN_VOLTRONIC, PAGE_CAN_PAGE_2, ROW_FIRST},
    {E_CAN_LXP, PAGE_CAN_PAGE_2, ROW_SECOND},
    {E_CAN_DEYE, PAGE_CAN_PAGE_2, ROW_THREE},
    {E_CAN_SOFAR, PAGE_CAN_PAGE_2, ROW_LAST},
    {E_CAN_SOLIS, PAGE_CAN_PAGE_3, ROW_FIRST},
    {E_CAN_SUNGROW, PAGE_CAN_PAGE_3, ROW_SECOND},
    {E_CAN_STUDER, PAGE_CAN_PAGE_3, ROW_THREE},
    {E_CAN_HUAWEI, PAGE_CAN_PAGE_3, ROW_LAST},
    {E_CAN_SMA, PAGE_CAN_PAGE_4, ROW_FIRST},
    {E_CAN_INVT, PAGE_CAN_PAGE_4, ROW_SECOND},
    {E_CAN_KSTAR, PAGE_CAN_PAGE_4, ROW_THREE},
    {E_CAN_MUST, PAGE_CAN_PAGE_4, ROW_LAST},
    {E_CAN_SRNE, PAGE_CAN_PAGE_5, ROW_FIRST},
    {E_CAN_FOXES, PAGE_CAN_PAGE_5, ROW_SECOND},
    {E_CAN_BST, PAGE_CAN_PAGE_5, ROW_THREE},
    {E_CAN_AISWEI, PAGE_CAN_PAGE_5, ROW_LAST},
    {E_CAN_SCHNEIDER, PAGE_CAN_PAGE_6, ROW_FIRST},
    {E_485_PYLON, PAGE_RS485_PAGE_1, ROW_FIRST},
    {E_485_SMK, PAGE_RS485_PAGE_1, ROW_SECOND},
    {E_485_VOLTRONIC, PAGE_RS485_PAGE_1, ROW_THREE},
    {E_485_GROWATT, PAGE_RS485_PAGE_1, ROW_LAST},
    {E_485_SRNE, PAGE_RS485_PAGE_2, ROW_FIRST},
    {E_485_GP, PAGE_RS485_PAGE_2, ROW_SECOND},
    {E_485_HENG_RUI, PAGE_RS485_PAGE_2, ROW_THREE},
    {E_485_WOLONG, PAGE_RS485_PAGE_2, ROW_LAST},
    {E_485_SACREDSUN, PAGE_RS485_PAGE_3, ROW_FIRST},
    {E_485_LI_BATTERY_V102, PAGE_RS485_PAGE_3, ROW_SECOND},
    {E_485_LINGYU, PAGE_RS485_PAGE_3, ROW_THREE},
    {E_485_ELTEK, PAGE_RS485_PAGE_3, ROW_LAST},
    {E_485_HUAWEI, PAGE_RS485_PAGE_4, ROW_FIRST},
    {E_485_VIRTIV, PAGE_RS485_PAGE_4, ROW_SECOND},
};
#else
static protocol_struct_t protocol_arr[PROTOCOL_ARR_LENGTH] = {
    {E_CAN_VICTORN, PAGE_CAN_PAGE_1, ROW_FIRST},     {E_CAN_PYLON, PAGE_CAN_PAGE_1, ROW_SECOND},
    {E_CAN_GROWATT, PAGE_CAN_PAGE_1, ROW_THREE},     {E_CAN_LXP, PAGE_CAN_PAGE_1, ROW_LAST},
    {E_CAN_DEYE, PAGE_CAN_PAGE_2, ROW_FIRST},        {E_CAN_SOFAR, PAGE_CAN_PAGE_2, ROW_SECOND},
    {E_CAN_SOLIS, PAGE_CAN_PAGE_2, ROW_THREE},       {E_CAN_SMA, PAGE_CAN_PAGE_2, ROW_LAST},
    {E_CAN_MUST, PAGE_CAN_PAGE_3, ROW_FIRST},        {E_CAN_AISWEI, PAGE_CAN_PAGE_3, ROW_SECOND},
    {E_485_PYLON, PAGE_RS485_PAGE_1, ROW_FIRST},     {E_485_SRNE, PAGE_RS485_PAGE_1, ROW_SECOND},
    {E_485_VOLTRONIC, PAGE_RS485_PAGE_1, ROW_THREE}, {E_485_GROWATT, PAGE_RS485_PAGE_1, ROW_LAST},
};
#endif

extern uint8_t getValidData;
uint8_t Time_Buffer[12] = {
    '2', '0', '9', '9', '0', '0', '0', '0', '0', '0', '0', '0',
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
    Lcd_showStringEN(ROW_FIRST, 0 * 8, "--", 0);
    Lcd_showStringEN(ROW_SECOND, 0 * 8, "--", 0);
    Lcd_showStringEN(ROW_THREE, 0 * 8, "--", 0);
    Lcd_showStringEN(ROW_LAST, 0 * 8, "--", 0);
    switch (Line_Num) {
        case 0: Lcd_showStringEN(ROW_FIRST, 0 * 8, "->", 1); break;
        case 1: Lcd_showStringEN(ROW_SECOND, 0 * 8, "->", 1); break;
        case 2: Lcd_showStringEN(ROW_THREE, 0 * 8, "->", 1); break;
        case 3: Lcd_showStringEN(ROW_LAST, 0 * 8, "->", 1); break;
        default: break;
    }

    switch (Old_Page_Status) {
        case PAGE_MENU_2:
        case PAGE_SYSTEMSET:
        case PAGE_PARASET:
#if PROTOCOL_EN_ALL == 1
        case PAGE_RS485_PAGE_4:
            Lcd_showStringEN(ROW_THREE, 0 * 8, "  ", 0);
            Lcd_showStringEN(ROW_LAST, 0 * 8, "  ", 0);
            break;
        case PAGE_CAN_PAGE_6:
            Lcd_showStringEN(ROW_SECOND, 0 * 8, "  ", 0);
            Lcd_showStringEN(ROW_THREE, 0 * 8, "  ", 0);
            Lcd_showStringEN(ROW_LAST, 0 * 8, "  ", 0);
            break;
#else
        case PAGE_CAN_PAGE_3:
            Lcd_showStringEN(ROW_THREE, 0 * 8, "  ", 0);
            Lcd_showStringEN(ROW_LAST, 0 * 8, "  ", 0);
            break;
#endif
        default: break;
    }

    uint8_t is_exist = 0;
    uint8_t go_row   = 0;
    for (int i = 0; i < PROTOCOL_ARR_LENGTH; ++i) {
        if (protocol_arr[i].protocol_page == Old_Page_Status) {
            /*APP_PRINT("page_protocol_t->rs485_protocol : 0x%02X, \n", page_protocol_t.rs485_protocol)
            APP_PRINT("page_protocol_t->can_protocol : 0x%02X,\n", page_protocol_t.can_protocol)
            APP_PRINT("protocol_arr[i].index : 0x%02X,\n", protocol_arr[i].index)*/
            if ((page_protocol_t.rs485_protocol > E_CAN_MAX_NUM && page_protocol_t.rs485_protocol == protocol_arr[i].index)
                || (page_protocol_t.can_protocol < E_CAN_MAX_NUM && page_protocol_t.can_protocol == protocol_arr[i].index)) {
                is_exist = 1;
                go_row   = protocol_arr[i].protocol_row;
                break;
            }
        }
    }
    if (is_exist) {
        Lcd_showChar6_8(go_row, 15 * 8 - 3, '^', 0);
    }
}

//======================================================================
// Function:	Page_Welcome_1()
// Description:
//======================================================================
void Page_Welcome_1(void) {
    // Page_Welcome();
    Page_Welcome_new();
    New_Page_Status = PAGE_WELCOME;
}
//======================================================================
// Function:	Page_Welcome()
// Description: 首页面
//======================================================================
void Show_TemperatureNull(uint8_t Row) { Lcd_showStringEN(Row, 9 * 8, "  -- ", 0); }

// ... existing code ...


// void Page_Welcome_new(void) {
//     uint8_t i, j;
//     const uint8_t *battery_icon = Battery_Icon_SOC_0_24x16;
//
//     /* 清空整个屏幕 */
//     for(j = 0; j < 8; j++) {
//         for(i = 0; i < 21; i++) {
//             Lcd_showCha6_7(j, i * 6, ' ', 1);
//         }
//     }
//
//     /* 第 0 行：BMS Status 标题 (居中) */
//     Lcd_showStringEN(0, 34, "BMS Status", 0);
//
//     /* 左侧SOC大数字 + 百分号 */
//     if (((Soc / 10) % 10) == 0) {
//         Lcd_showChar(2, 0, ' ', 0);
//     } else {
//         Lcd_showChar(2, 0, (uint8_t) ((Soc / 10) % 10) + '0', 0);
//     }
//     Lcd_showChar(2, 8, (uint8_t) (Soc % 10) + '0', 0);
//     Lcd_showChar(2, 20, '%', 0);
//
//     /* 左侧电池图标：每格25% */
//     if (Soc >= 100) {
//         battery_icon = Battery_Icon_SOC_100_24x16;
//     } else if (Soc >= 75) {
//         battery_icon = Battery_Icon_SOC_75_24x16;
//     } else if (Soc >= 50) {
//         battery_icon = Battery_Icon_SOC_50_24x16;
//     } else if (Soc >= 25) {
//         battery_icon = Battery_Icon_SOC_25_24x16;
//     } else {
//         battery_icon = Battery_Icon_SOC_0_24x16;
//     }
//     Display_Custom_Bitmap_8x16(2, 30, 24, battery_icon);
//
//     /* 右上总压框（闪电图标 + 预留电压显示区） */
//     Display_Custom_Bitmap_8x16(1, 60, 64, Right_Frame_Lightning_64x16);
//
//     /* 总压/电流：6x8 字体，只占框下半行，比左侧 SOC 的 8x16 更矮更小；列不超出屏宽 */
//     Show_TotalVoltage(2, 76, PackData.Vsum);
//     Lcd_showChar6_8(2, 118, 'V', 0);
//
//     /* 右下电流框（小图标 + 电流值区域） */
//     Display_Custom_Bitmap_8x16(4, 60, 64, Right_Frame_Current_64x16);
//     Show_Current(4, 68, PackData.Current);
//     Lcd_showChar6_8(4, 118, 'A', 0);
// }

void Page_Welcome_new(void) {
    uint8_t i, j;
    // extern const uint8_t Battery_Icon_SOC_0_30x16[60];
    const uint8_t *battery_icon = Battery_Icon_SOC_0_30x16;

     Soc = 100;

    /* 清空整个屏幕 */
    for(j = 0; j < 8; j++) {
        for(i = 0; i < 21; i++) {
            Lcd_showCha6_7(j, i * 6, ' ', 1);
        }
    }

    /* 第 0 行：BMS Status 标题 (居中) */
    Lcd_showStringEN(0, 34, "BMS Status", 0);

    /* 左侧SOC大数字 + 百分号 */
    if (Soc >= 100) {
        // 100% 的情况
        // Lcd_showChar_Shift4(1, 0, '1', 0);
        // Lcd_showChar_Shift4(1, 6, '0', 0);
        // Lcd_showChar_Shift4(1, 14, '0', 0);
        Lcd_showChar(3, 0, '1', 0);
        Lcd_showChar(3, 6, '0', 0);
        Lcd_showChar(3, 14, '0', 0);
        // Lcd_showChar(3, 26, '%', 0);
    } else {
        // 低于 100% 的情况：显示两位数
        if (((Soc / 10) % 10) == 0) {
            // Lcd_showChar_Shift4(1, 2, ' ', 0);
            Lcd_showChar(3, 2, ' ', 0);
        } else {
            // Lcd_showChar_Shift4(1, 2, (uint8_t) ((Soc / 10) % 10) + '0', 0);
            Lcd_showChar(3, 2, (uint8_t) ((Soc / 10) % 10) + '0', 0);
        }
        // Lcd_showChar_Shift4(1, 12, (uint8_t) (Soc % 10) + '0', 0);
        Lcd_showChar(3, 12, (uint8_t) (Soc % 10) + '0', 0);

    }
    Lcd_showChar(3, 23, '%', 0);
    //  选择电池图标
    if (Soc >= 100) {
        battery_icon = Battery_Icon_SOC_100_30x16;
    } else if (Soc >= 75) {
        battery_icon = Battery_Icon_SOC_75_30x16;
    } else if (Soc >= 50) {
        battery_icon = Battery_Icon_SOC_50_30x16;
    } else if (Soc >= 25) {
        battery_icon = Battery_Icon_SOC_25_30x16;
    } else {
        battery_icon = Battery_Icon_SOC_0_30x16;
    }

    // 显示位移 4 像素后的电池图标 (起始 Row 1 + 4px = Centered in Box)
    Display_Custom_Bitmap_8x16(3, 30, 30, battery_icon);
    
    /* 右上总压框（闪电图标 + 预留电压显示区） */
    Display_Custom_Bitmap_8x24(1, 60, 64, Right_Frame_Lightning_64x24);

    /* 总压/电流：6x8 字体，只占框下半行，比左侧 SOC 的 8x16 更矮更小；列不超出屏宽 */
    Show_TotalVoltage(2, 76, PackData.Vsum);
    Lcd_showChar6_8(2, 116, 'V', 0);

    /* 右下电流框（小图标 + 电流值区域） */
    Display_Custom_Bitmap_8x24(4, 60, 64, Right_Frame_Current_64x24);
    Show_Current(5, 78, PackData.Current); // 起始位置进一步右移，防止负号重叠图标
    Lcd_showChar6_8(5, 116, 'A', 0); // 单位靠右

    /* ====== 底部反白状态栏 (12像素高度) ====== */
    //  铺满底部黑条背景 (Page 6 底部 4 像素 + Page 7 全满 8 像素)
    // Lcd12864_Addr(6, 4); // 定位到 Page 6
    // for(i = 0; i < 128; i++) {
    //     LcdSend_Data(0xC0); // 填充 Page 6 底部的 2 像素
    // }
    // Lcd12864_Addr(7, 0); // 定位到第 7 页（最底部），第 0 列
    // for(i = 0; i < 128; i++) {
    //     LcdSend_Data(0xFF); // 写入 128 个字节的 0x00
    // }
    // for(i = 0; i < 21; i++) {
    //     Lcd_showStringEN(7, i * 6, " ", 1);
    // }
    // Lcd_showStringEN(7, 122, " ", 1);


    // Lcd_showStringEN(5, 0, "WARING:", 0);  // 固定显示 WARN
    {
        const char *warn_code = " ";  // 默认空白
        if      (PackData.Prp_State.BitName.bShortFault) warn_code = "SC";
        else if (PackData.Prp_State.BitName.bNeg)        warn_code = "NEG";//反接保护
        else if (PackData.Prp_State.BitName.bSmpErr)     warn_code = "SMP!ERR";//采样错误(字库中"!"为"_")
        else if (PackData.Prp_State.BitName.bChipSmpErr) warn_code = "CHIP!ERR";//芯片采样错误
        else if (PackData.Prp_State.BitName.bCellVoltOV) warn_code = "OV";
        else if (PackData.Prp_State.BitName.bCellVoltUL) warn_code = "UV";
        else if (PackData.Prp_State.BitName.bTempOV)     warn_code = "OT";
        else if (PackData.Prp_State.BitName.bTempUL)     warn_code = "UT";
        else if (PackData.Prp_State.BitName.bCurrOV)     warn_code = "OC";
        else if (PackData.Prp_State.BitName.bChgMosErr)  warn_code = "CMOS!ERR";
        else if (PackData.Prp_State.BitName.bDisMosErr)  warn_code = "DMOS!ERR";
        Lcd_showString_ShiftUp(6, 0, warn_code, 0,4);  // 列偏移30px，紧跟WARN后
    }


    // 最左边缩进4像素并打印 "TEMP "
    Lcd_showStringEN(7, 0, "TEMP:", 0);

    /* 显示电芯平均温度 BAT_TEMP */
    {
        int16_t t_avg = PackData.BAT_TEMP;//平均温度
        // int16_t t_avg = 275;
        uint8_t temp_col = 28; //  "TEMP:"与温度的起始间距

        if (t_avg < 0) {
            Lcd_showChar6_8(7, temp_col, '-', 0);
            temp_col += 4;
            t_avg = -t_avg;
        }
        // 显示百位数 (例如 >= 100.0)
        if (t_avg >= 1000) {
            Lcd_showChar6_8(7, temp_col, (t_avg/1000)%10 + '0', 0);
            temp_col += 6;
        }
        // 显示十位数 (例如 >= 10.0)
        if (t_avg >= 100) {
            Lcd_showChar6_8(7, temp_col, (t_avg/100)%10 + '0', 0);
            temp_col += 6;
        }
        // 显示个位数 (始终显示)
        Lcd_showChar6_8(7, temp_col, (t_avg/10)%10 + '0', 0);
        temp_col += 6;
        
        // 显示小数点
        Lcd_showChar6_8(7, temp_col, '.', 0);
        temp_col += 6;
        // 显示小数位
        Lcd_showChar6_8(7, temp_col, t_avg%10 + '0', 0);
        temp_col += 6;
        
        // 打印摄氏度符号 _ 为度数, C
        Lcd_showChar6_8(7, temp_col, '_', 0);
        temp_col += 6;
        Lcd_showChar6_8(7, temp_col, 'C', 0);
    }

    // 往右调整并显示 "Status:" (或者将其缩短)，给左边的小数点留出显示空间
    // Lcd_showStringEN(7, 74, "Stat:", 0);




}

// ... existing code ...










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
        Lcd_showCha6_7(0, 17 * 6, (uint8_t) ((SOH / 100) % 10) + '0', 1);

    if ((((SOH / 10) % 10) == 0) && (((SOH / 100) % 10) == 0)) Lcd_showCha6_7(0, 18 * 6, ' ', 0);
    else
        Lcd_showCha6_7(0, 18 * 6, (uint8_t) ((SOH / 10) % 10) + '0', 1);
    Lcd_showCha6_7(0, 19 * 6, (uint8_t) (SOH % 10) + '0', 1);
    Lcd_showStringENG(0, 20 * 6, " ", 0); // 反白

    /*首页图片*/
    Lcd_showChar(3, 8 * 8, '#', 0); // show "电芯温度"
    Lcd_showChar(5, 8 * 8, '!', 0); // show SOC图片
    Lcd_showChar(5, 9 * 8, '&', 0); // show "SOC图片"
    Lcd_showChar(1, 8 * 8, '+', 0); // show "环境温度图片"

    /*首页总压*/
    Lcd_showChar(3, 8 * 6, 'V', 0); // show "V"

    Show_TotalVoltage1(3, 0 * 8, (PackData.Vsum));

    /*充放电电流*/
    Lcd_showChar(1, 8 * 6, 'A', 0); // show "A"

    Show_Current1(1, 0 * 8, (PackData.Current));

    /*剩余容量*/
    Lcd_showChar(5, 5 * 8, 'A', 0); // show "A"
    Lcd_showChar(5, 6 * 8, 'H', 0); // show "H"

    /*电芯温度*/
    Show_Temperature1(3, 9 * 8, GetHighTemp());
    Lcd_showChar6_16(3, 13 * 8, '#', 0); // 摄氏度

    /*环境温度*/
    Show_Temperature1(1, 9 * 8, PackData.s16EnvirTemp);
    Lcd_showChar6_16(1, 13 * 8, '#', 0); // 摄氏度
}

//======================================================================
// Function:	Page_Menu_1()
// Description:菜单页面
//======================================================================
void Page_Menu_1(void) {
    Show_SelectCh(SelectCH_Line_Index);
    Lcd_showStringEN(ROW_FIRST, 2 * 8, "BMS Parameter   ", 0);
    Lcd_showChar6_8(ROW_FIRST, 15 * 8, '<', 0); // show "》"

    Lcd_showStringEN(ROW_SECOND, 2 * 8, "Battery Status  ", 0);
    Lcd_showChar6_8(ROW_SECOND, 15 * 8, '<', 0); // show "》"

    Lcd_showStringEN(ROW_THREE, 2 * 8, "GYRO Status     ", 0);
    Lcd_showChar6_8(ROW_THREE, 15 * 8, '<', 0); // show "》"

    Lcd_showStringEN(ROW_LAST, 2 * 8, "Version Number  ", 0);
    Lcd_showChar6_8(ROW_LAST, 15 * 8, '<', 0); // show "》"
}
//======================================================================
// Function:	Page_Menu_2()
// Description:菜单页面
//======================================================================
void Page_Menu_2(void) {
    Show_SelectCh(SelectCH_Line_Index);
    Lcd_showStringEN(ROW_FIRST, 2 * 8, "CAN Protocol    ", 0);
    Lcd_showChar6_8(ROW_FIRST, 15 * 8, '<', 0); // show "》"
    Lcd_showStringEN(ROW_SECOND, 2 * 8, "RS485 Protocol  ", 0);
    Lcd_showChar6_8(ROW_SECOND, 15 * 8, '<', 0); // show "》"
}

//======================================================================
// Function:	Page_Analog()
// Description:
//======================================================================
void Page_Analog_1(void) {
    Show_SelectCh(SelectCH_Line_Index);

    Lcd_showStringEN(ROW_FIRST, 2 * 8, "Voltage:         V", 0);
    Lcd_showStringEN(ROW_SECOND, 2 * 8, "Current:         A", 0);
    Lcd_showStringEN(ROW_THREE, 2 * 8, "CellTemp      ", 0);
    Lcd_showStringEN(ROW_LAST, 2 * 8, "CellVolt      ", 0);
    Lcd_showChar6_8(ROW_THREE, 11 * 8, '<', 0); // show "》"
    Lcd_showChar6_8(ROW_LAST, 11 * 8, '<', 0);  // show "》"
}

//======================================================================
// Function:	Page_Status()
// Description:
//======================================================================
void Page_Status(void) {
    Show_SelectCh(SelectCH_Line_Index);

    Lcd_showStringEN(ROW_FIRST, 2 * 8, "Status:       ", 0);
    Lcd_showStringEN(ROW_SECOND, 2 * 8, "Alarm Status ", 0);
    Lcd_showStringEN(ROW_THREE, 2 * 8, "Protect Status", 0);
    Lcd_showStringEN(ROW_LAST, 2 * 8, "Failure Alarm", 0);

    Lcd_showChar6_8(ROW_SECOND, 15 * 8, '<', 0); // show "》"
    Lcd_showChar6_8(ROW_THREE, 15 * 8, '<', 0);  // show "》"
    Lcd_showChar6_8(ROW_LAST, 15 * 8, '<', 0);   // show "》"
}

/*******************************************************************************
 * Function:	Page_ParaSet()
 * Description:
 *陀螺仪设置（选配）默认关闭
 *
 *******************************************************************************/
void Page_ParaSet(void) {
    Show_SelectCh(SelectCH_Line_Index);

    Lcd_showStringEN(ROW_FIRST, 2 * 8, "Set X axis:   ", 0);
    Lcd_showStringEN(ROW_SECOND, 2 * 8, "Place Option: ", 0);
}

//======================================================================
// Function:	Page_SystemSet()
// Description:
//======================================================================
void Page_SystemSet(void) {
    Show_SelectCh(SelectCH_Line_Index);

    Lcd_showStringEN(ROW_FIRST, 2 * 8, "BMS Version  ", 0);
    Lcd_showStringEN(ROW_SECOND, 2 * 8, "LCD Version  ", 0);
    Lcd_showChar6_8(ROW_FIRST, 15 * 8, '<', 0);  // show ""
    Lcd_showChar6_8(ROW_SECOND, 15 * 8, '<', 0); // show ""
}

//======================================================================
// Function:	Page_Temperature_1()
// Description:
//======================================================================
void Page_Temperature_1(void) {
    Lcd_showStringEN(ROW_FIRST, 0 * 6, "--Temp01           C", 0);
    Lcd_showStringEN(ROW_SECOND, 0 * 6, "--Temp02           C", 0);
    Lcd_showStringEN(ROW_THREE, 0 * 6, "--Temp03           C", 0);
    Lcd_showStringEN(ROW_LAST, 0 * 6, "--Temp04           C", 0);
    Lcd_showChar6_8(ROW_FIRST, 18 * 6, '_', 0); // show "摄氏度"
    Lcd_showChar6_8(ROW_SECOND, 18 * 6, '_', 0);
    Lcd_showChar6_8(ROW_THREE, 18 * 6, '_', 0);
    Lcd_showChar6_8(ROW_LAST, 18 * 6, '_', 0);
}
//======================================================================
// Function:	Page_Temperature_2()
// Description: 用8路NTC时可打开
//======================================================================
void Page_Temperature_2(void) {
    Lcd_showStringEN(ROW_FIRST, 0 * 6, "--Temp05:      C", 0);
    Lcd_showStringEN(ROW_SECOND, 0 * 6, "--Temp06:      C", 0);
    Lcd_showStringEN(ROW_THREE, 0 * 6, "--Temp07:      C", 0);
    Lcd_showStringEN(ROW_LAST, 0 * 6, "--Temp08:      C", 0);
    Lcd_showChar6_8(ROW_FIRST, 18 * 6, '_', 0);  // show "C"
    Lcd_showChar6_8(ROW_SECOND, 18 * 6, '_', 0); // show "C"
    Lcd_showChar6_8(ROW_THREE, 18 * 6, '_', 0);  // show "C"
    Lcd_showChar6_8(ROW_LAST, 18 * 6, '_', 0);   // show "C"
}
//======================================================================
// Function:	Page_Temperature_3()
// Description:
//======================================================================
void Page_Temperature_3(void) {
    Lcd_showStringEN(ROW_FIRST, 0 * 6, "MOS Temp           C", 0); // PCB--> MOS
    Lcd_showStringEN(ROW_SECOND, 0 * 6, "Env Temp           C", 0);
    Lcd_showChar6_8(ROW_FIRST, 18 * 6, '_', 0);  // show "C"
    Lcd_showChar6_8(ROW_SECOND, 18 * 6, '_', 0); // show "C"
}

//======================================================================
// Function:	Page_Voltage_1()
// Description:
//======================================================================
void Page_Voltage_1(void) {
    Lcd_showStringEN(ROW_FIRST, 0 * 8, "--Cell01          mV", 0);
    Lcd_showStringEN(ROW_SECOND, 0 * 8, "--Cell02          mV", 0);
    Lcd_showStringEN(ROW_THREE, 0 * 8, "--Cell03          mV", 0);
    Lcd_showStringEN(ROW_LAST, 0 * 8, "--Cell04          mV", 0);
}

//======================================================================
// Function:	Page_Voltage_2()
// Description:
//======================================================================
void Page_Voltage_2(void) {
    Lcd_showStringEN(ROW_FIRST, 0 * 8, "--Cell05          mV", 0);
    Lcd_showStringEN(ROW_SECOND, 0 * 8, "--Cell06          mV", 0);
    Lcd_showStringEN(ROW_THREE, 0 * 8, "--Cell07          mV", 0);
    Lcd_showStringEN(ROW_LAST, 0 * 8, "--Cell08          mV", 0);
}

//======================================================================
// Function:	Page_Voltage_3()
// Description:
//======================================================================
void Page_Voltage_3(void) {
    Lcd_showStringEN(ROW_FIRST, 0 * 8, "--Cell09          mV", 0);
    Lcd_showStringEN(ROW_SECOND, 0 * 8, "--Cell10          mV", 0);
    Lcd_showStringEN(ROW_THREE, 0 * 8, "--Cell11          mV", 0);
    Lcd_showStringEN(ROW_LAST, 0 * 8, "--Cell12          mV", 0);
}

//======================================================================
// Function:	Page_Voltage_4()
// Description:
//======================================================================
void Page_Voltage_4(void) {
    Lcd_showStringEN(ROW_FIRST, 0 * 8, "--Cell13          mV", 0);
    Lcd_showStringEN(ROW_SECOND, 0 * 8, "--Cell14          mV", 0);
    Lcd_showStringEN(ROW_THREE, 0 * 8, "--Cell15          mV", 0);
    Lcd_showStringEN(ROW_LAST, 0 * 8, "--Cell16          mV", 0);
}

//======================================================================
// Function:	Page_Capacity()
// Description:
//======================================================================
void Page_Capacity(void) {
    Lcd_showStringEN(ROW_FIRST, 0 * 8, "   SOC    :        %", 0);
    // Lcd_showStringEN(ROW_SECOND, 0 * 8, "Nominal   :        AH", 0);
    // Lcd_showStringEN(ROW_THREE, 0 * 8, "Remain    :        AH", 0);//
    Lcd_showStringEN(ROW_SECOND, 0 * 8, "BMS Cycles:         0", 0);
}

//======================================================================
// Function:	Page_Warning_1()
// Description:
//======================================================================
void Page_Warning_1(void) {
    Lcd_showStringEN(ROW_FIRST, 0 * 8, "--Over Volt :   ", 0);
    Lcd_showStringEN(ROW_SECOND, 0 * 8, "--Low Volt  :   ", 0);
    Lcd_showStringEN(ROW_THREE, 0 * 8, "--Over Temp :   ", 0);
    Lcd_showStringEN(ROW_LAST, 0 * 8, "--Low Temp  :   ", 0);
}

//======================================================================
// Function:	Page_Warning_2()
// Description:
//======================================================================
void Page_Warning_2(void) {
    Lcd_showStringEN(ROW_FIRST, 0 * 8, "--Low SOC   :   ", 0);
    Lcd_showStringEN(ROW_SECOND, 0 * 8, "--Diff Volt :   ", 0);
    Lcd_showStringEN(ROW_THREE, 0 * 8, "--Over Curr :   ", 0);
    Lcd_showStringEN(ROW_LAST, 0 * 8, "--Reverse   :   ", 0);
}

//======================================================================
// Function:	Page_Protection_1()
// Description:
//======================================================================
void Page_Protection_1(void) {
    Lcd_showStringEN(ROW_FIRST, 0 * 8, "--Over Volt :   ", 0);
    Lcd_showStringEN(ROW_SECOND, 0 * 8, "--Low Volt  :   ", 0);
    Lcd_showStringEN(ROW_THREE, 0 * 8, "--Over Temp :   ", 0);
    Lcd_showStringEN(ROW_LAST, 0 * 8, "--Low Temp  :   ", 0);
}

//======================================================================
// Function:	Page_Protection_2()
// Description:
//======================================================================
void Page_Protection_2(void) {
    Lcd_showStringEN(ROW_FIRST, 0 * 8, "--Over Curr :   ", 0);
    Lcd_showStringEN(ROW_SECOND, 0 * 8, "--Short Curr:   ", 0);
}

/*******************************************************************************
 * Function:	Page_FailAlarm_1()
 * Description: show the Fail Alarm info in 3 pages
 *******************************************************************************/
void Page_FailAlarm_1(void) {
    Lcd_showStringEN(ROW_FIRST, 0 * 8, "--Sample Line:  ", 0);
    Lcd_showStringEN(ROW_SECOND, 0 * 8, "--Charge MOS :  ", 0);
    Lcd_showStringEN(ROW_THREE, 0 * 8, "--DisCHG MOS :  ", 0);
    Lcd_showStringEN(ROW_LAST, 0 * 8, "--Sample Chip:  ", 0);
}

/*******************************************************************************
 * Function:	Page_FailAlarm_2()
 * Description: show the Fail Alarm info in 3 pages
 *******************************************************************************/
void Page_FailAlarm_2(void) {
    Lcd_showStringEN(ROW_FIRST, 0 * 8, "SC Times   :    ", 0);
    Lcd_showStringEN(ROW_SECOND, 0 * 8, "OverTempCNT:    ", 0);
    Lcd_showStringEN(ROW_THREE, 0 * 8, "OverCurrCNT:    ", 0);
    Lcd_showStringEN(ROW_LAST, 0 * 8, "OverChgCNT :    ", 0);
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
        if (j > 29) // 规定长度
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
 * KS-V2.1.1:更改首页电流显示三位后面不清空问题,剩余容量
 * KS-V2.1.3:更改首页为满充容量,增加协议选择
 * KS-V2.1.4:更改首页为剩余容量,增加协议选择
 * KS-V2.1.5:更改首页为剩余容量,协议选择-考虑自动分配地址
 * KS-V2.1.6:更改首页为满充容量,协议选择-考虑自动分配地址
 * KS-V2.1.7:更改首页为满充容量,协议选择-考虑自动分配地址
 *******************************************************************************/
void Page_LCD_Version(void) {
   Lcd_showStringEN(ROW_FIRST, 0 * 8, "LCD SW Version: ", 0); // 主板协议回复不在判断cid1
   Lcd_showStringEN(ROW_SECOND, 0 * 8, " LG-V3.0.0", 0);
    Lcd_showStringEN(ROW_THREE, 0 * 8, "LCD HW Version: ", 0);
    Lcd_showStringEN(ROW_LAST, 0 * 8, " LG-LCD-03-V1", 0);
}

/*******************************************************************************
 * Function:	GyroSet()
 * Description: Gyro setting process 设置过程
 *******************************************************************************/
void GyroSet(void) {
    static uint8_t ParaSetFlag = 0;
    static uint8_t PlaceFlag   = 0;
    static uint8_t X_axisFlag  = 0;
    // 菜单切换
    if (0 == ParaSetFlag) {
        if ((Key_Value & BIT2)) {
            if (SelectCH_Line_Index == 0) {
                SelectCH_Line_Index = 2;
            }
            SelectCH_Line_Index--;
            Show_SelectCh(SelectCH_Line_Index);
        } else if ((Key_Value & BIT3)) {
            if (++SelectCH_Line_Index >= 2) {
                SelectCH_Line_Index = 0;
            }
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
            ParaSetFlag = 0; // 返回页面初始值
        } else {
            New_Page_Status     = PAGE_MENU_1; // 回主菜单
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
                        SetCommondFlag = 1;   //Uart_SetParaCMD_1(X_axisFlag);
     //发送X轴指令 ParaSetFlag = 0; X_axisFlag_COM = X_axisFlag;
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
                        SetCommondFlag = 2;   //Uart_SetParaCMD_2(PlaceFlag);
     //陀螺仪垂直、水平指令 ParaSetFlag = 0; PlaceFlag_COM = PlaceFlag;
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
#if PROTOCOL_EN_ALL == 1
/*******************************************************************************
 * Function:	page_can_protocol_1()
 * Description: show the Can Protocol info in 1 pages
 *******************************************************************************/
void page_can_protocol_1(void) {
    Show_SelectCh(SelectCH_Line_Index);
    Lcd_showStringEN(ROW_FIRST, 2 * 8, "CAN Victron      ", 0);
    Lcd_showStringEN(ROW_SECOND, 2 * 8, "CAN Pylon       ", 0);
    Lcd_showStringEN(ROW_THREE, 2 * 8, "CAN Goodwe      ", 0);
    Lcd_showStringEN(ROW_LAST, 2 * 8, "CAN Growatt     ", 0);
}

/*******************************************************************************
 * Function:	page_can_protocol_2()
 * Description: show the Can Protocol info in 2 pages
 *******************************************************************************/
void page_can_protocol_2(void) {
    Show_SelectCh(SelectCH_Line_Index);
    Lcd_showStringEN(ROW_FIRST, 2 * 8, "CAN Voltronic   ", 0);
    Lcd_showStringEN(ROW_SECOND, 2 * 8, "CAN LXP         ", 0);
    Lcd_showStringEN(ROW_THREE, 2 * 8, "CAN Deye        ", 0);
    Lcd_showStringEN(ROW_LAST, 2 * 8, "CAN Sofar       ", 0);
}

/*******************************************************************************
 * Function:	page_can_protocol_3()
 * Description: show the Can Protocol info in 3 pages
 *******************************************************************************/
void page_can_protocol_3(void) {
    Show_SelectCh(SelectCH_Line_Index);
    Lcd_showStringEN(ROW_FIRST, 2 * 8, "CAN Solis       ", 0);
    Lcd_showStringEN(ROW_SECOND, 2 * 8, "CAN SunGrow     ", 0);
    Lcd_showStringEN(ROW_THREE, 2 * 8, "CAN Studer      ", 0);
    Lcd_showStringEN(ROW_LAST, 2 * 8, "CAN Huawei      ", 0);
}

/*******************************************************************************
 * Function:	page_can_protocol_4()
 * Description: show the Can Protocol info in 4 pages
 *******************************************************************************/
void page_can_protocol_4(void) {
    Show_SelectCh(SelectCH_Line_Index);
    Lcd_showStringEN(ROW_FIRST, 2 * 8, "CAN SMA         ", 0);
    Lcd_showStringEN(ROW_SECOND, 2 * 8, "CAN INVT        ", 0);
    Lcd_showStringEN(ROW_THREE, 2 * 8, "CAN Kstar       ", 0);
    Lcd_showStringEN(ROW_LAST, 2 * 8, "CAN MUST        ", 0);
}

/*******************************************************************************
 * Function:	page_can_protocol_5()
 * Description: show the Can Protocol info in 5 pages
 *******************************************************************************/
void page_can_protocol_5(void) {
    Show_SelectCh(SelectCH_Line_Index);
    Lcd_showStringEN(ROW_FIRST, 2 * 8, "CAN SRNE        ", 0);
    Lcd_showStringEN(ROW_SECOND, 2 * 8, "CAN FoxEss      ", 0);
    Lcd_showStringEN(ROW_THREE, 2 * 8, "CAN BST         ", 0);
    Lcd_showStringEN(ROW_LAST, 2 * 8, "CAN AISWEI      ", 0);
}

/*******************************************************************************
 * Function:	page_can_protocol_6()
 * Description: show the Can Protocol info in 6 pages
 *******************************************************************************/
void page_can_protocol_6(void) {
    Show_SelectCh(SelectCH_Line_Index);
    Lcd_showStringEN(ROW_FIRST, 2 * 8, "CAN Schneider   ", 0);
}

/*******************************************************************************
 * Function:	page_rs485_protocol_1()
 * Description: show the RS485 Protocol info in 1 pages
 *******************************************************************************/
void page_rs485_protocol_1(void) {
    Show_SelectCh(SelectCH_Line_Index);
    Lcd_showStringEN(ROW_FIRST, 2 * 8, "RS485 Pylon     ", 0);
    Lcd_showStringEN(ROW_SECOND, 2 * 8, "RS485 SMK Solar ", 0);
    Lcd_showStringEN(ROW_THREE, 2 * 8, "RS485 Voltronic ", 0);
    Lcd_showStringEN(ROW_LAST, 2 * 8, "RS485 Growatt   ", 0);
}

/*******************************************************************************
 * Function:	page_rs485_protocol_2()
 * Description: show the RS485 Protocol info in 2 pages
 *******************************************************************************/
void page_rs485_protocol_2(void) {
    Show_SelectCh(SelectCH_Line_Index);
    Lcd_showStringEN(ROW_FIRST, 2 * 8, "RS485 SRNE      ", 0);
    Lcd_showStringEN(ROW_SECOND, 2 * 8, "RS485 GP        ", 0);
    Lcd_showStringEN(ROW_THREE, 2 * 8, "RS485 HengRui   ", 0);
    Lcd_showStringEN(ROW_LAST, 2 * 8, "RS485 Wolong    ", 0);
}

/*******************************************************************************
 * Function:	page_rs485_protocol_3()
 * Description: show the RS485 Protocol info in 3 pages
 *******************************************************************************/
void page_rs485_protocol_3(void) {
    Show_SelectCh(SelectCH_Line_Index);
    Lcd_showStringEN(ROW_FIRST, 2 * 8, "RS485 SacredSun ", 0);
    Lcd_showStringEN(ROW_SECOND, 2 * 8, "RS485 LI BAT    ", 0);
    Lcd_showStringEN(ROW_THREE, 2 * 8, "RS485 LingYu    ", 0);
    Lcd_showStringEN(ROW_LAST, 2 * 8, "RS485 ELTEK     ", 0);
}

/*******************************************************************************
 * Function:	page_rs485_protocol_4()
 * Description: show the RS485 Protocol info in 4 pages
 *******************************************************************************/
void page_rs485_protocol_4(void) {
    Show_SelectCh(SelectCH_Line_Index);
    Lcd_showStringEN(ROW_FIRST, 2 * 8, "RS485 Huawei    ", 0);
    Lcd_showStringEN(ROW_SECOND, 2 * 8, "RS485 VIRTIV    ", 0);
}
#else
/*******************************************************************************
 * Function:	page_can_protocol_1()
 * Description: show the Can Protocol info in 1 pages
 *******************************************************************************/
void page_can_protocol_1(void) {
    Show_SelectCh(SelectCH_Line_Index);
    Lcd_showStringEN(ROW_FIRST, 2 * 8, "CAN Victorn     ", 0);
    Lcd_showStringEN(ROW_SECOND, 2 * 8, "CAN Pylon       ", 0);
    Lcd_showStringEN(ROW_THREE, 2 * 8, "CAN Growatt     ", 0);
    Lcd_showStringEN(ROW_LAST, 2 * 8, "CAN LXP         ", 0);
}

/*******************************************************************************
 * Function:	page_can_protocol_2()
 * Description: show the Can Protocol info in 2 pages
 *******************************************************************************/
void page_can_protocol_2(void) {
    Show_SelectCh(SelectCH_Line_Index);
    Lcd_showStringEN(ROW_FIRST, 2 * 8, "CAN Deye        ", 0);
    Lcd_showStringEN(ROW_SECOND, 2 * 8, "CAN Sofar       ", 0);
    Lcd_showStringEN(ROW_THREE, 2 * 8, "CAN Solis       ", 0);
    Lcd_showStringEN(ROW_LAST, 2 * 8, "CAN SMA        ", 0);
}

/*******************************************************************************
 * Function:	page_can_protocol_3()
 * Description: show the Can Protocol info in 3 pages
 *******************************************************************************/
void page_can_protocol_3(void) {
    Show_SelectCh(SelectCH_Line_Index);
    Lcd_showStringEN(ROW_FIRST, 2 * 8, "CAN MUST       ", 0);
    Lcd_showStringEN(ROW_SECOND, 2 * 8, "CAN AISWEI     ", 0);
}
/*******************************************************************************
 * Function:	page_rs485_protocol_1()
 * Description: show the RS485 Protocol info in 1 pages
 *******************************************************************************/
void page_rs485_protocol_1(void) {
    Show_SelectCh(SelectCH_Line_Index);
    Lcd_showStringEN(ROW_FIRST, 2 * 8, "RS485 Pylon     ", 0);
    Lcd_showStringEN(ROW_SECOND, 2 * 8, "RS485 SRNE      ", 0);
    Lcd_showStringEN(ROW_THREE, 2 * 8, "RS485 Voltronic ", 0);
    Lcd_showStringEN(ROW_LAST, 2 * 8, "RS485 Growatt   ", 0);
}

#endif
void choose_clear() {
    Lcd_showChar6_8(ROW_FIRST, 15 * 8, ' ', 0);
    Lcd_showChar6_8(ROW_SECOND, 15 * 8, ' ', 0);
    Lcd_showChar6_8(ROW_THREE, 15 * 8, ' ', 0);
    Lcd_showChar6_8(ROW_LAST, 15 * 8, ' ', 0);
}

void choose_row(uint8_t index, uint8_t row_addr) {
    BMS_RT_INFO_T *p_bms = bms_get_rt_info();
    if (p_bms->bms_addr != 0 && p_bms->bms_addr != 1) {
        // 只有0 才可以通过屏幕修改协议
        // + 地址1 自动分配地址 add 2024年10月29日11:13:56
        return;
    }
    for (int i = 0; i < PROTOCOL_ARR_LENGTH; ++i) {
        if (protocol_arr[i].protocol_page == index && protocol_arr[i].protocol_row == (row_addr * 2)) {
            if (protocol_arr[i].index > E_CAN_MAX_NUM) {
                page_protocol_t.rs485_protocol = protocol_arr[i].index;
            } else {
                page_protocol_t.can_protocol = protocol_arr[i].index;
            }
        }
    }
    D_BMS_SET_OPTION_PARAM_T *bms_set_opt_t = bms_get_set_opt_info();
    bms_set_opt_t->page                     = Old_Page_Status;
    bms_set_opt_t->is_finished              = false;
    bms_set_opt_t->is_option                = true;
    switch (row_addr) {
        case 0:
            Lcd_showChar6_8(ROW_FIRST, 15 * 8 - 3, '^', 0);
            Lcd_showChar6_8(ROW_SECOND, 15 * 8 - 3, ' ', 0);
            Lcd_showChar6_8(ROW_THREE, 15 * 8 - 3, ' ', 0);
            Lcd_showChar6_8(ROW_LAST, 15 * 8 - 3, ' ', 0);
            break;
        case 1:
            Lcd_showChar6_8(ROW_FIRST, 15 * 8 - 3, ' ', 0);
            Lcd_showChar6_8(ROW_SECOND, 15 * 8 - 3, '^', 0);
            Lcd_showChar6_8(ROW_THREE, 15 * 8 - 3, ' ', 0);
            Lcd_showChar6_8(ROW_LAST, 15 * 8 - 3, ' ', 0);
            break;
        case 2:
            Lcd_showChar6_8(ROW_FIRST, 15 * 8 - 3, ' ', 0);
            Lcd_showChar6_8(ROW_SECOND, 15 * 8 - 3, ' ', 0);
            Lcd_showChar6_8(ROW_THREE, 15 * 8 - 3, '^', 0);
            Lcd_showChar6_8(ROW_LAST, 15 * 8 - 3, ' ', 0);
            break;
        case 3:
            Lcd_showChar6_8(ROW_FIRST, 15 * 8 - 3, ' ', 0);
            Lcd_showChar6_8(ROW_SECOND, 15 * 8 - 3, ' ', 0);
            Lcd_showChar6_8(ROW_THREE, 15 * 8 - 3, ' ', 0);
            Lcd_showChar6_8(ROW_LAST, 15 * 8 - 3, '^', 0);
            break;
        default: break;
    }
}

void press_menu_key(void) {
    switch (Old_Page_Status) {
        case PAGE_WELCOME: //新增按键操作，数据更新
        case PAGE_MENU_1:
        case PAGE_MENU_2:
        case PAGE_ANALOG_1:
        // case PAGE_ANALOG_2:
        case PAGE_STATUS:
        case PAGE_PARASET:
        case PAGE_SYSTEMSET:
        case PAGE_TEMPERATURE_1:
        // case PAGE_TEMPERATURE_2: //8路NTC
        case PAGE_TEMPERATURE_3:
        case PAGE_VOLTAGE_1:
        case PAGE_VOLTAGE_2:
        case PAGE_VOLTAGE_3:
        case PAGE_VOLTAGE_4:
        case PAGE_CAPACITY:
        case PAGE_WARNING_1:
        case PAGE_WARNING_2:
        case PAGE_PROTECTION_1:
        case PAGE_PROTECTION_2:
        // case PAGE_PROTECTION_3:
        case PAGE_FAILALARM_1:
        case PAGE_FAILALARM_2:
        case PAGE_FAILALARM_3:
        case PAGE_BMS_VERSION:
        case PAGE_LCD_VERSION:
            New_Page_Status = PAGE_MENU_1;
            if (Old_Page_Status != PAGE_MENU_1) {
                SelectCH_Line_Index = 0;
            }
            break;
        case PAGE_CAN_PAGE_1:
        case PAGE_CAN_PAGE_2:
        case PAGE_CAN_PAGE_3:
        case PAGE_CAN_PAGE_4:
        case PAGE_CAN_PAGE_5:
        case PAGE_CAN_PAGE_6:
            New_Page_Status     = PAGE_MENU_2;
            SelectCH_Line_Index = 0;
            break;
        case PAGE_RS485_PAGE_1:
        case PAGE_RS485_PAGE_2:
        case PAGE_RS485_PAGE_3:
        case PAGE_RS485_PAGE_4:
            New_Page_Status     = PAGE_MENU_2;
            SelectCH_Line_Index = 1;
            break;
        default: New_Page_Status = PAGE_MENU_1; break;
    }
}

void press_ok_key(void) {
    switch (Old_Page_Status) {
        case PAGE_WELCOME: //新增按键操作，数据更新
            break;
        case PAGE_MENU_1:
            if (SelectCH_Line_Index == 0) {
                New_Page_Status = PAGE_ANALOG_1;
            } else if (SelectCH_Line_Index == 1) {
                New_Page_Status = PAGE_STATUS;
            } else if (SelectCH_Line_Index == 2) {
                New_Page_Status = PAGE_PARASET;
                SetCommondFlag  = 3;
                Uart_GetParaCMD_1(); // 读X轴指令
                Delay_ms(10);
                Uart_GetParaCMD_2(); // 读水平、垂直指令
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
            break;
        case PAGE_MENU_2:
            if (SelectCH_Line_Index == 0) {
                New_Page_Status = PAGE_CAN_PAGE_1;
            } else if (SelectCH_Line_Index == 1) {
                New_Page_Status = PAGE_RS485_PAGE_1;
            }
            SelectCH_Line_Index = 0;
            break;
        case PAGE_ANALOG_1:
            if (SelectCH_Line_Index == 2) {
                New_Page_Status     = PAGE_TEMPERATURE_1;
                SelectCH_Line_Index = 0;
            } else if (SelectCH_Line_Index == 3) {
                New_Page_Status     = PAGE_VOLTAGE_1;
                SelectCH_Line_Index = 0;
            }
            break;
        // case PAGE_ANALOG_2:
        case PAGE_STATUS:
            if (SelectCH_Line_Index == 1) {
                New_Page_Status     = PAGE_WARNING_1;
                SelectCH_Line_Index = 0;
            } else if (SelectCH_Line_Index == 2) {
                New_Page_Status     = PAGE_PROTECTION_1;
                SelectCH_Line_Index = 0;
            } else if (SelectCH_Line_Index == 3) {
                New_Page_Status     = PAGE_FAILALARM_1;
                SelectCH_Line_Index = 0;
            }
            break;
        case PAGE_PARASET: GyroSet(); break;
        case PAGE_SYSTEMSET:
            if (SelectCH_Line_Index == 0) {
                New_Page_Status = PAGE_BMS_VERSION;
            } else if (SelectCH_Line_Index == 1) {
                New_Page_Status = PAGE_LCD_VERSION;
            }
            SelectCH_Line_Index = 0;
            break;
        case PAGE_TEMPERATURE_1: break;
        // case PAGE_TEMPERATURE_2: break; //8路NTC
        case PAGE_TEMPERATURE_3: break;
        case PAGE_VOLTAGE_1: break;
        case PAGE_VOLTAGE_2: break;
        case PAGE_VOLTAGE_3: break;
        case PAGE_VOLTAGE_4: break;
        case PAGE_CAPACITY: break;
        case PAGE_WARNING_1: break;
        case PAGE_WARNING_2: break;
        case PAGE_PROTECTION_1: break;
        case PAGE_PROTECTION_2: break;
        // case PAGE_PROTECTION_3: break;
        case PAGE_FAILALARM_1: break;
        case PAGE_FAILALARM_2: break;
        case PAGE_FAILALARM_3: break;
        case PAGE_BMS_VERSION: break;
        case PAGE_LCD_VERSION: break;
        case PAGE_CAN_PAGE_1:
        case PAGE_CAN_PAGE_2:
        case PAGE_CAN_PAGE_3:
        case PAGE_CAN_PAGE_4:
        case PAGE_CAN_PAGE_5:
        case PAGE_CAN_PAGE_6:
        case PAGE_RS485_PAGE_1:
        case PAGE_RS485_PAGE_2:
        case PAGE_RS485_PAGE_3:
        case PAGE_RS485_PAGE_4: choose_row(Old_Page_Status, SelectCH_Line_Index); break;
        default: New_Page_Status = PAGE_MENU_1; break;
    }
}

void press_page_sub_key(void) {
    switch (Old_Page_Status) {
        case PAGE_WELCOME: break; //新增按键操作，数据更新
        case PAGE_MENU_1:
            if (SelectCH_Line_Index-- == 0) {
                SelectCH_Line_Index = 3;
                New_Page_Status     = PAGE_MENU_2;
            }
            Show_SelectCh(SelectCH_Line_Index);
            break;
        case PAGE_MENU_2:
            if (SelectCH_Line_Index-- == 0) {
                SelectCH_Line_Index = 1;
                New_Page_Status     = PAGE_MENU_1;
            }
            Show_SelectCh(SelectCH_Line_Index);
            break;
        case PAGE_ANALOG_1:
            if (SelectCH_Line_Index-- == 0) {
                SelectCH_Line_Index = 3;
            }
            Show_SelectCh(SelectCH_Line_Index);
            break;
        // case PAGE_ANALOG_2: break;
        case PAGE_STATUS:
            if (SelectCH_Line_Index-- == 0) {
                SelectCH_Line_Index = 3;
            }
            Show_SelectCh(SelectCH_Line_Index);
            break;
        case PAGE_PARASET: GyroSet(); break;
        case PAGE_SYSTEMSET:
            if (SelectCH_Line_Index-- == 0) {
                SelectCH_Line_Index = 3;
            }
            Show_SelectCh(SelectCH_Line_Index);
            break;
        case PAGE_TEMPERATURE_1: New_Page_Status = PAGE_TEMPERATURE_3; break;
        // case PAGE_TEMPERATURE_2: break; //8路NTC
        case PAGE_TEMPERATURE_3:
            if (PackData.TempNum > 6) {
                New_Page_Status = PAGE_TEMPERATURE_3;
            } else {
                New_Page_Status = PAGE_TEMPERATURE_1;
            }
            break;
        case PAGE_VOLTAGE_1: New_Page_Status = PAGE_CAPACITY; break;
        case PAGE_VOLTAGE_2: New_Page_Status = PAGE_VOLTAGE_1; break;
        case PAGE_VOLTAGE_3: New_Page_Status = PAGE_VOLTAGE_2; break;
        case PAGE_VOLTAGE_4: New_Page_Status = PAGE_VOLTAGE_3; break;
        case PAGE_CAPACITY: New_Page_Status = PAGE_VOLTAGE_4; break;
        case PAGE_WARNING_1: New_Page_Status = PAGE_WARNING_2; break;
        case PAGE_WARNING_2: New_Page_Status = PAGE_WARNING_1; break;
        case PAGE_PROTECTION_1: New_Page_Status = PAGE_PROTECTION_2; break;
        case PAGE_PROTECTION_2: New_Page_Status = PAGE_PROTECTION_1; break;
        // case PAGE_PROTECTION_3: break;
        case PAGE_FAILALARM_1: New_Page_Status = PAGE_FAILALARM_3; break;
        case PAGE_FAILALARM_2: New_Page_Status = PAGE_FAILALARM_1; break;
        case PAGE_FAILALARM_3: New_Page_Status = PAGE_FAILALARM_2; break;
        case PAGE_BMS_VERSION: break;
        case PAGE_LCD_VERSION: break;
#if PROTOCOL_EN_ALL == 1
        case PAGE_CAN_PAGE_1:
            if (SelectCH_Line_Index-- == 0) {
                SelectCH_Line_Index = 0;
                New_Page_Status     = PAGE_CAN_PAGE_6;
            }
            Show_SelectCh(SelectCH_Line_Index);
            break;
        case PAGE_CAN_PAGE_2:
            if (SelectCH_Line_Index-- == 0) {
                SelectCH_Line_Index = 3;
                New_Page_Status     = PAGE_CAN_PAGE_1;
            }
            Show_SelectCh(SelectCH_Line_Index);
            break;
        case PAGE_CAN_PAGE_3:
            if (SelectCH_Line_Index-- == 0) {
                SelectCH_Line_Index = 3;
                New_Page_Status     = PAGE_CAN_PAGE_2;
            }
            Show_SelectCh(SelectCH_Line_Index);
            break;
        case PAGE_CAN_PAGE_4:
            if (SelectCH_Line_Index-- == 0) {
                SelectCH_Line_Index = 3;
                New_Page_Status     = PAGE_CAN_PAGE_3;
            }
            Show_SelectCh(SelectCH_Line_Index);
            break;
        case PAGE_CAN_PAGE_5:
            if (SelectCH_Line_Index-- == 0) {
                SelectCH_Line_Index = 3;
                New_Page_Status     = PAGE_CAN_PAGE_4;
            }
            Show_SelectCh(SelectCH_Line_Index);
            break;
        case PAGE_CAN_PAGE_6:
            if (SelectCH_Line_Index-- == 0) {
                SelectCH_Line_Index = 3;
                New_Page_Status     = PAGE_CAN_PAGE_5;
            }
            Show_SelectCh(SelectCH_Line_Index);
            break;
        case PAGE_RS485_PAGE_1:
            if (SelectCH_Line_Index-- == 0) {
                SelectCH_Line_Index = 1;
                New_Page_Status     = PAGE_RS485_PAGE_4;
            }
            Show_SelectCh(SelectCH_Line_Index);
            break;
        case PAGE_RS485_PAGE_2:
            if (SelectCH_Line_Index-- == 0) {
                SelectCH_Line_Index = 3;
                New_Page_Status     = PAGE_RS485_PAGE_1;
            }
            Show_SelectCh(SelectCH_Line_Index);
            break;
        case PAGE_RS485_PAGE_3:
            if (SelectCH_Line_Index-- == 0) {
                SelectCH_Line_Index = 3;
                New_Page_Status     = PAGE_RS485_PAGE_2;
            }
            Show_SelectCh(SelectCH_Line_Index);
            break;
        case PAGE_RS485_PAGE_4:
            if (SelectCH_Line_Index-- == 0) {
                SelectCH_Line_Index = 3;
                New_Page_Status     = PAGE_RS485_PAGE_3;
            }
            Show_SelectCh(SelectCH_Line_Index);
            break;
#else
        case PAGE_CAN_PAGE_1:
            if (SelectCH_Line_Index-- == 0) {
                SelectCH_Line_Index = 0;
                New_Page_Status     = PAGE_CAN_PAGE_3;
            }
            Show_SelectCh(SelectCH_Line_Index);
            break;
        case PAGE_CAN_PAGE_2:
            if (SelectCH_Line_Index-- == 0) {
                SelectCH_Line_Index = 3;
                New_Page_Status     = PAGE_CAN_PAGE_1;
            }
            Show_SelectCh(SelectCH_Line_Index);
            break;
        case PAGE_CAN_PAGE_3:
            if (SelectCH_Line_Index-- == 0) {
                SelectCH_Line_Index = 3;
                New_Page_Status     = PAGE_CAN_PAGE_2;
            }
            Show_SelectCh(SelectCH_Line_Index);
            break;
        case PAGE_RS485_PAGE_1:
            if (SelectCH_Line_Index-- == 0) {
                SelectCH_Line_Index = 3;
            }
            Show_SelectCh(SelectCH_Line_Index);
            break;
#endif
        default: break;
    }
}

void press_page_add_key(void) {
    switch (Old_Page_Status) {
        case PAGE_WELCOME: break; //新增按键操作，数据更新
        case PAGE_MENU_1:
            if (++SelectCH_Line_Index >= 4) {
                SelectCH_Line_Index = 0;
                New_Page_Status     = PAGE_MENU_2;
            }
            Show_SelectCh(SelectCH_Line_Index);
            break;
        case PAGE_MENU_2:
            if (++SelectCH_Line_Index >= 2) {
                SelectCH_Line_Index = 0;
                New_Page_Status     = PAGE_MENU_1;
            }
            Show_SelectCh(SelectCH_Line_Index);
            break;
        case PAGE_ANALOG_1:
            if (SelectCH_Line_Index >= 3) {
                SelectCH_Line_Index = 0;
                Show_SelectCh(SelectCH_Line_Index);
            } else {
                SelectCH_Line_Index++;
                Show_SelectCh(SelectCH_Line_Index);
            }
            break;
        // case PAGE_ANALOG_2: break;
        case PAGE_STATUS:
            if (++SelectCH_Line_Index >= 4) {
                SelectCH_Line_Index = 0;
            }
            Show_SelectCh(SelectCH_Line_Index);
            break;
        case PAGE_PARASET: GyroSet(); break;
        case PAGE_SYSTEMSET:
            if (++SelectCH_Line_Index >= 2) {
                SelectCH_Line_Index = 0;
            }
            Show_SelectCh(SelectCH_Line_Index);
            break;
        case PAGE_TEMPERATURE_1:
            if (PackData.TempNum > 6) {
                New_Page_Status = PAGE_TEMPERATURE_3;
            } else {
                New_Page_Status = PAGE_TEMPERATURE_1;
            }
            break;
        // case PAGE_TEMPERATURE_2: break; //8路NTC
        case PAGE_TEMPERATURE_3: New_Page_Status = PAGE_TEMPERATURE_1; break;
        case PAGE_VOLTAGE_1: New_Page_Status = PAGE_VOLTAGE_2; break;
        case PAGE_VOLTAGE_2: New_Page_Status = PAGE_VOLTAGE_3; break;
        case PAGE_VOLTAGE_3: New_Page_Status = PAGE_VOLTAGE_4; break;
        case PAGE_VOLTAGE_4: New_Page_Status = PAGE_CAPACITY; break;
        case PAGE_CAPACITY: New_Page_Status = PAGE_VOLTAGE_1; break;
        case PAGE_WARNING_1: New_Page_Status = PAGE_WARNING_2; break;
        case PAGE_WARNING_2: New_Page_Status = PAGE_WARNING_1; break;
        case PAGE_PROTECTION_1: New_Page_Status = PAGE_PROTECTION_2; break;
        case PAGE_PROTECTION_2: New_Page_Status = PAGE_PROTECTION_1; break;
        // case PAGE_PROTECTION_3: break;
        case PAGE_FAILALARM_1: New_Page_Status = PAGE_FAILALARM_2; break;
        case PAGE_FAILALARM_2: New_Page_Status = PAGE_FAILALARM_3; break;
        case PAGE_FAILALARM_3: New_Page_Status = PAGE_FAILALARM_1; break;
        case PAGE_BMS_VERSION: break;
        case PAGE_LCD_VERSION: break;
#if PROTOCOL_EN_ALL == 1
        case PAGE_CAN_PAGE_1:
            if (++SelectCH_Line_Index >= 4) {
                SelectCH_Line_Index = 0;
                New_Page_Status     = PAGE_CAN_PAGE_2;
                choose_clear();
            }
            Show_SelectCh(SelectCH_Line_Index);
            break;
        case PAGE_CAN_PAGE_2:
            if (++SelectCH_Line_Index >= 4) {
                SelectCH_Line_Index = 0;
                New_Page_Status     = PAGE_CAN_PAGE_3;
                choose_clear();
            }
            Show_SelectCh(SelectCH_Line_Index);
            break;
        case PAGE_CAN_PAGE_3:
            if (++SelectCH_Line_Index >= 4) {
                SelectCH_Line_Index = 0;
                New_Page_Status     = PAGE_CAN_PAGE_4;
                choose_clear();
            }
            Show_SelectCh(SelectCH_Line_Index);
            break;
        case PAGE_CAN_PAGE_4:
            if (++SelectCH_Line_Index >= 4) {
                SelectCH_Line_Index = 0;
                New_Page_Status     = PAGE_CAN_PAGE_5;
                choose_clear();
            }
            Show_SelectCh(SelectCH_Line_Index);
            break;
        case PAGE_CAN_PAGE_5:
            if (++SelectCH_Line_Index >= 4) {
                SelectCH_Line_Index = 0;
                New_Page_Status     = PAGE_CAN_PAGE_6;
                choose_clear();
            }
            Show_SelectCh(SelectCH_Line_Index);
            break;
        case PAGE_CAN_PAGE_6:
            if (++SelectCH_Line_Index >= 1) {
                SelectCH_Line_Index = 0;
                New_Page_Status     = PAGE_CAN_PAGE_1;
                choose_clear();
            }
            Show_SelectCh(SelectCH_Line_Index);
            break;
        case PAGE_RS485_PAGE_1:
            if (++SelectCH_Line_Index >= 4) {
                SelectCH_Line_Index = 0;
                New_Page_Status     = PAGE_RS485_PAGE_2;
                choose_clear();
            }
            Show_SelectCh(SelectCH_Line_Index);
            break;
        case PAGE_RS485_PAGE_2:
            if (++SelectCH_Line_Index >= 4) {
                SelectCH_Line_Index = 0;
                New_Page_Status     = PAGE_RS485_PAGE_3;
                choose_clear();
            }
            Show_SelectCh(SelectCH_Line_Index);
            break;
        case PAGE_RS485_PAGE_3:
            if (++SelectCH_Line_Index >= 4) {
                SelectCH_Line_Index = 0;
                New_Page_Status     = PAGE_RS485_PAGE_4;
                choose_clear();
            }
            Show_SelectCh(SelectCH_Line_Index);
            break;
        case PAGE_RS485_PAGE_4:
            if (++SelectCH_Line_Index >= 2) {
                SelectCH_Line_Index = 0;
                New_Page_Status     = PAGE_RS485_PAGE_1;
                choose_clear();
            }
            Show_SelectCh(SelectCH_Line_Index);
            break;
#else
        case PAGE_CAN_PAGE_1:
            if (++SelectCH_Line_Index >= 4) {
                SelectCH_Line_Index = 0;
                New_Page_Status     = PAGE_CAN_PAGE_2;
                choose_clear();
            }
            Show_SelectCh(SelectCH_Line_Index);
            break;
        case PAGE_CAN_PAGE_2:
            if (++SelectCH_Line_Index >= 4) {
                SelectCH_Line_Index = 0;
                New_Page_Status     = PAGE_CAN_PAGE_3;
                choose_clear();
            }
            Show_SelectCh(SelectCH_Line_Index);
            break;
        case PAGE_CAN_PAGE_3:
            if (++SelectCH_Line_Index >= 2) {
                SelectCH_Line_Index = 0;
                New_Page_Status     = PAGE_CAN_PAGE_1;
                choose_clear();
            }
            Show_SelectCh(SelectCH_Line_Index);
            break;
        case PAGE_RS485_PAGE_1:
            if (++SelectCH_Line_Index >= 4) {
                SelectCH_Line_Index = 0;
            }
            Show_SelectCh(SelectCH_Line_Index);
            break;
#endif
        default: break;
    }
}

void press_esc_key(void) {
    switch (Old_Page_Status) {
        case PAGE_WELCOME: break; //新增按键操作，数据更新
        case PAGE_MENU_1:
        case PAGE_MENU_2:
            // 按ESC回到首页面
            Key_Flag        = 0;
            New_Page_Status = PAGE_WELCOME;
            break;
        case PAGE_ANALOG_1:
        case PAGE_ANALOG_2:
        case PAGE_STATUS:
            New_Page_Status     = PAGE_MENU_1;
            SelectCH_Line_Index = 1;
            break;
        case PAGE_PARASET: GyroSet(); break;
        case PAGE_SYSTEMSET:
            New_Page_Status     = PAGE_MENU_1;
            SelectCH_Line_Index = 3;
            break;
        case PAGE_TEMPERATURE_1:
        case PAGE_TEMPERATURE_2:
        case PAGE_TEMPERATURE_3:
            New_Page_Status     = PAGE_ANALOG_1;
            SelectCH_Line_Index = 2;
            break;
        case PAGE_VOLTAGE_1:
        case PAGE_VOLTAGE_2:
        case PAGE_VOLTAGE_3:
        case PAGE_VOLTAGE_4:
        case PAGE_CAPACITY:
            New_Page_Status     = PAGE_ANALOG_1;
            SelectCH_Line_Index = 3;
            break;
        case PAGE_WARNING_1:
        case PAGE_WARNING_2:
            New_Page_Status     = PAGE_STATUS;
            SelectCH_Line_Index = 1;
            break;
        case PAGE_PROTECTION_1:
        case PAGE_PROTECTION_2:
        case PAGE_PROTECTION_3:
            New_Page_Status     = PAGE_STATUS;
            SelectCH_Line_Index = 2;
            break;
        case PAGE_FAILALARM_1:
        case PAGE_FAILALARM_2:
        case PAGE_FAILALARM_3:
            New_Page_Status     = PAGE_STATUS;
            SelectCH_Line_Index = 3;
            break;
        case PAGE_BMS_VERSION:
        case PAGE_LCD_VERSION:
            New_Page_Status     = PAGE_SYSTEMSET;
            SelectCH_Line_Index = 0;
            break;
        case PAGE_CAN_PAGE_1:
        case PAGE_CAN_PAGE_2:
        case PAGE_CAN_PAGE_3:
        case PAGE_CAN_PAGE_4:
        case PAGE_CAN_PAGE_5:
        case PAGE_CAN_PAGE_6:
            New_Page_Status     = PAGE_MENU_2;
            SelectCH_Line_Index = 0;
            break;
        case PAGE_RS485_PAGE_1:
        case PAGE_RS485_PAGE_2:
        case PAGE_RS485_PAGE_3:
        case PAGE_RS485_PAGE_4:
            New_Page_Status     = PAGE_MENU_2;
            SelectCH_Line_Index = 1;
            break;
        default:
            Key_Flag        = 0;
            New_Page_Status = PAGE_WELCOME;
            break;
    }
}

void page_handler(void) {
    if (!Key_Value) return;
    if (Key_Value & BIT0) {
        press_menu_key();
    } else if (Key_Value & BIT1) {
        press_ok_key();
    } else if (Key_Value & BIT2) {
        press_page_sub_key();
    } else if (Key_Value & BIT3) {
        press_page_add_key();
    } else if (Key_Value & BIT4) {
        press_esc_key();
    }
}

//======================================================================
// Function:	Page_Select_Ctrl()
// Description:  页面的按键选择和控制
//======================================================================
/*void Page_Select_Ctrl(void) {
    if (!Key_Value) return;
    if ((Key_Value & BIT0)) {
        New_Page_Status = PAGE_MENU;
        if (Old_Page_Status != PAGE_MENU) {
            SelectCH_Line_Index = 0;
        }
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
                    SetCommondFlag  = 3;
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
                if (SelectCH_Line_Index == 0) {
                    SelectCH_Line_Index = 4;
                }
                SelectCH_Line_Index--;
                Show_SelectCh(SelectCH_Line_Index);
            } else if ((Key_Value & BIT3)) {
                if (++SelectCH_Line_Index >= 4) {
                    SelectCH_Line_Index = 0;
                }
                Show_SelectCh(SelectCH_Line_Index);
            }

            else if ((Key_Value & BIT4)) //按ESC回到首页面
            {
                Key_Flag        = 0;
                New_Page_Status = PAGE_WELCOME;
            }
            break;

        case PAGE_ANALOG_1:
            if ((Key_Value & BIT1)) {
                if (SelectCH_Line_Index == 2) {
                    New_Page_Status     = PAGE_TEMPERATURE_1;
                    SelectCH_Line_Index = 0;
                } else if (SelectCH_Line_Index == 3) {
                    New_Page_Status     = PAGE_VOLTAGE_1;
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
                New_Page_Status     = PAGE_MENU;
                SelectCH_Line_Index = 0;
            }
            break;

        case PAGE_STATUS:
            if ((Key_Value & BIT1)) {
                if (SelectCH_Line_Index == 1) {
                    New_Page_Status     = PAGE_WARNING_1;
                    SelectCH_Line_Index = 0;
                } else if (SelectCH_Line_Index == 2) {
                    New_Page_Status     = PAGE_PROTECTION_1;
                    SelectCH_Line_Index = 0;
                } else if (SelectCH_Line_Index == 3) {
                    New_Page_Status     = PAGE_FAILALARM_1;
                    SelectCH_Line_Index = 0;
                }
            } else if ((Key_Value & BIT2)) {
                if (SelectCH_Line_Index == 0) {
                    SelectCH_Line_Index = 4;
                }
                SelectCH_Line_Index--;
                Show_SelectCh(SelectCH_Line_Index);
            } else if ((Key_Value & BIT3)) {
                if (++SelectCH_Line_Index >= 4) {
                    SelectCH_Line_Index = 0;
                }
                Show_SelectCh(SelectCH_Line_Index);
            } else if ((Key_Value & BIT4)) {
                New_Page_Status     = PAGE_MENU;
                SelectCH_Line_Index = 1;
            }
            break;
        case PAGE_PARASET: GyroSet(); break; //陀螺仪设置参数

        // case PAGE_PARASET:
        //     if ((Key_Value & BIT4)) {
        //         New_Page_Status     = PAGE_MENU;
        //         SelectCH_Line_Index = 2;
        //     }
        //     break;

        // case PAGE_SYSTEMSET:
        //     if ((Key_Value & BIT4)) {
        //         New_Page_Status     = PAGE_MENU;
        //         SelectCH_Line_Index = 3;
        //     }
        //     break;

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
                New_Page_Status     = PAGE_ANALOG_1;
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
                New_Page_Status     = PAGE_ANALOG_1;
                SelectCH_Line_Index = 2;
            }
            break;

        case PAGE_VOLTAGE_1:
            if ((Key_Value & BIT2)) {
                New_Page_Status = PAGE_CAPACITY;
            } else if ((Key_Value & BIT3)) {
                New_Page_Status = PAGE_VOLTAGE_2;
            } else if ((Key_Value & BIT4)) {
                New_Page_Status     = PAGE_ANALOG_1;
                SelectCH_Line_Index = 3;
            }
            break;

        case PAGE_VOLTAGE_2:
            if ((Key_Value & BIT2)) {
                New_Page_Status = PAGE_VOLTAGE_1;
            } else if ((Key_Value & BIT3)) {
                New_Page_Status = PAGE_VOLTAGE_3;
            } else if ((Key_Value & BIT4)) {
                New_Page_Status     = PAGE_ANALOG_1;
                SelectCH_Line_Index = 3;
            }
            break;

        case PAGE_VOLTAGE_3:
            if ((Key_Value & BIT2)) {
                New_Page_Status = PAGE_VOLTAGE_2;
            } else if ((Key_Value & BIT3)) {
                New_Page_Status = PAGE_VOLTAGE_4;
            } else if ((Key_Value & BIT4)) {
                New_Page_Status     = PAGE_ANALOG_1;
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
                New_Page_Status     = PAGE_ANALOG_1;
                SelectCH_Line_Index = 3;
            }
            break;

        case PAGE_CAPACITY:
            if ((Key_Value & BIT2)) {
                New_Page_Status = PAGE_VOLTAGE_4;
            } else if ((Key_Value & BIT3)) {
                New_Page_Status = PAGE_VOLTAGE_1;
            } else if ((Key_Value & BIT4)) {
                New_Page_Status     = PAGE_ANALOG_1;
                SelectCH_Line_Index = 3;
            }
            break;

        case PAGE_WARNING_1: //告警状态
            if ((Key_Value & (BIT2 | BIT3))) {
                New_Page_Status = PAGE_WARNING_2;
            } else if ((Key_Value & BIT4)) {
                New_Page_Status     = PAGE_STATUS;
                SelectCH_Line_Index = 1;
            }
            break;

        case PAGE_WARNING_2:
            if ((Key_Value & (BIT2 | BIT3))) {
                New_Page_Status = PAGE_WARNING_1;
            } else if ((Key_Value & BIT4)) {
                New_Page_Status     = PAGE_STATUS;
                SelectCH_Line_Index = 1;
            }
            break;

        case PAGE_PROTECTION_1: //保护状态
            if ((Key_Value & BIT2)) {
                New_Page_Status = PAGE_PROTECTION_2;
            } else if ((Key_Value & BIT3)) {
                New_Page_Status = PAGE_PROTECTION_2;
            } else if ((Key_Value & BIT4)) {
                New_Page_Status     = PAGE_STATUS;
                SelectCH_Line_Index = 2;
            }
            break;

        case PAGE_PROTECTION_2:
            if ((Key_Value & BIT2)) {
                New_Page_Status = PAGE_PROTECTION_1;
            } else if ((Key_Value & BIT3)) {
                New_Page_Status = PAGE_PROTECTION_1;
            } else if ((Key_Value & BIT4)) {
                New_Page_Status     = PAGE_STATUS;
                SelectCH_Line_Index = 2;
            }
            break;

        case PAGE_FAILALARM_1: //故障状态
            if ((Key_Value & BIT2)) {
                New_Page_Status = PAGE_FAILALARM_3;
            } else if ((Key_Value & BIT3)) {
                New_Page_Status = PAGE_FAILALARM_2;
            } else if ((Key_Value & BIT4)) {
                New_Page_Status     = PAGE_STATUS;
                SelectCH_Line_Index = 3;
            }
            break;

        case PAGE_FAILALARM_2:
            if ((Key_Value & BIT2)) {
                New_Page_Status = PAGE_FAILALARM_1;
            } else if ((Key_Value & BIT3)) {
                New_Page_Status = PAGE_FAILALARM_3;
            } else if ((Key_Value & BIT4)) {
                New_Page_Status     = PAGE_STATUS;
                SelectCH_Line_Index = 3;
            }
            break;

        case PAGE_FAILALARM_3:
            if ((Key_Value & BIT2)) {
                New_Page_Status = PAGE_FAILALARM_2;
            } else if ((Key_Value & BIT3)) {
                New_Page_Status = PAGE_FAILALARM_1;
            } else if ((Key_Value & BIT4)) {
                New_Page_Status     = PAGE_STATUS;
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
                if (SelectCH_Line_Index == 0) {
                    SelectCH_Line_Index = 3;
                }
                SelectCH_Line_Index--;
                Show_SelectCh(SelectCH_Line_Index);
            } else if ((Key_Value & BIT3)) {
                if (++SelectCH_Line_Index >= 2) {
                    SelectCH_Line_Index = 0;
                }
                Show_SelectCh(SelectCH_Line_Index);
            } else if ((Key_Value & BIT4)) {
                New_Page_Status     = PAGE_MENU;
                SelectCH_Line_Index = 3;
            }
            break;

        case PAGE_BMS_VERSION: //按键返回上一界面
            if ((Key_Value & BIT4)) {
                New_Page_Status     = PAGE_SYSTEMSET;
                SelectCH_Line_Index = 0;
            }
            break;
        case PAGE_LCD_VERSION:
            if ((Key_Value & BIT4)) {
                New_Page_Status     = PAGE_SYSTEMSET;
                SelectCH_Line_Index = 1;
            }
            break;

        default: break;
    }
}*/

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
            Lcd_showChar6_8(Row, Col + 1 * 8, (uint8_t) (Tmp / 1000) + '0', 0);
            Lcd_showChar6_8(Row, Col + 2 * 8, (uint8_t) ((Tmp / 100) % 10) + '0', 0);
        } else {
            Lcd_showChar6_8(Row, Col + 1 * 8, ' ', 0);
            if (Tmp >= 100) {
                Lcd_showChar6_8(Row, Col + 2 * 8, (uint8_t) ((Tmp / 100) % 10) + '0', 0);
            } else {
                Lcd_showChar6_8(Row, Col + 2 * 8, ' ', 0);
            }
        }
        Lcd_showChar6_8(Row, Col + 3 * 8, (uint8_t) ((Tmp / 10) % 10) + '0', 0);
    } else {
        Tmp = 0 - Data;
        if (Tmp > 999) {
            Tmp = 999;
        }
        Tmp = Tmp * 10 / 10;
        Lcd_showChar6_8(Row, Col + 0 * 8, ' ', 0);
        Lcd_showChar6_8(Row, Col + 4 * 8, ' ', 0);
        if (Tmp >= 100) {
            Lcd_showChar6_8(Row, Col + 1 * 8, '-', 0);
            Lcd_showChar6_8(Row, Col + 2 * 8, (uint8_t) (Tmp / 100) + '0', 0);
        } else {
            Lcd_showChar6_8(Row, Col + 1 * 8, ' ', 0);
            Lcd_showChar6_8(Row, Col + 2 * 8, '-', 0);
        }
        Lcd_showChar6_8(Row, Col + 3 * 8, (uint8_t) ((Tmp / 10) % 10) + '0', 0);
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
            Lcd_showChar6_16(Row, Col + 1 * 8, (uint8_t) (Tmp / 1000) % 10 + '0', 0);
            Lcd_showChar6_16(Row, Col + 2 * 8, (uint8_t) ((Tmp / 100) % 10) + '0', 0);
        } else {
            Lcd_showChar6_16(Row, Col + 1 * 8, ' ', 0);
            if (Tmp >= 100) {
                Lcd_showChar6_16(Row, Col + 2 * 8, (uint8_t) ((Tmp / 100) % 10) + '0', 0);
            } else {
                Lcd_showChar6_16(Row, Col + 2 * 8, ' ', 0);
            }
        }
        Lcd_showChar6_16(Row, Col + 3 * 8, (uint8_t) ((Tmp / 10) % 10) + '0', 0);
    } else {
        Tmp = 0 - Data;
        if (Tmp > 999) {
            Tmp = 999;
        }
        Tmp = Tmp * 10 / 10;
        Lcd_showChar6_16(Row, Col + 0 * 8, ' ', 0);
        Lcd_showChar6_16(Row, Col + 4 * 8, ' ', 0);

        if (Tmp >= 100) {
            Lcd_showChar6_16(Row, Col + 1 * 8, '-', 0);
            Lcd_showChar6_16(Row, Col + 2 * 8, (uint8_t) (Tmp / 100) % 10 + '0', 0);
        } else {
            Lcd_showChar6_16(Row, Col + 1 * 8, ' ', 0);
            Lcd_showChar6_16(Row, Col + 2 * 8, '-', 0);
        }
        Lcd_showChar6_16(Row, Col + 3 * 8, (uint8_t) ((Tmp / 10) % 10) + '0', 0);
    }
}

/*******************************************************************************
 *Function:      Show_TemperatureNull()
 *Description:   没有温度数据显示时，显示"   -- "
 *
 *******************************************************************************/
// void Show_TemperatureNull(uint8_t Row) { Lcd_showStringEN(Row, 9 * 8, "  --
// ", 0); }

//======================================================================
// Function:	Show_Cell_Voltage()
// Description:子菜单单体电压
//======================================================================
void Show_Data_4Bit(uint8_t Row, uint8_t Col, uint16_t Data) {
    if (Data >= 1000) {
        if (Data > 9999) {
            Data = 9999;
        }
        Lcd_showChar6_8(Row, Col + 0 * 8, (uint8_t) (Data / 1000) + '0', 0);
        Lcd_showChar6_8(Row, Col + 1 * 8, (uint8_t) ((Data / 100) % 10) + '0', 0);
        Lcd_showChar6_8(Row, Col + 2 * 8, (uint8_t) ((Data / 10) % 10) + '0', 0);
        Lcd_showChar6_8(Row, Col + 3 * 8, (uint8_t) (Data % 10) + '0', 0);
    } else if (Data >= 100) {
        Lcd_showChar6_8(Row, Col + 0 * 8, ' ', 0);
        Lcd_showChar6_8(Row, Col + 1 * 8, (uint8_t) (Data / 100) + '0', 0);
        Lcd_showChar6_8(Row, Col + 2 * 8, (uint8_t) ((Data / 10) % 10) + '0', 0);
        Lcd_showChar6_8(Row, Col + 3 * 8, (uint8_t) (Data % 10) + '0', 0);
    } else if (Data >= 10) {
        Lcd_showChar6_8(Row, Col + 0 * 8, ' ', 0);
        Lcd_showChar6_8(Row, Col + 1 * 8, (uint8_t) (Data / 10) + '0', 0);
        Lcd_showChar6_8(Row, Col + 2 * 8, (uint8_t) (Data % 10) + '0', 0);
        Lcd_showChar6_8(Row, Col + 3 * 8, ' ', 0);
    } else {
        Lcd_showChar6_8(Row, Col + 0 * 8, ' ', 0);
        Lcd_showChar6_8(Row, Col + 1 * 8, ' ', 0);
        Lcd_showChar6_8(Row, Col + 2 * 8, (uint8_t) Data + '0', 0);
        Lcd_showChar6_8(Row, Col + 3 * 8, ' ', 0);
    }
}

// 首页单体电压显示6*7字体
void Show_Data_4Bit1(uint8_t Row, uint8_t Col, uint16_t Data) {
    if (Data >= 1000) {
        if (Data > 9999) {
            Data = 9999;
        }
        Lcd_showCha6_7(Row, Col + 0 * 6, (uint8_t) (Data / 1000) + '0', 1);
        Lcd_showCha6_7(Row, Col + 1 * 6, (uint8_t) ((Data / 100) % 10) + '0', 1);
        Lcd_showCha6_7(Row, Col + 2 * 6, (uint8_t) ((Data / 10) % 10) + '0', 1);
        Lcd_showCha6_7(Row, Col + 3 * 6, (uint8_t) (Data % 10) + '0', 1);
    } else if (Data >= 100) {
        Lcd_showCha6_7(Row, Col + 0 * 6, ' ', 0);
        Lcd_showCha6_7(Row, Col + 1 * 6, (uint8_t) (Data / 100) + '0', 1);
        Lcd_showCha6_7(Row, Col + 2 * 6, (uint8_t) ((Data / 10) % 10) + '0', 1);
        Lcd_showCha6_7(Row, Col + 3 * 6, (uint8_t) (Data % 10) + '0', 1);
    } else if (Data >= 10) {
        Lcd_showCha6_7(Row, Col + 0 * 6, ' ', 0);
        Lcd_showCha6_7(Row, Col + 1 * 6, (uint8_t) (Data / 10) + '0', 1);
        Lcd_showCha6_7(Row, Col + 2 * 6, (uint8_t) (Data % 10) + '0', 1);
        Lcd_showCha6_7(Row, Col + 3 * 6, ' ', 0);
    } else {
        Lcd_showCha6_7(Row, Col + 0 * 6, ' ', 0);
        Lcd_showCha6_7(Row, Col + 1 * 6, ' ', 0);
        Lcd_showCha6_7(Row, Col + 2 * 6, (uint8_t) Data + '0', 1);
        Lcd_showCha6_7(Row, Col + 3 * 6, ' ', 0);
    }
}

void Display_Custom_Bitmap_8x24(uint8_t Row, uint8_t Col, uint8_t WidthBytes, const uint8_t *bitmap_data);
void Display_SOC_12x20(uint8_t Row, uint8_t Col, uint8_t Index);
//======================================================================
// Function:	Show_Current()
// Description:  子菜单显示电流
//======================================================================
void Show_Current(uint8_t Row, uint8_t Col, int16_t Data) {
    uint16_t Tmp;
    uint8_t BaiWei, ShiWei, GeWei, Fen_10, Fen_100;

    Tmp = abs(Data);
    // 方案一：屏蔽顶头的全宽占位擦除（保护左侧大图标！）
    // Lcd_showChar6_8(Row, Col, ' ', 0);
    BaiWei  = 0;
    ShiWei  = 0;
    GeWei   = 0;
    Fen_10  = 0;
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

    if (0 == BaiWei) // 最高位为0，显示空格
    {
        BaiWei = ' ';
        if (0 == ShiWei) ShiWei = ' ';
        else {
            if (Data < 0) {
                BaiWei = '-';
            }
            ShiWei += '0';
        }
    } else {
        if (Data < 0) {
            Lcd_showChar6_8(Row, Col, '-', 0);
        }
        BaiWei += '0';
        ShiWei += '0';
    }

    if ((Data < 0) && (BaiWei == ' ') && (ShiWei == ' ')) {
        ShiWei = '-';
    }
    GeWei += '0';
    Fen_10 += '0';
    Fen_100 += '0';

    // 优化间距：数字间距6px，小数点后间距 4px，
    if (BaiWei != ' ') Lcd_showChar6_8(Row, Col + 7,  BaiWei, 0);
    if (ShiWei != ' ') Lcd_showChar6_8(Row, Col + 12, ShiWei, 0);
    Lcd_showChar6_8(Row, Col + 19, GeWei, 0);
    Lcd_showChar6_8(Row, Col + 25, '.', 0);
    Lcd_showChar6_8(Row, Col + 29, Fen_10, 0);
}

/*首页显示电流*/
void Show_Current1(uint8_t Row, uint8_t Col, int16_t Data) {
    uint16_t Tmp;
    uint8_t BaiWei, ShiWei, GeWei, Fen_10, Fen_100;

    Tmp = abs(Data);
    Lcd_showChar(Row, Col, ' ', 0);
    BaiWei  = 0;
    ShiWei  = 0;
    GeWei   = 0;
    Fen_10  = 0;
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
    /*while (Tmp >= 1) {
        Tmp -= 1;
        Fen_100++;
    }*/

    if (0 == BaiWei) { // 最高位为0，显示空格
        BaiWei = ' ';
        if (0 == ShiWei) ShiWei = ' ';
        else {
            if (Data < 0) {
                BaiWei = '-';
            }
            ShiWei += '0';
        }
    } else {
        if (Data < 0) {
            Lcd_showChar(Row, Col, '-', 0);
        }
        BaiWei += '0';
        ShiWei += '0';
    }

    if ((Data < 0) && (BaiWei == ' ') && (ShiWei == ' ')) {
        ShiWei = '-';
    }
    GeWei += '0';
    Fen_10 += '0';
    //    Fen_100 += '0';

    Lcd_showChar(Row, Col + 8, BaiWei, 0);
    Lcd_showChar(Row, Col + 16, ShiWei, 0);
    Lcd_showChar(Row, Col + 24, GeWei, 0);
    Lcd_showChar(Row, Col + 32, '.', 0);
    Lcd_showChar(Row, Col + 40, Fen_10, 0);
}

//======================================================================
// Function:	Show_TotalVoltage()
// Description:  子菜单显示总压
//======================================================================
void Show_TotalVoltage(uint8_t Row, uint8_t Col, uint16_t Data) {
    if (Data >= 10000) {
        Lcd_showChar6_8(Row, Col + 0 * 8, (uint8_t) (Data / 10000) + '0', 0);
        Lcd_showChar6_8(Row, Col + 1 * 8, (uint8_t) ((Data / 1000) % 10) + '0', 0);
    } else {
        // 方案一：屏蔽最高位的空白字符强制覆写
        // Lcd_showChar6_8(Row, Col + 0 * 8, ' ', 0);
        Lcd_showChar6_8(Row, Col + 1 * 8, (uint8_t) (Data / 1000) + '0', 0);
    }
    Lcd_showChar6_8(Row, Col + 2 * 8, '.', 0);
    Lcd_showChar6_8(Row, Col + 3 * 8, (uint8_t) ((Data / 100) % 10) + '0', 0);
    Lcd_showChar6_8(Row, Col + 4 * 8, (uint8_t) ((Data / 10) % 10) + '0', 0);
}

// 首页10*12显示电压
void Show_TotalVoltage1(uint8_t Row, uint8_t Col, uint16_t Data) {
    if (Data >= 10000) {
        Lcd_showChar(Row, Col + 0 * 8, (uint8_t) (Data / 10000) + '0', 0);
        Lcd_showChar(Row, Col + 1 * 8, (uint8_t) ((Data / 1000) % 10) + '0', 0);
    } else {
        Lcd_showChar(Row, Col + 0 * 8, ' ', 0);
        Lcd_showChar(Row, Col + 1 * 8, (uint8_t) (Data / 1000) + '0', 0);
    }
    Lcd_showChar(Row, Col + 2 * 8, '.', 0);
    Lcd_showChar(Row, Col + 3 * 8, (uint8_t) ((Data / 100) % 10) + '0', 0);
    /*Lcd_showChar(Row, Col + 4 * 8, (uint8_t) ((Data / 10) % 10) + '0',
                 0); //后两位小数点*/
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
            if (((Soc / 100) % 10) == 0) Lcd_showChar6_8(0, 11 * 8, ' ', 0); // SOC
            else
                Lcd_showChar6_8(0, 11 * 8, (uint8_t) ((Soc / 100) % 10) + '0', 0);

            if ((((Soc / 10) % 10) == 0) && (((Soc / 100) % 10) == 0)) Lcd_showChar6_8(0, 12 * 8, ' ', 0);
            else
                Lcd_showChar6_8(0, 12 * 8, (uint8_t) ((Soc / 10) % 10) + '0', 0);
            Lcd_showChar6_8(0, 13 * 8, (uint8_t) (Soc % 10) + '0', 0);

            // if (PackData.Fcc >= 10000) { // 总容量
            //     Lcd_showChar6_8(2, 9 * 8, (uint8_t) (PackData.Fcc / 10000) + '0', 0);
            //     Lcd_showChar6_8(2, 10 * 8, (uint8_t) ((PackData.Fcc / 1000) % 10) + '0', 0);
            // } else if (PackData.Fcc >= 1000) {
            //     Lcd_showChar6_8(2, 9 * 8, ' ', 0);
            //     Lcd_showChar6_8(2, 10 * 8, (uint8_t) (PackData.Fcc / 1000) + '0', 0);
            // } else {
            //     Lcd_showChar6_8(2, 9 * 8, ' ', 0);
            //     Lcd_showChar6_8(2, 10 * 8, ' ', 0);
            // }
            // Lcd_showChar6_8(2, 11 * 8, (uint8_t) ((PackData.Fcc / 100) % 10) + '0', 0);
            // Lcd_showChar6_8(2, 12 * 8, '.', 0);
            // Lcd_showChar6_8(2, 13 * 8, (uint8_t) ((PackData.Fcc / 10) % 10) + '0', 0);

            // if (PackData.Rm >= 10000) { // 剩余容量
            //     Lcd_showChar6_8(4, 9 * 8, (uint8_t) (PackData.Rm / 10000) + '0', 0);
            //     Lcd_showChar6_8(4, 10 * 8, (uint8_t) ((PackData.Rm / 1000) % 10) + '0', 0);//
            // } else if (PackData.Rm >= 1000) {
            //     Lcd_showChar6_8(4, 9 * 8, ' ', 0);
            //     Lcd_showChar6_8(4, 10 * 8, (uint8_t) (PackData.Rm / 1000) + '0', 0);
            // } else {
            //     Lcd_showChar6_8(4, 9 * 8, ' ', 0);
            //     Lcd_showChar6_8(4, 10 * 8, ' ', 0);
            // }
            // Lcd_showChar6_8(4, 11 * 8, (uint8_t) ((PackData.Rm / 100) % 10) + '0', 0);
            // Lcd_showChar6_8(4, 12 * 8, '.', 0);
            // Lcd_showChar6_8(4, 13 * 8, (uint8_t) ((PackData.Rm / 10) % 10) + '0', 0);

            Show_Data_4Bit(2, 12 * 8, PackData.Cycle); // 循环次数
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

        case PAGE_FAILALARM_3: Show_Data_4Bit(0, 12 * 8, Record.Over_Dchg_Count); break;

        case PAGE_BMS_VERSION: // 版本号
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
        // case PAGE_WELCOME: Page_Welcome(); break;     //新增按键操作，数据更新
        case PAGE_WELCOME: Page_Welcome_new(); break; // 新增按键操作，数据更新
        case PAGE_MENU_1: Page_Menu_1(); break;
        case PAGE_MENU_2: Page_Menu_2(); break;
        case PAGE_ANALOG_1: Page_Analog_1(); break;
        // case PAGE_ANALOG_2: Page_Analog_2(); break;
        case PAGE_STATUS: Page_Status(); break;
        case PAGE_PARASET: Page_ParaSet(); break;
        case PAGE_SYSTEMSET: Page_SystemSet(); break;
        case PAGE_TEMPERATURE_1: Page_Temperature_1(); break;
        // case PAGE_TEMPERATURE_2: Page_Temperature_2(); break; //8路NTC
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
        // case PAGE_PROTECTION_3: Page_Protection_3(); break;
        case PAGE_FAILALARM_1: Page_FailAlarm_1(); break;
        case PAGE_FAILALARM_2: Page_FailAlarm_2(); break;
        case PAGE_FAILALARM_3: Page_FailAlarm_3(); break;
        case PAGE_BMS_VERSION: Page_BMS_Version(); break;
        case PAGE_LCD_VERSION: Page_LCD_Version(); break;
#if PROTOCOL_EN_ALL == 1
        case PAGE_CAN_PAGE_1: page_can_protocol_1(); break;
        case PAGE_CAN_PAGE_2: page_can_protocol_2(); break;
        case PAGE_CAN_PAGE_3: page_can_protocol_3(); break;
        case PAGE_CAN_PAGE_4: page_can_protocol_4(); break;
        case PAGE_CAN_PAGE_5: page_can_protocol_5(); break;
        case PAGE_CAN_PAGE_6: page_can_protocol_6(); break;
        case PAGE_RS485_PAGE_1: page_rs485_protocol_1(); break;
        case PAGE_RS485_PAGE_2: page_rs485_protocol_2(); break;
        case PAGE_RS485_PAGE_3: page_rs485_protocol_3(); break;
        case PAGE_RS485_PAGE_4: page_rs485_protocol_4(); break;
#else
        case PAGE_CAN_PAGE_1: page_can_protocol_1(); break;
        case PAGE_CAN_PAGE_2: page_can_protocol_2(); break;
        case PAGE_CAN_PAGE_3: page_can_protocol_3(); break;
        case PAGE_RS485_PAGE_1: page_rs485_protocol_1(); break;
#endif
        default: break;
    }
    Page_Digital_Show();
}

//======================================================================
// Function:	Show_Ctrl()
// Description:
//======================================================================
void Show_Ctrl(void) {
    // Page_Select_Ctrl();
    page_handler();
    Page_Change_Ctrl();

    if (!Timer[DATA_SHOW_TIMER].Flag) return;
    Set_Timer(DATA_SHOW_TIMER, 20);
    Page_Digital_Show();
}

//======================================================================
// ENDFILE    1600
//======================================================================
