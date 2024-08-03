/*
 * Copyright (c) Union Co., Ltd. Development Team 2020-2020. All rights reserved.
 * File name:   bms_rt_info.h
 * Author: Donn1e   ID:     Version:    Date: 11/03/2021
 * Description: // 用于详细说明此程序文件完成的主要功能，与其他模块
 *              // 或函数的接口，输出值、取值范围、含义及参数间的控
 *              // 制、顺序、独立或依赖等关系
 * Others:      // 其它内容的说明
 * History:     // 修改历史记录列表，每条修改记录应包括修改日期、修改
 *              // 者及修改内容简述
 *              Date           Author       Notes
 */
#pragma once
#ifndef BMS_RT_INFO_H__
    #define BMS_RT_INFO_H__

    #include <stdint.h>

    #include "app_config.h"
    #include "e2p_cfg.h"

// #define DEBUG_ALL_ADDR_RESPONSE
// #define DEBUG_DISPLAY_REAL_CURRENT_EN
// #define DEBUG_TEMP_EN

/**
 * @brief definition 4 BMS working mode
 */
    #define MODE_IDLE        0
    #define MODE_CHG         1        /*充电模式*/
    #define MODE_DISCH       2        /*放电模式*/
    #define MODE_INIT        3        /*初始化*/
    #define MODE_FILTER_TIME 5

    #define STATE_CHG_MOS_ON   0x1000 //0为导通1为断关
    #define STATE_DISCH_MOS_ON 0x2000 //0为导通1为断关

                                      /**
 * @brief Definition 4 alarm mode
 */
    #define MSK_CELL_OV_ALARM 0x01
    #define MSK_CELL_UV_ALARM 0x02
    #define MSK_PACK_OV_ALARM 0x04
    #define MSK_PACK_UV_ALARM 0x08

    #define MSK_CHG_O_C_ALARM     0x10
    #define MSK_DISCH_O_C_ALARM   0x20
    #define MSK_CELL_O_TEMP_ALARM 0x40
    #define MSK_CELL_U_TEMP_ALARM 0x80

    #define MSK_ENV_O_TEMP_ALARM 0x100
    #define MSK_ENV_U_TEMP_ALARM 0x200
    #define MSK_MOS_O_TEMP_ALARM 0x400 //modifypackO_TPCB_MOS
    #define MSK_SOC_L_ALARM      0x800

    #define MSK_V_DIFF_ALARM           0x1000
    #define MSK_AFE_C_OFFSET_ERR_ALARM 0x2000

    #define MSK_SOC_L_ALARM2     0x10000
    #define SAMPLE_ERR_ALARM_MSK 0x20000
    #define MSK_CHG_O_TEMP_ALARM 0x40000
    #define MSK_CHG_U_TEMP_ALARM 0x80000

    #define MSK_DISCH_O_TEMP_ALARM 0x100000
    #define MSK_DISCH_U_TEMP_ALARM 0x200000
    #define HEAT_FILM_FAULT_ALARM  0x400000

/**
 * @brief Definition 4 protect mode
 */
    #define MSK_CELL_OV_PROT 0x01         //bit0
    #define MSK_PACK_OV_PROT 0x02         //bit1
    #define MSK_CELL_UV_PROT 0x04         //bit2
    #define MSK_PACK_UV_PROT 0x08         //bit3

    #define MSK_CHG_O_C1_PROT   0x10      //bit4
    #define MSK_CHG_O_C2_PROT   0x20      //bit5
    #define MSK_DISCH_O_C1_PROT 0x40      //bit6
    #define MSK_DISCH_O_C2_PROT 0x80      //bit7

    #define MSK_SC_PROT           0x100   //bit8
    #define MSK_CHG_O_TEMP_PROT   0x200   //bit9
    #define MSK_CHG_U_TEMP_PROT   0x400   //bit10
    #define MSK_DISCH_O_TEMP_PROT 0x800   //bit11

    #define MSK_DISCH_U_TEMP_PROT 0x1000  //bit12
    #define MSK_BAT_FULL_PROT     0x2000  //bit13
    #define MSK_CELL_NO_BALANCE   0x4000  //bit14
    #define MSK_L_SOC_PROT        0x8000  //bit15

    #define MSK_L_1V_PROT       0x10000   //bit16
    #define MSK_L_26V_PROT      0x20000   //bit17
    #define MSK_V_DIFF_H_PROT   0x40000   //bit18
    #define MOS_O_TEMP_PROT_MSK 0x80000   //bit19

    #define MSK_AFE_FAULT_PROT  0x100000  //bit20
    #define MSK_NTC_FAULT_PROT  0x200000  //bit21
    #define MSK_ANTI_THEFT_PROT 0x400000  //bit22
    #define MSK_ENV_O_TEMP_PROT 0x800000  //bit23

    #define MSK_ENV_U_TEMP_PROT 0x1000000 //bit24

    #define MSK_DISCH_O_C3_PROT 0x800000  //bit7

    // static
    #pragma pack(4)
// static
typedef struct {
    int16_t val; // Eg. 250 250 / precision -> 25.0°C
    uint8_t status;
    uint8_t user_code;
    uint8_t option;
} NTC_T;

// static
typedef struct {
    NTC_T temp[NTC_NODES_NUM];
    int16_t offset;     // Eg. -50 val=25°C -> 25+(-50) = -25°C
    uint16_t precision; // 0.1°C 250-> 25.0°C
    uint32_t chg_ot;
    uint32_t chg_ut;
    uint32_t disch_ot;
    uint32_t disch_ut;
    uint32_t chg_ot_alm;
    uint32_t chg_ut_alm;
    uint32_t ntc_line_broken;
    int16_t temp_all_highest;
    int16_t temp_all_2nd_high;
    int16_t temp_all_lowest;
    int16_t temp_all_2nd_low;
    int16_t temp_all_cell_avg;
    int16_t temp_cell_highest;
    int16_t temp_cell_2nd_high;
    int16_t temp_cell_lowest;
    int16_t temp_cell_2nd_low;
    int16_t temp_cell_cell_avg;
} CD4052_NTC_T;

typedef struct {
    uint32_t op_mode;
    uint32_t stop_run_flag;
    // LATCH_UNION_TYPEDEF latch_mode;
    uint16_t *param_tbl[IDX_END];
    // uint16_t enter_standby_timer;
    // uint32_t no_cur_delay;
    // VOL_SPL_UNIT_T cell_nodes[CELL_NUM];
    uint16_t num_of_cells;
    // uint16_t cell_values[CELL_NUM];
    uint16_t highest_cell_volts;
    // uint16_t highest_cell_idx;
    // uint16_t lowest_cell_volts;
    // uint16_t lowest_cell_idx;
    // // uint16_t cell_v_diff;
    // uint16_t aver_cell_volts;
    // float c_self_consumption;
    //
    // //battery pack voltage
    // VOL_SPL_UNIT_T pack_vol;
    // #if PRE_CHG_EN > 0
    // VOL_SPL_UNIT_T prechg_pack_vol;
    // uint16_t prechg_refvol[MAX_PRECHG_REFVOL_POINT];
    // uint16_t prechg_con_flag;
    // uint32_t pre_charge_count;
    // uint32_t pre_charge_delay;
    // #endif
    // uint32_t pack_spl_tick;
    //
    // //temperature
    CD4052_NTC_T temp_nodes;
    //
    // ADC_STRUCT_T adc;
    uint16_t lc_assert; // 限流标志位  0 is C_LIMIT_OFF
    // uint8_t lc_step_chg;   // chg限流尝试次数
    // uint8_t lc_step_disch; // disch限流尝试次数
    //
    // CUR_SPL_T chg_cur;
    // CUR_SPL_T disch_cur;
    //
    // uint32_t shut_off_charge_count;
    // uint32_t g_cur_limit_timer;
    //
    // //LCC
    // uint32_t lcc_flag;
    // uint32_t lcc_ov_status;
    // uint32_t lcc_val_status;
    // //soc
    // SOC_CALC_T soc_unit;
    // //capacity
    // float out_cap_sum;
    // float out_cap_sum_bak;

    union {
        struct {
            uint32_t cell_ov : 1;
            uint32_t cell_uv : 1;
            uint32_t pack_ov : 1;
            uint32_t pack_uv : 1;

            uint32_t chg_o_c : 1;
            uint32_t disch_o_c : 1;
            uint32_t cell_o_temp : 1;
            uint32_t cell_u_temp : 1;

            uint32_t env_o_temp : 1;
            uint32_t env_u_temp : 1;
            uint32_t mos_o_temp : 1;
            uint32_t soc_l : 1;

            uint32_t v_diff : 1;
            uint32_t afe_c_offset_err : 1;
            uint32_t rev15 : 1;
            uint32_t rev16 : 1;

            uint32_t soc_l_2 : 1;
            uint32_t sample_err : 1;
            uint32_t chg_o_temp : 1;
            uint32_t chg_u_temp : 1;

            uint32_t disch_o_temp : 1;
            uint32_t disch_u_temp : 1;
            uint32_t heat_film_fault : 1;
            uint32_t rev24 : 1;

            uint32_t rev25 : 1;
            uint32_t rev26 : 1;
            uint32_t rev27 : 1;
            uint32_t rev28 : 1;

            uint32_t rev29 : 1;
            uint32_t rev30 : 1;
            uint32_t rev31 : 1;
            uint32_t rev32 : 1;
        };
        uint32_t bytes;
    } alarm;
    union {
        struct {
            uint32_t cell_ov : 1;
            uint32_t pack_ov : 1;
            uint32_t cell_uv : 1;
            uint32_t pack_uv : 1;

            uint32_t chg_o_c1 : 1;
            uint32_t chg_o_c2 : 1;
            uint32_t disch_o_c1 : 1;
            uint32_t disch_o_c2 : 1;

            uint32_t sc : 1;
            uint32_t chg_o_temp : 1;
            uint32_t chg_u_temp : 1;
            uint32_t disch_o_temp : 1;

            uint32_t disch_u_temp : 1;
            uint32_t bat_full : 1;
            uint32_t cell_no_balance : 1;
            uint32_t l_soc_prot : 1;

            uint32_t cell_fail_prot : 1;
            uint32_t rev17 : 1;
            uint32_t v_diff_h : 1;
            uint32_t mos_o_temp : 1;

            uint32_t afe_fault : 1;
            uint32_t ntc_fault : 1;
            uint32_t anti_theft : 1;
            uint32_t env_o_temp : 1;

            uint32_t env_u_temp : 1;
            uint32_t rev26 : 1;
            uint32_t rev27 : 1;
            uint32_t rev28 : 1;

            uint32_t rev29 : 1;
            uint32_t rev30 : 1;
            uint32_t rev31 : 1;
            uint32_t rev32 : 1;
        };
        uint32_t bytes;
    } protect;
    union {
        struct {
            uint32_t ntc_disc : 1;
            uint32_t msk_adc_error : 1;
            uint32_t reversed : 1;
            uint32_t vol_broken : 1;

            uint32_t state_v_cable_broken : 1;
            uint32_t chg_mos : 1;
            uint32_t disch_mos : 1;
            uint32_t afe : 1;

            uint32_t blown_fuse : 1;
            uint32_t rev9_32 : 23;
        };
        uint32_t bytes;
    } error;
    uint32_t status;
    // uint16_t wakeup_source;
    //
    // uint32_t alarm_arb_tick;
    //
    // //short fault record write
    // uint32_t sc_cnt_write_flag;
    // uint32_t sc_flag;
    // uint32_t sc_cnt;
    //
    // uint32_t sc_power_on_msk_cnt;
    // uint32_t sc_filter_cnt;
    // uint32_t sc_recovery_delay_cnt;
    // uint32_t sc_recovery_trial_cnt;
    //
    // union {
    //     struct {
    //         uint32_t task_protect_arb : 1;
    //         uint32_t task_soc_unit : 1;
    //         uint32_t task_led_disp : 1;
    //         uint32_t task_gpio : 1;
    //         uint32_t task_fault_log : 1;
    //         uint32_t task_can : 1;
    //         uint32_t task_power_down : 1;
    //
    //         uint32_t rsvd : 25;
    //     } bit;
    // } task_time_arrival;
    // uint32_t chg_o_v_forced_recovery;
    // uint32_t cls_chg_mos_after_ov_cnt;
    // uint32_t chg_ov_cnt;
    //
    // uint32_t disch_uv_cnt;
    // //pack uv
    // uint32_t puv_recover_retry_cnt;
    // uint32_t cuv_recover_retry_cnt;
    // //discharge current
    //
    // uint32_t chg_disch_oc_cnt;
    // uint32_t disch_oc_recover_retry_cnt;
    // /*	low power mode---------------	*/
    // #if POWER_SWITCH_EN > 0
    // uint32_t power_down_flag;
    // #endif
    // //balance
    uint32_t balance_state;
    uint32_t balance_idle_en;
    uint32_t idle_sleep_en;
    uint32_t balance_bat_msg;

    uint32_t balance_flag;
    //
    // //load remove detection
    // #if LOAD_REMOVE_DETECT_EN > 0
    // uint32_t load_removed_op_ste;
    // #endif
    //
    uint32_t force_sleep_flag;
    //
    // //led
    // uint32_t task_led_disp_tick_INT; //200-->1s
    // uint8_t task_led_disp_en;
    // //address
    uint16_t bms_addr;
    // uint16_t bms_addr_dido;
    // uint8_t is_master;
    // #if FAULT_LOG_RETENTION_EN > 0
    // uint32_t task_fault_log_tick;
    // #endif
    //
    #if HEAT_ON_EN > 0
    uint16_t heat_con_flag;
    #endif
    // uint8_t flag_high_volts;
    //
    // #if REVERSE_CONNECT_PROTECT > 0
    // uint32_t inversed_grafting_delay;
    // #endif
    // uint32_t disch_ot_cnt;
    uint8_t balance_max_num;
    // uint16_t cali_I_K_disch_0_0001; //range:4_0000 - 0_0500
    // uint16_t cali_I_K_chg_0_0001;   //range:4_0000 - 0_0500
    uint8_t factory_mode;
    // uint8_t sleep_mode;
    // uint8_t before_sleep_close_disch;
    uint8_t force_low_power;
    // uint8_t sleep_mode_back;
    // uint32_t boot_version;
    // uint32_t soft_version;
} BMS_RT_INFO_T;

BMS_RT_INFO_T *bms_get_rt_info(void);

void bms_get_addr(uint8_t *addr);
    #pragma pack() // 恢复对齐状态
#endif             //BMS_RT_INFO_H__
