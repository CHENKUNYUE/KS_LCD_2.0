//======================================================================
//======================================================================
//FileName:  User_Usart.c
//
//Author:    Wang Zhong Guo
//
//Version:  1.0.0 
//
//Date:     2017.6.2
//
//Description: 
//
//======================================================================

#include "User_Usart.h"


//--------------------------全局变量------------------------------------
uint8_t Temp[2];


//======================================================================
//Function:	Usart1_Init()
//
//Description:	Usart1初始化
//
//======================================================================
void Usart1_Init(uint32_t BaudRate)
{
//    CLK_PeripheralClockConfig(CLK_Peripheral_USART1, ENABLE);
//    
//    USART_DeInit(USART1);
//    USART_Init(USART1, BaudRate, USART_WordLength_8b, USART_StopBits_1,\
//               USART_Parity_No, (USART_Mode_TypeDef)(USART_Mode_Rx | USART_Mode_Tx));
//    
//    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
//    USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
//    USART_Cmd(USART1, DISABLE);
}


//======================================================================
//Function:	TwoAsciiToHex()
//Description:	两个ASCII码转成一个十六进制数
//======================================================================
uint8_t TwoAsciiToHex(uint8_t Ch1, uint8_t Ch2)
{
	uint8_t temp;
    
	if(Ch1 >= '0' && Ch1 <= '9')
	{
		temp = Ch1 - 0x30;
	}
	else if(Ch1 >= 'A' && Ch1 <= 'F')
	{
		temp = Ch1 - 0x37;
	}
	else if(Ch1 >= 'a' && Ch1 <= 'f')
	{
		temp = Ch1 - 0x57;
	}
	else
	{
		temp = 0;
	}
	
	temp <<= 4;	
	if(Ch2 >= '0' && Ch2 <= '9')
	{
		temp += Ch2 - 0x30;
	}
	else if(Ch2 >= 'A' && Ch2 <= 'F')
	{
		temp += Ch2 - 0x37;
	}
	else if(Ch2 >= 'a' && Ch2 <= 'f')
	{
		temp += Ch2 - 0x57;
	}
	else
	{
		temp += 0;
	}
	
	return temp;
}

//======================================================================
//Function:	HextoTwoAscii()
//Description:	一个十六进制数转成两个ASCII
//======================================================================
void HextoTwoAscii(uint8_t Ch, uint8_t *Pchar)
{
	uint8_t temp;
    
	temp = Ch >> 4;
	if(temp < 10)
	{
		*Pchar = temp + 0x30;
	}
	else if(temp >= 10)
	{
		*Pchar = temp + 0x37;
	}
	else
	{
		*Pchar = 0;
	}
	
	temp = Ch & 0x0F;
	if(temp < 10)
	{
		*(Pchar + 1) = temp + 0x30;
	}
	else if(temp >= 10)
	{
		*(Pchar + 1) = temp + 0x37;
	}
	else
	{
		*(Pchar + 1) = 0;
	}
}

//======================================================================
//Function:	Check_LCHKSUM()
//Description:	检验LCHKSUM有效性
//======================================================================
/*uint8_t Check_LCHKSUM(uint8_t *Pchar)
{
	uint8_t LenL, LenH, sum = 0;
    
	LenH = TwoAsciiToHex(Pchar[9], Pchar[10]);
	LenL = TwoAsciiToHex(Pchar[11], Pchar[12]);
	sum = (LenH & 0x0F) + (LenL >> 4) + (LenL & 0x0F);
	sum = ~(sum % 16) + 1;
	
	if((sum & 0x0F) == (LenH >> 4))
		return 1;
	else 
		return 0;
}

//======================================================================
//Function:	Check_CHKSUM()
//Description:	检验CHKSUM有效性
//======================================================================
uint8_t Check_CHKSUM(uint8_t *Pchar)
{
	uint16_t i, Length, CHK_SUM, sum = 0;
    
	Length = TwoAsciiToHex(Pchar[9], Pchar[10]);
	Length <<= 8;
	Length += TwoAsciiToHex(Pchar[11], Pchar[12]);
	Length &= 0x0FFF;
	Length += 12;
	
	for(i = 1; i <= Length; i++)
	{
	 	sum += Pchar[i];
	}
	sum = ~(sum % 65536) + 1;
	CHK_SUM = TwoAsciiToHex(Pchar[Length + 1], Pchar[Length + 2]);
	CHK_SUM <<= 8;
	CHK_SUM += TwoAsciiToHex(Pchar[Length + 3], Pchar[Length + 4]);
	
	if(sum == CHK_SUM)
		return 1;
	else 
		return 0;
}
*/
//======================================================================
//Function:	Length_Calculate()
//Description:	计算响应信息的Length
//======================================================================
void Length_Calculate(uint8_t *Pchar, uint16_t Len)
{
	uint8_t sum;
	uint16_t Length;
	
	sum = (Len & 0x000F) + ((Len >> 4) & 0x000F) + ((Len >> 8) & 0x000F);
	sum = ~(sum % 16) + 1;
	
	Length = (((uint16_t)sum) << 12) + (Len & 0x0FFF); 
	
	HextoTwoAscii((uint8_t)(Length >> 8), Temp);
	Pchar[9] = Temp[0];
	Pchar[10] = Temp[1];
	HextoTwoAscii((uint8_t)(Length & 0x00FF), Temp);
	Pchar[11] = Temp[0];
	Pchar[12] = Temp[1];
}

//======================================================================
//Function:	Rtn_Create()
//Description:	生成RTN
//======================================================================
void Rtn_Create(uint8_t *Pchar, uint8_t Rtn)
{
	HextoTwoAscii(Rtn, Temp);
	Pchar[7] = Temp[0];
	Pchar[8] = Temp[1];
}

//======================================================================
//ENDFILE
//======================================================================

