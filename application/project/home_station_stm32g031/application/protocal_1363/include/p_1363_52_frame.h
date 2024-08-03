/*
 * Copyright (c) Union Co., Ltd. Development Team 2022-2023. All rights reserved.
 * @FilePath     : /esp32_c3_eth/main/flume/protocol/service/p_1363_52_frame.h
 * @Author       : your name
 * @Date         : 2023-11-06 19:31:56
 * @Description  : 用于详细说明此程序文件完成的主要功能，与其他模块或函数的接口，输出值、取值范围、含义及参数间的控制、顺序、独立或依赖等关系
 * @Others       : 其它内容的说明
 * @History      : 修改历史记录列表，每条修改记录应包括修改日期、修改者及修改内容简述
 */
#ifndef P_1363_52_FRAME_H__
#define P_1363_52_FRAME_H__
#pragma once

#include <stdint.h>

#include "bms_common.h"

typedef struct {
    uint8_t is_finished;
} D_SM_ADDR_T;
D_SM_ADDR_T *bms_get_sm_addr(void);

BMS_RTN_ID sm_addr_net_init();
/**
 * @brief 描述: 接口BMS返回参数入口
 * @param {void} *ptr
 * @return {*}
 */
void protocol_52_respond(void *ptr);
uint8_t protocol_52_require(void *ptr);

#endif