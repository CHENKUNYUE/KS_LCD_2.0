/*
 * Copyright (c) Daren Hi-Tech Electronics Co., Ltd. Development Team 2020-2020.
 * All rights reserved. File name:   com_protocal1363_app.c Author: jychen   ID:
 * Version:    Date: 2021-01-14 Description: //
 * 用于详细说明此程序文件完成的主要功能，与其他模块
 *              // 或函数的接口，输出值、取值范围、含义及参数间的控
 *              // 制、顺序、独立或依赖等关系
 * Others:      // 其它内容的说明
 * History:     // 修改历史记录列表，每条修改记录应包括修改日期、修改
 *              // 者及修改内容简述
 *              Date           Author       Notes
 */

#include "protocal1363_app.h"

#include "paras.h"
#include "protocol1363_data.h"
#include "protocol1363_handler.h"
#include "protocol1363_transfer.h"
#include "protocol_1363_get_para.h"

protocol1363_init_paras_struct protocol1363_init_paras;
protocol_id_params const protocol_group_info[] = {
    {0x00, &protocol1363_Null} // 结束行-不动
};

/*************华丽的分界线******************************************************************************/

#if PROTOCOL_1363_RTN_ENABLE > 0
protocol_id_params const protocol_rtn_group_info[] = {
    {0x42, &protocol_parallel_S2M_get_sys_int_params}, // 获取模拟量量化后数据
                                                       // (定点数) 42H
    // {0x43, &protocol_app_get_chg_disch_status}, //获取开关输入状态 43H
    // {0x44, &protocol_app_get_warm_status},      //获取告警状态 44H
    // {0X45, &protocol_app_remote},               //遥控 45H
    // {0x47, &protocol_app_get_sys_params},       //获取系统参数 (定点数) 47H
    // {0x49, &protocol_app_set_sys_int_params},   //设定系统参数 (定点数) 49H
    // {0x4B, &protocol_app_get_history_data},     //获取历史数据 (定点数) 4BH
    // {0X4C, &protocol_app_get_history_warm},     //获取历史告警 4CH
    {0x4D, &protocol_parallel_S2M_get_time}, // 获取时间 4DH
    // {0X4E, &protocol_app_set_data_time},        //设定时间 4E
    // {0x4F, &protocol_app_get_protocol_ver},     //获取协议版本号 4F
    // {0x50, &protocol1363_Null},                 //获取sm设备地址
    //     {0x51, &protocol_app_get_sm_code},          //获取设备 (SM) 厂家信息
    {0x51, &protocol_app_S2M_get_sm_code}, // 获取设备 (SM) 厂家信息
    // {0x80, &protocol_app_get_basic_params_new}, //获取基本参数 (扩展命令) 80H
    // {0x81, &protocol_app_set_basic_params_new}, //设定基本参数 81H
    // {0x82, &protocol_app_set_calibration},      //校准82h
    {0x83, &protocol_parallel_S2M_get_warm_cnt}, // 获取告警次数 83H
    // {0x84, &protocol_app_get_slave_reals_paras},
    // {0x85, &protocol_app_set_slave_reals_paras},
    // {0x8A, &protocol_app_get_extd_paras},       //获取扩展参数 8AH
    // {0x8b, &protocol_app_set_extd_paras},       //设置拓展参数 8BH
    // {0x8C, &protocol_app_get_batt_id},          //获取电池 ID 8CH
    // {0x8D, &protocol_app_set_batt_id},          //设定电池 ID 8DH
    // {0x8E, &protocol_app_get_Server_domain},    //获取服务器域名 8EH
    // {0x8F, &protocol_app_set_Server_domain},    //设定服务器域名 8FH
    // {0xA0, &protocol_app_ext_module_operation}, //外设模块操作函数
    {0x00, &protocol1363_Null} // 结束行-不动
};

/**
 *  @description  slave to master
 */

/**
 * @Description: S2M 从机向主机返回数据 42H
 * @param  - .
 * @return
 */
extern uint8_t Uart_RecOK;
void protocol_parallel_S2M_get_sys_int_params(void *ptr) {
  sys_status_typedef s_sys_status = {0};

  uint8_t i;
  uint8_t s_command_group = 0;
  comm_struct *s_comm_struct;
  s_comm_struct = ptr;
  const PROTOCOL_S2M_VAR_TYPEDEF s_protocol1363_get_sys_status[] = {
      {&s_sys_status.SOC, enum_data_var_u16, 0},
      {&s_sys_status.TOT_Vs, enum_data_var_u16, 0},
      {&s_sys_status.num_of_cells, enum_data_var_u8, 0},
      {&s_sys_status.cell_list[0], enum_data_var_u16,
       &s_sys_status.num_of_cells}, // 4
      {&s_sys_status.ENV_TEMP, enum_data_var_u16, 0},
      {&s_sys_status.MOS_TEMP, enum_data_var_u16, 0},
      {&s_sys_status.PCBA_TEMP, enum_data_var_u16, 0},
      {&s_sys_status.TOT_TEMPs, enum_data_var_u8, 0}, // 8
      {&s_sys_status.TEMP_list[0], enum_data_var_u16,
       &s_sys_status.TOT_TEMPs}, //
      {&s_sys_status.pack_current, enum_data_var_u16, 0},
      {&s_sys_status.pack_inter_RES, enum_data_var_u16, 0},
      {&s_sys_status.SOH, enum_data_var_u16, 0}, // 12
      {&s_sys_status.user_costum1, enum_data_var_u8, 0},
      {&s_sys_status.full_CAP, enum_data_var_u16, 0},
      {&s_sys_status.remaining_CAP, enum_data_var_u16, 0},
      {&s_sys_status.Cycles, enum_data_var_u16, 0}, // 16
      {&s_sys_status.V_status_list.value, enum_data_var_u16, 0},
      {&s_sys_status.C_status_list.value, enum_data_var_u16, 0},
      {&s_sys_status.TEMP_status_list.value, enum_data_var_u16, 0},
      {&s_sys_status.warning_status_list.value, enum_data_var_u16, 0},
      {&s_sys_status.FET_status_list.value, enum_data_var_u16, 0},
      {&s_sys_status.OVV_PROT_status_list.value[0], enum_data_var_u16, 0},
      {&s_sys_status.UNDV_PROT_status_list.value[0], enum_data_var_u16, 0},
      {&s_sys_status.high_V_alarm_status_list.value[0], enum_data_var_u16, 0},
      {&s_sys_status.low_V_alarm_status_list.value[0], enum_data_var_u16, 0},
      {&s_sys_status.balance_status_status_list.value[0], enum_data_var_u16, 0},
      {&s_sys_status.balance_status_status_list.value[1], enum_data_var_u16, 0},
      {&s_sys_status.OVV_PROT_status_list.value[1], enum_data_var_u16, 0},
      {&s_sys_status.UNDV_PROT_status_list.value[1], enum_data_var_u16, 0},
      {&s_sys_status.high_V_alarm_status_list.value[1], enum_data_var_u16, 0},
      {&s_sys_status.low_V_alarm_status_list.value[1], enum_data_var_u16, 0},
      {&s_sys_status.machine_status_list.value, enum_data_var_u8, 0},
      {&s_sys_status.IO_status_list.value, enum_data_var_u16, 0},
      {0, enum_data_var_end, 0}, // 14
  };
  protocol_parallel_para_var_set(s_comm_struct, s_protocol1363_get_sys_status,
                                 17);
  protocol_1363_get_slave_para_42(&s_sys_status);
  Uart_RecOK = 1;
  return;
}

// 4DH
void protocol_parallel_S2M_get_time(void *ptr) {
  uint8_t i;
  uint8_t s_command_group = 0;
  data_time_typedef s_data_time = {0};
  comm_struct *ss_comm_struct;
  ss_comm_struct = ptr;

  protocol_var_typedef s_protocol1363_sys_data_time[7] = {
      {&s_data_time.year, enum_data_var_u16, 0},
      {&s_data_time.month, enum_data_var_u8, 0},
      {&s_data_time.day, enum_data_var_u8, 0}, // 4
      {&s_data_time.hour, enum_data_var_u8, 0},
      {&s_data_time.minute, enum_data_var_u8, 0},
      {&s_data_time.second, enum_data_var_u8, 0}, // 4
      {0, enum_data_var_end, 0},                  // 14
  };

  protocol1363_set_handler_cid2_new(ss_comm_struct,
                                    s_protocol1363_sys_data_time, 13);
  protocol_1363_S2M_send_data_time_4E(&s_data_time);
  Uart_RecOK = 1;

  return;
}
sm_info_typedef g_sm_info;
// 51版本号
void protocol_app_S2M_get_sm_code(void *ptr) {
  uint8_t i;
  uint8_t s_command_group = 0;
  uint8_t s_operation = 0;
  comm_struct *s_comm_struct;
  s_comm_struct = ptr;
  // sm_info_typedef s_sm_info;
  protocol_var_typedef s_protocol1363_sm_info[] = {
      {&g_sm_info.Hardware_type[0], enum_data_var_u8, 10},
      {&g_sm_info.Product_code[0], enum_data_var_u8, 10},
      {&g_sm_info.Project_code[0], enum_data_var_u8, 10},
      {&g_sm_info.Software_Ver[0], enum_data_var_u8, 3},
      {&g_sm_info.BOOT_Ver[0], enum_data_var_u8, 2},
      {0, enum_data_var_end, 0}, // 14
  };

  protocol1363_set_handler_cid2_new(s_comm_struct, s_protocol1363_sm_info, 2);
  protocol_1363_S2M_get_sm_code_51(&g_sm_info);
  Uart_RecOK = 1;

  //    protocol_1363_get_sm_code_51(&s_sm_info);
  //    protocol1363_send_frame_begin(s_comm_struct);
  //    protocol1363_send_handler_cid2_new(s_comm_struct,
  //    s_protocol1363_sm_info); protocol1363_send_frame_end(s_comm_struct);
}

/**
 * @Description:83H
 * @param  - .
 * @return  获取保护次数
 */
void protocol_parallel_S2M_get_warm_cnt(void *ptr) {
  uint8_t i;
  uint8_t s_command_group = 0;
  uint8_t s_operation = 0;
  comm_struct *s_comm_struct;
  s_comm_struct = ptr;
  warm_cnt_typedef s_warm_cnt;
  // 系统变量指针，变量指针列表，数组长度 83H
  protocol_var_typedef s_protocol1363_warm_cnt[] = {
      {&s_warm_cnt.cell_ov_times, enum_data_var_u16, 0},
      {&s_warm_cnt.cell_uv_times, enum_data_var_u16, 0},
      {&s_warm_cnt.chg_oc_times, enum_data_var_u16, 0},
      {&s_warm_cnt.chg_ot_times, enum_data_var_u16, 0},
      {&s_warm_cnt.sc_times, enum_data_var_u16, 0},
      {&s_warm_cnt.mos_ot_times, enum_data_var_u16, 0},
      {&s_warm_cnt.env_ot_times, enum_data_var_u16, 0},
      {&s_warm_cnt.env_ut_times, enum_data_var_u16, 0},
      {&s_warm_cnt.pack_ov_times, enum_data_var_u16, 0},
      {&s_warm_cnt.pack_uv_times, enum_data_var_u16, 0},
      {&s_warm_cnt.disch_oc_times, enum_data_var_u16, 0},
      {&s_warm_cnt.chg_ut_times, enum_data_var_u16, 0},
      {&s_warm_cnt.disch_ot_times, enum_data_var_u16, 0},
      {&s_warm_cnt.disch_ut_times, enum_data_var_u16, 0},
      {0, enum_data_var_end, 0},
  };
  protocol1363_set_handler_cid2_new(s_comm_struct, s_protocol1363_warm_cnt, 17);
  protocol_1363_S2M_send_warm_cnt_4d(&s_warm_cnt);
  Uart_RecOK = 1;
}

/**
 * @Description: 向从机获取其他参数
 * @param  - .
 * @return
 */
void protocol_parallel_M2S_set_slave_paras(void *ptr, uint8_t addr,
                                           uint8_t cmd_type, uint16_t value) {
  uint8_t s_command_group = 0;
  uint8_t s_command_type = 0;
  comm_struct *s_comm_struct;

  s_comm_struct = ptr;
  s_comm_struct->cid2 = 0x81;
  s_comm_struct->tx_buff[0] = PROTOCOL_1363_SOI;
  s_comm_struct->tx_buff_cnt = 1;
  // protocol1363_send_frame_begin(s_comm_struct);
  protocol1363_send_byte(s_comm_struct,
                         PROTOCOL_1363_VER); // com->ver_num); // ver
  protocol1363_send_byte(s_comm_struct, addr);
  protocol1363_send_byte(s_comm_struct, PROTOCOL_1363_CID1); // com->Cid1);
  protocol1363_send_byte(s_comm_struct, s_comm_struct->cid2);
  protocol1363_send_byte(s_comm_struct, 0);
  protocol1363_send_byte(s_comm_struct, 0);
  protocol1363_send_byte(s_comm_struct, 0xff);     // s_command_group
  protocol1363_send_byte(s_comm_struct, cmd_type); // type
  protocol1363_send_half_word(s_comm_struct, value);
  protocol1363_send_frame_end(s_comm_struct);
}

/**
 * @Description:
 * @param  - .
 * @return
 */
void protocol_parallel_M2S_get_basic_params(void *ptr, uint8_t addr) {
  uint8_t s_command_group = 0;
  uint8_t s_command_type = 0;
  comm_struct *s_comm_struct;

  s_comm_struct = ptr;
  s_comm_struct->cid2 = 0x80;
  s_comm_struct->tx_buff[0] = PROTOCOL_1363_SOI;
  s_comm_struct->tx_buff_cnt = 1;
  // protocol1363_send_frame_begin(s_comm_struct);
  protocol1363_send_byte(s_comm_struct,
                         PROTOCOL_1363_VER); // com->ver_num); // ver
  protocol1363_send_byte(s_comm_struct, addr);
  protocol1363_send_byte(s_comm_struct, PROTOCOL_1363_CID1); // com->Cid1);
  protocol1363_send_byte(s_comm_struct, s_comm_struct->cid2);
  protocol1363_send_byte(s_comm_struct, 0);
  protocol1363_send_byte(s_comm_struct, 0);
  protocol1363_send_byte(s_comm_struct, 0xff); // s_command_group
  protocol1363_send_frame_end(s_comm_struct);
}

// 版本号
void protocol_app_S2M_get_sm_code_(void *ptr, uint8_t addr) {
  uint8_t s_command_group = 0;
  uint8_t s_command_type = 0;
  comm_struct *s_comm_struct;
  s_comm_struct = ptr;
  s_comm_struct->cid2 = 0x51;
  s_comm_struct->tx_buff[0] = PROTOCOL_1363_SOI;
  s_comm_struct->tx_buff_cnt = 1;

  protocol1363_send_byte(s_comm_struct,
                         PROTOCOL_1363_VER); // com->ver_num); // ver
  protocol1363_send_byte(s_comm_struct, addr);
  protocol1363_send_byte(s_comm_struct, PROTOCOL_1363_CID1); // com->Cid1);
  protocol1363_send_byte(s_comm_struct, s_comm_struct->cid2);
  protocol1363_send_byte(s_comm_struct, 0);
  protocol1363_send_byte(s_comm_struct, 0);
  //    protocol1363_send_byte(s_comm_struct, 0xff); //s_command_group
  protocol1363_send_frame_end(s_comm_struct);
}

/**
 * @Description: 向从机获取基本参数
 * @param  - .
 * @return
 */
void protocol_parallel_M2S_get_sys_int_params(void *ptr, uint8_t addr) {
  uint8_t s_command_group = 0;
  uint8_t s_command_type = 0;
  comm_struct *s_comm_struct;

  s_comm_struct = ptr;
  s_comm_struct->cid2 = 0x42;
  s_comm_struct->tx_buff[0] = PROTOCOL_1363_SOI;
  s_comm_struct->tx_buff_cnt = 1;
  // protocol1363_send_frame_begin(s_comm_struct);
  protocol1363_send_byte(s_comm_struct,
                         PROTOCOL_1363_VER); // com->ver_num); // ver
  protocol1363_send_byte(s_comm_struct, addr);
  protocol1363_send_byte(s_comm_struct, PROTOCOL_1363_CID1); // com->Cid1);
  protocol1363_send_byte(s_comm_struct, s_comm_struct->cid2);
  protocol1363_send_byte(s_comm_struct, 0);
  protocol1363_send_byte(s_comm_struct, 0);
  protocol1363_send_byte(s_comm_struct, 0xff); // s_command_group
  protocol1363_send_frame_end(s_comm_struct);
}

/**
 * @Description:
 * @param  - .
 * @return
 */
void protocol_parallel_M2S_get_time(void *ptr, uint8_t addr) {
  uint8_t s_command_group = 0;
  uint8_t s_command_type = 0;
  comm_struct *s_comm_struct;

  s_comm_struct = ptr;
  s_comm_struct->cid2 = 0x4d;
  s_comm_struct->tx_buff[0] = PROTOCOL_1363_SOI;
  s_comm_struct->tx_buff_cnt = 1;
  // protocol1363_send_frame_begin(s_comm_struct);
  protocol1363_send_byte(s_comm_struct,
                         PROTOCOL_1363_VER); // com->ver_num); // ver
  protocol1363_send_byte(s_comm_struct, addr);
  protocol1363_send_byte(s_comm_struct, PROTOCOL_1363_CID1); // com->Cid1);
  protocol1363_send_byte(s_comm_struct, s_comm_struct->cid2);
  protocol1363_send_byte(s_comm_struct, 0);
  protocol1363_send_byte(s_comm_struct, 0);
  // protocol1363_send_byte(s_comm_struct, 0xff); //s_command_group
  protocol1363_send_frame_end(s_comm_struct);
}

/**
 * @Description:
 * @param  - .
 * @return
 */
void protocol_parallel_M2S_get_warm_cnt(void *ptr, uint8_t addr) {
  uint8_t s_command_group = 0;
  uint8_t s_command_type = 0;
  comm_struct *s_comm_struct;
  s_comm_struct = ptr;
  s_comm_struct->cid2 = 0x83;
  s_comm_struct->tx_buff[0] = PROTOCOL_1363_SOI;
  s_comm_struct->tx_buff_cnt = 1;
  // protocol1363_send_frame_begin(s_comm_struct);
  protocol1363_send_byte(s_comm_struct,
                         PROTOCOL_1363_VER); // com->ver_num); // ver
  protocol1363_send_byte(s_comm_struct, addr);
  protocol1363_send_byte(s_comm_struct, PROTOCOL_1363_CID1); // com->Cid1);
  protocol1363_send_byte(s_comm_struct, s_comm_struct->cid2);
  protocol1363_send_byte(s_comm_struct, 0);
  protocol1363_send_byte(s_comm_struct, 0);
  protocol1363_send_byte(s_comm_struct, 0xff); // s_command_group
  protocol1363_send_byte(s_comm_struct, 01);
  protocol1363_send_frame_end(s_comm_struct);
}
#endif
