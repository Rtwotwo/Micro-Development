/**
 * @file touch.c
 * @brief
 * @author HeYingmeng (XiaoMengmeng@lanmeng.com)
 * @version 1.0
 * @date 2022-05-12
 *
 *   * @copyright Copyright (c) 2022 ���ȵ���
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
//���ڽ��ն���
//uartQueue screenQueue;
uint8_t uart_state = UART_IDLE;//���ڵ�״̬����
uint8_t temp_uart1[9];
uint8_t temp_uart6[9];
uint8_t lengthOfUart;//���ڽ��������ݵĳ��ȣ�����0x83��
uint16_t key_reg;
uint16_t key_data;
void initTouch(void)
{
    HAL_UART_Receive_IT(&huart1, temp_uart1, 9);
    HAL_UART_Receive_IT(&huart6, temp_uart6, 9);
}
/**
 * @brief ���ݴ���
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
//��ȡ����������ֵ��0Ϊ��ȡʧ�ܣ�
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
//�����жϻص�����
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    //����1�����Դ���->��Ӱ��
    if(huart->Instance == USART1)
    {
        HAL_UART_Receive_IT(&huart1, temp_uart1, 9);
    }
    //����6������������->������
    else if(huart->Instance ==USART6)
    {
        HAL_UART_Receive_IT(&huart6, temp_uart6, 9);
        //HAL_UART_Transmit_IT(&huart1, temp_uart6, 9);
        handout_data();
    }
}

