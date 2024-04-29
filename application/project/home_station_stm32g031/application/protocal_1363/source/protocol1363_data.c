/*
 * Copyright (c) Daren Hi-Tech Electronics Co., Ltd. Development Team 2020-2020. All rights reserved.
 * File name:   protocol1363_data.c
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


#include "protocol1363_data.h"

/**
* @Description:将U8转成2个ascii字符放在 u16变量中 如果0x23 转换完位 H4bit： ‘2’  L4bit： ‘3’
* @param  - . uint8_t  ref_byte
* @return     uint16_t
*/
uint16_t u8_hex_to_two_ascii(uint8_t ref_byte) {
    uint8_t s_th;
    uint8_t s_tl;
    uint16_t s_rxd = 0;

    s_tl = ref_byte & 0x0f;
    s_th = (ref_byte & 0xf0) >> 4;
    if (s_th <= 9) {
        s_rxd = (s_th + 0x30) << 8;
    } else if ((s_th >= 10) && (s_th <= 15)) {
        s_rxd = (s_th + 0x37) << 8;
    }
    if (s_tl <= 9) {
        s_rxd = s_rxd + s_tl + 0x30;
    } else if ((s_tl >= 10) && (s_tl <= 15)) {
        s_rxd = s_rxd + s_tl + 0x37;
    }
    return (s_rxd);
}

/**
 * @Description:将u16的转成4个ASCII字符 如0x4321 转化完成在内存中为为 ‘4’+‘3’+‘2’+‘1’
 * @param  - . uint8_t  ref_byte
 * @return uint32_t
 */
uint32_t u16_hex_to_four_ascii(uint16_t ref_byte) {
    unsigned char s_temp;
    uint32_t s_num = 0;
    s_temp = (ref_byte & 0xff00) >> 8;
    s_num = (u8_hex_to_two_ascii(s_temp)) << 16;
    s_temp = ref_byte & 0x00ff;
    s_num |= u8_hex_to_two_ascii(s_temp);
    return (s_num);
}

/**
 * @Description: 将一个字符转成0x00-0x0F的数
 * @param  - .uint8_t *ref_ascii
 * @return uint8_t
 */
uint8_t one_ascii_to_num(uint8_t *ref_ascii) {
    uint8_t s_num;
    s_num = *ref_ascii;
    if ((s_num >= '0') && (s_num <= '9'))
        return (s_num - '0');
    if ((s_num >= 'a') && (s_num <= 'f'))///A
        return (s_num - 'a' + 10);
    if ((s_num >= 'A') && (s_num <= 'F'))//a
        return (s_num - 'A' + 10);
    return (0);  // 转换地址时，只能是0-15,否则就置为 16
}

/**
 * @Description: 将两个字符转成u8，如 “23”转成0x23
 * @param  - .uint8_t *ref_ascii
 * @return uint8_t
 */
uint8_t two_ascii_to_u8(uint8_t *ref_ascii) {
    uint8_t s_num;
    s_num = one_ascii_to_num((ref_ascii));
    s_num = (s_num << 4) | one_ascii_to_num((ref_ascii + 1));
    return (s_num);
}

/**
 * @Description: 降4个字符 转成0x0000-0xffff之间的数字，如“4321” 转成0x4321
 * @param  - .uint8_t *buffer
 * @return    uint16_t
 */
uint16_t four_ascii_to_u16(uint8_t *buffer) {
    uint16_t s_num;//s_num = *buffer;

    s_num = one_ascii_to_num(buffer++);
    s_num = (s_num << 4) | one_ascii_to_num(buffer++);
    s_num = (s_num << 4) | one_ascii_to_num(buffer++);
    s_num = (s_num << 4) | one_ascii_to_num(buffer++);

    return (s_num);
}


uint8_t changeASCtoByteByAddr(comm_struct *com, uint8_t buffer[], uint16_t rx_info_cnt)
{
    unsigned char  s_num;
    s_num = one_ascii_to_num(&com->rx_buff[rx_info_cnt++]);
    s_num = s_num << 4;
    s_num = s_num | one_ascii_to_num(&com->rx_buff[rx_info_cnt++]);
    return(s_num);
}

uint8_t  two_ascii_to_u8_recount(comm_struct *com)
{
    unsigned char  s_num;
    if(com->rx_buff_cnt > 505)
        return 0;
    s_num = one_ascii_to_num(&com->rx_buff[com->rx_buff_cnt++]);
    s_num = s_num << 4;
    s_num = s_num | one_ascii_to_num(&com->rx_buff[com->rx_buff_cnt++]);
    return(s_num);
}
////////////**********************///////////////////
uint16_t  four_ascii_to_u16_recount(comm_struct *com)
{
    uint16_t  s_num;
    if(com->rx_buff_cnt > 505)
        return 0;
    s_num = one_ascii_to_num(&com->rx_buff[com->rx_buff_cnt++] );
    s_num = s_num << 4;
    s_num = s_num | one_ascii_to_num(&com->rx_buff[com->rx_buff_cnt++]);
    s_num = s_num << 4;
    s_num = s_num | one_ascii_to_num(&com->rx_buff[com->rx_buff_cnt++] );
    s_num = s_num << 4;
    s_num = s_num | one_ascii_to_num(&com->rx_buff[com->rx_buff_cnt++]);
    return(s_num);
}






/**
 * @Description:1363通讯协议 数据校验
 * @param  - .uint8_t* buffer, uint16_t length
 * @return uint16_t
 */
uint16_t protocol1363_get_checksum(uint8_t *buffer, uint16_t length) {
    uint32_t s_sum, i;
    s_sum = 0;
    for (i = 0; i < length; i++)
        s_sum += buffer[i];
    return (~(s_sum & 0x0ffff) + 1);
}

