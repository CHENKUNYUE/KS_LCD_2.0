/*
 * Copyright (c) Daren Hi-Tech Electronics Co., Ltd. Development Team 2020-2020. All rights reserved.
 * File name:   protocol1363_hw.c
 * Author: cq   ID:     Version:    Date: 20/12/2020
 * Description: // 用于详细说明此程序文件完成的主要功能，与其他模块
 *              // 或函数的接口，输出值、取值范围、含义及参数间的控
 *              // 制、顺序、独立或依赖等关系
 * Others:      // 其它内容的说明
 * History:     // 修改历史记录列表，每条修改记录应包括修改日期、修改
 *              // 者及修改内容简述
 *              Date           Author       Notes
 */


#include "protocol1363_transfer.h"
#include "protocol1363_hw.h"


PROTOCOL_SEND_FUNC_TYPEDEF g_protocol_usart1_send_func = NULL;
PROTOCOL_SEND_FUNC_TYPEDEF g_protocol_usart2_send_func = NULL;
PROTOCOL_SEND_FUNC_TYPEDEF g_protocol_usart3_send_func = NULL;

void protocol_hw_set_uart1_send_callback(PROTOCOL_SEND_FUNC_TYPEDEF protocol_send_ptr) {
    g_protocol_usart1_send_func = protocol_send_ptr;
}

/**
 * @Description:调用硬件函数，发送一帧
 * @param  - . ref: comm_struct *com， uint8_t *buffer， uint16_t len
 * @return
 */
void protocol_hw_send_one_frame(comm_struct *com, uint8_t *buffer, uint16_t len) {
#if DEBUG_PROTOCOL1363_HW == 0
    if (com->com_no == 1) {
        if (g_protocol_usart1_send_func != NULL) { (*g_protocol_usart1_send_func)(buffer, len, com->is_rs485_yes); }
    } else if (com->com_no == 2) {
        if (g_protocol_usart2_send_func != NULL) { (*g_protocol_usart2_send_func)(buffer, len, com->is_rs485_yes); }
    } else if (com->com_no == 3) {
        if (g_protocol_usart3_send_func != NULL) { (*g_protocol_usart3_send_func)(buffer, len, com->is_rs485_yes); }
    }
#elif DEBUG_PROTOCOL1363_HW == 1
    uint8_t i;
    if (com->com_no == 0) {
        if (com->is_rs485_yes) { printf("this RS485 com-0 \r\n"); }
        for (i = 0; i < len; i++) { printf("%c", *(buffer + i)); }
    } else if (com->com_no == 1) {
        if (com->is_rs485_yes) { printf("this com-1 \r\n"); }
        for (i = 0; i < len; i++) { printf("%c", buffer + i); }
    } else if (com->com_no == 3) {
        if (com->is_rs485_yes) { printf("this com-2 \r\n"); }
        for (i = 0; i < len; i++) { printf("%c", buffer + i); }
    }
#endif
}