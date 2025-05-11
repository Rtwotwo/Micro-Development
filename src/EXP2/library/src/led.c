/**
 * @file led.c
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
#include "led.h"
//正常模式，导通RP5,RP6
void normalMode(void)
{
    ResetA();
    ResetB();
    SetC();
}
//顶部失真模式，导通RP5,RP7
void upDistortionMode(void)
{
    SetA();
    ResetB();
    SetC();
}
//顶部失真模式，导通RP4,RP6
void downDistortionMode(void)
{
    ResetA();
    SetB();
    ResetC();
}
//双向失真模式，导通RP4,RP7
void doubleDistortionMode(void)
{
    SetA();
    SetB();
    ResetC();
}
