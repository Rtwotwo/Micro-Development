/**
 * @file touch.h
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
 * @par 修改日志:串口屏触摸按键返回键值（当前只有这个功能，未来不排除会有扩展）
 */
#ifndef __TOUCH_H_
#define __TOUCH_H_

#include "main.h"

//串口状态机->为啥TMD就我还是单身！老子要女朋友！老子不搞基！
typedef enum
{
	UART_IDLE,            //串口空闲状态
	UART_GET_FIRST,	      //接收到第一个0xA5
	UART_GET_SECOND, 	  //接收到第二个0x5A
    UART_GET_NUM,         //接收到数量
    UART_GET_CMD,         //接收到0x83指令，开始接收数据
    UART_GET_END          //数据全部接收完全
} uart_state_event_t;

uint16_t getScreenValue(void);
void handout_data(void);
void initTouch(void);

#endif
