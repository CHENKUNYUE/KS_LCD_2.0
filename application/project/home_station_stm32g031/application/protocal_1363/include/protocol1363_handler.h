/*
 * Copyright (c) Daren Hi-Tech Electronics Co., Ltd. Development Team 2020-2020. All rights reserved.
 * File name:   protocol1363_handler.h
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
#ifndef COM_PROTOCAL_1363_PROTOCOL1363_HANDLER_H__
#define     COM_PROTOCAL_1363_PROTOCOL1363_HANDLER_H__        
#include <stdint.h>
#include "protocol1363_transfer.h"


extern protocol_id_params const protocol_group_info[];

#if PROTOCOL_1363_RTN_ENABLE>0
extern protocol_id_params const protocol_rtn_group_info[];
#endif

    //中断函数中调用即可
uint8_t protocol1363_get_char(comm_struct *com, uint8_t rcvd_char);
//放在任务中调用
void protocol1363_task_handler(comm_struct *com);


uint8_t protocol1363_send_handler_cid2_new(comm_struct *com, protocol_var_typedef *protocol_var_ptr);

uint8_t protocol1363_set_handler_cid2_new(comm_struct *com, protocol_var_typedef *protocol_var_ptr, uint8_t start_pos);

uint8_t protocol_var_set(comm_struct *com, const protocol_var_typedef *protocol_var, uint8_t start_pos);
uint8_t protocol_var_send(comm_struct *com, const protocol_var_typedef *protocol_var_ptr, uint8_t start_pos);

#if PROTOCOL_1363_RTN_ENABLE>0
uint8_t protocol_parallel_para_var_set(comm_struct *com, const PROTOCOL_S2M_VAR_TYPEDEF *protocol_var_ptr, uint8_t start_pos);
#endif
extern comm_struct protocol_com;


#endif //COM_PROTOCAL_1363_PROTOCOL1363_HANDLER_H__
