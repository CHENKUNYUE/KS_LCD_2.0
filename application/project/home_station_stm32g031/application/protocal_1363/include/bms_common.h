/*
 * Copyright (c) Union Co., Ltd. Development Team 2020-2020. All rights reserved.
 * File name:   bms_common.h
 * Author: Donn1e   ID:     Version:    Date: 11/03/2021
 * Description: // 用于详细说明此程序文件完成的主要功能，与其他模块
 *              // 或函数的接口，输出值、取值范围、含义及参数间的控
 *              // 制、顺序、独立或依赖等关系
 * Others:      // 其它内容的说明
 * History:     // 修改历史记录列表，每条修改记录应包括修改日期、修改
 *              // 者及修改内容简述
 *              Date           Author       Notes
 *              2023年03月19日     gj        data_type_enum增加enum_data_var_floatr, reason : 日月源modbus协议同步
 */

#ifndef BMS_COMMON_H__
#define BMS_COMMON_H__

#include <stdint.h>
#define MIN_IN_MS              (60000)
#define SIZE_OF(test_sequence) (sizeof(test_sequence) / sizeof(*test_sequence))
#define UINT16_T_MAX           (0xffff)

#ifndef ON
    #define ON 1
#endif

#ifndef OFF
    #define OFF 0
#endif

#ifndef UNKNOWN
    #define UNKNOWN 0xFF
#endif

#ifndef ACTION
    #define ACTION     1
    #define NON_ACTION 0
#endif

#define SET_STATUS(bms_status, mask, is_set) (((is_set) == 1) ? ((bms_status) |= (mask)) : ((bms_status) &= ~(mask)))
#define GET_STATUS(bms_status, mask)         (((bms_status) & (mask)) ? (0x01) : (0x00))

// 定义宏来提取前四个比特和后四个比特的值
#define GET_LOWER_NIBBLE(byte) ((byte) & 0x0F)
#define GET_UPPER_NIBBLE(byte) (((byte) >> 4) & 0x0F)

// 定义宏来将两个四比特值合并为一个字节
#define MERGE_NIBBLES(upper, lower) (((upper) << 4) | (lower))
typedef enum {
    E_OK = 0, /**< There is no error */
    E_ERROR,  /**< A generic error happens */
    E_CALLING_API_ERROR,
    E_OVERFLOW,
    E_BUSY,
    E_NOT_READY,
    E_OPERATION_INCORRECT,
} BMS_RTN_ID;

uint16_t u8_hex_2_u16_ascii(uint8_t ref_byte);
uint32_t u16_hex_2_u32_ascii(uint16_t ref_byte);

void bubble_sort(uint16_t *arr, int len);
void bubble_sort_idx(uint16_t *arr, int len, uint8_t *idx_arr, uint8_t is_up);
void bubble_sort_s16(int16_t *arr, int len);
void bubble_sort_s16_idx(int16_t *arr, uint8_t *p_idx_buf, int len);
void quick_sort(uint16_t *arr, uint16_t h, uint16_t t);
uint8_t str_xor_chk(uint8_t *pbuf, uint8_t buf_len);
uint8_t hex_to_bcd(uint8_t temp);
uint8_t bcd_to_hex(uint8_t temp);
float _abs_f(float in);
uint16_t _ceil_f(float num);
void bubble_sort_float(float *arr, uint16_t len);
uint8_t trim_mean(uint16_t *array, uint8_t len, uint8_t percent, uint16_t *rtn_val);
uint8_t binary_search_upper_2_lower(uint16_t *array, uint16_t len, uint16_t val, uint16_t *idx);
int binary_search_lower_2_upper(uint32_t *array, uint16_t len, uint32_t val, uint16_t *idx);

typedef struct {
    uint8_t (*ascii_to_num)(const uint8_t *ref_ascii);
    uint8_t (*two_ascii_2_u8)(const uint8_t *ref_ascii);
    uint8_t (*two_ascii_2_u8_inc)(const uint8_t *p_buf, uint16_t *p_idx);
    uint16_t (*four_ascii_to_u16)(const uint8_t *ref_ascii);
    uint8_t (*find_loc_u8)(uint8_t target, const uint8_t *p_buf, uint8_t len);
    uint16_t (*find_loc_u16)(uint16_t target, const uint16_t *p_buf, uint16_t len);

} UTILS_T;

extern const UTILS_T utils_t;
#endif //BMS_COMMON_H__
