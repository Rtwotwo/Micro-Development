/**
 * @file touch.h
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
 * @par �޸���־:�����������������ؼ�ֵ����ǰֻ��������ܣ�δ�����ų�������չ��
 */
#ifndef __TOUCH_H_
#define __TOUCH_H_

#include "main.h"

//����״̬��->ΪɶTMD���һ��ǵ�������ҪŮ���ѣ����Ӳ������
typedef enum
{
	UART_IDLE,            //���ڿ���״̬
	UART_GET_FIRST,	      //���յ���һ��0xA5
	UART_GET_SECOND, 	  //���յ��ڶ���0x5A
    UART_GET_NUM,         //���յ�����
    UART_GET_CMD,         //���յ�0x83ָ���ʼ��������
    UART_GET_END          //����ȫ��������ȫ
} uart_state_event_t;

uint16_t getScreenValue(void);
void handout_data(void);
void initTouch(void);

#endif
