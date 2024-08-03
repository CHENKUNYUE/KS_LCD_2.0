/*
 * Copyright (c) Union Co., Ltd. Development Team 2020-2021. All rights reserved.
 * File name:   protocol_common.h
 * Author: Donn1e   ID:     Version:    Date: 03/03/2022
 * Description: // 用于详细说明此程序文件完成的主要功能，与其他模块
 *              // 或函数的接口，输出值、取值范围、含义及参数间的控
 *              // 制、顺序、独立或依赖等关系
 * Others:      // 其它内容的说明
 * History:     // 修改历史记录列表，每条修改记录应包括修改日期、修改
 *              // 者及修改内容简述
 *              Date           Author       Notes
 */

#ifndef PROTOCOL_COMMON_H__
#define PROTOCOL_COMMON_H__

#include <stdint.h>
#define PARALLEL_NUM (16) // min16 max 64

typedef enum {
    E_PORT_485A = 1,
    E_PORT_485B,
    E_PORT_485C,
    E_PORT_485D,
    E_PORT_485E,
    E_PORT_485F,
    E_PORT_CAN0 = 11,
    E_PORT_CAN1,
    E_PORT_MAX_NUM,
} COM_PORT_T; //communication port

typedef enum {
    E_CAN_DEFAULT = 0,          /*0 默认*/
    E_CAN_VICTORN = 1,          /* 1-Victron-维克托*/
    E_CAN_PYLON,                /* 2-Pylon-派能*/
    E_CAN_GOODWE,               /* 3-Goodwe-固德威*/
    E_CAN_GROWATT,              /* 4-Growatt-古瑞瓦特*/
    E_CAN_VOLTRONIC,            /* 5-Voltronic-日月元*/
    E_CAN_LXP,                  /* 6-LXP-鹏城*/
    E_CAN_DEYE,                 /* 7-Deye-德业*/
    E_CAN_SOFAR,                /* 8-Sofar-首航*/
    E_CAN_SOLIS,                /* 9-Solis-锦浪*/
    E_CAN_SUNGROW,              /* 10-SunGrow-阳光*/
    E_CAN_STUDER,               /* 11-StuderInnotec-易恩孚*/
    E_CAN_HUAWEI,               /* 12-Huawei-华为*/
    E_CAN_SMA,                  /* 13-SMA-艾思玛*/
    E_CAN_INVT,                 /* 14-INVT-英威腾*/
    E_CAN_KSTAR,                /* 15-Kstar-科士达*/
    E_CAN_MUST,                 /* 16-MUST-美克/美世乐*/
    E_CAN_SRNE,                 /* 17-SRNE-朔日*/
    E_CAN_FOXES,                /* 18-Foxes-麦田*/
    E_CAN_BST,                  /* 19-BST-电科*/
    E_CAN_AISWEI,               /* 20-AISWEI-爱士惟*/
    E_CAN_SCHNEIDER,            /* 21-施耐德*/
    E_CAN_ALL,                  /* -全部*/
    E_CAN_MAX_NUM,              /* CAN的协议数量 */
    E_485_NULL = 200,           /* 200 */
    E_485_PYLON,                /* 201-Pylon */
    E_485_SMK,                  /* 202-SMK solar */
    E_485_VOLTRONIC,            /* 203-VOLTRONIC-日月元 */
    E_485_GROWATT,              /* 204-GROWATT-古瑞瓦特 */
    E_485_SRNE,                 /* 205-SRNE-朔日 */
    E_485_GP,                   /* 206-GP */
    E_485_HENG_RUI,             /* 207-HengRui 恒瑞 */
    E_485_WOLONG,               /* 208-Wolong 卧龙 */
    E_485_SACREDSUN,            /* 209-SACREDSUN-圣阳 */
    E_485_LI_BATTERY_V102,      /* 210-锂电池协议 */
    E_485_LINGYU,               /* 211-领域协议 */
    E_485_ELTEK,                /* 212-eltek协议 */
    E_485_HUAWEI,               /* 213-华为协议 */
    E_485_VIRTIV,               /* 214-维蒂协议 */
    E_485_BATTERY_SWAP_CABINET, /* 215-换电柜 */
    E_485_VOLT_AGV,             /* 216-伏特 agv */
    E_485_MCU2DSP,              /* 217-MCU2DSP协议 */
    E_485_MAX_NUM,
    E_ALL_MAX_NUM,
} INV_PID_T; //protocol id

#endif //PROTOCOL_COMMON_H__
