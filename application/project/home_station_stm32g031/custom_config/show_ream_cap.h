/*
* . All rights reserved.
* File name:   build_parameters.h
* Author: Donn1e   ID:     Version:V1.0    Date: 2026/1/1
* Description:  客户定制H文件
* Others:
* History:
*/
#ifndef BUILD_PARAMETERS_H__
#define BUILD_PARAMETERS_H__
//#include "cu_code.h"

/**************** BASIC PARA ***********************************************************************/

#define     LCD_HW_VERSION                                  "LG-LCD-03-V1"//硬件版本
#define     LCD_SW_VERSION                                  "LG-V3.0.0-C"//软件版本

#define     BAUD_RATE_19200                                 (19200)
#define     BAUD_RATE_9600                                  (9600)

//波特率
#define     BAUD_RATE                                       (BAUD_RATE_9600)

#define DEFAULT_IS_OLD_UI_ENABLED                           (1) // 0: 新界面, 1: 旧界面
#define DEFAULT_SHOW_OLD_CAPACITY                           (1) // 0: 上电不显示剩余容量, 1: 上电显示剩余容量 Rm+AH


#endif
