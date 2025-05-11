/**
 * @file led.h
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
 * @par 修改日志:
 */
#ifndef __LED_H_
#define __LED_H_

#include "main.h"

#define SetC() HAL_GPIO_WritePin(C_GPIO_Port, C_Pin, GPIO_PIN_SET)
#define SetB() HAL_GPIO_WritePin(B_GPIO_Port, B_Pin, GPIO_PIN_SET)
#define SetA() HAL_GPIO_WritePin(A_GPIO_Port, A_Pin, GPIO_PIN_SET)

#define ResetC() HAL_GPIO_WritePin(C_GPIO_Port, C_Pin, GPIO_PIN_RESET)
#define ResetB() HAL_GPIO_WritePin(B_GPIO_Port, B_Pin, GPIO_PIN_RESET)
#define ResetA() HAL_GPIO_WritePin(A_GPIO_Port, A_Pin, GPIO_PIN_RESET)

#define TogC() HAL_GPIO_TogglePin(C_GPIO_Port,C_Pin)
#define TogB() HAL_GPIO_TogglePin(B_GPIO_Port,B_Pin)
#define TogA() HAL_GPIO_TogglePin(A_GPIO_Port,A_Pin)

void normalMode(void);
void upDistortionMode(void);
void downDistortionMode(void);
void doubleDistortionMode(void);

#endif
