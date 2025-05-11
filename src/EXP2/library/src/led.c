/**
 * @file led.c
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
 * @par �޸���־:
 */
#include "led.h"
//����ģʽ����ͨRP5,RP6
void normalMode(void)
{
    ResetA();
    ResetB();
    SetC();
}
//����ʧ��ģʽ����ͨRP5,RP7
void upDistortionMode(void)
{
    SetA();
    ResetB();
    SetC();
}
//����ʧ��ģʽ����ͨRP4,RP6
void downDistortionMode(void)
{
    ResetA();
    SetB();
    ResetC();
}
//˫��ʧ��ģʽ����ͨRP4,RP7
void doubleDistortionMode(void)
{
    SetA();
    SetB();
    ResetC();
}
