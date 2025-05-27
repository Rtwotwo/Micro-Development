/**
 * @file touch.c
 * @brief
 * @author HeYingmeng (XiaoMengmeng@lanmeng.com)
 * @version 1.0
 * @date 2022-05-12
 *
 *   * @copyright Copyright (c) 2022 蓝萌电子
 *   * All rights reserved.
 *
 *   * This software is licensed under terms that can be found in the LICENSE file
 *   * in the root directory of this software component.
 *   * If no LICENSE file comes with this software, it is provided GPL3.0.
 *
 * @par 
 */
#include "touch.h"
#include "usart.h"
//#include "uartQueue.h"
#include "stdbool.h"
//串口接收队列
//uartQueue screenQueue;
uint8_t uart_state = UART_IDLE;//串口的状态变量
uint8_t temp_uart1[9];
uint8_t temp_uart6[9];
uint8_t lengthOfUart;//串口接下来数据的长度（包括0x83）
uint16_t key_reg;
uint16_t key_data;
void initTouch(void)
{
    HAL_UART_Receive_IT(&huart1, temp_uart1, 9);
    HAL_UART_Receive_IT(&huart6, temp_uart6, 9);
}
/**
 * @brief 数据处理
 */
void handout_data(void)
{
    key_reg = 0x0000;
    key_data = 0x0000;
    key_reg |= temp_uart6[4];
    key_reg <<= 8;
    key_reg |= temp_uart6[5];
    key_reg <<= 8;
    key_data |= temp_uart6[7];
    key_data <<= 8;
    key_data |= temp_uart6[8];
    uart_state = UART_GET_END;
}
//获取触屏按键键值（0为获取失败）
uint16_t getScreenValue(void)
{
    if(uart_state == UART_GET_END)
    {
        uart_state = UART_IDLE;
        return key_data;
    }
    else
    {
        return 0x0000;
    }
}
//串口中断回调函数
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    //串口1：调试串口->何影萌
    if(huart->Instance == USART1)
    {
        HAL_UART_Receive_IT(&huart1, temp_uart1, 9);
    }
    //串口6：串口屏串口->陈晓蓝
    else if(huart->Instance ==USART6)
    {
        HAL_UART_Receive_IT(&huart6, temp_uart6, 9);
        //HAL_UART_Transmit_IT(&huart1, temp_uart6, 9);
        handout_data();
    }
}

