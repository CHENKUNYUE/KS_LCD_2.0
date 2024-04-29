/*
 * Copyright (c) Daren Hi-Tech Electronics Co., Ltd. Development Team 2020-2020. All rights reserved.
 * File name:   com_protocal1363_app.h
 * Author: cq   ID:     Version:    Date: 2021-01-14
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
#ifndef COM_PROTOCAL_1363_COM_PROTOCAL1363_APP_H__
#define     COM_PROTOCAL_1363_COM_PROTOCAL1363_APP_H__

#include <stdint.h>

typedef enum {
    enum_get_first1 = 0,
    enum_get_next1,
    enum_get_again,
} command_type_typedef;

void protocol_parallel_app_get_slave_reals_paras(void *ptr,uint8_t add);
void protocol_parallel_S2M_get_warm_cnt(void *ptr);
void protocol_parallel_S2M_get_sys_int_params(void *ptr);
void protocol_parallel_S2M_get_time(void *ptr);
void protocol_app_S2M_get_sm_code(void *ptr);


void protocol_parallel_M2S_set_slave_paras(void *ptr, uint8_t addr, uint8_t cmd_type, uint16_t value);
void protocol_parallel_M2S_get_sys_int_params(void *ptr,uint8_t addr);
void protocol_parallel_M2S_get_time(void *ptr,uint8_t addr);
void protocol_parallel_M2S_get_warm_cnt(void *ptr,uint8_t addr);
void protocol_app_S2M_get_sm_code_(void *ptr,uint8_t addr);


#endif //COM_PROTOCAL_1363_COM_PROTOCAL1363_APP_H__
