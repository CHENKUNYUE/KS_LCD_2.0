//======================================================================
//======================================================================
//FileName:  main.c
//Author:
//Version: 1.0.0
//Date:
//Description:
//======================================================================

#include <protocol1363_hw.h>

#include "Data_Init.h"
#include "Font_Library.h"
#include "Key_Scan.h"
#include "LCD_12864.h"
#include "Mcu_Init.h"
#include "Show_Ctrl.h"
#include "Uart_Comm.h"
#include "User_Conf.h"
#include "bms_rt_info.h"
#include "gpio.h"
#include "p_1363_52_frame.h"
#include "p_1363_B0_frame.h"
#include "paras.h"
#include "protocal1363_app.h"
#include "protocol1363_handler.h"
#include "protocol1363_transfer.h"
#include "protocol_1363_get_para.h"
#include "usart.h"
uint8_t get_protocol_time_flag = 0;
uint8_t Flag                   = 0;
uint8_t Comm_Flag              = 0;
uint8_t tx_buf[]               = "123456789";
uint16_t count_disp;
uint8_t flag             = 0;
comm_struct protocol_com = {1, {0}, {0}, 1, 0x05, 0XFF, 0, 0, 0, 0, 0, 0, 01, 0, PROTOCOL_1363_CID1, PROTOCOL_1363_CID1};

extern void IWDG_Feed();
extern void IWDG_Config(uint8_t SetReload);

int main(void) {
    static uint8_t disp_cnt = 0;
    static uint8_t set_can  = 0;
    static uint8_t set_485  = 0;
    HAL_Init();

    IWDG_Config(200);
    HAL_SetTickFreq(HAL_TICK_FREQ_1KHZ);
    MX_GPIO_Init();
    MX_USART1_UART_Init();
    uart1_set_receive_it();
    Lcd12864_Init();
    protocol_1363_init_paras();
    Timer_Init();
    Delay_ms(10);
    EN_INT;
    protocol_hw_set_uart1_send_callback(&usart1_send_buf);
    PackData_Init();
    Page_Welcome_1();

    Set_Timer(SAMP_TIME, 10);
    Set_Timer(SAMP_TIME_2, 10);
    Key_Flag    = 0;
    Comm_Flag_1 = 1;

    while (1) {
        BMS_RT_INFO_T *p_bms                    = bms_get_rt_info();
        D_BMS_SET_OPTION_PARAM_T *bms_set_opt_t = bms_get_set_opt_info();
        PAGE_PROTOCOL_T *protocol_t             = get_page_protocol_t();
        if (get_protocol_time_flag && Sleep_Active == 0) {
            get_protocol_time_flag = 0;                                    //清标志位
            if (bms_set_opt_t->is_option && !bms_set_opt_t->is_finished) { // 设置协议

                switch (bms_set_opt_t->page) {
                    case PAGE_CAN_PAGE_1:
                    case PAGE_CAN_PAGE_2:
                    case PAGE_CAN_PAGE_3:
                        setting_option_port_cfg_by_addr(&protocol_com, p_bms->bms_addr, E_PORT_CAN1, protocol_t->can_protocol);
                        set_can = 1;
                        break;
                    case PAGE_RS485_PAGE_1:
                        setting_option_port_cfg_by_addr(&protocol_com, p_bms->bms_addr, E_PORT_485A, protocol_t->rs485_protocol);
                        set_485 = 1;
                        break;
                    default: break;
                }
            } else {
                if (set_can == 1) {
                    set_can = 0;
                    request_option_port_cfg_by_addr(&protocol_com, p_bms->bms_addr, E_PORT_CAN1);
                    continue;
                }

                if (set_485 == 1) {
                    set_485 = 0;
                    request_option_port_cfg_by_addr(&protocol_com, p_bms->bms_addr, E_PORT_485A);
                    continue;
                }

                disp_cnt++;
                switch (disp_cnt) {
                    case 1:
                        protocol_parallel_M2S_get_sys_int_params(&protocol_com, 0x0FF); //发送42H
                        break;
                    case 2:
                        protocol_parallel_M2S_get_time(&protocol_com, 0x0FF); //时间4DH
                        break;
                    case 3:
                        protocol_parallel_M2S_get_warm_cnt(&protocol_com, 0x0FF); //告警次数83H
                        //disp_cnt = 1;
                        break;
                    case 4:
                        protocol_app_S2M_get_sm_code_(&protocol_com, 0x0FF); //51H
                        break;
                    case 5: protocol_52_require(&protocol_com); break;
                    case 6: request_option_port_cfg_by_addr(&protocol_com, p_bms->bms_addr, E_PORT_CAN1); break;
                    case 7: request_option_port_cfg_by_addr(&protocol_com, p_bms->bms_addr, E_PORT_485A); break;
                    default: disp_cnt = 0; break;
                }
            }
        }
        if (Timer[SAMP_TIME].Flag) {
            Reset_Timer(SAMP_TIME);
            Set_Timer(SAMP_TIME, 40); //  80
            if (0 == SetCommondFlag) {
                Flag++;
                Comm_Flag = 1;
            } else
                Comm_Flag = 0;
        }

        if ((Flag == 1) && (Comm_Flag == 1)) {
            Uart_RequestData();
            Comm_Flag = 0;
        } else if ((Flag == 2) && (Comm_Flag == 1)) {
            Uart_RequestData_2();
            //   Flag=0;
            Comm_Flag = 0;
        } else if ((Flag >= 3) && (Comm_Flag == 1)) {
            //Uart_RequestData_3();
            Flag      = 0;
            Comm_Flag = 0;
        }

        Uart_DataRespond();
        Uart_DataRespond_2();
        //Uart_DataRespond_3();
        Key_Scan();
        if (Sleep_Active == 0) Show_Ctrl();
        Sleep_Ctrl();
        IWDG_Feed();
    }
}

void SystemClock_Config(void) //
{
    RCC_OscInitTypeDef RCC_OscInitStruct   = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct   = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

    HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

    RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState            = RCC_HSI_ON;
    RCC_OscInitStruct.HSIDiv              = RCC_HSI_DIV1;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource       = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM            = RCC_PLLM_DIV1;
    RCC_OscInitStruct.PLL.PLLN            = 8;
    RCC_OscInitStruct.PLL.PLLP            = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ            = RCC_PLLQ_DIV2;
    RCC_OscInitStruct.PLL.PLLR            = RCC_PLLR_DIV2;

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        //Error_Handler();
    }

    RCC_ClkInitStruct.ClockType      = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1;
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
        //Error_Handler();
    }

    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
    PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
        //Error_Handler();
    }
}

void Error_Handler(void) {
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */

    /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line) {
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
