/*
 * Copyright (c) Daren Hi-Tech Electronics Co., Ltd. Development Team 2020-2020. All rights reserved.
 * File name:  com_protocal_1363.h
 * Author:jychen     ID：      Version:V1.0      Date:2021年1月6日
 * Description:
 * Others:
 * History:
 *
 */
#ifndef HANDLER_COM_PROTOCAL_COM_PROTOCAL_1363_H_
#define     HANDLER_COM_PROTOCAL_COM_PROTOCAL_1363_H_

#include <stdint.h>
#include "paras.h"

#ifndef CELL_NUM
#define  CELL_NUM  20
#define  TEMP_NUM  6
#define  PACK_NUM  1
#define  BATT_TEMP_NUM  20
#endif

#define     VERSION_PROTOCOL                                0x22
#define     VERSION_LD                                      0x25
#define     COMM_BUFF_LEN                                   512
#define     COMM_TX_MAXLEN                                  512
#define     COMM_RX_MAXLEN                                  512

#define     USE_RCVD_TIMEOUT                                0

//#define     PROTOCOL_1363_SOI                               0x7E //ascii ~
#define     PROTOCOL_1363_SOI                               0x3E //ascii ~
#define     PROTOCOL_1363_EOI                               0x0D //ascii 回车
#define     PROTOCOL_1363_VER                               0x22
#define     PROTOCOL_1363_ADD                               0x01
#define     PROTOCOL_1363_CID1                              0x4A
#define     SOL1363                                         0

typedef enum {
  /*0*/com_ok = 0,
  /*1*/com_ver_err,
  /*2*/com_checksum_err,
  /*3*/com_lchecksum_err,
  /*4*/com_cid2_err,
  /*5*/com_err,
  /*6*/com_data_err,
  /*7*/com_run_err,
  /*8*/com_no_run,

} COM_RTN_ENUM_TYPEDEF;



typedef enum {
    get_obtain_quantized_analog_floating_data_id = 0,
    get_obtain_quantized_analog_integer_data_id,
    /*need to add all*/
} protocol1363_set_cid2;

typedef enum {
    set_obtain_quantized_analog_floating_data_id = 0,
    set_obtain_quantized_analog_integer_data_id,
    /*need to add all*/
} protocol1363_get_cid2;

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
//typedef unsigned int uint32_t;



typedef enum {
    enum_data_var_end = 0,
    enum_data_var_u8,
    enum_data_var_s8,
    enum_data_var_u16,
    enum_data_var_s16,
    enum_data_var_u32,
    enum_data_var_s32,
    enum_data_array_u8,
    enum_data_var_float,
} protocol_var_type_enum;

typedef struct {
    void *var_ptr;
    protocol_var_type_enum format;
    uint8_t array_lenth;
} protocol_var_typedef;

typedef struct {
 //   const protocol_var_typedef *params;
    uint8_t id;
    void (*app_func_ptr)(void *ptr);
} protocol_id_params;

typedef struct {
    uint8_t com_no;                 //串口号
    uint8_t rx_buff[COMM_BUFF_LEN]; //接收数组
    uint8_t tx_buff[COMM_BUFF_LEN]; //接收数组
    uint8_t is_rs485_yes;           //是否为485
    uint8_t rx_cmd;                 //存储接收CID2
    uint8_t rx_add;                 //存储接收addr
    uint16_t tx_buff_cnt;
    uint16_t rx_buff_cnt;
    uint16_t rx_buff_pos;
    uint16_t rx_num;
    uint16_t tx_num;
    uint8_t uart_data_ready;
    uint8_t copyTransmit;
    uint8_t com_return_code;
    uint32_t time_modbus_cnt;//超时计数
    uint8_t ver_num;    //软件版本号码
    uint8_t cid1;
    uint8_t cid2;

#if PROTOCOL_1363_RTN_ENABLE
    COM_STATE_ENUM_TYPEDEF  com_state;
    uint8_t  prepare_rcv;
    uint8_t  last_cid2;
    uint32_t rtn_time_cnt;
#endif

} comm_struct;


#if  PROTOCOL_1363_RTN_ENABLE>0
typedef struct {
    void *var_ptr;
    protocol_var_type_enum format;
    uint8_t *array_lenth;
} PROTOCOL_S2M_VAR_TYPEDEF;
#endif
extern comm_struct protocol_com;

uint8_t protocol1363_check_frame_lenth(comm_struct *com);

void protocol1363_send_frame_begin(comm_struct *com);

void protocol1363_send_byte(comm_struct *com, uint8_t sendda);

void protocol1363_send_half_word(comm_struct *com, uint16_t sendda);

void protocol1363_send_frame_end(comm_struct *com);

void protocl1363_send_byte_to_array(comm_struct *com, uint8_t data);

uint8_t protocol1363_get_frame_end(comm_struct *com);

void protocol1363_Null();

void protocol1363_return_frame(comm_struct *com);

#endif /* HANDLER_COM_PROTOCAL_COM_PROTOCAL_1363_H_ */


