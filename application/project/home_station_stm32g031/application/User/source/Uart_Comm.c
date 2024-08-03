//======================================================================
//Copyright (C), 2014-2999, Tech. Co., Ltd.
//======================================================================
//FileName:  Uart_Comm.c
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

#include "Uart_Comm.h"

#include "Data_Init.h"
#include "Show_Ctrl.h"
#include "User_Conf.h"
#include "common_utils.h"
#include "paras.h"
#include "protocol1363_handler.h"
#include "protocol1363_transfer.h"

/*全局变量*/
uint8_t Uart_Buffer[Uart_RX_BuffLen];  //Usart接受-发送缓冲区
uint8_t Uart_Buffer1[Uart_RX_BuffLen]; //Usart接受-发送缓冲区
uint8_t Uart_TxBuffer[30];             //Usart接受-发送缓冲区
uint8_t Uart_TxIndex     = 0;          //Usart当前发送序号
uint8_t Uart_TxCount     = 0;          //Usart发送计数器
uint8_t Uart_RecOK       = 0;          //Usart完成标志位
uint8_t Dis_Charge_Flag  = 0;
uint8_t sleep_flag       = 0;
uint16_t uart_error_flag = 0;
uint16_t CRC_3, g_CRC = 0;

uint8_t Comm_Flag_1 = 0, Comm_Flag_2 = 0, Comm_Flag_3 = 0;
uint8_t Comm_Flag_4 = 0, Comm_Flag_5 = 0;
uint8_t PlaceFlag_COM  = 0;
uint8_t X_axisFlag_COM = 0;
uint8_t SetCommondFlag = 0;
uint8_t PlaceFlag      = 0;
uint8_t X_axisFlag     = 0;

extern uint8_t getValidData;
extern comm_struct protocol_com;
//======================================================================
//Function:	GetCRC16CheckCode()
//Description:	CRC16校验
//======================================================================
uint16_t GetCRC16CheckCode(unsigned char *buf, uint16_t Len, uint16_t CRCPolynomial) {
    uint16_t i, j;
    uint16_t ii = 0xffff;
    uint16_t LSBNoZero;

    CRCPolynomial = 0xa001;
    for (i = 0; i < Len; i++) {
        ii = buf[i] ^ ii;
        for (j = 1; j <= 8; j++) {
            LSBNoZero = (ii & 0x0001) != 0;
            ii        = ii >> 1;
            if (LSBNoZero) ii = ii ^ CRCPolynomial;
        }
    }
    return ii;
}

#define SN_Len 47
//======================================================================
//Function:	Uart_ReceiveIQR()
//Description:	USART1接受中断服务
//======================================================================
void Uart_ReceiveIQR(uint8_t Buffer) {
    static uint16_t Uart_RecCycle = 0;
    static uint16_t Uart_RecCount  = 0;
    static uint8_t Uart_RecType   = SOI;

    uint16_t i;
    protocol1363_get_char(&protocol_com, Buffer);
    Uart_RecOK      = 1;
    uart_error_flag = 0;
    if (Uart_RecCount >= (Uart_RX_BuffLen - 1)) {
        Uart_RecCount = 0;
        Uart_RecType  = 0;
        Uart_RecCycle = 0;
        return;
    }
    if ((Buffer == 0x3A) && (Uart_RecCount == 0)) {
        Uart_RecType = SOI;
    }
    switch (Uart_RecType) {
        case SOI:
            if (Buffer == 0x3A) {
                Uart_RecCycle                 = 0;
                Uart_RecCount                 = 0;
                Uart_Buffer1[Uart_RecCount++] = Buffer;
                Uart_RecType                  = ADDR;
            }
            break;

        case ADDR: //VER:
            Uart_Buffer1[Uart_RecCount++] = Buffer;
            if (++Uart_RecCycle >= 2) {
                Uart_RecCycle = 0;
                Uart_RecType  = CMD;
            }

            break;

        case CMD: //ADR:
            Uart_Buffer1[Uart_RecCount++] = Buffer;
            if (++Uart_RecCycle >= 2) {
                Uart_RecCycle = 0;
                Uart_RecType  = VER;
            }

            break;

        case VER: // CID1:
            Uart_Buffer1[Uart_RecCount++] = Buffer;
            if (++Uart_RecCycle >= 2) {
                Uart_RecCycle = 0;
                Uart_RecType  = LENGTH;
            }

            break;

        case LENGTH: //CID2:
            Uart_Buffer1[Uart_RecCount++] = Buffer;
            if (++Uart_RecCycle >= 4) {
                Uart_RecCycle = 0;
                Uart_RecType  = DATA;
            }
            break;

        case DATA: //LENGTH:
            Uart_Buffer1[Uart_RecCount++] = Buffer;

            //(Uart_Buffer1[7]-0x30) (Uart_Buffer1[8]-0x30)Uart_Buffer1[9]Uart_Buffer1[10]
            if (Comm_Flag_1 == 1) // || (1 == Comm_Flag_3))
            {
                if (++Uart_RecCycle >= (TwoAsciiToHex(Uart_Buffer1[9], Uart_Buffer1[10]) - 16)) //164)//163)161//156
                {
                    Uart_RecCycle = 0;
                    Uart_RecType  = CHECKSUM;
                }
            } else if (Comm_Flag_2 == 1) {
                if (++Uart_RecCycle >= 20) //163)
                {
                    Uart_RecCycle = 0;
                    Uart_RecType  = CHECKSUM;
                }
            } else if ((Comm_Flag_4 == 1) || (1 == Comm_Flag_5)) {
                if (++Uart_RecCycle >= (TwoAsciiToHex(Uart_Buffer1[9], Uart_Buffer1[10]) - 16)) //164)//163)161//156
                {
                    Uart_RecCycle = 0;
                    Uart_RecType  = CHECKSUM;
                }
            } else if (1 == Comm_Flag_3) {
                /*    if(Buffer == 0x7E)
                    {
                        Uart_Buffer1[Uart_RecCount] = Buffer;
     
                     //   if(!Uart_TxIndex)
                        {
                            Uart_RecOK = 1;
                            Uart_RecType = SOI;
                        }
										}  */
                if (++Uart_RecCycle >= SN_Len) //163)
                {
                    Uart_RecCycle = 0;
                    Uart_RecType  = CHECKSUM;
                }
            }
            break;

        case CHECKSUM: //INFO:
            Uart_Buffer1[Uart_RecCount++] = Buffer;
            if (++Uart_RecCycle >= 4) {
                Uart_RecCycle = 0;
                //    if(1 != Comm_Flag_3)
                Uart_RecType = EOI;
            }

            break;

        case EOI:
            //   Uart_Buffer1[Uart_RecCount++] = Buffer;          //for dubug
            if (Buffer == 0x7E) {
                //Uart_RecOK = 1;
                Uart_Buffer1[Uart_RecCount] = Buffer;

                if (1 == Comm_Flag_4) {
                    Comm_Flag_4 = 0;
                    //  TwoAsciiToHex(Uart_Buffer1[13], Uart_Buffer1[14]);
                    if (0 == TwoAsciiToHex(Uart_Buffer1[13], Uart_Buffer1[14])) X_axisFlag_COM = 0; //0
                    if (180 == TwoAsciiToHex(Uart_Buffer1[13], Uart_Buffer1[14]))                   //180   //76
                        X_axisFlag_COM = 1;                                                         //1
                } else if (1 == Comm_Flag_5) {
                    Comm_Flag_5 = 0;
                    if ('0' == Uart_Buffer1[14]) PlaceFlag_COM = 0;
                    if ('1' == Uart_Buffer1[14]) PlaceFlag_COM = 1;
                } else {
                    if (!Uart_TxIndex) {
                        //Uart_RecOK = 1;
                        //Uart_RecType = SOI;
                        for (i = 0; i < Uart_RX_BuffLen; i++) {
                            Uart_Buffer[i] = Uart_Buffer1[i];
                        }
                    }
                }
                Uart_RecType  = 0;
                Uart_RecCount = 0;
            }
            break;
        default:
            Uart_RecCount = 0;
            Uart_RecType  = 0;
            Uart_RecCycle = 0;

            break;
    }
}

//======================================================================
//Function:	Uart_TansmitIQR()
//Description:	Usart发送中断服务
//======================================================================
void Uart_TansmitIQR(void) {
    /*if(USART_GetITStatus(USART1, USART_IT_TXE)!= RESET)
	{
		if(Uart_TxIndex < Uart_TxCount)
		{
		     USART1->DR = Uart_TxBuffer[Uart_TxIndex++];

		}
		else
		{
		     //USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
		     Uart_TxIndex = 0;
		}
	}*/
}
//======================================================================
//Function:	Uart_RequestData()
//Description:	请求PACK数据
//======================================================================
void Uart_RequestData(void) {
    if (!Timer[DATA_SAMPLE_TIMER].Flag) return;
    Set_Timer(DATA_SAMPLE_TIMER, 40);
    if (sleep_flag == 0) {
        if (++uart_error_flag > UART_ERROR_TIME_10MS) {
            uart_error_flag = 0;
            PackData_Init();
        }
    }
    Uart_TxCount     = 1;
    Uart_TxBuffer[0] = 0x01;
    Uart_TxIndex     = 0;
    Comm_Flag_1      = 1;
    Comm_Flag_2      = 0;
    Comm_Flag_3      = 0;
    //USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
}
//======================================================================
//Function:	Uart_RequestData_2()
//Description:	请求PACK数据
//======================================================================
void Uart_RequestData_2(void) {
    if (!Timer[DATA_SAMPLE_TIMER_2].Flag) return;
    Set_Timer(DATA_SAMPLE_TIMER_2, 45);
    if (sleep_flag == 0) {
        if (++uart_error_flag > UART_ERROR_TIME_10MS) {
            uart_error_flag = 0;
            PackData_Init();
        }
    }
    Uart_TxCount     = 1;
    Uart_TxBuffer[0] = 0x02;
    Uart_TxIndex     = 0;
    Comm_Flag_1      = 0;
    Comm_Flag_2      = 1;
    Comm_Flag_3      = 0;
    //USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
}

/*******************************************************************************
* Function:	Uart_SetParaCMD_1()
* Description:	设置参数命令，设置陀螺仪角度
*******************************************************************************/
void Uart_SetParaCMD_1(uint8_t Data) {
    //    Uart_TxCount = 0;
    //    Uart_TxBuffer[Uart_TxCount++] = ':';	       //SOI
    //    Uart_TxBuffer[Uart_TxCount++] = 0x80;
    //    Uart_TxBuffer[Uart_TxCount++] = 0x02;
    //    if(1 == Data)       //180=0xB4
    //    {
    //        Uart_TxBuffer[Uart_TxCount++] = 0x00;
    //        Uart_TxBuffer[Uart_TxCount++] = 0xB4;
    //    }
    //    else if(0 == Data)
    //    {
    //        Uart_TxBuffer[Uart_TxCount++] = 0x00;
    //        Uart_TxBuffer[Uart_TxCount++] = 0x00;
    //    }
    //    Uart_TxBuffer[Uart_TxCount++] = '~';
    //
    //
    //    Uart_TxIndex = 0;
    //
    //    //USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
}

/*******************************************************************************
* Function:	Uart_SetParaCMD_2()
* Description:	命令2，陀螺仪0--水平；1--垂直
*******************************************************************************/
void Uart_SetParaCMD_2(uint8_t Data) {
    //    Uart_TxCount = 0;
    //    Uart_TxBuffer[Uart_TxCount++] = ':';	       //SOI
    //    Uart_TxBuffer[Uart_TxCount++] = 0x81;
    //    Uart_TxBuffer[Uart_TxCount++] = 0x02;
    //    if(1 == Data)
    //    {
    //        Uart_TxBuffer[Uart_TxCount++] = 0x00;
    //        Uart_TxBuffer[Uart_TxCount++] = 0x01;
    //    }
    //    else if(0 == Data)
    //    {
    //        Uart_TxBuffer[Uart_TxCount++] = 0x00;
    //        Uart_TxBuffer[Uart_TxCount++] = 0x00;
    //    }
    //    Uart_TxBuffer[Uart_TxCount++] = '~';
    //
    //
    //    Uart_TxIndex = 0;
    //
    //    //USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
}

/*******************************************************************************
* Function:	Uart_GetParaCMD_1()
* Description:	读取陀螺仪角度
*******************************************************************************/
void Uart_GetParaCMD_1(void) {
    //    Uart_TxCount = 0;
    //    Uart_TxBuffer[Uart_TxCount++] = ':';	       //SOI
    //    Uart_TxBuffer[Uart_TxCount++] = 0x80;
    //    Uart_TxBuffer[Uart_TxCount++] = 0x00;
    //    Uart_TxBuffer[Uart_TxCount++] = '~';
    //    Uart_TxIndex = 0;
    //    Comm_Flag_4 = 1;
    //    //USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
}

/*******************************************************************************
* Function:	Uart_GetParaCMD_2()
* Description:	读取水平或垂直
*******************************************************************************/
void Uart_GetParaCMD_2(void) {
    //    Uart_TxCount = 0;
    //    Uart_TxBuffer[Uart_TxCount++] = ':';	       //SOI
    //    Uart_TxBuffer[Uart_TxCount++] = 0x81;
    //    Uart_TxBuffer[Uart_TxCount++] = 0x00;
    //    Uart_TxBuffer[Uart_TxCount++] = '~';
    //    Uart_TxIndex = 0;
    //    Comm_Flag_5 = 1;
    //    //USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
}

//======================================================================
//Function:	Uart_BuffClean()
//Description:	解析完对应的数据，将串口接收Buff清空
//======================================================================
void Uart_BuffClean(void) {
    uint8_t i;

    for (i = 0; i < Uart_RX_BuffLen; i++) {
        //        Uart_Buffer[i] = 0;
        //        Uart_RecCount=0;
    }
}
//======================================================================
//Function:	Uart_ConvertData()
//Description:	处理PACK响应数据
//======================================================================
void Uart_ConvertData(void) {
    uint8_t i, Index;
    //uint16_t Tmp;
    uint8_t Data_1 = 0, Data_2 = 0;
    uint16_t Current_Data1 = 0, Current_Data2 = 0;

    Index         = 27;
    Data_1        = TwoAsciiToHex(Uart_Buffer[Index], Uart_Buffer[Index + 1]); //总电压
    Data_2        = TwoAsciiToHex(Uart_Buffer[Index + 2], Uart_Buffer[Index + 3]);
    PackData.Vsum = (Data_1 << 8) | Data_2;
    PackData.Vsum = PackData.Vsum * 2;
    Index += 4;

    Data_1        = TwoAsciiToHex(Uart_Buffer[Index], Uart_Buffer[Index + 1]); //充电电流
    Data_2        = TwoAsciiToHex(Uart_Buffer[Index + 2], Uart_Buffer[Index + 3]);
    Current_Data1 = (Data_1 << 8) | Data_2;
    Index += 4;

    Data_1        = TwoAsciiToHex(Uart_Buffer[Index], Uart_Buffer[Index + 1]); //放电电流
    Data_2        = TwoAsciiToHex(Uart_Buffer[Index + 2], Uart_Buffer[Index + 3]);
    Current_Data2 = (Data_1 << 8) | Data_2;
    Index += 4;

    if (Current_Data1 > 0) //充放电只能显示一个
    {
        PackData.Current = Current_Data1;
        Dis_Charge_Flag  = 1;
    } else if (Current_Data2 > 0) {
        PackData.Current = Current_Data2;
        Dis_Charge_Flag  = 2;
    } else {
        PackData.Current = 0;
        Dis_Charge_Flag  = 0;
    }
    if ((Current_Data1 == 0) && (Current_Data2 == 0)) {
        //PackData.Current=0;
        //Dis_Charge_Flag=0;
    }

    Soc = TwoAsciiToHex(Uart_Buffer[Index], Uart_Buffer[Index + 1]);
    Index += 4;
    //Index += 2;

    Data_1      = TwoAsciiToHex(Uart_Buffer[Index], Uart_Buffer[Index + 1]); //剩余容量
    Data_2      = TwoAsciiToHex(Uart_Buffer[Index + 2], Uart_Buffer[Index + 3]);
    PackData.Rm = (Data_1 << 8) | Data_2;
    PackData.Rm = PackData.Rm * 10;
    Index += 4;

    Data_1       = TwoAsciiToHex(Uart_Buffer[Index], Uart_Buffer[Index + 1]); //满充容量
    Data_2       = TwoAsciiToHex(Uart_Buffer[Index + 2], Uart_Buffer[Index + 3]);
    PackData.Fcc = (Data_1 << 8) | Data_2;
    PackData.Fcc = PackData.Fcc * 10;
    Index += 4;

    PackData.DesignCap = PackData.Fcc;

    Data_1         = TwoAsciiToHex(Uart_Buffer[Index], Uart_Buffer[Index + 1]); //循环次数
    Data_2         = TwoAsciiToHex(Uart_Buffer[Index + 2], Uart_Buffer[Index + 3]);
    PackData.Cycle = (Data_1 << 8) | Data_2;
    Index += 8;

    PackData.BatNum = TwoAsciiToHex(Uart_Buffer[Index], Uart_Buffer[Index + 1]); //电池串数
    Index += 2;
    for (i = 0; i < PackData.BatNum; i++) //实际值
    {
        PackData.Vol[i] = TwoAsciiToHex(Uart_Buffer[Index], Uart_Buffer[Index + 1]);
        PackData.Vol[i] <<= 8;
        PackData.Vol[i] += TwoAsciiToHex(Uart_Buffer[Index + 2], Uart_Buffer[Index + 3]);
        Index += 4;
    }

    PackData.TempNum = TwoAsciiToHex(Uart_Buffer[Index], Uart_Buffer[Index + 1]); //温度个数
    Index += 2;
    for (i = 0; i < PackData.TempNum; i++) //实际值
    {
        PackData.Temp[i] = TwoAsciiToHex(Uart_Buffer[Index], Uart_Buffer[Index + 1]);
        if (PackData.Temp[i] == 189) {
            PackData.Temp[i] = 0;
        }
        Index += 2;
    }

    Data_1           = TwoAsciiToHex(Uart_Buffer[Index], Uart_Buffer[Index + 1]); //״̬��1
    Data_2           = TwoAsciiToHex(Uart_Buffer[Index + 2], Uart_Buffer[Index + 3]);
    PackData.Status1 = (Data_1 << 8) | Data_2;
    Index += 4;

    Data_1           = TwoAsciiToHex(Uart_Buffer[Index], Uart_Buffer[Index + 1]); //״̬��2
    Data_2           = TwoAsciiToHex(Uart_Buffer[Index + 2], Uart_Buffer[Index + 3]);
    PackData.Status2 = (Data_1 << 8) | Data_2;
    Index += 4;

    Data_1           = TwoAsciiToHex(Uart_Buffer[Index], Uart_Buffer[Index + 1]); //״̬��5
    Data_2           = TwoAsciiToHex(Uart_Buffer[Index + 2], Uart_Buffer[Index + 3]);
    PackData.Status5 = (Data_1 << 8) | Data_2;
    Index += 4;

    Data_1            = TwoAsciiToHex(Uart_Buffer[Index], Uart_Buffer[Index + 1]); //������1
    Data_2            = TwoAsciiToHex(Uart_Buffer[Index + 2], Uart_Buffer[Index + 3]);
    PackData.Warning1 = (Data_1 << 8) | Data_2;
    Index += 4;

    Data_1            = TwoAsciiToHex(Uart_Buffer[Index], Uart_Buffer[Index + 1]); //������2
    Data_2            = TwoAsciiToHex(Uart_Buffer[Index + 2], Uart_Buffer[Index + 3]);
    PackData.Warning2 = (Data_1 << 8) | Data_2;

    getValidData = 1;

    Uart_BuffClean();
}

void Uart_ConvertData_2(void) {
    uint8_t Index;
    uint8_t Data_1 = 0, Data_2 = 0;

    Index                 = 11;
    Data_1                = TwoAsciiToHex(Uart_Buffer[Index], Uart_Buffer[Index + 1]);
    Data_2                = TwoAsciiToHex(Uart_Buffer[Index + 2], Uart_Buffer[Index + 3]); //过充次数
    Record.Over_Chg_Count = (Data_1 << 8) | Data_2;

    Index += 4;
    Data_1                 = TwoAsciiToHex(Uart_Buffer[Index], Uart_Buffer[Index + 1]);
    Data_2                 = TwoAsciiToHex(Uart_Buffer[Index + 2], Uart_Buffer[Index + 3]); //过放次数
    Record.Over_Dchg_Count = (Data_1 << 8) | Data_2;

    Index += 4;
    Data_1                 = TwoAsciiToHex(Uart_Buffer[Index], Uart_Buffer[Index + 1]);
    Data_2                 = TwoAsciiToHex(Uart_Buffer[Index + 2], Uart_Buffer[Index + 3]); //过流次数
    Record.Over_Curr_Count = (Data_1 << 8) | Data_2;

    Index += 4;
    Data_1                 = TwoAsciiToHex(Uart_Buffer[Index], Uart_Buffer[Index + 1]);
    Data_2                 = TwoAsciiToHex(Uart_Buffer[Index + 2], Uart_Buffer[Index + 3]); //过温
    Record.Over_Temp_Count = (Data_1 << 8) | Data_2;

    Index += 4;
    Data_1             = TwoAsciiToHex(Uart_Buffer[Index], Uart_Buffer[Index + 1]);
    Data_2             = TwoAsciiToHex(Uart_Buffer[Index + 2], Uart_Buffer[Index + 3]); //短路次数
    Record.Short_Count = (Data_1 << 8) | Data_2;

    Uart_BuffClean();
}

void Uart_ConvertData_3(void) {
    uint8_t Index, i;

    Index += 4; //版本号
    for (i = 0; i < 20; i++) {
        BMS_Version[i] = Uart_Buffer[Index];
        Index++;
    }

    Uart_BuffClean();
}

//======================================================================
//Function:	Uart_DataRespond()
//Description:	数据响应
//======================================================================
void Uart_DataRespond(void) {
    //uint8_t Rtc;
    uint8_t CRC_1, CRC_2;
    uint8_t u8Len = 0;
    u8Len         = TwoAsciiToHex(Uart_Buffer[9], Uart_Buffer[10]);

    if ((Uart_RecOK == 1) && (Comm_Flag_1 == 1)) {
        Uart_RecOK = 0;
        //  Comm_Flag_1 = 0;
        //  Comm_Flag_2 = 0;
        //           g_CRC = GetCRC16CheckCode(Uart_Buffer+1, (u8Len-6), 0xa001);//166
        //           CRC_1 = TwoAsciiToHex(Uart_Buffer[u8Len-5], Uart_Buffer[u8Len-4]);
        //           CRC_2 = TwoAsciiToHex(Uart_Buffer[u8Len-3], Uart_Buffer[u8Len-2]);
        //           CRC_3 = (CRC_1<<8)|CRC_2;
        //   if(Key_Flag==0)
        if (New_Page_Status == PAGE_WELCOME) {
            Page_Welcome_1();
        }
        //           Time_Buffer[2] = Uart_Buffer[13];
        //           Time_Buffer[3] = Uart_Buffer[14];
        //           Time_Buffer[4] = Uart_Buffer[15];
        //           Time_Buffer[5] = Uart_Buffer[16];
        //           Time_Buffer[6] = Uart_Buffer[17];
        //           Time_Buffer[7] = Uart_Buffer[18];
        //           Time_Buffer[8] = Uart_Buffer[21];
        //           Time_Buffer[9] = Uart_Buffer[22];
        //           Time_Buffer[10] = Uart_Buffer[23];
        //           Time_Buffer[11] = Uart_Buffer[24];
        //
        //           if(g_CRC!=CRC_3) return;
        //
        //        Uart_ConvertData();
        uart_error_flag = 0;
        // APP_PRINT("%s, %d\n", __func__, uart_error_flag)
        // return;
    }
    // APP_PRINT(".");
    // return;
}

//======================================================================
//Function:	Uart_DataRespond()
//Description:	数据响应
//======================================================================
void Uart_DataRespond_2(void) {
    uint8_t CRC_1, CRC_2;
    if ((Uart_RecOK == 1) && (Comm_Flag_2 == 1)) {
        Uart_RecOK  = 0;
        Comm_Flag_1 = 0;
        Comm_Flag_2 = 0;
        g_CRC       = GetCRC16CheckCode(Uart_Buffer + 1, 30, 0xa001);
        CRC_1       = TwoAsciiToHex(Uart_Buffer[31], Uart_Buffer[32]);
        CRC_2       = TwoAsciiToHex(Uart_Buffer[33], Uart_Buffer[34]);
        CRC_3       = (CRC_1 << 8) | CRC_2;

        if (g_CRC != CRC_3) return;

        Uart_ConvertData_2();
        uart_error_flag = 0;
        // APP_PRINT("%s, %d\n", __func__, uart_error_flag)
        // return;
    }
    // APP_PRINT(".");
    // return;
}

/*******************************************************************************
* Function:	Uart_DataRespond_4()
* Description:	陀螺仪数据响应
*******************************************************************************/
void Uart_DataRespond_4(void) {
    uint8_t CRC_1, CRC_2;

    if ((Uart_RecOK == 1) && (Comm_Flag_4 == 1)) {
        Uart_RecOK  = 0;
        Comm_Flag_4 = 0;

        g_CRC = GetCRC16CheckCode(Uart_Buffer + 1, 30, 0xa001);
        CRC_1 = TwoAsciiToHex(Uart_Buffer[31], Uart_Buffer[32]);
        CRC_2 = TwoAsciiToHex(Uart_Buffer[33], Uart_Buffer[34]);
        CRC_3 = (CRC_1 << 8) | CRC_2;

        //   if(g_CRC!=CRC_3)
        //      return;
        if ('0' == Uart_Buffer[11]) X_axisFlag_COM = 0;
        if ('1' == Uart_Buffer[11]) X_axisFlag_COM = 1;
        //    X_axisFlag_COM = Uart_Buffer[11];
        uart_error_flag = 0;
    }
}

/*******************************************************************************
* Function:	Uart_DataRespond_5()
* Description:	
*******************************************************************************/
void Uart_DataRespond_5(void) {
    uint8_t CRC_1, CRC_2;

    if ((Uart_RecOK == 1) && (Comm_Flag_5 == 1)) {
        Uart_RecOK  = 0;
        Comm_Flag_5 = 0;

        g_CRC = GetCRC16CheckCode(Uart_Buffer + 1, 30, 0xa001);
        CRC_1 = TwoAsciiToHex(Uart_Buffer[31], Uart_Buffer[32]);
        CRC_2 = TwoAsciiToHex(Uart_Buffer[33], Uart_Buffer[34]);
        CRC_3 = (CRC_1 << 8) | CRC_2;

        //   if(g_CRC!=CRC_3)
        //      return;
        if ('0' == Uart_Buffer[11]) PlaceFlag_COM = 0;
        if ('1' == Uart_Buffer[11]) PlaceFlag_COM = 1;
        uart_error_flag = 0;
    }
}

/*******************************************************************************
* Function:	UartInit()
* Description:	
*******************************************************************************/
void UartInit(void) {
    Uart_TxIndex = 0; //Usart当前发送序号
    Uart_TxCount = 0; //Usart计数
    Uart_RecOK   = 0; //Usart接收完成标志
    //Dis_Charge_Flag=0;
    uart_error_flag = 0;
    sleep_flag      = 0;
    CRC_3           = 0;
    g_CRC           = 0;

    Comm_Flag_1    = 0;
    Comm_Flag_2    = 0;
    Comm_Flag_3    = 0;
    Comm_Flag_4    = 0;
    Comm_Flag_5    = 0;
    PlaceFlag_COM  = 0;
    X_axisFlag_COM = 0;
    SetCommondFlag = 0;
    Key_Flag       = 0;
}

//======================================================================
//Function:	GetHighTemp()
//Description:	获取最高温度值
//======================================================================
int16_t GetHighTemp(void) {
    int8_t i;
    int16_t Temperature;

    Temperature = PackData.Temp[0];
    //    for(i = 0; i < PackData.TempNum; i++)
    for (i = 0; i < 4; i++) {
        if (PackData.Temp[i] > Temperature) Temperature = PackData.Temp[i];
    }
    return Temperature;
}

//======================================================================
//Function:	GetLowTemp()
//Description:	获取最低温度值
//======================================================================
int16_t GetLowTemp(void) {
    int8_t i;
    int16_t Temperature;

    Temperature = PackData.Temp[0];
    for (i = 0; i < 4; i++) {
        if (PackData.Temp[i] < Temperature) Temperature = PackData.Temp[i];
    }
    return Temperature;
}

//======================================================================
//Function:	GetHighBatVol()
//Description:	获取最高单体电压值
//======================================================================
uint16_t GetHighBatVol(void) {
    uint8_t i;
    uint16_t BatVol;

    BatVol = PackData.Vol[0];
    for (i = 0; i < PackData.BatNum; i++)
    //       for(i = 0; i < 15; i++)
    {
        if (PackData.Vol[i] > BatVol) BatVol = PackData.Vol[i];
    }
    return BatVol;
}
//======================================================================
//Function:	GetLowBatVol()
//Description:	获取最低单体电压值
//======================================================================
uint16_t GetLowBatVol(void) {
    uint8_t i;
    uint16_t BatVol;

    BatVol = PackData.Vol[0];
    for (i = 0; i < PackData.BatNum; i++)
    //        for(i = 0; i < 15; i++)   //小于15串做比较
    {
        if (PackData.Vol[i] < BatVol) BatVol = PackData.Vol[i];
    }
    return BatVol;
}
