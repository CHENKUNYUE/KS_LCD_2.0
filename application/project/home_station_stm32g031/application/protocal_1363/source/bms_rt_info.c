/*
 * Copyright (c) Union Co., Ltd. Development Team 2020-2020. All rights reserved.
 * File name:   bms_rt_info.c
 * Author: Donn1e   ID:     Version:    Date: 11/03/2021
 * Description: // 用于详细说明此程序文件完成的主要功能，与其他模块
 *              // 或函数的接口，输出值、取值范围、含义及参数间的控
 *              // 制、顺序、独立或依赖等关系
 * Others:      // 其它内容的说明
 * History:     // 修改历史记录列表，每条修改记录应包括修改日期、修改
 *              // 者及修改内容简述
 *              Date           Author       Notes
 */

#include "bms_rt_info.h"

static BMS_RT_INFO_T bms = {0};

BMS_RT_INFO_T *bms_get_rt_info(void) { return &bms; }

/**
 * @brief 获取BMS地址
 * @param *addr - 获取地址的指针.
 * @return 0 on successful, 1 on failed.
 */
void bms_get_addr(uint8_t *addr) { *addr = (uint8_t) bms.bms_addr; }