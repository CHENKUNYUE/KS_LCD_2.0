/*
 * Copyright (c) Union Co., Ltd. Development Team 2022-2023. All rights reserved.
 * @FilePath     : /ESP32-S3-LCD/main/config/app_config.h
 * @Author       : your name
 * @Date         : 2023-11-04 22:07:37
 * @Description  : 用于详细说明此程序文件完成的主要功能，与其他模块或函数的接口，输出值、取值范围、含义及参数间的控制、顺序、独立或依赖等关系
 * @Others       : 其它内容的说明
 * @History      : 修改历史记录列表，每条修改记录应包括修改日期、修改者及修改内容简述
 */
#ifndef APP_CONFIG_H__
#define APP_CONFIG_H__
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#define BMS_HW_TYPE         (0x1000) // 10为硬件序号 00代表版本 E.g. DR48100JC-10-V0
#define BMS_SW_VER_MAJOR    (2)
#define BMS_SW_VER_MINOR    (0)
#define BMS_SW_VER_REVISION (0)

#define BAT_NTC_1 (1)
#define BAT_NTC_2 (1)
#define BAT_NTC_3 (1)
#define BAT_NTC_4 (1)
#define BAT_NTC_5 (1)
#define BAT_NTC_6 (1)

#define BAT_NTC_NUM           (BAT_NTC_1 + BAT_NTC_2 + BAT_NTC_3 + BAT_NTC_4) // 电芯温度数量
#define NTC_NODES_NUM         (BAT_NTC_NUM + BAT_NTC_5 + BAT_NTC_6)           //(NUMBER_OF_BQ_DEVICES * 2 + 8)
#define MOS_IDX               (BAT_NTC_NUM + 0)                               // MOS温度引索
#define ENV_IDX               (BAT_NTC_NUM + 1)                               // 环境温度引索
#define TEMP_NUM              6
#define CELL_NUM              (16)
#define PACK_NUM 1
#define BATT_TEMP_NUM 20
#define PARALLEL_NUM          (16)                                            // min16 max 64
#define PARALLEL_COUNT_ADDR_0 (1)                                             // 包含0？
#if PRE_CHG_EN > 0
    #define USED_ADC_CHANNEL_NUM (11)
#else
    #define USED_ADC_CHANNEL_NUM 1 + 6 + 1
#endif
#define RUN_MODE                     0
#define STOP_MODE                    1
#define POWER_DOWN_MODE              2
#define HEAT_ON_EN                   1
#define CUSTOMER_REQUEST_RECOVER_SOC (5) // 强充恢复条件的SOC值
#define BASIC_PARAMS_LEN             (107)
#define TASK_TICK_MS                 5
#define PROTOCOL1363_LCD_ADDR        0xFF

#define C_LIMIT_OFF                   (0)
#define C_LIMIT_EN                    (1)
#define O_C_10A_ON_V1_10A_ON_V2_5A_ON (2)           //过流开启10A 限流 总电压达到V1 10A V2 5A
#define O_C_5A_ON                     (3)           //过流开启5A
#define O_C_10A_ON                    (4)           //过流开启10A
#define ALWAYS_5A_ON                  (5)           //一直走5A限流板
#define TEST_5A_ON                    (6)           //测试5A限流板
#define TEST_10A_ON                   (7)           //测试10A限流板
#define ALWAYS_10A_ON                 (10)          //一直走10A限流板
#define CU_LIMIT_CURRENT              (C_LIMIT_OFF) // ALWAYS_10A_ON C_LIMIT_OFF

#define LED_OFF_TIME_S  45
#define OFF_LINE_TIME_S 10
#define USART_BAUD_RATE 19200

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif // APP_CONFIG_H__