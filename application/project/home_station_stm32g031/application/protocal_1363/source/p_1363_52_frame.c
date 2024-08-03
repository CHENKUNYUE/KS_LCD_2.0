/*
 * Copyright (c) Union Co., Ltd. Development Team 2022-2023. All rights reserved.
 * @FilePath     : /esp32_c3_eth/main/flume/protocol/service/p_1363_52_frame.c
 * @Author       : your name
 * @Date         : 2023-11-06 19:31:36
 * @Description  : 用于详细说明此程序文件完成的主要功能，与其他模块或函数的接口，输出值、取值范围、含义及参数间的控制、顺序、独立或依赖等关系
 * @Others       : 其它内容的说明
 * @History      : 修改历史记录列表，每条修改记录应包括修改日期、修改者及修改内容简述
 */
#include "p_1363_52_frame.h"

#include <stdbool.h>

#include "bms_common.h"
#include "bms_rt_info.h"
#include "protocol1363_transfer.h"

static D_SM_ADDR_T sm_addr_t = {0};
/**
 * @brief 51H ask
 * @param  - .
 * @return
 */
uint8_t protocol_52_require(void *ptr) {
    comm_struct *s_comm_struct;
    s_comm_struct         = ptr;
    s_comm_struct->cid2   = 0x52; //获取厂商信息
    s_comm_struct->rx_add = 0xFF;

    s_comm_struct->tx_buff[0]  = PROTOCOL_1363_SOI;
    s_comm_struct->tx_buff_cnt = 1;
    //p_1363_frame_head(s_comm_struct);
    protocol1363_send_byte(s_comm_struct, PROTOCOL_1363_VER);
    protocol1363_send_byte(s_comm_struct, s_comm_struct->rx_add);
    protocol1363_send_byte(s_comm_struct, PROTOCOL_1363_CID1);
    protocol1363_send_byte(s_comm_struct, s_comm_struct->cid2);
    // p_1363_send_u8(s_comm_struct, s_comm_struct->com_return_code);//RTN
    protocol1363_send_byte(s_comm_struct, 0);
    protocol1363_send_byte(s_comm_struct, 0);
    protocol1363_send_byte(s_comm_struct, s_comm_struct->rx_add); //s_command_group

    protocol1363_send_frame_end(s_comm_struct);
    return 0;
}

/**
 * @brief 获取设备 (SM) 厂家信息
 * @param  - .
 * @return
 */
void protocol_52_respond(void *ptr) {
    sm_info_typedef sm_params = {0};
    //系统变量指针，变量指针列表，数组长度 51H
    protocol_var_typedef s_protocol1363_sm_addr[2] = {
        {0, enum_data_var_u8, 0},
        {0, enum_data_var_end, 0}, //2
    };
    comm_struct *s_comm_struct = ptr;
    uint8_t s_command_group    = utils_t.two_ascii_2_u8((s_comm_struct->rx_buff + P_1363_INFO_S));

    BMS_RT_INFO_T *p_bms_t = bms_get_rt_info();
    p_bms_t->bms_addr      = s_comm_struct->rx_add;
    sm_addr_t.is_finished  = true;
}

/**
 * @brief 
 * @return
 */
BMS_RTN_ID sm_addr_net_init() {
    sm_addr_t.is_finished = false; // 数据是否已收集完毕
    return 0;
}