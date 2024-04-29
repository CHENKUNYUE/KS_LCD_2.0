/*
 * Copyright (c) Daren Hi-Tech Electronics Co., Ltd. Development Team 2020-2020.
 * All rights reserved. File name:   protocol_1363_get_para.c Author: jyChen ID:
 * Version:V1.0    Date: 2021/3/22 Description: //
 * 用于详细说明此程序文件完成的主要功能，与其他模块
 *              // 或函数的接口，输出值、取值范围、含义及参数间的控
 *              // 制、顺序、独立或依赖等关系
 * Others:      // 其它内容的说明
 * History:     // 修改历史记录列表，每条修改记录应包括修改日期、修改
 *              // 者及修改内容简述
 *              Date           Author       Notes
 *              2021415         jyChen      修改满充容量和剩余容量的设置
 *
 */
#include "protocol_1363_get_para.h"

#include "Data_Init.h"
#include "paras.h"
#include "protocol1363_transfer.h"
#include "string.h"

#if FAULT_LOG_NEW_ENALBLE > 0
#include "bms_log.h"
#endif

#define CELL_V_CAL_IS_A_THOLD 3100
#define CELL_V_CAL_IS_B_THOLD 3500

#define CAL_CANCEL_VALUE 0
#define WITHOUT_ACTION 0x7FFF
#define CAL_WITHOUT_ACTION WITHOUT_ACTION

/**
 * @Description: 获取BMS的地址
 * @param  - .
 * @return
 */
void protocol_1363_init_paras(void) {
  protocol_1363_set_hw_paras(&protocol1363_init_paras);
}

/**
 * @Description:
 * @param  - .
 * @return
 */
void protocol_1363_get_hw_addr(uint8_t *addr) {
  *addr = PROTOCOL1363_LCD_ADDR; /*(uint8_t) bms.bms_addr; */
}

#if PROTOCOL_1363_RTN_ENABLE
/**
 * @Description:
 * @param  - .
 * @return
 */
uint32_t protocol_1363_get_sys_tick_ms(void) { /* return get_tick_ms();*/
}
#endif

/**
 * @Description:
 * @param  - .
 * @return
 */
void protocol_1363_set_hw_paras(
    protocol1363_init_paras_struct *protocol1363_init_paras) {
  protocol1363_init_paras->num_of_cell = 16; // bms.num_of_cells;
  protocol1363_init_paras->machine_addr =
      PROTOCOL1363_LCD_ADDR;              // bms.bms_addr;
  protocol1363_init_paras->is_master = 1; // get_is_master();
}
/**************************************************************************************************/

#if PROTOCOL_1363_RTN_ENABLE > 0

extern uint8_t Time_Buffer[12];
extern uint8_t BMS_HW_V[10];
extern uint8_t Uart_RecOK;
static uint8_t u8RecvState = 0;
/**
 * @Description:
 * @param  - .
 * @return
 */
uint8_t protocol_1363_S2M_send_data_time_4E(data_time_typedef *data_time_ptr) {
  uint8_t temp = 0;
  //	uint8_t temp_buf[6];
  uint16_t syear;
  uint8_t smon, sday, hour, min, sec;
  if ((data_time_ptr->year < 2100) && (data_time_ptr->month < 13) &&
      (data_time_ptr->day < 32) && (data_time_ptr->hour < 25) &&
      (data_time_ptr->minute < 60) && (data_time_ptr->second < 60)) {
    // suspend write protect
    syear = data_time_ptr->year;
    Time_Buffer[0] = (syear / 1000) + 0x30;
    Time_Buffer[1] = (syear / 100 % 10) + 0x30;
    Time_Buffer[2] = (syear / 10 % 10) + 0x30;
    Time_Buffer[3] = (syear % 10) + 0x30;

    smon = data_time_ptr->month;
    Time_Buffer[4] = (smon / 10) + 0x30;
    Time_Buffer[5] = (smon % 10) + 0x30;
    sday = data_time_ptr->day;
    Time_Buffer[6] = (sday / 10) + 0x30;
    Time_Buffer[7] = (sday % 10) + 0x30;

    hour = data_time_ptr->hour;
    Time_Buffer[8] = (hour / 10) + 0x30;
    Time_Buffer[9] = (hour % 10) + 0x30;
    min = data_time_ptr->minute;
    Time_Buffer[10] = (min / 10) + 0x30;
    Time_Buffer[11] = (min % 10) + 0x30;
    sec = data_time_ptr->second;

    u8RecvState |= 0x01;
    if (u8RecvState == 0x03) {
      u8RecvState = 0;
      Uart_RecOK = 1;
    }
    // Uart_RecOK = 1;
  } else {
    temp = 1;
  }
  return temp;
}

/**
 * @Description: protocol_1363_S2M_send_warm_cnt_4d
 * @param  - .
 * @return
 */
uint8_t protocol_1363_S2M_send_warm_cnt_4d(warm_cnt_typedef *s_warm_ptr) {

  Record.Over_Chg_Count = s_warm_ptr->cell_ov_times;    //过充次数
  Record.Over_Dchg_Count = s_warm_ptr->cell_uv_times; //过放次数
  Record.Over_Curr_Count = s_warm_ptr->chg_oc_times;       //过流次数
  Record.Over_Temp_Count = s_warm_ptr->chg_ot_times;   //电芯过温次数
  Record.Short_Count = s_warm_ptr->sc_times;           //短路次数
}

#define     BMS_PROJECT_CODE                                "16S200A   "
uint8_t protocol_1363_S2M_get_sm_code_51(sm_info_typedef *sm_info_ptr) {
    uint8_t i;
//    strncpy(PackData.Project_code_, BMS_PROJECT_CODE, 10);
//    PackData.Project_code_[0] = sm_info_ptr->Project_code;


//    for (i = 0; (i<=10); i++ ){
//        PackData.Project_code_[i] = sm_info_ptr->Project_code[i];
//    }
}

/**
 * @Description: 获取从机返回 参数转换 0x42
 * @param  - .
 * @return
 */
extern uint8_t getValidData;
extern uint8_t Dis_Charge_Flag;

void protocol_1363_get_slave_para_42(sys_status_typedef *sys_status_ptr) {
  uint8_t i = 0;
  uint16_t s_temp = 0;
  uint16_t full_CAP;
  uint16_t Cycles;
  uint16_t remaining_CAP;
  uint8_t TOT_TEMPs;
  uint8_t num_of_cells;
  uint16_t TOT_Vs;
  uint16_t Current_Data1 = 0, Current_Data2 = 0;
  uint16_t SOC;
//  uint16_t SOH;
  uint8_t pack_L_CAP_alarm;

  PackData.Vsum = sys_status_ptr->TOT_Vs * 10;              //总压
    if(PackData.Vsum==0){
        PackData.Vsum=0;
    }
  PackData.BAT_TEMP = sys_status_ptr->remaining_CAP;              //剩余容量

  PackData.Rm = sys_status_ptr->remaining_CAP;              //剩余容量
  PackData.Fcc = sys_status_ptr->full_CAP;                  //满充容量
  PackData.Cycle = sys_status_ptr->Cycles;                  //循环
  PackData.Current = (int16_t)sys_status_ptr->pack_current; //充电
  Soc = sys_status_ptr->SOC*0.01;                           // SOC
  SOH = sys_status_ptr->SOH;

  //告警状态：
  PackData.Alarm_State.BitName.bCellVoltOV =
      sys_status_ptr->V_status_list.bit.cell_OVV_alarm |
      sys_status_ptr->V_status_list.bit.TOT_OVV_alarm;
  PackData.Alarm_State.BitName.bCellVoltUL =
      sys_status_ptr->V_status_list.bit.cell_UNDV_alarm |
      sys_status_ptr->V_status_list.bit.TOT_UNDV_alarm;
  PackData.Alarm_State.BitName.bTempOV =
      sys_status_ptr->TEMP_status_list.bit.CHG_H_TEMP_alarm |
      sys_status_ptr->TEMP_status_list.bit.DISCH_H_TEMP_alarm |
      sys_status_ptr->TEMP_status_list.bit.ENV_H_TEMP_alarm |
      sys_status_ptr->TEMP_status_list.bit.power_H_TEMP_alarm;
  PackData.Alarm_State.BitName.bTempUL =
      sys_status_ptr->TEMP_status_list.bit.CHG_L_TEMP_alarm |
      sys_status_ptr->TEMP_status_list.bit.DISCH_L_TEMP_alarm |
      sys_status_ptr->TEMP_status_list.bit.ENV_L_TEMP_alarm |
      sys_status_ptr->TEMP_status_list.bit.power_L_TEMP_alarm;

  PackData.Alarm_State.BitName.bSocUL =
      sys_status_ptr->warning_status_list.bit.low_BAT_alarm;

  PackData.Alarm_State.BitName.bVDiffOV =
      sys_status_ptr->V_status_list.bit.V_DIF_alarm;
  PackData.Alarm_State.BitName.bCurrOV =
      sys_status_ptr->C_status_list.bit.DISCH_C_alarm |
      sys_status_ptr->C_status_list.bit.CHG_C_alarm;

  //保护
  PackData.Prp_State.BitName.bCellVoltOV =
      sys_status_ptr->V_status_list.bit.cell_OVV_PROT |
      sys_status_ptr->V_status_list.bit.TOT_OVV_PROT;
  PackData.Prp_State.BitName.bCellVoltUL =
      sys_status_ptr->V_status_list.bit.cell_UNDV_PROT |
      sys_status_ptr->V_status_list.bit.TOT_UNDV_PROT;

  PackData.Prp_State.BitName.bTempOV =
      sys_status_ptr->TEMP_status_list.bit.CHG_H_TEMP_PROT |
      sys_status_ptr->TEMP_status_list.bit.DISCH_H_TEMP_PROT |
      sys_status_ptr->TEMP_status_list.bit.ENV_H_TEMP_PROT |
      sys_status_ptr->TEMP_status_list.bit.power_H_TEMP_PROT;
  PackData.Prp_State.BitName.bTempUL =
      sys_status_ptr->TEMP_status_list.bit.CHG_L_TEMP_PROT |
      sys_status_ptr->TEMP_status_list.bit.DISCH_L_TEMP_PROT |
      sys_status_ptr->TEMP_status_list.bit.CHG_L_TEMP_PROT |
      sys_status_ptr->TEMP_status_list.bit.power_L_TEMP_PROT;

  PackData.Prp_State.BitName.bCurrOV =
      sys_status_ptr->C_status_list.bit.CHG_OC_PROT |
      sys_status_ptr->C_status_list.bit.DISCH_OC_1_PROT |
      sys_status_ptr->C_status_list.bit.DISCH_OC_2_PROT;
  PackData.Prp_State.BitName.bShortFault =
      sys_status_ptr->C_status_list.bit.Short_circuit_PROT;
  PackData.Prp_State.BitName.bNeg = 0;

  /*故障*/
  PackData.Prp_State.BitName.bSmpErr =
      sys_status_ptr->warning_status_list.bit.cell_fail;
  PackData.Prp_State.BitName.bChgMosErr =
      sys_status_ptr->warning_status_list.bit.CHG_MOS_fail;     //充电MOS故障
  PackData.Prp_State.BitName.bDisMosErr =
      sys_status_ptr->warning_status_list.bit.DISCH_MOS_fail;   //放电MOS故障
  PackData.Prp_State.BitName.bChipSmpErr =
      sys_status_ptr->FET_status_list.bit.AFE_chip_fail;        //AFE故障，单节电压超（3.9V）搞或超低

  if (sys_status_ptr->C_status_list.bit.charging) {
    Dis_Charge_Flag = 1;
  } else if (sys_status_ptr->C_status_list.bit.discharging) {
    Dis_Charge_Flag = 2;
  } else {
    Dis_Charge_Flag = 0;
  }

  PackData.BatNum = sys_status_ptr->num_of_cells;//单体节数
  for (i = 0; (i < sys_status_ptr->num_of_cells) && (i < 16); i++) //单体电压
  {
    PackData.Vol[i] = sys_status_ptr->cell_list[i];
  }

//  PackData.TempNum = sys_status_ptr->TOT_TEMPs;//赋值温度实际个数
  for (i = 0; (i < sys_status_ptr->TOT_TEMPs) && (i < 6); i++) //温度
  {
    PackData.Temp[i] = sys_status_ptr->TEMP_list[i];
  }

  PackData.BAT_TEMP = sys_status_ptr->MOS_TEMP;  //平均温度


  PackData.s16MosTemp = sys_status_ptr->PCBA_TEMP;  //MOS温度
  PackData.s16EnvirTemp = sys_status_ptr->ENV_TEMP;

  getValidData = 1;

  u8RecvState |= 0x02;
  if (u8RecvState == 0x03) {
    u8RecvState = 0;
    Uart_RecOK = 1;
  }
}

#endif
