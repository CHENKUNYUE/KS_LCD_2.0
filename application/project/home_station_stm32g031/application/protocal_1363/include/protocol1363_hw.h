/*
 * Copyright (c) Daren Hi-Tech Electronics Co., Ltd. Development Team 2020-2020. All rights reserved.
 * File name:   protocol1363_hw.h
 * Author: cq   ID:     Version:    Date: 2021-01-11
 * Description: // 用于详细说明此程序文件完成的主要功能，与其他模块
 *              // 或函数的接口，输出值、取值范围、含义及参数间的控
 *              // 制、顺序、独立或依赖等关系
 * Others:      // 其它内容的说明
 * History:     // 修改历史记录列表，每条修改记录应包括修改日期、修改
 *              // 者及修改内容简述
 *              1. Date:
 *              Author:               ID:
 *              Modification:
 *              2. ...
 */
#ifndef COM_PROTOCAL_1363_PROTOCOL1363_HW_H__
#define     COM_PROTOCAL_1363_PROTOCOL1363_HW_H__

#include <stdint.h>
#include "paras.h"
#include "protocol1363_transfer.h"

#define DEBUG_PROTOCOL1363_HW 0
#ifndef NULL
#define NULL 0
#endif
#if (DEBUG_PROTOCOL1363_HW == 0)

#else
#include <stdbool.h>
    #include <stdio.h>
    #include <string.h>
#endif

typedef void (*PROTOCOL_SEND_FUNC_TYPEDEF)(uint8_t *ptr ,uint16_t length,uint8_t is_rs485);

void protocol_hw_send_one_frame(comm_struct *com, uint8_t *buffer, uint16_t len);
void protocol_hw_set_uart1_send_callback(PROTOCOL_SEND_FUNC_TYPEDEF protocol_send_ptr);
void protocol_hw_set_uart2_send_callback(PROTOCOL_SEND_FUNC_TYPEDEF protocol_send_ptr);
void protocol_hw_set_uart3_send_callback(PROTOCOL_SEND_FUNC_TYPEDEF protocol_send_ptr);

#endif //COM_PROTOCAL_1363_PROTOCOL1363_HW_H__
