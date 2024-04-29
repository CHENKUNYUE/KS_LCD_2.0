/*
 * Copyright (c) Daren Hi-Tech Electronics Co., Ltd. Development Team 2020-2020. All rights reserved.
 * File name:   protocol1363_data.h
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
#ifndef COM_PROTOCAL_1363_PROTOCOL1363_DATA_H__
#define     COM_PROTOCAL_1363_PROTOCOL1363_DATA_H__

#include <stdint.h>
#include "protocol1363_transfer.h"

uint16_t u8_hex_to_two_ascii(uint8_t ref_byte);

uint32_t u16_hex_to_four_ascii(uint16_t ref_byte);

uint8_t one_ascii_to_num(uint8_t *ref_ascii);

uint8_t two_ascii_to_u8(uint8_t *ref_ascii);

uint8_t  two_ascii_to_u8_recount(comm_struct *com);

uint16_t four_ascii_to_u16(uint8_t *buffer);

uint16_t  four_ascii_to_u16_recount(comm_struct *com);

uint16_t protocol1363_get_checksum(uint8_t *buffer, uint16_t length);



#endif //COM_PROTOCAL_1363_PROTOCOL1363_DATA_H__
