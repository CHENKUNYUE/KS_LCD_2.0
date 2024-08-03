/*
 * Copyright (c) Union Co., Ltd. Development Team 2022-2023. All rights reserved.
 * @FilePath     : /ESP32-S3-LCD/main/flume/protocol/service/p_1363_B0_frame.c
 * @Author       : your name
 * @Date         : 2023-11-07 16:36:29
 * @Description  : 用于详细说明此程序文件完成的主要功能，与其他模块或函数的接口，输出值、取值范围、含义及参数间的控制、顺序、独立或依赖等关系
 * @Others       : 其它内容的说明
 * @History      : 修改历史记录列表，每条修改记录应包括修改日期、修改者及修改内容简述
 */
#include "p_1363_B0_frame.h"

#include <stdbool.h>
#include <string.h>

#include "Show_Ctrl.h"
#include "bms_rt_info.h"
#include "common_utils.h"
#include "protocol1363_handler.h"
#include "protocol1363_transfer.h"

static D_BMS_OPTION_PARAM_T s_bms_opt_t            = {0}; // 参数操作结构体指针
static D_BMS_SET_OPTION_PARAM_T s_bms_set_opt_t    = {0}; // 参数操作结构体指针
static PARAM_OPT_OCV_T _para_opt_ocv[2]            = {0};
static PARAM_OPT_HW_PROT_T _para_opt_HW_prot[2]    = {0};
static PARAM_OPT_MFG_T _para_opt_product[2]        = {0};
static PARAM_OPT_CAP_T _para_opt_cap[2]            = {0};
static PARAM_OPT_CAN_T _para_opt_can[2]            = {0};
static PARAM_OPT_PROTOCOL_T _para_opt_port_485A[2] = {0};
static PARAM_OPT_PROTOCOL_T _para_opt_port_CAN1[2] = {0};
static PARAM_OPT_FUNC_EN_T _para_opt_func_en[2]    = {0};

/**
 * @brief B0H ask
 * @param  - .
 * @return
 */
uint8_t protocol_B0_require(void *ptr, uint8_t operation, uint8_t module, uint8_t function, uint8_t length, uint8_t add) {
    comm_struct *s_comm_struct;
    s_comm_struct         = ptr;
    s_comm_struct->cid2   = 0xB0; //获取基本参数
    s_comm_struct->rx_add = add;

    s_comm_struct->tx_buff[0]  = PROTOCOL_1363_SOI;
    s_comm_struct->tx_buff_cnt = 1;
    //p_1363_frame_head(s_comm_struct);
    protocol1363_send_byte(s_comm_struct, PROTOCOL_1363_VER);
    protocol1363_send_byte(s_comm_struct, s_comm_struct->rx_add);
    protocol1363_send_byte(s_comm_struct, PROTOCOL_1363_CID1);
    protocol1363_send_byte(s_comm_struct, s_comm_struct->cid2);
    // protocol1363_send_byte(s_comm_struct, s_comm_struct->com_return_code);//RTN
    protocol1363_send_byte(s_comm_struct, 0);
    protocol1363_send_byte(s_comm_struct, 0);

    protocol1363_send_byte(s_comm_struct, add);       // commandGroup
    protocol1363_send_byte(s_comm_struct, operation); // operation 1
    protocol1363_send_byte(s_comm_struct, module);    // module 4
    protocol1363_send_byte(s_comm_struct, function);  // function 255
    protocol1363_send_byte(s_comm_struct, length);    // length 0

    protocol1363_send_frame_end(s_comm_struct);
    return 0;
}

/**
 * @brief B0H ask
 * @param  - .
 * @return
 */
uint8_t protocol_B0_setting(void *ptr, uint8_t operation, uint8_t module, uint8_t function, uint8_t length, uint8_t add,
                            uint16_t protocol_code) {
    comm_struct *s_comm_struct;
    s_comm_struct         = ptr;
    s_comm_struct->cid2   = 0xB0; //获取基本参数
    s_comm_struct->rx_add = add;

    s_comm_struct->tx_buff[0]  = PROTOCOL_1363_SOI;
    s_comm_struct->tx_buff_cnt = 1;
    //p_1363_frame_head(s_comm_struct);
    protocol1363_send_byte(s_comm_struct, PROTOCOL_1363_VER);
    protocol1363_send_byte(s_comm_struct, s_comm_struct->rx_add);
    protocol1363_send_byte(s_comm_struct, PROTOCOL_1363_CID1);
    protocol1363_send_byte(s_comm_struct, s_comm_struct->cid2);
    // protocol1363_send_byte(s_comm_struct, s_comm_struct->com_return_code);//RTN
    protocol1363_send_byte(s_comm_struct, 0);
    protocol1363_send_byte(s_comm_struct, 0);

    protocol1363_send_byte(s_comm_struct, add);       // commandGroup
    protocol1363_send_byte(s_comm_struct, operation); // operation 1
    protocol1363_send_byte(s_comm_struct, module);    // module 4
    protocol1363_send_byte(s_comm_struct, function);  // function 255
    protocol1363_send_byte(s_comm_struct, length);    // length 04

    protocol1363_send_byte(s_comm_struct, 0x0E); // 115200
    protocol1363_send_half_word(s_comm_struct, protocol_code);
    protocol1363_send_byte(s_comm_struct, 0x00);

    protocol1363_send_frame_end(s_comm_struct);
    return 0;
}

/**
 * @brief 描述:
 * @param {uint8_t} addr
 * @param {PARAM_OPT_OCV_T} *para_opt_ocv
 * @param {uint16_t} size
 * @return {*}
 */
static BMS_RTN_ID response_option_ocv_param(uint8_t addr, PARAM_OPT_OCV_T *para_opt_ocv, uint16_t size) {
    // TODO
    memcpy(&s_bms_opt_t.opt_ocv_t[addr], para_opt_ocv, sizeof(*para_opt_ocv));
    return 0;
}

/**
 * @brief 描述:
 * @param {uint8_t} addr
 * @param {PARAM_OPT_HW_PROT_T} *para_opt_hw_prot
 * @param {uint16_t} size
 * @return {*}
 */
static BMS_RTN_ID response_option_hw_param(uint8_t addr, PARAM_OPT_HW_PROT_T *para_opt_hw_prot, uint16_t size) {
    // TODO
    memmove(&s_bms_opt_t.opt_hw_prot_t[addr], para_opt_hw_prot, sizeof(*para_opt_hw_prot));
    return 0;
}

/**
 * @brief 描述:
 * @param {uint8_t} addr
 * @param {PARAM_OPT_MFG_T} *para_opt_product
 * @param {uint16_t} size
 * @return {*}
 */
static BMS_RTN_ID response_option_product_param(uint8_t addr, PARAM_OPT_MFG_T *para_opt_product, uint16_t size) {
#if DEBUG
    printf("【set】bms_sn:");
    for (uint8_t i = 0; i < SIZE_OF(para_opt_product->bms_sn); i++) {
        /* code */
        printf("%02X ", para_opt_product->bms_sn[i]);
    }
    printf("\n");
    int size01 = SIZE_OF(para_opt_product->bms_sn); // 计算数组长度
    printf("para_opt_product.bms_sn.size:%d\n", size01);

    printf("【set】pack_sn:");
    for (uint8_t i = 0; i < SIZE_OF(para_opt_product->pack_sn); i++) {
        /* code */
        printf("%02X ", para_opt_product->pack_sn[i]);
    }
    printf("\n");
    size01 = SIZE_OF(para_opt_product->pack_sn); // 计算数组长度
    printf("para_opt_product.pack_sn->size:%d\n", size01);
#endif

    memmove(s_bms_opt_t.opt_pro_t[addr].bms_sn, para_opt_product->bms_sn, sizeof(para_opt_product->bms_sn));
    memmove(s_bms_opt_t.opt_pro_t[addr].manufacturer, para_opt_product->manufacturer, sizeof(para_opt_product->manufacturer));
    memmove(s_bms_opt_t.opt_pro_t[addr].pack_sn, para_opt_product->pack_sn, sizeof(para_opt_product->pack_sn));
    memmove(s_bms_opt_t.opt_pro_t[addr].produce_data, para_opt_product->produce_data, sizeof(para_opt_product->produce_data));
    memmove(s_bms_opt_t.opt_pro_t[addr].product_ID, para_opt_product->product_ID, sizeof(para_opt_product->product_ID));
#if DEBUG
    printf("【finish】bms_sn:");
    for (uint8_t i = 0; i < SIZE_OF(s_bms_opt_t.opt_pro_t[addr].bms_sn); i++) {
        /* code */
        printf("%02X ", s_bms_opt_t.opt_pro_t[addr].bms_sn[i]);
    }
    printf("\n");
    int size02 = SIZE_OF(s_bms_opt_t.opt_pro_t[addr].bms_sn); // 计算数组长度
    printf("para_opt_product.bms_sn.size:%d\n", size02);
#endif
    return 0;
}

/**
 * @brief 描述:
 * @param {uint8_t} addr
 * @param {PARAM_OPT_CAP_T} *opt_cap_param
 * @param {uint16_t} size
 * @return {*}
 */
static BMS_RTN_ID response_option_cap_param(uint8_t addr, PARAM_OPT_CAP_T *opt_cap_param, uint16_t size) {
    // memcpy(s_bms_opt_t.opt_cap_t[addr].cap_design, opt_cap_param->cap_design, sizeof(opt_cap_param->cap_design));
    // memcpy(s_bms_opt_t.opt_cap_t[addr].cap_full, opt_cap_param->cap_full, sizeof(opt_cap_param->cap_full));
    // memcpy(s_bms_opt_t.opt_cap_t[addr].cap_remaining, opt_cap_param->cap_remaining, sizeof(opt_cap_param->cap_remaining));
    // memcpy(s_bms_opt_t.opt_cap_t[addr].chg_cap_kwh, opt_cap_param->chg_cap_kwh, sizeof(opt_cap_param->chg_cap_kwh));
    // memcpy(s_bms_opt_t.opt_cap_t[addr].chg_cap_total, opt_cap_param->chg_cap_total, sizeof(opt_cap_param->chg_cap_total));
    // memcpy(s_bms_opt_t.opt_cap_t[addr].disch_cap_kwh, opt_cap_param->disch_cap_kwh, sizeof(opt_cap_param->disch_cap_kwh));
    // memcpy(s_bms_opt_t.opt_cap_t[addr].disch_cap_total, opt_cap_param->disch_cap_total, sizeof(opt_cap_param->disch_cap_total));
#if DEBUG
    printf("【set】disch_cap_total:%d\n", opt_cap_param->disch_cap_total);
#endif
    memmove(&s_bms_opt_t.opt_cap_t[addr], opt_cap_param, sizeof(*opt_cap_param));
#if DEBUG
    printf("【finish】disch_cap_total:%d\n", s_bms_opt_t.opt_cap_t[addr].disch_cap_total);
#endif
    return 0;
}
/**
 * @brief 描述:
 * @param {uint8_t} addr
 * @param {PARAM_OPT_PROTOCOL_T} *opt_cap_param
 * @param {uint16_t} size
 * @return {*}
 */
static BMS_RTN_ID response_option_port_cfg(uint8_t addr, PARAM_OPT_PROTOCOL_T *opt_port_cfg, uint16_t size, uint8_t function_type) {
    PAGE_PROTOCOL_T *protocol_t = get_page_protocol_t();
    switch (function_type) {
        case E_PORT_485A: protocol_t->rs485_protocol = opt_port_cfg->port_protocol; break;
        case E_PORT_CAN1: protocol_t->can_protocol = opt_port_cfg->port_protocol; break;
        default: break;
    }
    s_bms_opt_t.is_finished = true;
    return 0;
}

void setting_response_b0h_port_cfg(uint8_t addr, uint8_t function_type) {
    switch (function_type) {
        case E_PORT_485A: break;
        case E_PORT_CAN1: break;
        default: break;
    }
    s_bms_set_opt_t.is_finished = true;
    s_bms_set_opt_t.is_option   = false;
}
/**
 * @brief 参数操作 0xB0
 * @param  - .
 * @return
 */
void protocol_B0_respond(void *ptr) {
    PARAM_OPT_OCV_T para_opt_ocv         = {0};
    PARAM_OPT_HW_PROT_T para_opt_hw_prot = {0};
    PARAM_OPT_MFG_T para_opt_product     = {0};
    PARAM_OPT_CAP_T para_opt_cap         = {0};
    // PARAM_OPT_CAN_T param_opt_can         = {0};
    PARAM_OPT_PROTOCOL_T param_port_cfg = {0};
    // PARAM_OPT_FUNC_EN_T param_opt_func_en = {0};
    // uint8_t s_cid2                                 = 0;
    // uint8_t s_command_group                        = 0;
    uint8_t s_machine_add = 0;
    uint8_t s_operation   = 0;
    uint8_t s_module      = 0;
    uint8_t s_function    = 0;
    // uint8_t s_length                               = 0;
    // uint8_t s_return_length     = 0;
    // uint8_t s_gyro_ctr_function = 0;
    // uint8_t function_max_length = 0;
    uint8_t s_err = 0;
    comm_struct *s_comm_struct;
    s_comm_struct = ptr;

    protocol_var_typedef s_protocol1363_para_opt_ocv_para[] = {
        {&para_opt_ocv.OCV_config.value, enum_data_var_u16, 0},
        {&para_opt_ocv.offset_current, enum_data_var_u16, 0},
        {&para_opt_ocv.CHG_I_K_Value, enum_data_var_u16, 0},
        {&para_opt_ocv.DISCH_I_K_Value, enum_data_var_u16, 0},
        {&para_opt_ocv.Self_discharge_rate, enum_data_var_u16, 0},
        {&para_opt_ocv.volt_at_00_soc, enum_data_var_u16, 0},
        {&para_opt_ocv.volt_at_20_soc, enum_data_var_u16, 0},
        {&para_opt_ocv.volt_at_40_soc, enum_data_var_u16, 0},
        {&para_opt_ocv.volt_at_80_soc, enum_data_var_u16, 0},
        {&para_opt_ocv.volt_at_100_soc, enum_data_var_u16, 0},
        {&para_opt_ocv.proportion_of_cycles, enum_data_var_u16, 0},
        {&para_opt_ocv.cell_full_CHG_volt, enum_data_var_u16, 0},
        {&para_opt_ocv.cell_low_DISCH_volt, enum_data_var_u16, 0},
        {&para_opt_ocv.R_sensor_value, enum_data_var_u16, 0},
        {0, enum_data_var_end, 0},
    };

    protocol_var_typedef s_protocol1363_para_opt_HW_prot[] = {
        {&para_opt_hw_prot.OCP_value, enum_data_var_u16, 0},          {&para_opt_hw_prot.OCP_delay, enum_data_var_u16, 0},
        {&para_opt_hw_prot.SCP_value, enum_data_var_u16, 0},          {&para_opt_hw_prot.SCP_delay, enum_data_var_u16, 0},
        {&para_opt_hw_prot.SCP_recovery_delay, enum_data_var_u16, 0}, {&para_opt_hw_prot.CELL_OVP_value, enum_data_var_u16, 0},
        {&para_opt_hw_prot.CELL_OVP_delay, enum_data_var_u16, 0},     {&para_opt_hw_prot.CELL_UVP_value, enum_data_var_u16, 0},
        {&para_opt_hw_prot.CELL_UVP_delay, enum_data_var_u16, 0},     {0, enum_data_var_end, 0},
    };

    protocol_var_typedef s_protocol1363_para_opt_product_para[] = {
        {&para_opt_product.pack_sn[0], enum_data_var_u8, 30},      {&para_opt_product.product_ID[0], enum_data_var_u8, 30},
        {&para_opt_product.bms_sn[0], enum_data_var_u8, 30},       {&para_opt_product.produce_data[0], enum_data_var_u8, 3},
        {&para_opt_product.manufacturer[0], enum_data_var_u8, 20}, {0, enum_data_var_end, 0},
    };

    protocol_var_typedef s_protocol1363_para_opt_cap[] = {
        {&para_opt_cap.cap_remaining, enum_data_var_u16, 0},   {&para_opt_cap.cap_full, enum_data_var_u16, 0},
        {&para_opt_cap.cap_design, enum_data_var_u16, 0},      {&para_opt_cap.chg_cap_total, enum_data_var_u32, 0},
        {&para_opt_cap.disch_cap_total, enum_data_var_u32, 0}, {&para_opt_cap.chg_cap_kwh, enum_data_var_u16, 0},
        {&para_opt_cap.disch_cap_kwh, enum_data_var_u16, 0},   {0, enum_data_var_end, 0},
    };

    protocol_var_typedef s_p1363_param_opt_port_cfg[] = {
        {&param_port_cfg.port_bps, enum_data_var_u8, 0},
        {&param_port_cfg.port_protocol, enum_data_var_u16, 0},
        {&param_port_cfg.rvd, enum_data_var_u8, 0},
        {0, enum_data_var_end, 0},
    };

    // >2201B000D030B0010104FF00 4A38 4A38 4A38 0000 0000 0000 0001 0018 007B F3CB

    bms_get_addr(&s_machine_add);

    // s_cid2 = utils_t.two_ascii_2_u8((s_comm_struct->rx_buff + P_1363_INFO_S));

    // s_command_group = utils_t.two_ascii_2_u8((s_comm_struct->rx_buff + 15));

    s_operation = utils_t.two_ascii_2_u8((s_comm_struct->rx_buff + 17));

    s_module = utils_t.two_ascii_2_u8((s_comm_struct->rx_buff + 19));

    s_function = utils_t.two_ascii_2_u8((s_comm_struct->rx_buff + 21));

    // s_length = utils_t.two_ascii_2_u8((s_comm_struct->rx_buff + 23));

    // 参数设置
    if (s_operation == enum_module_set) {
        switch (s_module) {
            case E_OCV_PARAM: {
                break;
            }
            case E_HW_PROT_PARAM: {
                break;
            }
            case E_MFG_PARAM: {
                break;
            }
            case E_CAP_PARAM: {
                break;
            }
            case E_PORT_CFG_PARAM: { // get
                setting_response_b0h_port_cfg(s_comm_struct->rx_add, s_function);
                break;
            }
        }
    }

    // 参数查询
    if (s_operation == enum_module_get) {
        switch (s_module) {
            case E_OCV_PARAM: {
                // 将收到的数据解析成s_protocol1363_reals_paras结构体
                protocol1363_set_handler_cid2_new(s_comm_struct, s_protocol1363_para_opt_ocv_para, 25);
                response_option_ocv_param(s_comm_struct->rx_add, &para_opt_ocv, sizeof(para_opt_ocv));
                break;
            }
            case E_HW_PROT_PARAM: {
                // 将收到的数据解析成s_protocol1363_reals_paras结构体
                protocol1363_set_handler_cid2_new(s_comm_struct, s_protocol1363_para_opt_HW_prot, 25);
                response_option_hw_param(s_comm_struct->rx_add, &para_opt_hw_prot, sizeof(para_opt_hw_prot));
                break;
            }
            case E_MFG_PARAM: {
                // >2201B00040EEB0010103FF71313233343536373839000000000000000000000000000000000000000000
                // 313233343536373839000000000000000000000000000000000000000000
                // 313233343536373839000000000000000000000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFCBC4
                // 将收到的数据解析成s_protocol1363_reals_paras结构体

                protocol1363_set_handler_cid2_new(s_comm_struct, s_protocol1363_para_opt_product_para, 25);
#if DEBUG
                printf("【ready】bms_sn:");
                for (uint8_t i = 0; i < SIZE_OF(para_opt_product.bms_sn); i++) {
                    /* code */
                    printf("%02X ", para_opt_product.bms_sn[i]);
                }
                printf("\n");
                int size = SIZE_OF(para_opt_product.bms_sn); // 计算数组长度
                printf("para_opt_product.bms_sn.size:%d\n", size);

                printf("【ready】pack_sn:");
                for (uint8_t i = 0; i < SIZE_OF(para_opt_product.pack_sn); i++) {
                    /* code */
                    printf("%02X ", para_opt_product.pack_sn[i]);
                }
                printf("\n");
                size = SIZE_OF(para_opt_product.pack_sn); // 计算数组长度
                printf("para_opt_product.pack_sn.size:%d\n", size);
#endif
                response_option_product_param(s_comm_struct->rx_add, &para_opt_product, sizeof(para_opt_product));
                break;
            }
            case E_CAP_PARAM: {
                // >2201B000D030B0010104FF004A384A384A38000000000000000100180074F3D9
                // 将收到的数据解析成s_protocol1363_reals_paras结构体
                protocol1363_set_handler_cid2_new(s_comm_struct, s_protocol1363_para_opt_cap, 25);
#if DEBUG
                printf("【ready】disch_cap_total:%d\n", para_opt_cap.disch_cap_total);
#endif
                // 这里需要使用MQTT发送出去
                response_option_cap_param(s_comm_struct->rx_add, &para_opt_cap, sizeof(para_opt_cap));
                break;
            }
            case E_PORT_CFG_PARAM: { // get
                // >2201B000B014B0010107 0C04 06 0007 00 F993
                // >2201B000B014B0010107 0104 06 00CA 00 F988

                // APP_PRINT(">>>0x%02X,0x%02X,0x%02X,0x%02X\n", s_comm_struct->rx_buff[27], s_comm_struct->rx_buff[28], s_comm_struct->rx_buff[29], s_comm_struct->rx_buff[30])

                protocol1363_set_handler_cid2_new(s_comm_struct, s_p1363_param_opt_port_cfg, 25);
                response_option_port_cfg(s_comm_struct->rx_add, &param_port_cfg, sizeof(param_port_cfg), s_function);
                break;
            }
            default: break;
        }
        if (s_err != 0) {
            s_comm_struct->com_return_code = com_data_err;
        }
    }
}

/**
 * @brief 
 * @param ptr_option_sent - wifi require function.
 * @return
 */
BMS_RTN_ID bms_param_option_init() {
    // serial sent function is null, then return
    s_bms_opt_t.is_finished     = false; // 数据是否已收集完毕
    s_bms_opt_t.opt_cap_t       = _para_opt_cap;
    s_bms_opt_t.opt_hw_prot_t   = _para_opt_HW_prot;
    s_bms_opt_t.opt_ocv_t       = _para_opt_ocv;
    s_bms_opt_t.opt_pro_t       = _para_opt_product;
    s_bms_opt_t.opt_can_t       = _para_opt_can;
    s_bms_opt_t.opt_port_485A_t = _para_opt_port_485A;
    s_bms_opt_t.opt_port_CAN1_t = _para_opt_port_CAN1;
    s_bms_opt_t.opt_func_en_t   = _para_opt_func_en;
    s_bms_set_opt_t.is_finished = false;
    s_bms_set_opt_t.is_option   = false;
    return 0;
}

/**
 * @brief 描述: 
 * @param {void} *ptr
 * @param {uint8_t} operation
 * @param {uint8_t} module
 * @param {uint8_t} function
 * @param {uint8_t} length
 * @return {*}
 */
static BMS_RTN_ID request_option_param(void *ptr, uint8_t operation, uint8_t module, uint8_t function, uint8_t length) {
    static uint8_t opt_addr = 0;
    if (opt_addr >= PARALLEL_NUM) {
        opt_addr = 0;
    }
    protocol_B0_require(ptr, operation, module, function, length, opt_addr++);
    return 0;
}

/**
 * @brief 描述: 
 * @param {void} *ptr
 * @param {uint8_t} addr
 * @param {uint8_t} operation
 * @param {uint8_t} module
 * @param {uint8_t} function
 * @param {uint8_t} length
 * @return {*}
 */
static BMS_RTN_ID request_option_param_by_addr(void *ptr, uint8_t addr, uint8_t operation, uint8_t module, uint8_t function,
                                               uint8_t length) {
    protocol_B0_require(ptr, operation, module, function, length, addr);
    return E_OK;
}
/**
 * @brief 描述: 
 * @param {void} *ptr
 * @return {*}
 */
static BMS_RTN_ID request_option_product_param(void *ptr) { return request_option_param(ptr, enum_module_get, E_MFG_PARAM, 0xFF, 0x00); }
/**
 * @brief 描述: 
 * @param {void} *ptr
 * @return {*}
 */
static BMS_RTN_ID request_option_cap_param(void *ptr) { return request_option_param(ptr, enum_module_get, E_CAP_PARAM, 0xFF, 0x00); }
/**
 * @brief 描述: 
 * @param {void} *ptr
 * @return {*}
 */
static BMS_RTN_ID request_option_port_cfg(void *ptr, COM_PORT_T port_type) {
    return request_option_param(ptr, enum_module_get, E_PORT_CFG_PARAM, port_type, 0x00);
}
/**
 * @brief 描述: 
 * @param {void} *ptr
 * @param {uint8_t} addr
 * @return {*}
 */
static BMS_RTN_ID request_option_product_param_by_addr(void *ptr, uint8_t addr) {
    return request_option_param_by_addr(ptr, addr, enum_module_get, E_MFG_PARAM, 0xFF, 0x00);
}
/**
 * @brief 描述: 
 * @param {void} *ptr
 * @param {uint8_t} addr
 * @return {*}
 */
static BMS_RTN_ID request_option_cap_param_by_addr(void *ptr, uint8_t addr) {
    return request_option_param_by_addr(ptr, addr, enum_module_get, E_CAP_PARAM, 0xFF, 0x00);
}

/**
 * @brief 描述: 
 * @param {void} *ptr
 * @param {uint8_t} addr
 * @return {*}
 */
BMS_RTN_ID request_option_port_cfg_by_addr(void *ptr, uint8_t addr, COM_PORT_T port_type) {
    // s_bms_opt_t.is_finished = false;
    return request_option_param_by_addr(ptr, addr, enum_module_get, E_PORT_CFG_PARAM, port_type, 0x00);
}

/**
 * @brief 描述: 
 * @param {void} *ptr
 * @param {uint8_t} addr
 * @return {*}
 */
BMS_RTN_ID setting_option_port_cfg_by_addr(void *ptr, uint8_t addr, COM_PORT_T port_type, uint16_t protocol_code) {
    // s_bms_set_opt_t.is_finished = false;
    return protocol_B0_setting(ptr, enum_module_set, E_PORT_CFG_PARAM, port_type, 0x04, addr, protocol_code);
}

/**
 * @brief 描述: 获取参数操作结构体
 * @return {*}
 */
D_BMS_OPTION_PARAM_T *bms_get_opt_info(void) { return &s_bms_opt_t; }
D_BMS_SET_OPTION_PARAM_T *bms_get_set_opt_info(void) { return &s_bms_set_opt_t; }