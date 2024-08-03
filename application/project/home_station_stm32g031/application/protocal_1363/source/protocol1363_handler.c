/*
 * Copyright (c) Daren Hi-Tech Electronics Co., Ltd. Development Team 2020-2020. All rights reserved.
 * File name:   protocol1363_handler.c
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

#include "protocol1363_handler.h"

#include <stdint.h>

#include "common_utils.h"
#include "paras.h"
#include "protocol1363_data.h"
#include "protocol_1363_get_para.h"

#ifdef USE_PROTOCOL1363_SNMP
bms_protocol1363_snmp_typedef bms_protocol1363_snmp;
#endif

/**
 * @Description:空函数 无意义
 * @param  - .
 * @return
 */
void protocol1363_Null(void *ptr){};

/**
 * @Description: protocol1363 处理主任务函数，周期性调用
 * @param  - . com_struct *com
 * @return
 */
void protocol1363_task_handler(comm_struct *com) {
    uint8_t i;
    COM_RTN_ENUM_TYPEDEF com_err = com_cid2_err;
    uint8_t addr;

    protocol_1363_init_paras();

    uint8_t if_get_from_PC = (com->rx_add == protocol1363_init_paras.machine_addr) ? (1) : (0);

    uint8_t if_get_from_master = ((protocol1363_init_paras.is_master == 0) && (com->cid2 == 0x84)) ? (1) : (0);

    uint8_t if_get_from_slave = ((protocol1363_init_paras.is_master == 1) && (com->cid2 == 0x85)) ? (1) : (0);

    uint8_t if_get_from_LCD = ((com->rx_add == PROTOCOL1363_LCD_ADDR)&&((com->cid2 == 0x42)||(com->cid2 == 0x83)))?(1):(0);

    ///SLAVER return protocol to MASTER
    if ((com->uart_data_ready == 1) && (if_get_from_PC || if_get_from_master || if_get_from_slave ||if_get_from_LCD)) {
        switch (com->com_no) {
            case 1: { //串口1
                //数据处理
                for (i = 0; (protocol_group_info[i].id != 0); i++) {
                    if (protocol_group_info[i].id == com->cid2) {
                        com_err = com_ok;

                        if (protocol_group_info[i].app_func_ptr != &protocol1363_Null) {
                            (*protocol_group_info[i].app_func_ptr)(com);
                            return;
                        }
                    }
                }

                if ((com_err != com_ok)) {
                    com->com_return_code = com_err;
                    protocol1363_send_frame_begin(com);
                    protocol1363_send_frame_end(com);
                }
                com->uart_data_ready = 0;
            } break;

            case 2: { //串口2
                //数据处理
                for (i = 0; (protocol_group_info[i].id != 0); i++) {
                    if (protocol_group_info[i].id == com->cid2) {
                        com_err = com_ok;

                        if (protocol_group_info[i].app_func_ptr != &protocol1363_Null) {
                            (*protocol_group_info[i].app_func_ptr)(com);
                            return;
                        }
                    }
                }

                if ((com_err != com_ok)) {
                    com->com_return_code = com_err;
                    protocol1363_send_frame_begin(com);
                    protocol1363_send_frame_end(com);
                }
                com->uart_data_ready = 0;

            } break;

            case 3: { //串口3
                //数据处理
                //数据处理
                for (i = 0; (protocol_group_info[i].id != 0); i++) {
                    if (protocol_group_info[i].id == com->cid2) {
                        com_err = com_ok;

                        if (protocol_group_info[i].app_func_ptr != &protocol1363_Null) {
                            (*protocol_group_info[i].app_func_ptr)(com);
                            return;
                        }
                    }
                }

                if ((com_err != com_ok)) {
                    com->com_return_code = com_err;
                    protocol1363_send_frame_begin(com);
                    protocol1363_send_frame_end(com);
                }
                com->uart_data_ready = 0;
            } break;

            default: break;
        }
    }
#if PROTOCOL_1363_RTN_ENABLE>0
    ///MASTER get protocol from SLAVER
    if ((com->uart_data_ready == 1) && (com->cid2 == 0x00) && (protocol1363_init_paras.is_master) )
    {
        ///进入接收函数
        switch (com->com_no) {
            case 1: { //串口1
                //数据处理
                for (i = 0; (protocol_rtn_group_info[i].id != 0); i++) {
                    if (protocol_rtn_group_info[i].id == com->last_cid2) {
                        com_err = com_ok;

                        if (protocol_rtn_group_info[i].app_func_ptr != &protocol1363_Null) {
                            (*protocol_rtn_group_info[i].app_func_ptr)(com);
                            return;
                        }
                    }
                }

                com->uart_data_ready = 0;
            } break;
            default:break;
        }
    }
#endif

    // if (com->com_return_code != com_ok) {
    //     protocol1363_return_frame(com);
    // }
}

/**
 * @Description: 一个字符一个字符接收字符，并且将其保存到缓冲区
 * @param  - .com_struct *com,uint8_t rcvd_char
 * @return  uint8_t
 */
uint8_t protocol1363_get_char(comm_struct *com, uint8_t rcvd_char) {
    uint8_t s_res; //, comPort;
    static uint8_t Start1363Dat = 0;
    s_res = rcvd_char;

    //    comPort = com->comNo - 1;
    //    if (comPort > 6)
    //        comPort = 0;

    com->time_modbus_cnt = 0;

    if(Start1363Dat == 1)
    {
        if (com->rx_buff_pos < COMM_RX_MAXLEN - 1) //&& prepareToSleep==0)
            com->rx_buff[(com->rx_buff_pos)++] = s_res;
    }
#if USEMODBUSTIMER == 0
    if (s_res == PROTOCOL_1363_SOI) {
        com->rx_buff[0] = PROTOCOL_1363_SOI;
        com->rx_buff_pos = 1;
        Start1363Dat = 1;
    } else if (s_res == PROTOCOL_1363_EOI) {
        if (protocol1363_get_frame_end(com) == com_ok) {
            com->uart_data_ready = 1;
            Start1363Dat = 0;
            return com_ok;
        }
    }
#endif
    return com_err;
}
/**
 * @Description:
 * @param  - .
 * @return
 */
uint8_t protocol1363_set_handler_cid2_new(comm_struct *com, protocol_var_typedef *protocol_var_ptr, uint8_t start_pos) {
    uint8_t i = 0;
    uint8_t j = 0;

    if (start_pos <= 13) { start_pos = 13; }

    com->rx_buff_cnt = start_pos;

    for (i = 0; /*(i < 5) &*/ (protocol_var_ptr[i].format != enum_data_var_end); i++) {
        if (protocol_var_ptr[i].array_lenth != 0) {
            for (j = 0; j < protocol_var_ptr[i].array_lenth; j++) {
                switch (protocol_var_ptr[i].format) {
                    case enum_data_var_s8:
                    case enum_data_var_u8:
                        *((uint8_t *) (protocol_var_ptr[i].var_ptr) + 0 + (j)) = two_ascii_to_u8_recount(com);
                        break;
                    case enum_data_var_s16:
                    case enum_data_var_u16:
                        *((uint8_t *) (protocol_var_ptr[i].var_ptr) + 1 + (j << 1)) = two_ascii_to_u8_recount(com);
                        *((uint8_t *) (protocol_var_ptr[i].var_ptr) + 0 + (j << 1)) = two_ascii_to_u8_recount(com);
                        break;
                    case enum_data_var_s32:
                    case enum_data_var_u32:
                        *((uint8_t *) (protocol_var_ptr[i].var_ptr) + 3 + (j << 2)) = two_ascii_to_u8_recount(com);
                        *((uint8_t *) (protocol_var_ptr[i].var_ptr) + 2 + (j << 2)) = two_ascii_to_u8_recount(com);
                        *((uint8_t *) (protocol_var_ptr[i].var_ptr) + 1 + (j << 2)) = two_ascii_to_u8_recount(com);
                        *((uint8_t *) (protocol_var_ptr[i].var_ptr) + 0 + (j << 2)) = two_ascii_to_u8_recount(com);
                        break;
                    case enum_data_var_float:
                        *((uint8_t *) (protocol_var_ptr[i].var_ptr) + 3 + (j << 2)) = two_ascii_to_u8_recount(com);
                        *((uint8_t *) (protocol_var_ptr[i].var_ptr) + 2 + (j << 2)) = two_ascii_to_u8_recount(com);
                        *((uint8_t *) (protocol_var_ptr[i].var_ptr) + 1 + (j << 2)) = two_ascii_to_u8_recount(com);
                        *((uint8_t *) (protocol_var_ptr[i].var_ptr) + 0 + (j << 2)) = two_ascii_to_u8_recount(com);
                        break;
                    default: break;
                }
            }
        } else {
            switch (protocol_var_ptr[i].format) {
                case enum_data_var_s8:
                case enum_data_var_u8:
                    *((uint8_t *) (protocol_var_ptr[i].var_ptr) + 0) = two_ascii_to_u8_recount(com);
                    break;
                case enum_data_var_s16:
                case enum_data_var_u16:
                    *((uint8_t *) (protocol_var_ptr[i].var_ptr) + 1) = two_ascii_to_u8_recount(com);
                    *((uint8_t *) (protocol_var_ptr[i].var_ptr) + 0) = two_ascii_to_u8_recount(com);
                    break;
                case enum_data_var_s32:
                case enum_data_var_u32:
                    *((uint8_t *) (protocol_var_ptr[i].var_ptr) + 3) = two_ascii_to_u8_recount(com);
                    *((uint8_t *) (protocol_var_ptr[i].var_ptr) + 2) = two_ascii_to_u8_recount(com);
                    *((uint8_t *) (protocol_var_ptr[i].var_ptr) + 1) = two_ascii_to_u8_recount(com);
                    *((uint8_t *) (protocol_var_ptr[i].var_ptr) + 0) = two_ascii_to_u8_recount(com);
                    break;
                case enum_data_var_float:
                    *((uint8_t *) (protocol_var_ptr[i].var_ptr) + 3) = two_ascii_to_u8_recount(com);
                    *((uint8_t *) (protocol_var_ptr[i].var_ptr) + 2) = two_ascii_to_u8_recount(com);
                    *((uint8_t *) (protocol_var_ptr[i].var_ptr) + 1) = two_ascii_to_u8_recount(com);
                    *((uint8_t *) (protocol_var_ptr[i].var_ptr) + 0) = two_ascii_to_u8_recount(com);
                    break;
                default: break;
            }
        }
    }
}


#if PROTOCOL_1363_RTN_ENABLE>0
/**
 * @Description:
 * @param  - .
 * @return
 */
uint8_t protocol_parallel_para_var_set(comm_struct *com, const PROTOCOL_S2M_VAR_TYPEDEF *protocol_var_ptr, uint8_t start_pos) {
    uint8_t i = 0;
    uint8_t j = 0;

    if (start_pos <= 13) { start_pos = 13; }

    com->rx_buff_cnt = start_pos;
    for (i = 0; /*(i < 5) &*/ (protocol_var_ptr[i].format != enum_data_var_end); i++) {
        if (protocol_var_ptr[i].array_lenth != 0) {
            for (j = 0; j < *(protocol_var_ptr[i].array_lenth); j++) {
                switch (protocol_var_ptr[i].format) {
                    case enum_data_var_s8:
                    case enum_data_var_u8:
                        *((uint8_t *) (protocol_var_ptr[i].var_ptr) + 0 + (j)) = two_ascii_to_u8_recount(com);
                        break;
                    case enum_data_var_s16:
                    case enum_data_var_u16:
                        *((uint8_t *) (protocol_var_ptr[i].var_ptr) + 1 + (j << 1)) = two_ascii_to_u8_recount(com);
                        *((uint8_t *) (protocol_var_ptr[i].var_ptr) + 0 + (j << 1)) = two_ascii_to_u8_recount(com);
                        break;
                    case enum_data_var_s32:
                    case enum_data_var_u32:
                        *((uint8_t *) (protocol_var_ptr[i].var_ptr) + 3 + (j << 2)) = two_ascii_to_u8_recount(com);
                        *((uint8_t *) (protocol_var_ptr[i].var_ptr) + 2 + (j << 2)) = two_ascii_to_u8_recount(com);
                        *((uint8_t *) (protocol_var_ptr[i].var_ptr) + 1 + (j << 2)) = two_ascii_to_u8_recount(com);
                        *((uint8_t *) (protocol_var_ptr[i].var_ptr) + 0 + (j << 2)) = two_ascii_to_u8_recount(com);
                        break;
                    case enum_data_var_float:
                        *((uint8_t *) (protocol_var_ptr[i].var_ptr) + 3 + (j << 2)) = two_ascii_to_u8_recount(com);
                        *((uint8_t *) (protocol_var_ptr[i].var_ptr) + 2 + (j << 2)) = two_ascii_to_u8_recount(com);
                        *((uint8_t *) (protocol_var_ptr[i].var_ptr) + 1 + (j << 2)) = two_ascii_to_u8_recount(com);
                        *((uint8_t *) (protocol_var_ptr[i].var_ptr) + 0 + (j << 2)) = two_ascii_to_u8_recount(com);
                        break;
                    default: break;
                }
            }
        } else {
            switch (protocol_var_ptr[i].format) {
                case enum_data_var_s8:
                case enum_data_var_u8:
                    *((uint8_t *) (protocol_var_ptr[i].var_ptr) + 0) = two_ascii_to_u8_recount(com);
                    break;
                case enum_data_var_s16:
                case enum_data_var_u16:
                    *((uint8_t *) (protocol_var_ptr[i].var_ptr) + 1) = two_ascii_to_u8_recount(com);
                    *((uint8_t *) (protocol_var_ptr[i].var_ptr) + 0) = two_ascii_to_u8_recount(com);
                    break;
                case enum_data_var_s32:
                case enum_data_var_u32:
                    *((uint8_t *) (protocol_var_ptr[i].var_ptr) + 3) = two_ascii_to_u8_recount(com);
                    *((uint8_t *) (protocol_var_ptr[i].var_ptr) + 2) = two_ascii_to_u8_recount(com);
                    *((uint8_t *) (protocol_var_ptr[i].var_ptr) + 1) = two_ascii_to_u8_recount(com);
                    *((uint8_t *) (protocol_var_ptr[i].var_ptr) + 0) = two_ascii_to_u8_recount(com);
                    break;
                case enum_data_var_float:
                    *((uint8_t *) (protocol_var_ptr[i].var_ptr) + 3) = two_ascii_to_u8_recount(com);
                    *((uint8_t *) (protocol_var_ptr[i].var_ptr) + 2) = two_ascii_to_u8_recount(com);
                    *((uint8_t *) (protocol_var_ptr[i].var_ptr) + 1) = two_ascii_to_u8_recount(com);
                    *((uint8_t *) (protocol_var_ptr[i].var_ptr) + 0) = two_ascii_to_u8_recount(com);
                    break;
                default: break;
            }
        }
    }
}



#endif