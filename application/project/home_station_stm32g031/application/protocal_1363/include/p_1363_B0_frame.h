/*
 * Copyright (c) Union Co., Ltd. Development Team 2022-2023. All rights reserved.
 * @FilePath     : /ESP32-S3-LCD/main/flume/protocol/service/p_1363_B0_frame.h
 * @Author       : your name
 * @Date         : 2023-11-07 16:36:51
 * @Description  : 用于详细说明此程序文件完成的主要功能，与其他模块或函数的接口，输出值、取值范围、含义及参数间的控制、顺序、独立或依赖等关系
 * @Others       : 其它内容的说明
 * @History      : 修改历史记录列表，每条修改记录应包括修改日期、修改者及修改内容简述
 */
#ifndef P_1363_B0_FRAME_H__
#define P_1363_B0_FRAME_H__
#pragma once

#include "paras.h"
#include "bms_common.h"
#include "protocol_common.h"

typedef struct {
    uint8_t is_finished;
    PARAM_OPT_OCV_T *opt_ocv_t;
    PARAM_OPT_HW_PROT_T *opt_hw_prot_t;
    PARAM_OPT_MFG_T *opt_pro_t;
    PARAM_OPT_CAP_T *opt_cap_t;
    PARAM_OPT_CAN_T *opt_can_t;
    PARAM_OPT_PROTOCOL_T *opt_port_485A_t;
    PARAM_OPT_PROTOCOL_T *opt_port_CAN1_t;
    PARAM_OPT_FUNC_EN_T *opt_func_en_t;
} D_BMS_OPTION_PARAM_T;
D_BMS_OPTION_PARAM_T *bms_get_opt_info(void);

typedef struct {
    uint8_t page;
    uint8_t is_finished;
    uint8_t is_option;
} D_BMS_SET_OPTION_PARAM_T;
D_BMS_SET_OPTION_PARAM_T *bms_get_set_opt_info(void);

BMS_RTN_ID bms_param_option_init();
BMS_RTN_ID request_option_port_cfg_by_addr(void *ptr, uint8_t addr, COM_PORT_T port_type);
BMS_RTN_ID setting_option_port_cfg_by_addr(void *ptr, uint8_t addr, COM_PORT_T port_type, uint16_t protocol_code);
void protocol_B0_respond(void *ptr);

#endif