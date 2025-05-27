/**
 * 蓝萌电子 版权所有  *
 * @FilePath: fire.h
 * @Author: 沈玲玲
 * @Email: XiaoMengmeng@lanmeng.com
 * @Version: 1.0
 * @LastEditors:
 * @LastEditTime: 2022-06-09 10:22:14
 * @Language: C/C++
 * @Coding: GB2312
 * @  * Copyright: 2022 蓝萌电子 All Rights Reserved.
 * @  *  *
 * @  * This software is licensed under terms that can be found in the LICENSE file
 * @  * in the root directory of this software component.
 * @  * If no LICENSE file comes with this software, it is provided GPL3.0.
 * @  *  *
 * @Description:
 * copyright @2022(c) GPL-3.0  *
*/
#ifndef __FIRE_H__
#define __FIRE_H__

#include "main.h"

void fire_Init(void);
void debug_fire_value(void);
void transmit_Fire(uint16_t reg);

uint8_t getFireLevel(void);
void fireScreenFunction(void);
uint16_t Get_Adc_Average(uint16_t times);
#endif
