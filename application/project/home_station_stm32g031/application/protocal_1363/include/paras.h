/*
 * Copyright (c) Daren Hi-Tech Electronics Co., Ltd. Development Team 2020-2020. All rights reserved.
 * File name:   paras.h
 * Author: cq   ID:     Version:    Date: 2021-01-09
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
#ifndef COM_PROTOCAL_1363_PARAS_H__
#define COM_PROTOCAL_1363_PARAS_H__

#include <stdint.h>

#ifndef  CELL_NUM
#define  CELL_NUM  20
#define  TEMP_NUM  6
#define  PACK_NUM  1
#define  BATT_TEMP_NUM  20
#endif

#define PROTOCOL1363_LCD_ADDR 0xFF
#define USE_PROTOCOL1363_SNMP 1
#define USE_PROTOCOL1363_TIETA 0
#define USE_PROTOCOL1363_HUANDIAN 1
#define PROTOCOL_1363_RTN_ENABLE 1

#if PROTOCOL_1363_RTN_ENABLE > 0
    #define COM_TIME_OUT_MS 1500
#endif

typedef enum {
    limit_CHG_10A = 0,
    limit_CHG_20A,
    limit_CHG_25A,
    limit_CHG_off,

} limit_CHG_OPT_typedef;

typedef enum {
    remote_none = 0,
    remote_enter_sleep,
    remote_enter_reset,

} sleep_OPT;

typedef enum {
    com_idle,
    com_in_transmit,
    com_in_receive,
    com_wait_return,
    com_return_time_out,

} COM_STATE_ENUM_TYPEDEF;

//系统参数

typedef struct {
    uint16_t Cell_Vol_upper_limit;
    uint16_t Cell_Vol_lower_limit;
    uint16_t TEMP_upper_limit;
    uint16_t TEMP_lower_limit;
    uint16_t CHG_C_upper_limit;
    uint16_t TOT_Vol_upper_limit;
    uint16_t TOT_Vol_lower_limit;
    uint8_t user_costum0;
    uint16_t Num_of_cells;
    uint16_t CHG_C_limit;
    uint16_t Design_CAP;
    uint16_t Historical_data_storage_interval;
    uint16_t Balanced_mode;
    uint16_t Product_barcode[10];
    uint16_t BMS_barcode[10];
    union {
        struct {
            uint8_t warm_flag : 1;
            uint8_t rsvd0 : 3;
            uint8_t sw_flag : 1;
            uint8_t rsvd1 : 3;
        } bit;
        uint8_t value;
    } data_flag;
    uint8_t total_pack;
} sys_paras_typedef; //66

typedef union {
    struct {
        uint16_t bit0 : 1;
        uint16_t bit1 : 1;
        uint16_t bit2 : 1;
        uint16_t bit3 : 1;
        uint16_t bit4 : 1;
        uint16_t bit5 : 1;
        uint16_t bit6 : 1;
        uint16_t bit7 : 1;
        uint16_t bit8 : 1;
        uint16_t bit9 : 1;
        uint16_t bit10 : 1;
        uint16_t bit11 : 1;
        uint16_t bit12 : 1;
        uint16_t bit13 : 1;
        uint16_t bit14 : 1;
        uint16_t bit15 : 1;
    } bit;
    uint16_t value;
} pack_status_typedef;

//42H
typedef struct {
    uint16_t SOC;
    uint16_t TOT_Vs;
    uint8_t num_of_cells;//单节电压节数
    uint16_t cell_list[CELL_NUM];
    uint16_t ENV_TEMP;
    uint16_t MOS_TEMP;//电池平均温度
    uint16_t PCBA_TEMP;//MOS温度
    uint8_t TOT_TEMPs;
    uint16_t TEMP_list[TEMP_NUM];
    uint16_t pack_current;
    uint16_t pack_inter_RES;
    uint16_t SOH;
    uint8_t user_costum1;
    uint16_t full_CAP;
    uint16_t remaining_CAP;
    uint16_t Cycles;
    union {
        struct {
            uint16_t cell_OVV_PROT : 1;
            uint16_t cell_UNDV_PROT : 1;
            uint16_t TOT_OVV_PROT : 1;
            uint16_t TOT_UNDV_PROT : 1;
            uint16_t cell_OVV_alarm : 1;
            uint16_t cell_UNDV_alarm : 1;
            uint16_t TOT_OVV_alarm : 1;
            uint16_t TOT_UNDV_alarm : 1;
            uint16_t V_DIF_alarm : 1;
            uint16_t OVV_PROT_10 : 1;
            uint16_t UNDV_PROT_10 : 1;
            uint16_t TEMP_DIF_alarm : 1;
            uint16_t cell_fail : 1;
            uint16_t blown_fuse : 1;
            uint16_t cell_V_diff_H_PROT : 1;
            uint16_t SYS_sleep : 1;
        } bit;
        uint16_t value;
    } V_status_list;
    union {
        struct {
            uint16_t charging : 1;
            uint16_t discharging : 1;
            uint16_t CHG_OC_PROT : 1;
            uint16_t Short_circuit_PROT : 1;
            uint16_t DISCH_OC_1_PROT : 1;
            uint16_t DISCH_OC_2_PROT : 1;
            uint16_t CHG_C_alarm : 1;
            uint16_t DISCH_C_alarm : 1;
            uint16_t OC_10 : 1;
            uint16_t RVS_connnection : 1;
            uint16_t C_multiplier : 1;
            uint16_t C_limit : 1;
            uint16_t bat_status : 3;
            uint16_t no_load_status : 1;
        } bit;
        uint16_t value;
    } C_status_list;
    union {
        struct {
            uint16_t CHG_H_TEMP_PROT : 1;
            uint16_t CHG_L_TEMP_PROT : 1;
            uint16_t DISCH_H_TEMP_PROT : 1;
            uint16_t DISCH_L_TEMP_PROT : 1;
            uint16_t ENV_H_TEMP_PROT : 1;
            uint16_t ENV_L_TEMP_PROT : 1;
            uint16_t power_H_TEMP_PROT : 1;
            uint16_t power_L_TEMP_PROT : 1;
            uint16_t CHG_H_TEMP_alarm : 1;
            uint16_t CHG_L_TEMP_alarm : 1;
            uint16_t DISCH_H_TEMP_alarm : 1;
            uint16_t DISCH_L_TEMP_alarm : 1;
            uint16_t ENV_H_TEMP_alarm : 1;
            uint16_t ENV_L_TEMP_alarm : 1;
            uint16_t power_H_TEMP_alarm : 1;
            uint16_t power_L_TEMP_alarm : 1;
        } bit;
        uint16_t value;
    } TEMP_status_list;
    union {
        struct {
            uint16_t CHG_FET_status : 1;
            uint16_t DISCH_FET_status : 1;
            uint16_t CHG_FET_fail : 1;
            uint16_t DISCH_FET_fail : 1;
            uint16_t limit_C : 2;
            uint16_t heating_film : 1;
            uint16_t constant_C_MOS_status : 1;
            uint16_t close_4G : 1;
            uint16_t CHG_signal : 1;
            uint16_t pack_as_power : 1;
            uint16_t LED_alarm_status : 1;
            uint16_t buzzer_status : 1;
            uint16_t AFE_chip_fail : 1;
            uint16_t AFE_alarm_pin_fail : 1;
            uint16_t low_BAT_PROT : 1;
        } bit;
        uint16_t value;
    } FET_status_list;
    union {
        struct {
            uint16_t V_DIF_alarm : 1;
            uint16_t CHG_FET_fail_alarm : 1;
            uint16_t extern_SD_fail_alarm : 1;
            uint16_t SPI_fail_alarm : 1;
            uint16_t E2P_fail_alarm : 1;
            uint16_t LED_alarm : 1;
            uint16_t Buzzer_alarm : 1;
            uint16_t low_BAT_alarm : 1;
            uint16_t MOS_H_TEMP_PROT : 1;
            uint16_t heating_film_fail : 1;
            uint16_t limit_board_fail : 1;
            uint16_t sampling_fail : 1;
            uint16_t cell_fail : 1;
            uint16_t NTC_fail : 1;
            uint16_t CHG_MOS_fail : 1;
            uint16_t DISCH_MOS_fail : 1;
        } bit;
        uint16_t value;
    } warning_status_list;
    union {
        struct {
            uint16_t bit0 : 1;
            uint16_t bit1 : 1;
            uint16_t bit2 : 1;
            uint16_t bit3 : 1;
            uint16_t bit4 : 1;
            uint16_t bit5 : 1;
            uint16_t bit6 : 1;
            uint16_t bit7 : 1;
            uint16_t bit8 : 1;
            uint16_t bit9 : 1;
            uint16_t bit10 : 1;
            uint16_t bit11 : 1;
            uint16_t bit12 : 1;
            uint16_t bit13 : 1;
            uint16_t bit14 : 1;
            uint16_t bit15 : 1;
            uint32_t bit16 : 1;
            uint32_t bit17 : 1;
            uint32_t bit18 : 1;
            uint32_t bit19 : 1;
            uint32_t bit20 : 1;
            uint32_t bit21 : 1;
            uint32_t bit22 : 1;
            uint32_t bit23 : 1;
            uint32_t bit24 : 1;
            uint32_t bit25 : 1;
            uint32_t bit26 : 1;
            uint32_t bit27 : 1;
            uint32_t bit28 : 1;
            uint32_t bit29 : 1;
            uint32_t bit30 : 1;
            uint32_t bit31 : 1;
        } bit;
        uint16_t value[2];
    } OVV_PROT_status_list;
    union {
        struct {
            uint16_t bit0 : 1;
            uint16_t bit1 : 1;
            uint16_t bit2 : 1;
            uint16_t bit3 : 1;
            uint16_t bit4 : 1;
            uint16_t bit5 : 1;
            uint16_t bit6 : 1;
            uint16_t bit7 : 1;
            uint16_t bit8 : 1;
            uint16_t bit9 : 1;
            uint16_t bit10 : 1;
            uint16_t bit11 : 1;
            uint16_t bit12 : 1;
            uint16_t bit13 : 1;
            uint16_t bit14 : 1;
            uint16_t bit15 : 1;
            uint32_t bit16 : 1;
            uint32_t bit17 : 1;
            uint32_t bit18 : 1;
            uint32_t bit19 : 1;
            uint32_t bit20 : 1;
            uint32_t bit21 : 1;
            uint32_t bit22 : 1;
            uint32_t bit23 : 1;
            uint32_t bit24 : 1;
            uint32_t bit25 : 1;
            uint32_t bit26 : 1;
            uint32_t bit27 : 1;
            uint32_t bit28 : 1;
            uint32_t bit29 : 1;
            uint32_t bit30 : 1;
            uint32_t bit31 : 1;
        } bit;
        uint16_t value[2];
    } UNDV_PROT_status_list;
    union {
        struct {
            uint16_t bit0 : 1;
            uint16_t bit1 : 1;
            uint16_t bit2 : 1;
            uint16_t bit3 : 1;
            uint16_t bit4 : 1;
            uint16_t bit5 : 1;
            uint16_t bit6 : 1;
            uint16_t bit7 : 1;
            uint16_t bit8 : 1;
            uint16_t bit9 : 1;
            uint16_t bit10 : 1;
            uint16_t bit11 : 1;
            uint16_t bit12 : 1;
            uint16_t bit13 : 1;
            uint16_t bit14 : 1;
            uint16_t bit15 : 1;
            uint32_t bit16 : 1;
            uint32_t bit17 : 1;
            uint32_t bit18 : 1;
            uint32_t bit19 : 1;
            uint32_t bit20 : 1;
            uint32_t bit21 : 1;
            uint32_t bit22 : 1;
            uint32_t bit23 : 1;
            uint32_t bit24 : 1;
            uint32_t bit25 : 1;
            uint32_t bit26 : 1;
            uint32_t bit27 : 1;
            uint32_t bit28 : 1;
            uint32_t bit29 : 1;
            uint32_t bit30 : 1;
            uint32_t bit31 : 1;
        } bit;
        uint16_t value[2];
    } high_V_alarm_status_list;
    union {
        struct {
            uint16_t bit0 : 1;
            uint16_t bit1 : 1;
            uint16_t bit2 : 1;
            uint16_t bit3 : 1;
            uint16_t bit4 : 1;
            uint16_t bit5 : 1;
            uint16_t bit6 : 1;
            uint16_t bit7 : 1;
            uint16_t bit8 : 1;
            uint16_t bit9 : 1;
            uint16_t bit10 : 1;
            uint16_t bit11 : 1;
            uint16_t bit12 : 1;
            uint16_t bit13 : 1;
            uint16_t bit14 : 1;
            uint16_t bit15 : 1;
            uint32_t bit16 : 1;
            uint32_t bit17 : 1;
            uint32_t bit18 : 1;
            uint32_t bit19 : 1;
            uint32_t bit20 : 1;
            uint32_t bit21 : 1;
            uint32_t bit22 : 1;
            uint32_t bit23 : 1;
            uint32_t bit24 : 1;
            uint32_t bit25 : 1;
            uint32_t bit26 : 1;
            uint32_t bit27 : 1;
            uint32_t bit28 : 1;
            uint32_t bit29 : 1;
            uint32_t bit30 : 1;
            uint32_t bit31 : 1;
        } bit;
        uint16_t value[2];
    } low_V_alarm_status_list;
    union {
        struct {
            uint32_t bit0 : 1;
            uint32_t bit1 : 1;
            uint32_t bit2 : 1;
            uint32_t bit3 : 1;
            uint32_t bit4 : 1;
            uint32_t bit5 : 1;
            uint32_t bit6 : 1;
            uint32_t bit7 : 1;
            uint32_t bit8 : 1;
            uint32_t bit9 : 1;
            uint32_t bit10 : 1;
            uint32_t bit11 : 1;
            uint32_t bit12 : 1;
            uint32_t bit13 : 1;
            uint32_t bit14 : 1;
            uint32_t bit15 : 1;
            uint32_t bit16 : 1;
            uint32_t bit17 : 1;
            uint32_t bit18 : 1;
            uint32_t bit19 : 1;
            uint32_t bit20 : 1;
            uint32_t bit21 : 1;
            uint32_t bit22 : 1;
            uint32_t bit23 : 1;
            uint32_t bit24 : 1;
            uint32_t bit25 : 1;
            uint32_t bit26 : 1;
            uint32_t bit27 : 1;
            uint32_t bit28 : 1;
            uint32_t bit29 : 1;
            uint32_t bit30 : 1;
            uint32_t bit31 : 1;
        } bit;
        uint16_t value[2];
    } balance_status_status_list;
    union {
        struct {
            uint8_t initialization : 1;
            uint8_t self_check : 1;
            uint8_t ready : 1;
            uint8_t DISCH : 1;
            uint8_t CHG : 1;
            uint8_t fail : 1;
            uint8_t power_off : 1;
            uint8_t factory_mode : 1;
        } bit;
        uint8_t value;
    } machine_status_list;
    union {
        struct {
            uint16_t charger_online : 1;
            uint16_t ACC_signal : 1;
            uint16_t ON_signal : 1;
            uint16_t aerosol_detection : 1;
            uint16_t rsvd0 : 4;
            uint16_t pre_DISCH_MOS : 1;
            uint16_t rsvd1 : 4;
            uint16_t crystal_oscillator_fail : 1;
            uint16_t EEP_fail : 1;
            uint16_t rsvd2 : 1;
        } bit;
        uint16_t value;
    } IO_status_list;
    union {
        struct {
            uint8_t warm_flag : 1;
            uint8_t rsvd0 : 3;
            uint8_t sw_flag : 1;
            uint8_t rsvd1 : 3;
        } bit;
        uint8_t value;
    } data_flag;

    uint8_t No_CHG_DISCH_status_list[PACK_NUM];
    uint8_t total_pack;
} sys_status_typedef; //ID:0X43 LENGTH:112

typedef union {
    struct {
        uint16_t V : 1;
        uint16_t C : 1;
        uint16_t TEMP : 1;
        uint16_t PROT : 1;
        uint16_t alarm : 1;
        uint16_t fail : 1;
        uint16_t CHG : 1;
        uint16_t DISCH : 1;
        uint16_t Standby : 1;
        uint16_t note : 4;
        uint16_t rsvd0 : 2;
    } bit;
    uint16_t value;
} data_type_typedef; //2
//系统告警状态
typedef struct {
    uint8_t pack_OVCHG_alarm;
    uint8_t pack_OVC_alarm;
    uint8_t pack_DISCH_UNDV_alarm;
    uint8_t pack_DISCH_OC_alarm;
    uint8_t pack_polarity_reverse_alarm;
    uint8_t ENV_H_TEMP_alarm;		//环境高温告警
    uint8_t ENV_L_TEMP_alarm;
    uint8_t pack_H_TEMP_alarm; //8
    uint8_t PCBA_H_TEMP_alarm;
    uint8_t pack_L_CAP_alarm;
    uint8_t pack_TEMP_sensor_fail_alarm;
    uint8_t pack_V_sensor_fail_alarm;
    uint8_t pack_C_sensor_fail_alarm;
    uint8_t pack_fail_alarm; //6
    uint8_t monitor_cells_num;
    uint8_t cell_No_CHG_OV_alarm_list[CELL_NUM];
    uint8_t cell_No_DISCH_UNDV_alarm_list[CELL_NUM];
    uint8_t cell_No_fail_alarm_list[CELL_NUM];
    uint8_t monitor_TEMPs_num;
    uint8_t cell_H_TEMP_alarm_list[BATT_TEMP_NUM];
    uint8_t cell_No_TEMP_sensor_fail_alarm_lis[BATT_TEMP_NUM];
    union {
        struct {
            uint8_t warm_flag : 1;
            uint8_t rsvd0 : 3;
            uint8_t sw_flag : 1;
            uint8_t rsvd1 : 3;
        } bit;
        uint8_t value;
    } data_flag;
    uint8_t total_pack;
} warm_status_typedef; //ID:0x44
//时间格式
typedef struct {
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
} data_time_typedef; //6
//43
typedef struct {
    uint8_t total_pack;
    uint8_t No_CHG_DISCH_status_list[PACK_NUM];
} single_pack_typedef; //2
//45
typedef struct {
    uint8_t CHG_MOS_OPT;
    uint8_t DISCH_MOS_OPT;
    uint8_t buzzer_OPT;
    uint8_t AI_Intermittent_CHG_OPT;
    uint8_t limit_CHG_OPT;
    uint8_t heating_OPT;
    uint8_t alarm_LED_OPT;
    uint8_t sleep_OPT;
    uint8_t clear_history_recording;
    uint8_t restore_default_basic_param;
    uint8_t restore_default_adjust_param;
    uint8_t clear_alarm_cnt;
    uint8_t restore_Factory;
    uint8_t fun_OPT;
    uint8_t signal_point_1;
    uint8_t signal_point_2;
    uint8_t factory_sleep;
    uint8_t factory_mode;
    uint8_t pre_DISCH_MOS_OPT;
    uint8_t heating_control_OPT;
    union {
        struct {
            uint32_t bit0 : 1;
            uint32_t bit1 : 1;
            uint32_t bit2 : 1;
            uint32_t bit3 : 1;
            uint32_t bit4 : 1;
            uint32_t bit5 : 1;
            uint32_t bit6 : 1;
            uint32_t bit7 : 1;
            uint32_t bit8 : 1;
            uint32_t bit9 : 1;
            uint32_t bit10 : 1;
            uint32_t bit11 : 1;
            uint32_t bit12 : 1;
            uint32_t bit13 : 1;
            uint32_t bit14 : 1;
            uint32_t bit15 : 1;
            uint32_t bit16 : 1;
            uint32_t bit17 : 1;
            uint32_t bit18 : 1;
            uint32_t bit19 : 1;
            uint32_t bit20 : 1;
            uint32_t bit21 : 1;
            uint32_t bit22 : 1;
            uint32_t bit23 : 1;
            uint32_t bit24 : 1;
            uint32_t bit25 : 1;
            uint32_t bit26 : 1;
            uint32_t bit27 : 1;
            uint32_t bit28 : 1;
            uint32_t bit29 : 1;
            uint32_t bit30 : 1;
            uint32_t bit31 : 1;

        } bit;
        uint16_t value[2];

    } balance_OPT;
    uint8_t yi_xian_tong_OPT;
} remote_switch_typedef; //22

//51

#if USE_PROTOCOL1363_HUANDIAN
typedef struct {
    uint8_t Hardware_type[10];
    uint8_t Product_code[10];
    uint8_t Project_code[10];
    uint8_t Software_Ver[3];
    uint8_t BOOT_Ver[2];

} sm_info_typedef; //67
#elif USE_PROTOCOL1363_TIETA
typedef struct {
    uint8_t batt_factor_name[20];
    uint8_t batt_Ver[2];
    uint8_t bms_factor_name[20];
    uint8_t bms_model[2];
    uint8_t bms_protocol_ver[2];
    uint8_t batt_id[37];

} sm_info_typedef; //67
#endif

// 8A/8B_获取扩展参数的响应信息
typedef struct {
    uint16_t Start_C_limit;
    uint16_t Short_circuit_gear;
    uint16_t Baud_rate;
    uint16_t Heating_start_TEMP;
    uint16_t Heating_release_TEMP;
    uint16_t Heating_start_V;
    uint16_t Heating_release_V;
    uint16_t NTC_TEMP_DIF_alarm;
    uint16_t Parallel_pack_V_DIF;
    uint16_t SNMP_trap_addr_1_1;
    uint16_t SNMP_trap_addr_1_2;
    uint16_t SNMP_trap_addr_2_1;
    uint16_t SNMP_trap_addr_2_2;
    uint16_t Remote_server_IP_addr_1;
    uint16_t Remote_server_IP_addr_2;
    uint16_t Remote_service_port_Num;
    uint16_t GPS_longitude_value_H;
    uint16_t GPS_longitude_value_L;
    uint16_t GPS_latitude_value_H;
    uint16_t GPS_latitude_value_L;
    uint16_t Gyro_Install_settings;
    uint16_t Gyro_X_axis_restoration_position;
    uint16_t Gyro_Y_axis_restoration_position;
    uint16_t Gyro_Z_axis_restoration_position;
    uint16_t TOT_mileage_H;
    uint16_t TOT_mileage_L;
    uint16_t TOT_charge_CAP_H;
    uint16_t TOT_charge_CAP_L;
    uint16_t TOT_DISCH_CAP_H;
    uint16_t TOT_DISCH_CAP_L;
    uint16_t Report_frequency_when_moving;
    uint16_t Report_frequency_when_stationary;
    uint16_t Report_frequency_during_storage;
    uint16_t Estimated_DISCH_time;
    uint16_t Charge_OC_PROT_2_C_value;
    uint16_t Charge_OC_PROT_2_delay_time;
    uint16_t Battery_full_charge_cut_off_V;
    uint16_t Battery_full_charge_cut_off_C;
    uint16_t Enter_normal_sleep_delay_time;
    uint16_t Regular_wake_up_time_during_normal_sleep;
    uint16_t DISCH_wakeup_turn_on_DISCH_MOS_delay;
    uint16_t Communication_CHG_enable;
    uint16_t Cell_fail_L_V;
    uint16_t Cell_fail_H_V;
    uint16_t Deep_sleep_delay;
    uint16_t Battery_H_TEMP_2_PROT;
    uint16_t SOC_L_PROT;
    uint16_t C_limit_starting_C;
    uint16_t C_coefficient;
    uint16_t Num_of_TEMPs;
    uint8_t total_pack;
} extd_params_typedef; //100
//  single_pack_80 81  (from Model)
typedef struct {
    uint16_t balance_H_TEMP_PROT;
    uint16_t balance_L_TEMP_PROT;
    uint16_t balance_starting_V;
    uint16_t balance_starting_V_DIF;
    uint16_t single_H_V_alarm;
    uint16_t single_L_V_alarm;
    uint16_t TOT_V_H_V_alarm;
    uint16_t TOT_V_L_V_alarm;
    uint16_t ENV_H_TEMP_alarm;
    uint16_t ENV_L_TEMP_alarm;
    uint16_t power_H_TEMP_alarm;
    uint16_t power_L_TEMP_alarm;
    uint16_t CHG_H_TEMP_alarm;
    uint16_t CHG_L_TEMP_alarm;
    uint16_t DISCH_H_TEMP_alarm;
    uint16_t DISCH_L_TEMP_alarm;
    uint16_t CHG_C_alarm;
    uint16_t DISCH_C_alarm;
    uint16_t cell_OVV_PROT;
    uint16_t cell_OVV_PROT_REL;
    uint16_t cell_OVV_delay;
    uint16_t cell_UNDV_PROT;
    uint16_t cell_UNDV_PROT_REL;
    uint16_t cell_UNDV_PROT_delay;
    uint16_t TOT_OVV_PROT;
    uint16_t TOT_OVV_PROT_REL;
    uint16_t TOT_OVV_PROT_delay;
    uint16_t TOT_UNDV_PROT;
    uint16_t TOT_UNDV_PROT_REL;
    uint16_t TOT_UNDV_PROT_delay;
    uint16_t CHG_H_TEMP_PROT;
    uint16_t CHG_H_TEMP_PROT_REL;
    uint16_t CHG_L_TEMP_PROT;
    uint16_t CHG_L_TEMP_REL;
    uint16_t DISCH_H_TEMP_PROT;
    uint16_t DISCH_H_TEMP_REL;
    uint16_t DISCH_L_TEMP_PROT;
    uint16_t DISCH_L_TEMP_REL;
    uint16_t ENV_H_TEMP_PROT;
    uint16_t ENV_H_TEMP_REL;
    uint16_t ENV_L_TEMP_PROT;
    uint16_t ENV_L_TEMP_REL;
    uint16_t power_H_TEMP_PROT;
    uint16_t power_H_TEMP_REL;
    uint16_t V_DIF_alarm_value;
    uint16_t V_DIF_alarm_REL_value;
    uint16_t CHG_OC_PROT;
    uint16_t CHG_OC_PROT_delay;
    uint16_t DISCH_OC_1_PROT;
    uint16_t DISCH_OC_1_PROT_delay;
    uint16_t DISCH_OC_AUTO_REL_time;
    uint16_t DISCH_OC_2_PROT;
    uint16_t DISCH_OC_2_PROT_delay;
    uint16_t cell_sleep_V;
    uint16_t cell_sleep_delay;
    uint16_t Standby_sleep_delay;
    uint16_t L_BAT_alarm;
    uint16_t cell_OC_REL_CAP;
    uint16_t TOT_OC_REL_CAP;
    uint8_t total_pack;
} basic_params_typedef; //118


//80 basic_params_typedef
typedef struct { // protect
    uint16_t cell_OVV_PROT;
    uint16_t cell_OVV_delay;
    uint16_t cell_OVV_PROT_REL;

    uint16_t cell_UNDV_PROT;
    uint16_t cell_UNDV_PROT_delay;
    uint16_t cell_UNDV_PROT_REL;

    uint16_t V_DIF_PROT;
    uint16_t V_DIF_PROT_delay;
    uint16_t V_DIF_PROT_REL;

    uint16_t TOT_OVV_PROT;
    uint16_t TOT_OVV_PROT_delay;
    uint16_t TOT_OVV_PROT_REL;

    uint16_t TOT_UNDV_PROT;
    uint16_t TOT_UNDV_PROT_delay;
    uint16_t TOT_UNDV_PROT_REL;

    uint16_t CHG_OC_PROT;
    uint16_t CHG_OC_PROT_delay;
    uint16_t CHG_OC_PROT_REL;

    uint16_t CHG_OC2_PROT;
    uint16_t CHG_OC2_PROT_delay;
    uint16_t CHG_OC2_PROT_REL;

    uint16_t DISCH_OC_1_PROT;
    uint16_t DISCH_OC_1_PROT_delay;
    uint16_t DISCH_OC_1_PROT_REL;

    uint16_t DISCH_OC_2_PROT;
    uint16_t DISCH_OC_2_PROT_delay;
    uint16_t DISCH_OC_2_PROT_REL;

    int16_t CHG_H_TEMP_PROT;
    uint16_t CHG_H_TEMP_PROT_delay;
    int16_t CHG_H_TEMP_PROT_REL;

    int16_t CHG_L_TEMP_PROT;
    uint16_t CHG_L_TEMP_PROT_delay;
    int16_t CHG_L_TEMP_REL;

    int16_t DISCH_H_TEMP_PROT;
    uint16_t DISCH_H_TEMP_PROT_delay;
    int16_t DISCH_H_TEMP_REL;

    int16_t DISCH_L_TEMP_PROT;
    uint16_t DISCH_L_TEMP_PROT_delay;
    int16_t DISCH_L_TEMP_REL;

    int16_t MOS_H_TEMP_PROT;
    uint16_t MOS_H_TEMP_PROT_delay;
    int16_t MOS_H_TEMP_REL;

    int16_t ENV_H_TEMP_PROT;
    uint16_t ENV_H_TEMP_PROT_delay;
    int16_t ENV_H_TEMP_REL;

    int16_t ENV_L_TEMP_PROT;
    uint16_t ENV_L_TEMP_PROT_delay;
    int16_t ENV_L_TEMP_REL;

    uint16_t cell_OVV_alarm;
    uint16_t cell_OVV_alarm_delay;
    uint16_t cell_OVV_alarm_REL;
    uint16_t cell_UNDV_alarm;
    uint16_t cell_UNDV_alarm_delay;
    uint16_t cell_UNDV_alarm_REL;
    uint16_t V_DIF_alarm_value;
    uint16_t V_DIF_alarm_delay;
    uint16_t V_DIF_alarm_REL_value;
    uint16_t TOT_V_H_V_alarm;
    uint16_t TOT_V_H_V_alarm_delay;
    uint16_t TOT_V_H_V_alarm_REL;
    uint16_t TOT_V_L_V_alarm;
    uint16_t TOT_V_L_V_alarm_delay;
    uint16_t TOT_V_L_V_alarm_REL;
    uint16_t CHG_C_alarm;
    uint16_t CHG_C_alarm_delay;
    uint16_t CHG_C_alarm_REL;
    uint16_t CHG_C2_alarm;
    uint16_t CHG_C2_alarm_delay;
    uint16_t CHG_C2_alarm_REL;
    uint16_t DISCH_C_alarm;
    uint16_t DISCH_C_alarm_delay;
    uint16_t DISCH_C_alarm_REL;
    uint16_t DISCH_C2_alarm;
    uint16_t DISCH_C2_alarm_delay;
    uint16_t DISCH_C2_alarm_REL;
    int16_t CHG_H_TEMP_alarm;
    uint16_t CHG_H_TEMP_alarm_delay;
    int16_t CHG_H_TEMP_alarm_REL;

    int16_t CHG_L_TEMP_alarm;
    uint16_t CHG_L_TEMP_alarm_delay;
    int16_t CHG_L_TEMP_alarm_REL;
    int16_t DISCH_H_TEMP_alarm;
    uint16_t DISCH_H_TEMP_alarm_dela;
    int16_t DISCH_H_TEMP_alarm_REL;
    int16_t DISCH_L_TEMP_alarm;
    uint16_t DISCH_L_TEMP_alarm_dela;
    int16_t DISCH_L_TEMP_alarm_REL;
    int16_t MOS_H_TEMP_alarm;
    uint16_t MOS_H_TEMP_alarm_delay;
    int16_t MOS_H_TEMP_alarm_REL;
    // "MOS_L_TEMP_alarm;
    int16_t ENV_H_TEMP_alarm;
    uint16_t ENV_H_TEMP_alarm_delay;
    int16_t ENV_H_TEMP_alarm_REL;
    int16_t ENV_L_TEMP_alarm;
    uint16_t ENV_L_TEMP_alarm_delay;
    int16_t ENV_L_TEMP_alarm_REL;

    uint16_t cell_sleep_V;
    uint16_t cell_sleep_delay;
    int16_t balance_H_TEMP_PROT;
    int16_t balance_L_TEMP_PROT;
    uint16_t balance_starting_V;
    uint16_t balance_starting_V_DIF;
    uint16_t L_SOC_alarm;
    uint16_t L_SOC_alarm_REL;
    uint8_t totol_pack;
} basic_params_new_typedef;
//82 calibration
typedef struct {
    int16_t zero_C;
    int16_t CHG_C_1_less_5A;
    int16_t CHG_C_2_greater_5A;
    int16_t DISCH_C_1_less_5A;
    int16_t DISCH_C_2_greater_5A;
    int16_t gap_CHG_threshold;
    int16_t actual_full_CHG_capacity;
    int16_t The_remaining_capacity;
    int16_t num_of_CHG_and_DISCH_cycles;
    int16_t num_of_cells;
    int16_t cell_V_list[CELL_NUM];
    int16_t num_of_TEMPs;
    int16_t TEMP_lis[TEMP_NUM];
} cal_params_typedef; //72
//83
typedef struct {
    uint16_t cell_ov_times;
    uint16_t cell_uv_times;
    uint16_t chg_oc_times;
    uint16_t chg_ot_times;
    uint16_t sc_times;
    uint16_t mos_ot_times;
    uint16_t env_ot_times;
    uint16_t env_ut_times;
    uint16_t pack_ov_times;
    uint16_t pack_uv_times;
    uint16_t disch_oc_times;
    uint16_t chg_ut_times;
    uint16_t disch_ot_times;
    uint16_t disch_ut_times;
    uint8_t total_pack;
} warm_cnt_typedef; //10

typedef struct {
    uint8_t batt_id[30];       //8CH 8DH
    uint8_t protocol_ver;      //4F 50
    uint8_t dev_addr;          //50
    uint8_t Server_domain[32]; //8e 8F
    uint8_t total_pack;
} info_typedef; //64
//eeprom 获取历史数据 4Bh
typedef struct {
    uint16_t SOC;
    uint16_t TOT_Vs;
    uint8_t num_of_cells;
    uint16_t cell_list[CELL_NUM];
    uint16_t ENV_TEMP;
    uint16_t MOS_TEMP;
    uint16_t PCBA_TEMP;
    uint8_t TOT_TEMPs;
    uint16_t TEMP_list[BATT_TEMP_NUM];
    uint16_t pack_current;
    uint16_t pack_inter_RES;
    uint16_t SOH;
    uint8_t user_costum_remoter1;
    uint8_t user_costum;
    uint16_t pack_CHG_DISCH_status;
    uint16_t full_CAP;
    uint16_t remaining_CAP;		//剩余容量
    uint16_t Cycles;
    uint16_t V_status_list;
    uint16_t C_status_list;
    uint16_t TEMP_status_list;
    uint16_t warning_status_list;
    uint16_t FET_status_list;
    uint16_t OVV_PROT_status_list;
    uint16_t UNDV_PROT_status_list;
    uint16_t high_V_alarm_status_list;
    uint16_t low_V_alarm_status_list;
    uint16_t balance_status_status_list;
    data_type_typedef data_type;

} pack_history_data_typedef;

typedef struct {
    //   get_sys_int_params_typedef get_sys_int_params;
    sys_paras_typedef sys_paras;
    sys_status_typedef sys_status;
    data_type_typedef data_type;
    warm_status_typedef warm_status;
    data_time_typedef data_time;
    single_pack_typedef single_pack;
    remote_switch_typedef remote_switch;
    sm_info_typedef sm_info;
    extd_params_typedef extd_params;
    basic_params_typedef basic_params;
    cal_params_typedef cal_params;
    warm_cnt_typedef warm_cnt;
    info_typedef info;

} bms_params_typedef; //0x42

typedef struct {
    data_time_typedef data_time;
    pack_history_data_typedef pack_history_data;
    uint8_t return_data_type;
} bms_history_data_typedef;

typedef struct {
    data_time_typedef data_time;
    warm_status_typedef warm_status;
    uint8_t return_data_type;
    union {
        struct {
            uint8_t warm_flag : 1;
            uint8_t rsvd0 : 3;
            uint8_t sw_flag : 1;
            uint8_t rsvd1 : 3;
        } bit;
        uint8_t value;
    } data_flag;
    uint8_t total_pack;
} bms_history_warm_typedef;

typedef union {
    struct {
        uint32_t cell_OV_alarm : 1;
        uint32_t cell_UNDV_alarm : 1;
        uint32_t cell_abnormal : 1;
        uint32_t V_DIF_OH : 1;
        uint32_t accumulated_OVV_TOT_V : 1;
        uint32_t accumulated_UND_TOT_V : 1;
        uint32_t ENV_O_TEMP : 1;
        uint32_t ENV_UND_TEMP : 1;
        uint32_t MOS_UND_TEMP : 1;
        uint32_t MOS_O_TEMP : 1;
        uint32_t CHG_O_TEMP : 1;
        uint32_t CHG_UND_TEMP : 1;
        uint32_t DISCH_O_TEMP : 1;
        uint32_t DISCH_UND_TEMP : 1;
        uint32_t TEMP_DIF_OH : 1;
        uint32_t CHG_MOS_fail : 1;
        uint32_t DISCH_MOS_fail : 1;
        uint32_t CHG_OC : 1;
        uint32_t DISCH_OC : 1;
        uint32_t CHG_CAB_communication : 1;
        uint32_t GPRS_communication : 1;
        uint32_t fuse : 1;
        uint32_t SOC_L_PROT : 1;
        uint32_t Short_circuit_PROT : 1;
        uint32_t limit_C_PROT : 1;
        uint32_t sample_fail : 1;
        uint32_t NTC_fail : 1;
        uint32_t SYS_fail : 1;
        uint32_t revd1 : 4;

    } bits;
    uint32_t value;
} alarm_flag_list_typedef;

typedef union {
    struct {
        uint32_t rsvd : 21;
        uint32_t fuse : 1;
        uint32_t SOC_L_PROT : 1;
        uint32_t Short_circuit_PROT : 1;
        uint32_t limit_C_PROT : 1;
        uint32_t sample_fail : 1;
        uint32_t NTC_fail : 1;
        uint32_t SYS_fail : 1;
        uint32_t revd1 : 4;
    } bits;
    uint32_t value;
} PROT_flag_list_typedef;

typedef struct {
    uint16_t TOT_Vs;
    int16_t TOT_C;
    uint16_t Num_CHG_CNT;
    uint16_t Num_DISCH_CNT;
    uint16_t CHG_depth;
    uint16_t DISCH_depth;
    union {
        struct {
            uint16_t byte[2];
        } byte;
        uint32_t value;
    } runnning_mile;
    uint16_t SOC;
    uint16_t SOH;
    uint8_t MOS_status;
    uint16_t rated_CAP;
    int16_t ENV_TEMP;
    int16_t MOS_TEMP;
    alarm_flag_list_typedef alarm_flag_list;
    PROT_flag_list_typedef PROT_flag_list;
    uint8_t num_of_cells;
    uint16_t cell_list[CELL_NUM];
    uint8_t TOT_TEMPs;
    uint16_t TEMP_list[BATT_TEMP_NUM];
    uint16_t rsvd;
    uint16_t return_data_type;
    data_time_typedef data_time;
    union {
        struct {
            uint8_t warm_flag : 1;
            uint8_t rsvd0 : 3;
            uint8_t sw_flag : 1;
            uint8_t rsvd1 : 3;
        } bit;
        uint8_t value;
    } data_flag;
    uint8_t total_pack;
} pack_No_history_data_list;

#ifdef USE_PROTOCOL1363_SNMP
typedef struct {
    int16_t TOT_C;
    uint16_t TOT_Vs;
    uint16_t SOC;
    uint16_t SOH;
    uint16_t full_CAP;
    uint16_t remaining_CAP;
    uint16_t Design_CAP;
    uint16_t Cycles;
    uint16_t cell_list[16];
    int16_t TEMP_list[4];
    uint16_t MOS_TEMP;
    uint16_t ENV_TEMP;
    union {
        struct {
            uint16_t DISCH_OC : 1;
            uint16_t Short_circuit_PROT : 1;
            uint16_t UNDV_PROT : 1;
            uint16_t UNDV_alarm : 1;

            uint16_t OVV_PROT : 1;
            uint16_t OVV_alarm : 1;
            uint16_t L_TEMP_PROT : 1;
            uint16_t L_TEMP_alarm : 1;

            uint16_t H_TEMP_PROT : 1;
            uint16_t H_TEMP_alarm : 1;
            uint16_t statue_PROT : 1;
            uint16_t statue_alarm : 1;

            uint16_t DISCH_C_status : 1;
            uint16_t CHG_C_status : 1;
            uint16_t DISCH_FET_status : 1;
            uint16_t CHG_FET_status : 1;

        } bit;
        uint16_t value;
    } pack_status;
    uint16_t rsvd[9];
    uint8_t addr;
} snmp_reals_paras_typedef;

typedef struct {
    uint8_t ip_addr[4];
    uint8_t subnet_mask[4];
    uint8_t gateway[4];
    uint8_t mac[6];
} snmp_ip_typedef;

typedef struct {
    pack_status_typedef pack_status;
    snmp_ip_typedef snmp_ip_info;
    uint16_t resvd[9];

} bms_protocol1363_snmp_typedef;

typedef enum {
    enum_module_internet = 1,
    enum_module_gps,
    enum_module_bluetooth,
    enum_module_GYRO,
} ENUM_MODULE_TYPEDEF;

typedef enum {
    enum_module_set = 2,
    enum_module_get = 1,
} ENUM_MODULE_OPERATION_TYPEDEF;

typedef struct {
    uint8_t WiFi_mac[6];
    uint8_t SNMP_trap1[4];
    uint8_t SNMP_trap2[4];
    uint8_t mac[6];
    uint8_t remote_server_ip[4];
    uint16_t remote_server_port;
    uint8_t IP[4];
    uint8_t subnet_mask[4];
    uint8_t gateway[4];
} MODULE_INTERNET_TYPEDEF;

typedef struct {
    uint32_t longitude; //经度
    uint32_t Latitude;  //纬度
} MODULE_GPS_TYPEDEF;

typedef struct {
    uint8_t mac[6];

} MODULE_BLUETOOTH_TYPEDEF;

typedef struct {
    union {
        struct {
            uint16_t theft_proof_enable : 1;
            uint16_t comm_proof_enable : 1;
            uint16_t machine_proof_enable : 1;
            uint16_t charger_active : 1;
            uint16_t sleep_proof_enable : 1;
            uint16_t placement : 1;
            uint16_t RSVD : 10;

        } bits;
        uint16_t value;
    } setting;
    uint16_t xyz[3];
    uint8_t time_out_h;
    int16_t x_zero_site;
    int16_t y_zero_site;
    int16_t z_zero_site;

} MODULE_GYRO_TYPEDEF;

typedef struct {
    MODULE_INTERNET_TYPEDEF internet;
    MODULE_GPS_TYPEDEF gps;
    MODULE_BLUETOOTH_TYPEDEF bluetooth;
    MODULE_GYRO_TYPEDEF GYRO;
} PROTOCOL_MODULE_TYPEDEF;

extern bms_protocol1363_snmp_typedef bms_protocol1363_snmp;
extern sm_info_typedef g_sm_info;
#endif

#endif //COM_PROTOCAL_1363_PARAS_H__
