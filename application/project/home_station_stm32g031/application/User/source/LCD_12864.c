//======================================================================
//======================================================================
//FileName: LCD_12864.c
//Author:
//Version: 1.0.0
//Date:
//Description:
//======================================================================
//#include "User_Conf.h"
#include "LCD_12864.h"

#include "Font_Library.h"
#include "Mcu_Init.h"
#include "Show_Ctrl.h"

//======================================================================
//Function:	LcdSend_Command()
//Description:
//======================================================================
void LcdSend_Command(uint8_t Cmd) {
    uint8_t i;
    _CS;
    _A0;
    for (i = 0; i < 8; i++) {
        _SCK;
        if (Cmd & 0x80) {
            SDO;
        } else {
            _SDO;
        }
        SCK;
        Cmd <<= 1;
    }
    CS;
}

//======================================================================
//Function:	LcdSend_Data()
//Description:  接收
//======================================================================
void LcdSend_Data(uint8_t Data) {
    uint8_t i;

    _CS;
    A0;
    for (i = 0; i < 8; i++) {
        _SCK;
        if (Data & 0x80) {
            SDO;
        } else {
            _SDO;
        }
        SCK;
        Data <<= 1;
    }
    CS;
}

//======================================================================
//Function:	Lcd12864_Addr()
//Description:  Row: 0~63, Col: 0~127
//======================================================================
void Lcd12864_Addr(uint8_t Row, uint8_t Col) {
    // uint8_t C_tmp;
    if (Row > 64) { Row = 64; }
    if (Col > 128) { Col = 128; }

    /*控制显示屏开关、高4位地址、页面地址*/
    LcdSend_Command(((Col >> 4) & 0x0F) + 0x10);
    LcdSend_Command((Col & 0x0F | 0X01));
    LcdSend_Command(0xB0 | Row);
}

//======================================================================
//Function:	Clear_Screen()
//Description:  清屏
//======================================================================
void Clear_Screen(unsigned char data1, unsigned char data2) {
    int i, j, m;
    m = 0xb0;
    for (i = 0; i < 9; i++) {
        LcdSend_Command(m);
        LcdSend_Command(0x10);
        LcdSend_Command(0x00);
        for (j = 0; j < 66; j++) {
            LcdSend_Data(data1);
            LcdSend_Data(data2);
        }
        m++;
    }
}

//======================================================================
//Function:	Lcd_Close_Display()
//Description:  关闭_显示
//======================================================================
void Lcd_Close_Display(void) {
    BL_EN;
    Clear_Screen(0x00, 0x00);
    Clear_Screen(0xFF, 0xFF); //BLACK
    LcdSend_Command(0xAE);
    HAL_Delay(1);
}


//======================================================================
//Function:	Display_HanZi_16x16()
//Description:  ʾ16*16
//======================================================================
void Display_HanZi_16x16(uint8_t Row, uint8_t Col, uint8_t Reverse, uint8_t *dp, uint8_t Index) {
    uint8_t i, j;
    uint8_t *p;

    p = dp + (uint32_t) Index * 32;
    for (j = 0; j < 2; j++) {
        Lcd12864_Addr(Row + j, Col + 4); //Col 修复原来点阵偏左4列
        for (i = 0; i < 16; i++) {
            if (Reverse) {
                LcdSend_Data(~*p);
            } else {
                LcdSend_Data(*p);
            }
            p++;
        }
    }
}

void Lcd_showFont(uint8_t rowaddr, uint8_t columnaddr, uint16_t code2, uint8_t fb) //显示一个汉字
{
    uint16_t index;

    index = Lcd_searchFontCode(code2); //Lcd
    Display_HanZi_16x16(rowaddr, columnaddr, fb, (uint8_t *) HanZi16x16, index);
}

void Display_ZiFu1_16x16(uint8_t Row, uint8_t Col, uint8_t uchar, uint8_t fb) {
    uint8_t i = 0, j = 0;
    uint8_t data4 = 0;
    uint16_t index = 0;

    if (Row >= MAXROWCNT || Col >= MAXCOLUMNCNT) return;
    if (uchar & 0x80) return;   //this is not a char
    index = (uchar - ' ') * 16; //index(start)

    for (j = 0; j < 2; j++) {
        Lcd12864_Addr(Row + j, Col + 4); //Col
        for (i = 0; i < 16; i++) {
            data4 = ZiFu16x16[index + i + j * 8];
            if (1 == fb) data4 = ~data4;
            LcdSend_Data(data4);
        }
    }
}

/*******************************************************************************
*name:	      Lcd_showChar6_8()
*description: show char 						6*8
*parameter:   char(ASCII),rowaddr,columnaddr
*return:      None
*******************************************************************************/
void Lcd_showChar6_8(uint8_t Row, uint8_t Col, uint8_t uchar, uint8_t fb) {
    uint8_t i = 0;
    uint8_t data4 = 0;
    uint16_t index = 0;

    if (uchar & 0x80) return;    //this is not a char
    index = (uchar - ' ');       //index(start)
    Lcd12864_Addr(Row, Col + 4);
    for (i = 0; i < 6; i++) {
        data4 = ShuZi6x8[index][i];
        if (1 == fb) {
            data4 = ~data4;
            //data4 |= 0x01;
            //          data4 |= 0x80;
        }
        LcdSend_Data(data4);
    }
}

/*******************************
*name:	      Lcd_showChar()
*description: show char 						8*16
*parameter:   char(ASCII),rowaddr,columnaddr
*return:      None
*******************************************************************************/
void Lcd_showChar(uint8_t Row, uint8_t Col, uint8_t uchar, uint8_t fb) {
    uint8_t i = 0, j = 0;
    uint8_t data4 = 0;
    uint16_t index = 0;

    if (Row >= MAXROWCNT || Col >= MAXCOLUMNCNT) return;
    if (uchar & 0x80) return;   //this is not a char
    index = (uchar - ' ') * 16; //index(start)

    for (j = 0; j < 2; j++) {
        Lcd12864_Addr(Row + j, Col + 4);
        for (i = 0; i < 8; i++) {
            data4 = ASCIITAB[index + i + j * 8];
            if (1 == fb) data4 = ~data4;
            LcdSend_Data(data4);
        }
    }
}

/*******************************************************************************
* name:	      Lcd_showStringEN()			显示英文字符串
* description: show font,16*16,char
* parameter:   uint8_t* 
* return:      None
*******************************************************************************/
void Lcd_showStringEN(uint8_t rowaddr, uint8_t columnaddr, uint8_t *str, uint8_t fb) {
    uint8_t *pt = str;
    uint16_t code2 = 0;
    if (rowaddr >= MAXROWCNT || columnaddr >= MAXCOLUMNCNT) return;
    while (*pt != 0) {
        if ((*pt & 0x80) != 0) //chinese
        {
            code2 = (*(pt + 1)) + ((*pt) << 8);
            Lcd_showFont(rowaddr, columnaddr, code2, fb);
            columnaddr += 18;
            pt += 2;
        } else {
            code2 = *pt;
            Lcd_showChar6_8(rowaddr, columnaddr, code2, fb);
            columnaddr += 6;
            pt++;
        }
    }
}
/*******************************************************************************
* name:	      6*7首页显示日期字体
* description:
* parameter:   uint8_t*
* return:      None
*******************************************************************************/
void Lcd_showStringENG(uint8_t rowaddr, uint8_t columnaddr, uint8_t *str, uint8_t fb) {
    uint8_t *pt = str;
    uint16_t code2 = 0;
    if (rowaddr >= MAXROWCNT || columnaddr >= MAXCOLUMNCNT) return;
    while (*pt != 0) {
        if ((*pt & 0x80) != 0) //chinese
        {
            code2 = (*(pt + 1)) + ((*pt) << 8);
            Lcd_showFont(rowaddr, columnaddr, code2, fb);
            columnaddr += 18;
            pt += 2;
        } else {
            code2 = *pt;
            Lcd_showCha6_7(rowaddr, columnaddr, code2, fb);
            columnaddr += 6;
            pt++;
        }
    }
}

/*******************************************************************************
*name:
*description: 6*7首页显示日期字体 						8*16
*parameter:
*return:      None
*******************************************************************************/
void Lcd_showCha6_7(uint8_t Row, uint8_t Col, uint8_t uchar, uint8_t fb) {
    uint16_t i = 0;
    uint8_t data4 = 0;
    uint16_t index = 0;

    if (uchar & 0x80) return;    //this is not a char
    index = (uchar - ' ');       //index(start)
    Lcd12864_Addr(Row, Col + 4); //Col
    for (i = 0; i < 6; i++) {
        data4 = date6x7[index][i];
        if (1 == fb) {
            data4 = ~data4;
            data4 &= ~0x80;
        }
        LcdSend_Data(data4);
    }
}

void Lcd_showStringENGB(uint8_t rowaddr, uint8_t columnaddr, uint8_t *str, uint8_t fb) {
    uint8_t *pt = str;
    uint16_t code2 = 0;
    if (rowaddr >= MAXROWCNT || columnaddr >= MAXCOLUMNCNT) return;
    while (*pt != 0) {
        if ((*pt & 0x80) != 0) //chinese
        {
            code2 = (*(pt + 1)) + ((*pt) << 8);
            Lcd_showFont(rowaddr, columnaddr, code2, fb);
            columnaddr += 18;
            pt += 2;
        } else {
            code2 = *pt;
            Lcd_showChar8_8(rowaddr, columnaddr, code2, fb);
            columnaddr += 6;
            pt++;
        }
    }
}

void Lcd_showChar8_8(uint8_t Row, uint8_t Col, uint8_t uchar, uint8_t fb) {
    uint8_t i = 0;
    uint8_t data4 = 0;
    uint16_t index = 0;

    if (uchar & 0x80) return;    //this is not a char
    index = (uchar - ' ');       //index(start)
    Lcd12864_Addr(Row, Col + 6); //Col
    for (i = 0; i < 8; i++) {
        data4 = show8x8[index][i]; //
        if (1 == fb) data4 = ~data4;
        LcdSend_Data(data4);
    }
    index += 1;
    for (i = 0; i < 8; i++) {
        data4 = show8x8[index][i]; //
        if (1 == fb) data4 = ~data4;
        LcdSend_Data(data4);
    }
}

void Lcd_showChar6_16(uint8_t Row, uint8_t Col, uint8_t uchar, uint8_t fb) {
    uint8_t i = 0, j = 0;
    uint8_t data4 = 0;
    uint16_t index = 0;

    if (uchar & 0x80) return;   //this is not a char
    index = (uchar - ' ') * 12; //index(start)

    for (j = 0; j < 2; j++) {
        Lcd12864_Addr(Row + j, Col + 4);
        for (i = 0; i < 6; i++) {
            data4 = ShuZi6x16[index + i + j * 6];
            if (1 == fb) data4 = ~data4;
            LcdSend_Data(data4);
        }
    }
}
//======================================================================
//Function:	Lcd12864_Init()
//Description: 32P驱动
//======================================================================
void Lcd12864_Init(void) {
    _RESET;
    HAL_Delay(50);
    RESET;
    HAL_Delay(50);
    _BL_EN;//背光一直灭
    HAL_Delay(200);

    LcdSend_Command(0xaf); // display on
    LcdSend_Command(0x40); // display start line=0
    LcdSend_Command(0xa0); // ADC=1
    LcdSend_Command(0xa6); // normal display
    LcdSend_Command(0xa4); // Display all point = off
    LcdSend_Command(0xa2); // LCD bias = 1/9
    LcdSend_Command(0xc8); // Common output mode select= reverse

    LcdSend_Command(0x2f); // Power control = all on

    LcdSend_Command(0x24); // RA/RB setting

    LcdSend_Command(0xf8); // Booster Ratio Set= 2x,3x,4x
    LcdSend_Command(0x00); //(2byte command)

    LcdSend_Command(0x81); // E-Vol setting
    LcdSend_Command(0x2C); // (2byte command)

    Clear_Screen(0, 0);
}

/*******************************************************************************
*name:	      Lcd_searchFontCode()
*description:
*parameter:   font code(GBK)
*return:      Index
*******************************************************************************/
uint16_t Lcd_searchFontCode(uint16_t code2) {
    int16_t start, mid, end;
    start = 0;
    end = fontnum();
    do {
        mid = (start + end) / 2; //search
        if (dotData[mid].code == code2) break;
        if (dotData[mid].code > code2) end = mid - 1;
        if (dotData[mid].code < code2) start = mid + 1;
    } while (start <= end);

    if (dotData[mid].code == code2) {
        return dotData[mid].Index;
    } else {
        return 0;
    }
}

// ... existing code ...

/*******************************************************************************
*name:	      Display_Custom_Icon_8x8()
*description: 显示自定义 8x8 图标
*parameter:   Row, Col, icon_data(指向 8 字节数组的指针)
*return:      None
*******************************************************************************/
void Display_Custom_Icon_8x8(uint8_t Row, uint8_t Col, const uint8_t *icon_data) {
    uint8_t i;

    if (Row >= MAXROWCNT || Col >= MAXCOLUMNCNT) return;

    Lcd12864_Addr(Row, Col + 4);
    for (i = 0; i < 8; i++) {
        LcdSend_Data(icon_data[i]);
    }
}

/*******************************************************************************
*name:	      Display_Custom_Icon_8x16()
*description: 显示自定义 8x16 图标
*parameter:   Row, Col, icon_data(指向 16 字节数组的指针)
*return:      None
*******************************************************************************/
void Display_Custom_Icon_8x16(uint8_t Row, uint8_t Col, const uint8_t *icon_data) {
    uint8_t i, j;

    if (Row >= (MAXROWCNT - 1) || Col >= MAXCOLUMNCNT) return;

    for (j = 0; j < 2; j++) {
        Lcd12864_Addr(Row + j, Col + 4);
        for (i = 0; i < 8; i++) {
            LcdSend_Data(icon_data[j * 8 + i]);
        }
    }
}

/*******************************************************************************
*name:	      Display_Custom_Bitmap_8x16()
*description: 显示自定义宽度 8x16 位图
*parameter:   Row, Col, WidthBytes, bitmap_data(2*WidthBytes 字节)
*return:      None
*******************************************************************************/
void Display_Custom_Bitmap_8x16(uint8_t Row, uint8_t Col, uint8_t WidthBytes, const uint8_t *bitmap_data) {
    uint8_t i, j;

    if (bitmap_data == 0) return;
    if (WidthBytes == 0) return;
    if (Row >= (MAXROWCNT - 1)) return;
    /* 实际列 = Col + 4 起写 WidthBytes 列，末列须 < 128 */
    if ((uint16_t) Col + 4u + (uint16_t) WidthBytes > (uint16_t) MAXCOLUMNCNT) return;

    for (j = 0; j < 2; j++) {
        Lcd12864_Addr(Row + j, Col + 4);
        for (i = 0; i < WidthBytes; i++) {
            LcdSend_Data(bitmap_data[j * WidthBytes + i]);
        }
    }
}

/*******************************************************************************
*name:	      Display_Custom_Bitmap_8x24()
*description: 显示自定义宽度 8x24 位图
*parameter:   Row, Col, WidthBytes, bitmap_data(3*WidthBytes 字节)
*return:      None
*******************************************************************************/

void Display_Custom_Bitmap_8x24(uint8_t Row, uint8_t Col, uint8_t WidthBytes, const uint8_t *bitmap_data) {
    uint8_t i, j;

    if (bitmap_data == 0) return;
    if (WidthBytes == 0) return;
    if (Row >= (MAXROWCNT - 2)) return;
    /* 实际列 = Col + 4 起写 WidthBytes 列，末列须 < 128 */
    if ((uint16_t) Col + 4u + (uint16_t) WidthBytes > (uint16_t) MAXCOLUMNCNT) return;

    for (j = 0; j < 3; j++) {
        Lcd12864_Addr(Row + j, Col + 4);
        for (i = 0; i < WidthBytes; i++) {
            LcdSend_Data(bitmap_data[j * WidthBytes + i]);
        }
    }
}

/*******************************************************************************
*name:	      Lcd_showChar_Shift4()
*description: 显示偏移 4 像素的 8x16 字符（跨越 3 个 Page）
*parameter:   Row(起始页), Col, uchar, fb(反显)
*return:      None
*******************************************************************************/
void Lcd_showChar_Shift4(uint8_t Row, uint8_t Col, uint8_t uchar, uint8_t fb) {
    uint8_t i = 0, j = 0;
    uint8_t src_data[16];
    uint8_t out_data[3][8] = {0};
    uint16_t index = 0;

    if (Row >= (MAXROWCNT - 2) || Col >= MAXCOLUMNCNT) return;
    if (uchar & 0x80) return;
    index = (uchar - ' ') * 16;

    // 加载原始 16 像素高字符数据
    for (i = 0; i < 16; i++) {
        src_data[i] = ASCIITAB[index + i];
        if (1 == fb) src_data[i] = ~src_data[i];
    }

    // 4 像素向下位移逻辑 (Bit 0 为最高位)
    for (i = 0; i < 8; i++) {
        out_data[0][i] = (src_data[i] << 4);
        out_data[1][i] = (src_data[i] >> 4) | (src_data[i + 8] << 4);
        out_data[2][i] = (src_data[i + 8] >> 4);
    }

    // 绘制到连续的 3 个 Page
    for (j = 0; j < 3; j++) {
        Lcd12864_Addr(Row + j, Col + 4);
        for (i = 0; i < 8; i++) {
            LcdSend_Data(out_data[j][i]);
        }
    }
}

/*******************************************************************************
*name:	      Display_Custom_Bitmap_8x16_Shift4()
*description: 显示偏移 4 像素的自定义宽度 16 高位图（跨越 3 个 Page）
*parameter:   Row(起始页), Col, Width, data(指向位图数据的指针)
*return:      None
*******************************************************************************/
void Display_Custom_Bitmap_8x16_Shift4(uint8_t Row, uint8_t Col, uint8_t Width, const uint8_t *data) {
    uint8_t i, j;
    uint8_t out_byte;

    if (data == 0 || Width == 0) return;
    if (Row >= (MAXROWCNT - 2)) return;
    if ((uint16_t) Col + 4u + (uint16_t) Width > (uint16_t) MAXCOLUMNCNT) return;

    for (j = 0; j < 3; j++) {
        Lcd12864_Addr(Row + j, Col + 4);
        for (i = 0; i < Width; i++) {
            if (j == 0) {
                out_byte = (data[i] << 4);
            } else if (j == 1) {
                out_byte = (data[i] >> 4) | (data[Width + i] << 4);
            } else {
                out_byte = (data[Width + i] >> 4);
            }
            LcdSend_Data(out_byte);
        }
    }
}

// ... existing code ...


//======================================================================
//ENDFILE
//======================================================================
