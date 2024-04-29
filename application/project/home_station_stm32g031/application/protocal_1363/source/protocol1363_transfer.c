/*
 * Copyright (c) Daren Hi-Tech Electronics Co., Ltd. Development Team 2020-2020. All rights reserved.
 * File name:   protocol1363_transfer.c
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
#include "protocol1363_transfer.h"
#if PROTOCOL_1363_RTN_ENABLE
#include <protocol_1363_get_para.h>
#endif
#include "protocol1363_data.h"
#include "protocol1363_hw.h"

/*
 * __________________________________________________________
 *序号    1    2   3   4   5   6       7              8     9
 *       0    1   3   5   7   9       11         LENDID+11 LENID+15
 *字节数  1    1   1   1   1   2      LENID/2         2     1
 * 格式 SOI VER ADR CID1 CID2 LENTH COMMAND INFO  CECKSUM EOI
 * ----------------------------------------------------------
 */

uint8_t protocol1363_get_frame_end(comm_struct *com);

/**
 * @Description:1363通讯协议 发送帧头
 * @param  - .com_struct *com
 * @return
 */
void protocol1363_send_frame_begin(comm_struct *com) {
    com->tx_buff[0] = PROTOCOL_1363_SOI;
    com->tx_buff_cnt = 1;

    //if (com->RvCmd == 0xC1 || com->RvCmd == 0xC2 )
    {
        protocol1363_send_byte(com, PROTOCOL_1363_VER);  //com->ver_num); // ver
        protocol1363_send_byte(com, PROTOCOL_1363_ADD);
        protocol1363_send_byte(com, PROTOCOL_1363_CID1);  //com->Cid1);
        //protocol1363_send_byte(com, com->Cid2);
        protocol1363_send_byte(com, com->com_return_code);//RTN
        protocol1363_send_byte(com, 0);
        protocol1363_send_byte(com, 0);
    }
}

/**
 * @Description:1363通讯协议 发送数据 发送u8
 * @param  - com_struct *com, uint8_t sendda
 * @return void
 */
void protocol1363_send_byte(comm_struct *com, uint8_t sendda) {
    uint16_t bf;
    bf = u8_hex_to_two_ascii(sendda);

    protocl1363_send_byte_to_array(com, (bf >> 8) & 0xff);
    protocl1363_send_byte_to_array(com, (bf) & 0xff);
}

/**
 * @Description:1363通讯协议 发送数据 发送u16
 * @param  - com_struct *com, uint8_t sendda
 * @return void
 */
void protocol1363_send_half_word(comm_struct *com, uint16_t sendda) {
   // uint16_t bf;
   // bf = u8_hex_to_two_ascii(sendda);
    protocol1363_send_byte(com,(sendda>>8)&0xff);
    protocol1363_send_byte(com,(sendda   )&0xff);
}

/**
 * @Description:protocal1363 帧数处理，将校验位补足，同时发送到硬件，输出帧数据
 * @param  - .com_struct *com
 * @return
 */
void protocol1363_send_frame_end(comm_struct *com) {
    //uint8_t  recvaddress;
    uint32_t send_frame_len;
    uint32_t send_frame_len_ascii;

    uint8_t i;

    // frame len
    send_frame_len = com->tx_buff_cnt - 13;
    i = ((send_frame_len >> 8) & 0x0f);
    i += ((send_frame_len >> 4) & 0x0f);
    i += ((send_frame_len >> 0) & 0x0f);
    i = i & 0x0f;
    i = ((~i) + 1) & 0x0f;
    send_frame_len = (send_frame_len & 0x0FFF) | (i << 12);

    //send length
    send_frame_len_ascii = u16_hex_to_four_ascii(send_frame_len);
    com->tx_buff[9] = (send_frame_len_ascii >> 24) & 0xff;
    com->tx_buff[10] = (send_frame_len_ascii >> 16) & 0xff;
    com->tx_buff[11] = (send_frame_len_ascii >> 8) & 0xff;
    com->tx_buff[12] = (uint8_t)(send_frame_len_ascii & 0xff);
    // checksum
    protocol1363_send_half_word(com, protocol1363_get_checksum(com->tx_buff + 1, (com->tx_buff_cnt) - 1));
    com->tx_buff[com->tx_buff_cnt++] = PROTOCOL_1363_EOI;
    //==================
    com->tx_num = com->tx_buff_cnt;
    // send frame
    if (com->com_no < 5){
#if PROTOCOL_1363_RTN_ENABLE
        com->com_state = com_in_transmit;
#endif
        protocol_hw_send_one_frame(com, com->tx_buff, com->tx_num);
        
#if PROTOCOL_1363_RTN_ENABLE
        com->com_state = com_wait_return;
        com->last_cid2 = com->cid2;
        com->rtn_time_cnt = protocol_1363_get_sys_tick_ms() ;
#endif
    }
    com->tx_buff_cnt = 0;
    com->tx_num = 0;
}

/**
 * @Description: 将1363 command info发送到字符串数组缓存去
 * @param  - .  com_struct *com, uint8_t data
 * @return
 */
void protocl1363_send_byte_to_array(comm_struct *com, uint8_t data) {
    if (com->tx_buff_cnt < COMM_TX_MAXLEN - 1)
        com->tx_buff[com->tx_buff_cnt++] = data;
}

/**
 * @Description:检查帧长度
 * @param  - .
 * @return
 */
uint8_t protocol1363_check_frame_lenth(comm_struct *com) {
    uint8_t s_cid1;
    uint8_t s_cid2;
    uint16_t s_lenth;
    uint8_t s_lencheck1;
    uint8_t s_lencheck2;
    uint16_t s_checksum;
    uint16_t s_checksum2;
    uint8_t s_version;

    // version
    s_version = two_ascii_to_u8((com->rx_buff + 1));
    com->ver_num = s_version;

    if (!(s_version == VERSION_PROTOCOL || s_version == VERSION_LD))
        return com_ver_err;

    // checksum
    if (com->rx_num < 18)
        return com_err;

    s_checksum = four_ascii_to_u16((com->rx_buff) + com->rx_num - 5);
    s_checksum2 = protocol1363_get_checksum((com->rx_buff) + 1, com->rx_num - 6);

    if (s_checksum != s_checksum2)
        return com_checksum_err;

    //lchecksum
    s_lenth = four_ascii_to_u16((com->rx_buff) + 9);
    s_lencheck1 = (((s_lenth & 0x0f00) >> 8) + ((s_lenth & 0x0f0) >> 4) + ((s_lenth & 0x0f) >> 0)) & 0x0f;
    s_lencheck2 = ((s_lenth & 0x0f000) >> 12) & 0x0f;

    if ((((~s_lencheck1) + 1) & 0x0f) != s_lencheck2)
        return com_lchecksum_err;

    s_cid1 = two_ascii_to_u8((com->rx_buff) + 5);
    com->cid1 = s_cid1;

//    if (!(s_cid1 == 0x4a || s_cid1 == 0x46))
//        return com_err;//主板协议回复不在判断cid1

    s_cid2 = two_ascii_to_u8((com->rx_buff) + 7);
    com->cid2 = s_cid2;
    com->rx_cmd = s_cid2;

    if ((com->rx_num) - 18 != (s_lenth & 0x0fff))
        return com_err;

    //====================
    return com_ok;
}

/**
 * @Description: 接收帧尾工作，包括计算长度，和帧的基础内容
 * @param  - .   com_struct *com
 * @return       uint8_t
 */
uint8_t protocol1363_get_frame_end(comm_struct *com) {
    uint8_t s_ret;

    s_ret = com_err;

    if (com->rx_buff[0] == PROTOCOL_1363_SOI)
    {

        if (com->rx_buff_pos < 18) {
            com->com_return_code = com_data_err;
            return com_data_err;
        }

        com->rx_num = com->rx_buff_pos;
        com->ver_num = two_ascii_to_u8((com->rx_buff + 1));
        com->rx_add = two_ascii_to_u8((com->rx_buff + 3));
        com->cid1 = two_ascii_to_u8((com->rx_buff + 5));
        com->cid2 = two_ascii_to_u8((com->rx_buff + 7));
        com->rx_cmd = com->cid2;
        com->com_return_code = protocol1363_check_frame_lenth(com);

        if (com->com_return_code != com_ok)  // Error
        {
            s_ret = com_err;
            return s_ret;
        }

        if ((com->com_no <= 4) ||
            //(com->comNo > 2 && reals.address == com->RvAddr) ||
            //(com->comNo > 2 && com->RvCmd == MASTERSLAVECMD84 ) ||
            //(com->comNo > 2 && com->RvCmd == MASTERSLAVECMD85 ) ||
            (0)) {
            s_ret = com_ok;
        }

    }
/*    else if (com->rx_buff[0] == 1)
    {

        if (com->rx_buff_pos >= 4) {
            s_ret = com_ok;
        }

    }else
    {
    }*/
    com->rx_num = com->rx_buff_pos;
    com->rx_buff_pos = 0;
//    com->uart_data_ready = 1;
    return s_ret;
}

/**
 * @Description:
 * @param  - .
 * @return
 */

void protocol1363_return_frame(comm_struct *com)
{
    protocol1363_send_frame_begin(com);
    protocol1363_send_frame_end(com);
    com->com_return_code = com_ok;
}
