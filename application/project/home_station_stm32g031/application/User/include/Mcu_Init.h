//======================================================================
//Copyright (C), 2014-2999, Tech. Co., Ltd.
//======================================================================
//FileName: Data_Init.h
//Author:
//Version: 1.0.0
//Date:
//Description:
//======================================================================
#ifndef __MCU_INIT_H
#define __MCU_INIT_H
#include "stm32g0xx_hal.h"


/*中断定义*/
#define  EN_INT             __enable_irq()		    //开全局中断
#define  DIS_INT            __disable_irq()           //关全局中断

#define  EN_KEY1_INT        HAL_NVIC_EnableIRQ(EXTI4_15_IRQn)
#define  DIS_KEY1_INT       HAL_NVIC_DisableIRQ(EXTI4_15_IRQn)

#define  EN_KEY2_INT        HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
#define  DIS_KEY2_INT       HAL_NVIC_DisableIRQ(EXTI4_15_IRQn)

#define  EN_KEY3_INT        HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
#define  DIS_KEY3_INT       HAL_NVIC_DisableIRQ(EXTI4_15_IRQn)

#define  EN_KEY4_INT        HAL_NVIC_EnableIRQ(EXTI0_1_IRQn);
#define  DIS_KEY4_INT       HAL_NVIC_DisableIRQ(EXTI0_1_IRQn)

#define  EN_KEY5_INT        HAL_NVIC_EnableIRQ(EXTI0_1_IRQn);
#define  DIS_KEY5_INT       HAL_NVIC_DisableIRQ(EXTI0_1_IRQn)

/*输出端口定义*/
//#define  BL_EN             HAL_NVIC_EnableIRQ(EXTI4_15_IRQn)//GPIOB->ODR |= GPIO_Pin_7    //中断
//#define  _BL_EN            HAL_NVIC_DisableIRQ(EXTI4_15_IRQn)//GPIOB->ODR &= (uint8_t)(~GPIO_Pin_7)

/*输入端口定义*/
//#define  KEY_DOWN          HAL_GPIO_ReadPin(GPIOB ,GPIO_PIN_0)//XS/ok 4
#define  KEY_ESC          HAL_GPIO_ReadPin(GPIOB ,GPIO_PIN_0)//XS/ok 4

//#define  KEY_UP            HAL_GPIO_ReadPin(GPIOA ,GPIO_PIN_7)//XX/OK 3
#define  KEY_DOWN          HAL_GPIO_ReadPin(GPIOA ,GPIO_PIN_7)//XS/ok 4

#define  KEY_MENU          HAL_GPIO_ReadPin(GPIOA ,GPIO_PIN_5)//CD/ok  1

#define  KEY_ENTER         HAL_GPIO_ReadPin(GPIOA ,GPIO_PIN_6)//JR   2


//#define  KEY_ESC           HAL_GPIO_ReadPin(GPIOB ,GPIO_PIN_1)// TC   5



/*函数声明*/
void Mcu_Init(void);
void IWDG_ReloadCounter(void);
#endif
