/*
 * Copyright (c) Union Co., Ltd. Development Team 2020-2021. All rights reserved.
 * File name:   e2p_cfg.h
 * Author: Donn1e   ID:     Version:    Date: 18/05/2022
 * Description: // 用于详细说明此程序文件完成的主要功能，与其他模块
 *              // 或函数的接口，输出值、取值范围、含义及参数间的控
 *              // 制、顺序、独立或依赖等关系
 * Others:      // 其它内容的说明
 * History:     // 修改历史记录列表，每条修改记录应包括修改日期、修改
 *              // 者及修改内容简述
 *              Date           Author       Notes
 */
#pragma once
#ifndef E2P_CFG_H__
    #define E2P_CFG_H__

    #include <stdint.h>
/**
 * @Description: Battery Pack Parameters
 */
typedef enum {
    // 此处的顺序不要作更改，因为与协议发送的顺序保持一致
    // 每个元素占据16bit
    // 3*0 protect
    IDX_CELL_OV_PROT = 0,
    IDX_CELL_OV_PROT_DELAY,
    IDX_CELL_OV_PROT_RECOVERY,
    // 3*1
    IDX_CELL_UV_PROT,
    IDX_CELL_UV_PROT_DELAY,
    IDX_CELL_UV_PROT_RECOVERY,
    // 3*2
    IDX_CELL_V_DIFF_H_BUF,
    IDX_CELL_V_DIFF_H_DELAY,
    IDX_CELL_V_DIFF_H_RECOVERY_BUF,
    // 3*3
    IDX_PACK_OV_PROT,          //充电总电压保护点
    IDX_PACK_OV_PROT_DELAY,    //兖电总电压保护恢复时间
    IDX_PACK_OV_PROT_RECOVERY, //充电总电压保护恢复
    // 3*4
    IDX_PACK_UV_PROT,          //放电总电压保护点
    IDX_PACK_UV_PROT_DELAY,    //放电总电压保护恢复时间
    IDX_PACK_UV_PROT_RECOVERY, //放电总电压保护恢复 50
    // 3*5
    IDX_CHG_OC1_BUF,
    IDX_CHG_OC1_DELAY_BUF,
    IDX_CHG_OC1_RECOVER,
    // 3*6
    IDX_CHG_OC2_BUF,
    IDX_CHG_OC2_DELAY_BUF,
    IDX_CHG_OC2_RECOVER,
    // 3*7
    IDX_DISCH_OC1_BUF,
    IDX_DISCH_OC1_DELAY_BUF,
    IDX_DISCH_OC1_RECOVER,
    // 3*8
    IDX_DISCH_OC2_BUF,
    IDX_DISCH_OC2_DELAY_BUF,
    IDX_DISCH_OC2_RECOVER,
    // 3*9
    IDX_CHG_H_TEMP_BUF,
    IDX_CHG_H_TEMP_DELAY_BUF,
    IDX_CHG_H_TEMP_RECOVERY_BUF,
    // 3*10
    IDX_CHG_L_TEMP_BUF,
    IDX_CHG_L_TEMP_DELAY_BUF,
    IDX_CHG_L_TEMP_RECOVERY_BUF,
    // 3*11
    IDX_DISCH_H_TEMP_BUF,
    IDX_DISCH_H_TEMP_DELAY_BUF,
    IDX_DISCH_H_TEMP_RECOVERY_BUF,
    // 3*12
    IDX_DISCH_L_TEMP_BUF,
    IDX_DISCH_L_TEMP_DELAY_BUF,
    IDX_DISCH_L_TEMP_RECOVERY_BUF,
    // 3*13
    IDX_MOS_H_TEMP_BUF,
    IDX_MOS_H_TEMP_DELAY,
    IDX_MOS_H_TEMP_RECOVERY_BUF,
    // 3*14
    IDX_ENV_H_TEMP_BUF,
    IDX_ENV_H_TEMP_DELAY_BUF, //30
    IDX_ENV_H_TEMP_RECOVERY_BUF,
    // 3*15
    IDX_ENV_L_TEMP_BUF,
    IDX_ENV_L_TEMP_DELAY_BUF,
    IDX_ENV_L_TEMP_RECOVERY_BUF,

    // 3*16 warning
    IDX_E2P_CHG_O_V_ALARM,
    IDX_E2P_CHG_O_V_ALARM_DELAY,
    IDX_E2P_CHG_O_V_ALARM_RECOVERY,
    // 3*17
    IDX_E2P_CHG_U_V_ALARM,
    IDX_E2P_CHG_U_V_ALARM_DELAY,
    IDX_E2P_CHG_U_V_ALARM_RECOVERY,
    // 3*18
    IDX_E2P_CELL_V_DIFF_H_ALARM,
    IDX_E2P_CELL_V_DIFF_H_ALARM_DELAY,
    IDX_E2P_CELL_V_DIFF_H_ALARM_RECOVERY,
    // 3*19
    IDX_E2P_PACK_O_V_ALARM,
    IDX_E2P_PACK_O_V_ALARM_DELAY,
    IDX_E2P_PACK_O_V_ALARM_RECOVERY,
    // 3*20
    IDX_E2P_PACK_U_V_ALARM,
    IDX_E2P_PACK_U_V_ALARM_DELAY,
    IDX_E2P_PACK_U_V_ALARM_RECOVERY,
    // 3*21
    IDX_E2P_CHG_O_C_ALARM,
    IDX_E2P_CHG_O_C_ALARM_DELAY,
    IDX_E2P_CHG_O_C_ALARM_RECOVERY,
    // 3*22
    IDX_E2P_CHG_OC2_ALARM,
    IDX_E2P_CHG_OC2_ALARM_DELAY,
    IDX_E2P_CHG_OC2_ALARM_RECOVERY,
    // 3*23
    IDX_E2P_DISCH_O_C_ALARM,
    IDX_E2P_DISCH_O_C_ALARM_DELAY,
    IDX_E2P_DISCH_O_C_ALARM_RECOVERY,
    // 3*24
    IDX_E2P_DISCH_OC2_ALARM,
    IDX_E2P_DISCH_OC2_ALARM_DELAY,
    IDX_E2P_DISCH_OC2_ALARM_RECOVERY,
    // 3*25
    IDX_E2P_CELL_CHG_O_TEMP_ALARM,          /*电芯充电高低温告警*/
    IDX_E2P_CELL_CHG_O_TEMP_ALARM_DELAY,    /*电芯充电高低温告警*/
    IDX_E2P_CELL_CHG_O_TEMP_ALARM_RECOVERY, /*电芯充电高低温告警*/
    // 3*26
    IDX_E2P_CELL_CHG_U_TEMP_ALARM,          /*电芯充电高低温告警*/
    IDX_E2P_CELL_CHG_U_TEMP_ALARM_DELAY,    /*电芯充电高低温告警*/
    IDX_E2P_CELL_CHG_U_TEMP_ALARM_RECOVERY, /*电芯充电高低温告警*/
    // 3*27
    IDX_E2P_CELL_DISCH_O_TEMP_ALARM,          /*电芯放电高低温告警*/
    IDX_E2P_CELL_DISCH_O_TEMP_ALARM_DELAY,    /*电芯放电高低温告警*/
    IDX_E2P_CELL_DISCH_O_TEMP_ALARM_RECOVERY, /*电芯放电高低温告警*/
    // 3*28
    IDX_E2P_CELL_DISCH_U_TEMP_ALARM,          /*电芯放电高低温告警*/
    IDX_E2P_CELL_DISCH_U_TEMP_ALARM_DELAY,    /*电芯放电高低温告警*/
    IDX_E2P_CELL_DISCH_U_TEMP_ALARM_RECOVERY, /*电芯放电高低温告警*/
    // 3*29
    IDX_E2P_MOS_O_TEMP_ALARM,
    IDX_E2P_MOS_O_TEMP_ALARM_DELAY,
    IDX_E2P_MOS_O_TEMP_ALARM_RECOVERY,
    // 3*30
    IDX_E2P_ENV_O_TEMP_ALARM,
    IDX_E2P_ENV_O_TEMP_ALARM_DELAY,
    IDX_E2P_ENV_O_TEMP_ALARM_RECOVERY,
    // 3*31
    IDX_E2P_ENV_U_TEMP_ALARM,
    IDX_E2P_ENV_U_TEMP_ALARM_DELAY,
    IDX_E2P_ENV_U_TEMP_ALARM_RECOVERY,
    // 3*32 = 96
    IDX_E2P_RES_96,
    IDX_E2P_RES_97,

    // 98
    IDX_CELL_SLEEP_V_BUF,     // 单体电压休眠电压
    IDX_CELL_SLEEP_DELAY_BUF, // 单节休眠延时
    // 100
    IDX_BALANCE_H_TEMP_STOP,      // 均衡高温禁止
    IDX_BALANCE_L_TEMP_STOP,      // 均衡低温禁止
    IDX_BALANCE_START_V_BUF,      // 均衡启动电压
    IDX_BALANCE_START_DIFF_V_BUF, // 均衡启动压差
    IDX_E2P_SOC_L_ALARM,          // SOC 低告警
    IDX_E2P_SOC_L_ALARM_RECOVERY, // SOC 低告警恢复
    IDX_E2P_CV_BUF,               // 恒压
    IDX_E2P_CC_BUF,               // 恒流
    IDX_SOC_L_PROTECT,            // SOC 低保护
    IDX_SOC_L_PROTECT_RECOVERY,   // SOC 低保护恢复

    IDX_LC_FLAG_BUF = 263,        // 限流模式
    IDX_BUZZER_CON,               // 蜂鸣器使能
    IDX_END,                      //需要考虑Log的开始位置
} E2P_IDX_ENUM_T;
    #define E2P_SIZE IDX_END
#endif                            //E2P_CFG_H__
