/*
 * Copyright (c) Daren Hi-Tech Electronics Co., Ltd. Development Team 2020-2020. All rights reserved.
 * File name:   protocol_1363_get_para.h
 * Author: cq   ID:     Version:V1.0    Date: 2021/3/22
 * Description: // 用于详细说明此程序文件完成的主要功能，与其他模块
 *              // 或函数的接口，输出值、取值范围、含义及参数间的控
 *              // 制、顺序、独立或依赖等关系
 * Others:      // 其它内容的说明
 * History:     // 修改历史记录列表，每条修改记录应包括修改日期、修改
 *              // 者及修改内容简述
 *              Date           Author       Notes
 */
#ifndef F103VC_MODULE_MODULE_PROTOCOL_1363_GET_PARA_PROTOCOL_1363_GET_PARA_H_
#define F103VC_MODULE_MODULE_PROTOCOL_1363_GET_PARA_PROTOCOL_1363_GET_PARA_H_

#include "paras.h"
#include "stdint.h"


typedef struct{
    uint8_t machine_addr;
    uint8_t num_of_cell;
    uint8_t num_of_temp;
    uint8_t is_master;
}protocol1363_init_paras_struct;



extern protocol1363_init_paras_struct protocol1363_init_paras;
void protocol_1363_init_paras(void);
void protocol_1363_get_hw_addr(uint8_t *addr );
#if PROTOCOL_1363_RTN_ENABLE
uint32_t protocol_1363_get_sys_tick_ms(void);
#endif
void protocol_1363_set_hw_paras(protocol1363_init_paras_struct *protocol1363_init_paras);

uint8_t  protocol_app_get_uart_lose(void *ptr,uint32_t time_out_ms);
uint8_t protocol_1363_S2M_send_data_time_4E(data_time_typedef *data_time_ptr);
uint8_t protocol_1363_S2M_send_warm_cnt_4d(warm_cnt_typedef *s_warm_ptr);
uint8_t protocol_1363_S2M_get_sm_code_51(sm_info_typedef *sm_info_ptr);
void protocol_parallel_S2M_get_sys_basic_params_80(basic_params_new_typedef *basic_params_ptr);
COM_STATE_ENUM_TYPEDEF protocol_parallel_app_get_return_infomation(void *ptr);
void protocol_1363_get_slave_para_42(sys_status_typedef *sys_status_ptr);

#endif //
