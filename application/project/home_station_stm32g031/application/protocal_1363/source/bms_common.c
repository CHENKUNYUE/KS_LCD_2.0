/*
 * Copyright (c) Union Co., Ltd. Development Team 2020-2020. All rights reserved.
 * File name:   bms_common.c
 * Author: Donn1e   ID:     Version:    Date: 11/03/2021
 * Description: // 用于详细说明此程序文件完成的主要功能，与其他模块
 *              // 或函数的接口，输出值、取值范围、含义及参数间的控
 *              // 制、顺序、独立或依赖等关系
 * Others:      // 其它内容的说明
 * History:     // 修改历史记录列表，每条修改记录应包括修改日期、修改
 *              // 者及修改内容简述
 *              Date           Author       Notes
 */

#include "bms_common.h"

#include <string.h>

/**
* @Description:将U8转成2个ascii字符放在 u16变量中 如果0x23 转换完位 H4bit： ‘2’  L4bit： ‘3’
* @param ref_byte - .
* @return .
*/
uint16_t u8_hex_2_u16_ascii(uint8_t ref_byte) {
    uint8_t s_th;
    uint8_t s_tl;
    uint16_t s_rxd = 0;

    s_tl = ref_byte & 0x0f;
    s_th = (ref_byte & 0xf0) >> 4;

    s_rxd = (s_th < 10) ? (s_th + '0') << 8 : (s_th - 10 + 'A') << 8;
    s_rxd |= (s_tl < 10) ? (s_tl + '0') : (s_tl - 10 + 'A');

    return s_rxd;
}

/**
 * @Description:将u16的转成4个ASCII字符 如0x4321 转化完成在内存中为为 ‘4’+‘3’+‘2’+‘1’
 * @param  - . uint8_t  ref_byte
 * @return uint32_t
 */
uint32_t u16_hex_2_u32_ascii(uint16_t ref_byte) {
    unsigned char s_temp;
    uint32_t s_num = 0;
    s_temp         = (ref_byte & 0xff00) >> 8;
    s_num          = (u8_hex_2_u16_ascii(s_temp)) << 16;
    s_temp         = ref_byte & 0x00ff;
    s_num |= u8_hex_2_u16_ascii(s_temp);
    return (s_num);
}

void quick_sort(uint16_t *arr, uint16_t h, uint16_t t) {
    if (h >= t) return;
    int mid = (h + t) / 2, i = h, j = t, x;
    x = arr[mid];
    while (1) {
        while (arr[i] < x) i++;
        while (arr[j] > x) j--;
        if (i >= j) break;
        uint16_t temp = arr[i];
        arr[i]        = arr[j];
        arr[j]        = temp;
    }
    arr[j] = x;
    quick_sort(arr, h, j - 1);
    quick_sort(arr, j + 1, t);
    return;
}

void bubble_sort(uint16_t *arr, int len) {
    int i, j;
    uint16_t temp;
    for (i = 0; i < len - 1; i++) {
        for (j = 0; j < len - 1 - i; j++) {
            if (arr[j] > arr[j + 1]) {
                temp       = arr[j];
                arr[j]     = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

/**
 * @Description: 对数组进行排序，0 idx 为最小值， len - 1 idx 为最大值
 * @param *arr - 数组指针.
 * @param len - 数组长度.
 * @param *idx_arr - 脚标数组指针.
 * @param is_up - 0: 降序; 1: 升序.
 * @return
 */
void bubble_sort_idx(uint16_t *arr, int len, uint8_t *idx_arr, uint8_t is_up) {
    int i, j, t1, t;
    if (is_up) {
        for (j = 0; j < len; j++) {
            for (i = 0; i < len - 1 - j; i++) {
                if (arr[i] > arr[i + 1]) {
                    t          = arr[i];
                    arr[i]     = arr[i + 1];
                    arr[i + 1] = t;

                    t1             = idx_arr[i];
                    idx_arr[i]     = idx_arr[i + 1];
                    idx_arr[i + 1] = t1;
                }
            }
        }
    } else {
        for (j = 0; j < len; j++) {
            for (i = 0; i < len - 1 - j; i++) {
                if (arr[i] < arr[i + 1]) {
                    t          = arr[i];
                    arr[i]     = arr[i + 1];
                    arr[i + 1] = t;

                    t1             = idx_arr[i];
                    idx_arr[i]     = idx_arr[i + 1];
                    idx_arr[i + 1] = t1;
                }
            }
        }
    }
}

/**
 * @Description: 最小的在最前
 * @param  - .
 * @return 0 on successful, 1 on failed.
 */
void bubble_sort_s16(int16_t *arr, int len) {
    int i, j;
    int16_t temp;
    for (i = 0; i < len - 1; i++) {
        for (j = 0; j < len - 1 - i; j++) {
            if (arr[j] > arr[j + 1]) {
                temp       = arr[j];
                arr[j]     = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void bubble_sort_s16_idx(int16_t *arr, uint8_t *p_idx_buf, int len) {
    int i, j;
    int16_t temp;
    uint8_t _idx_temp;
    for (i = 0; i < len - 1; i++) {
        for (j = 0; j < len - 1 - i; j++) {
            if (arr[j] > arr[j + 1]) {
                temp             = arr[j];
                arr[j]           = arr[j + 1];
                arr[j + 1]       = temp;
                _idx_temp        = p_idx_buf[j];
                p_idx_buf[j]     = p_idx_buf[j + 1];
                p_idx_buf[j + 1] = _idx_temp;
            }
        }
    }
}

void bubble_sort_float(float *arr, uint16_t len) {
    uint16_t i, j;
    float temp;
    for (i = 0; i < len - 1; i++) {
        for (j = 0; j < len - 1 - i; j++) {
            if (arr[j] > arr[j + 1]) {
                temp       = arr[j];
                arr[j]     = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

uint8_t str_xor_chk(uint8_t *pbuf, uint8_t buf_len) {
    uint8_t i, xor = 0;
    uint16_t sum = 0;
    for (i = 0; i < buf_len; i++) {
        xor ^= pbuf[i];
        sum += pbuf[i];
    }
    return (xor^sum) & 0xFF;
}

uint8_t hex_to_bcd(uint8_t temp) {
    uint8_t data;

    data = (temp / 10) * 16;
    data += temp % 10;

    return data;
}

uint8_t bcd_to_hex(uint8_t temp) {
    uint8_t data;
    data = (temp >> 4) * 10 + (temp & 0x0f);
    return data;
}

float _abs_f(float in) { return (in >= 0) ? in : -in; }

uint16_t _ceil_f(float num) {
    uint16_t input_num = (uint16_t) num;
    if (num == (float) input_num) {
        return input_num;
    }
    return input_num + 1;
}

/**
 * @Description: TRIM-MEAN algorithm
 * @param *array - input array pointer.
 * @param len - array length.
 * @param percent - delete ratio of head& tail  100: 100%; 80:80%.
 * @return
 */
uint8_t trim_mean(uint16_t *array, uint8_t len, uint8_t percent, uint16_t *rtn_val) {
    if (percent >= 50) return 1;
    if (len >= 100) return 1;
    uint32_t total_v      = 0;
    uint16_t _cell_v[100] = {0};
    memcpy(_cell_v, array, sizeof(uint16_t) * len);
    bubble_sort(_cell_v, len);
    uint8_t remove_num = (uint8_t) _ceil_f(len * percent / 100.0 / 2.0);
    for (uint8_t i = remove_num; i < len - remove_num; ++i) {
        total_v += (*(_cell_v + i));
    }

    *rtn_val = total_v / (len - 2 * remove_num);
    return 0;
}

/**
 * @Description: 二分法查找数组中的某个值/查找的值在那个位置范围内
 * @param  - .
 * @return
 */
uint8_t binary_search_upper_2_lower(uint16_t *array, uint16_t len, uint16_t val, uint16_t *idx) {
    if (val > array[0]) return 1;
    if (val < array[len - 1]) {
        *idx = len;
        return 0;
    }
    if (len < 1) return 1;

    uint16_t low = 0, high = len - 1, mid;
    while (low <= high) {
        mid = (low + high) / 2;
        if ((array[mid] >= val) && (array[mid + 1] < val)) {
            *idx = mid;
            return 0;
        } else if (array[mid] > val) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return 1;
}

int binary_search_lower_2_upper(uint32_t *array, uint16_t len, uint32_t val, uint16_t *idx) {
    if (val > array[len - 1]) return -1;
    if (val < array[0]) {
        *idx = 0;
        return 0;
    }
    if (len < 1) return -1;

    uint32_t low = 0, high = len - 1, mid;
    while (low <= high) {
        mid = (low + high) / 2;
        if (mid == 0) {
            *idx = mid;
            return 0;
        }
        if ((array[mid] >= val) && (array[mid - 1] < val)) {
            *idx = mid;
            return 0;
        } else if (array[mid] < val) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return -1;
}

/**
 * @Description: 将一个字符转成0x00-0x0F的数 转换地址时，只能是0-15,否则就置为 0x0F
 * @param *ref_ascii - .
 * @return
 */
static uint8_t _ascii_to_num(const uint8_t *ref_ascii) {
    uint8_t ascii = *ref_ascii;
    if (ascii >= 'A' && ascii <= 'F') {
        return ascii - 'A' + 10;
    } else if (ascii >= 'a' && ascii <= 'f') {
        return ascii - 'a' + 10;
    } else if (ascii >= '0' && ascii <= '9') {
        return ascii - '0';
    } else {
        // Handle invalid input
        return 0x0F;
    }
}

/**
 * @Description: 将两个字符转成u8，如 “23”转成0x23
 * @param  - .uint8_t *ref_ascii
 * @return uint8_t
 */
static uint8_t _two_ascii_to_u8(const uint8_t *ref_ascii) {
    uint8_t s_num;
    s_num = _ascii_to_num((ref_ascii));
    s_num = (s_num << 4) | _ascii_to_num((ref_ascii + 1));
    return (s_num);
}

/**
 * @Description: convert 2 ascii hex to 1 uint8_t hex & buffer index increasing automatically
 *                  E.g. 0x37 0x32 --> 0x72
 * @param  - .
 * @return uint8_t data that represent 2 ascii.
 */
uint8_t _two_ascii_to_u8_idx_increase(const uint8_t *p_buf, uint16_t *p_idx) {
    unsigned char s_num;
    s_num = _ascii_to_num(&p_buf[(*p_idx)++]);
    s_num = s_num << 4;
    s_num = s_num | _ascii_to_num(&p_buf[(*p_idx)++]);
    return (s_num);
}

/**
 * @Description: 降4个字符 转成0x0000-0xffff之间的数字，如“4321” 转成0x4321
 * @param  - .uint8_t *buffer
 * @return    uint16_t
 */
static uint16_t _four_ascii_to_u16(const uint8_t *ref_ascii) {
    uint16_t s_num; //s_num = *buffer;

    s_num = _ascii_to_num(ref_ascii++);
    s_num = (s_num << 4) | _ascii_to_num(ref_ascii++);
    s_num = (s_num << 4) | _ascii_to_num(ref_ascii++);
    s_num = (s_num << 4) | _ascii_to_num(ref_ascii++);

    return (s_num);
}

static uint8_t _find_loc_u8(uint8_t target, const uint8_t *p_buf, uint8_t len) {
    uint8_t loc = 0;

    for (uint8_t i = 0; i < len; ++i) {
        if (target < p_buf[i]) {
            loc = i;
            break;
        }
    }

    return loc;
}

static uint16_t _find_loc_u16(uint16_t target, const uint16_t *p_buf, uint16_t len) {
    uint16_t loc = 0;

    for (uint16_t i = 0; i < len; ++i) {
        if (target < p_buf[i]) {
            loc = i;
            break;
        }
    }

    return loc;
}

const UTILS_T utils_t = {
    .ascii_to_num       = _ascii_to_num,
    .two_ascii_2_u8     = _two_ascii_to_u8,
    .two_ascii_2_u8_inc = _two_ascii_to_u8_idx_increase,
    .four_ascii_to_u16  = _four_ascii_to_u16,
    .find_loc_u8        = _find_loc_u8,
    .find_loc_u16       = _find_loc_u16,
};