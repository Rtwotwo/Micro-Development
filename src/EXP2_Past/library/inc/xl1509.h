/**
 * 蓝萌电子 版权所有  *
 * @FilePath: xl1509.h
 * @Author: 沈玲玲
 * @Email: XiaoMengmeng@lanmeng.com
 * @Version: 1.0
 * @LastEditors:
 * @LastEditTime: 2022-05-24 08:32:02
 * @  * Copyright: 2022 蓝萌电子 All Rights Reserved.
 * @  *  *
 * @  * This software is licensed under terms that can be found in the LICENSE file
 * @  * in the root directory of this software component.
 * @  * If no LICENSE file comes with this software, it is provided GPL3.0.
 * @  *  *
 * @Description:
 * @修改日志:
 * copyright @2022(c) GPL-3.0  *
*/
#ifndef __XL1509_H__
#define __XL1509_H__
#include "main.h"
//#include "led.h"

#define XL1509_ON() HAL_GPIO_WritePin(C_GPIO_Port,C_Pin,GPIO_PIN_RESET)
#define XL1509_OFF() HAL_GPIO_WritePin(C_GPIO_Port,C_Pin,GPIO_PIN_SET)

//初始化
void XL1509_Init(void);
//设置电压值
void XL1509_SetVoltage(uint16_t vol);
//DAC的函数
uint16_t function(uint16_t vol);
//返回电压值
uint16_t getVoltage(void);
//返回开关状态
uint8_t getOnOff(void);
//小晴晴
float xiaoqingqing_function(float vol);

#endif
