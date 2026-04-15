//======================================================================
//Copyright (C), 2014-2999, Tech. Co., Ltd.
//======================================================================
//FileName:  Mcu_Init.c
//Author:
//Version: 1.0.0
//Date:
//Description:
//======================================================================
#include "Mcu_Init.h"
#include "User_Conf.h"
#include "stm32g0xx_hal.h"
#include <gpio.h>
#include "stm32g0xx_ll_iwdg.h"

//======================================================================
//Function:	CLK_Init()
//Description:  8MHZ
//======================================================================
void CLK_Init(void)
{
    //CLK_LSICmd(ENABLE);
    //while(CLK_GetFlagStatus(CLK_FLAG_LSIRDY) == (uint8_t)RESET);
    
    //CLK_SYSCLKSourceSwitchCmd(ENABLE);
    /*CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSI);
    CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_2);
    while(CLK_GetSYSCLKSource() != CLK_SYSCLKSource_HSI);
    
    ITC->ISPR1 = 0x55;
    ITC->ISPR2 = 0x55;
    ITC->ISPR3 = 0x55;
    ITC->ISPR4 = 0x55;
    ITC->ISPR5 = 0x55;
    ITC->ISPR6 = 0x55;
    ITC->ISPR7 = 0x55;
    ITC->ISPR8 = 0x05;
    
    ITC_SetSoftwarePriority(USART1_RX_IRQn, ITC_PriorityLevel_2); */       //Usart1_R   
}
//======================================================================
//Function:	GPIO_Config()
//Description:  ODR输出数据寄存器
//              DDR方向寄存器，1：输出，0：输入
//              CR1控制寄存器，OUTPUT：0：漏极开路，1：推挽
//                             INPUT：0：悬浮输入，1：上拉输入
//              CR2控制寄存器，OUTPUT：0：2MHZ，1：10MHZ，
//                             INPUT：0：禁止外部中断，1：使能外部中断
//======================================================================
void GPIO_Config(void)
{
//PORT A GROUP  
    /*BIT     7     6     5     4     3     2     1     0   */
    /*ODR     0     0     0     0     0     0     0     0   */
    /*DDR     1     1     1     1     1     1     1     1   */
    /*CR1     1     1     1     1     1     1     1     1   */
    /*CR2     0     0     0     0     0     0     0     0   */
    /*NAME  W/R(-)  RS    -     -     -     -   NRST  SWIM  */
    /*		                                                */
    
    //GPIOA->ODR = 0x00;
    //GPIOA->DDR = 0xFF;
    //GPIOA->CR1 = 0xFF;
    //GPIOA->CR2 = 0x00;
    
//PORT B GROUP  
    /*BIT     7     6     5     4     3     2     1     0   */
    /*ODR     0     0     0     0     0     0     0     0   */
    /*DDR     1     1     1     1     1     1     1     1   */
    /*CR1     1     1     1     1     1     1     1     1   */
    /*CR2     0     0     0     0     0     0     0     0   */
    /*NAME D7/SDO D6/SCK  D5   D4    D3    D2    D1    D0   */
    /*		                                                */
    
//    GPIOB->ODR = 0x00;                             
//    GPIOB->DDR = 0xFF;
//    GPIOB->CR1 = 0xFF;
//    GPIOB->CR2 = 0x00;
    
//PORT C GROUP  
    /*BIT     7     6     5     4     3     2     1     0   */
    /*ODR     0     0     0     0     1     0     0     0   */
    /*DDR     1     1     1     1     1     0     1     1   */
    /*CR1     1     1     1     1     1     1     0     0   */
    /*CR2     0     0     0     0     0     0     0     0   */
    /*NAME   P/S   C86    -     -    TX1   RX1    -     -   */
    /*       CS	    	                         SDA   SCL  */
    
//    GPIOC->ODR = 0x0B;//0x03;
//    GPIOC->DDR = 0xFB;//0xFB
//    GPIOC->CR1 = 0xFF;//0xFC;
//    GPIOC->CR2 = 0x00;
    
    //GPIOC->ODR = 0x08;
    //GPIOC->DDR = 0xEB;
    //GPIOC->CR1 = 0xEC;
   // GPIOC->CR2 = 0x00;
    
//PORT D GROUP  
    /*BIT     7     6     5     4     3     2     1     0   */
    /*ODR     0     0     0     0     0     0     0     0   */
    /*DDR     1     1     1     1     0     0     0     0   */
    /*CR1     1     1     1     1     0     0     0     0   */
    /*CR2     0     0     0     0     0     0     0     0   */
    /*NAME    -     -     -     -    KEY5  KEY4  KEY3  KEY2 */
    /*	     KEY5  KEY4  KEY3  KEY2                                            */
    
//    GPIOD->ODR = 0x00;
//    GPIOD->DDR = 0x0F;//0xF0;
//    GPIOD->CR1 = 0x0F;//0xF0;
//    GPIOD->CR2 = 0x00;
    
//PORT E GROUP  
    /*BIT     7     6     5     4     3     2     1     0   */
    /*ODR     0     0     0     0     0     0     0     0   */
    /*DDR     1     1     1     0     1     1     1     1   */
    /*CR1     1     1     1     0     1     1     1     1   */
    /*CR2     0     0     0     0     0     0     0     0   */
    /*NAME   RST   RD(-)  -    KEY1   -   BL_EN   CS    -   */
    /*	     A0     RES	                                                */
    
//    GPIOE->ODR = 0x00;
//    GPIOE->DDR = 0xEE;  //0xEF		//LCD_SDA_OUT ���� E0 ��Ϊ����
//    GPIOE->CR1 = 0xEE;  //0xEF
//    GPIOE->CR2 = 0x00;
    
//PORT F GROUP  
    /*BIT     7     6     5     4     3     2     1     0   */
    /*ODR     0     0     0     0     0     0     0     0   */
    /*DDR     1     1     1     1     1     1     1     1   */
    /*CR1     1     1     1     1     1     1     1     1   */
    /*CR2     0     0     0     0     0     0     0     0   */
    /*NAME    -     -     -     -     -     -     -     -   */
    /*		                                        K1  */
    
//    GPIOF->ODR = 0x00;
//    GPIOF->DDR = 0xFE;//0xFF;
//    GPIOF->CR1 = 0xFE;//0xFF;
//    GPIOF->CR2 = 0x00;
}

//======================================================================
//Function:	EXTI_Config()
//Description:	
//======================================================================
void EXTI_Config(void)
{
    /*HAL_NVIC_EnableIRQ(EXTI_LINE_0, EXTI_TRIGGER_FALLING);
    
    HAL_NVIC_EnableIRQ(EXTI_LINE_4, EXTI_TRIGGER_FALLING);
    
    HAL_NVIC_EnableIRQ(EXTI_LINE_5, EXTI_TRIGGER_FALLING);
    
    HAL_NVIC_EnableIRQ(EXTI_LINE_6, EXTI_TRIGGER_FALLING);
    
    HAL_NVIC_EnableIRQ(EXTI_LINE_7, EXTI_TRIGGER_FALLING);
    
    EXTI->SR1 = 0xFF;*/
}

//======================================================================
//Function:	IWDG_Config()
//Description:  配置独立看门狗
//======================================================================
//IWDG_HandleTypeDef IWDG_Handle;
void IWDG_Config(uint8_t SetReload)
{
    IWDG_Init(7,SetReload);

//    IWDG_Handle.Instance = IWDG;
//	IWDG_Handle.Init.Prescaler = IWDG_PRESCALER_128;
//	IWDG_Handle.Init.Reload = SetReload;
//	IWDG_Handle.Init.Window = IWDG_WINDOW_DISABLE;
//	HAL_IWDG_Init(&IWDG_Handle);
//	__HAL_IWDG_START(&IWDG_Handle);
//    HAL_IWDG_Refresh(&IWDG_Handle);
}


//======================================================================
//Function:	Mcu_Init()
//Description:	MCU初始化
//======================================================================
void Mcu_Init(void)
{
    CLK_Init();
    GPIO_Config();
    MX_GPIO_Init();
    Delay_ms(10000);
	  
    
    EXTI_Config();
    TIM2_Init();
    
    Usart1_Init(9600);
    // Usart1_Init(19200);
    IWDG_Config(200);
	
    /*FLASH_Unlock(FLASH_MemType_Data);
    if(0x02 != FLASH_ReadByte(0x4803))
        FLASH_ProgramOptionByte(0x4803, 0x02);
    if(0x02 != FLASH_ReadByte(0x4808))
        FLASH_ProgramOptionByte(0x4808, 0x02);
    
    FLASH_Lock(FLASH_MemType_Data);*/
}
//======================================================================
//ENDFILE
//======================================================================

