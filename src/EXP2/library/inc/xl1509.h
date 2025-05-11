/**
 * ���ȵ��� ��Ȩ����  *
 * @FilePath: xl1509.h
 * @Author: ������
 * @Email: XiaoMengmeng@lanmeng.com
 * @Version: 1.0
 * @LastEditors:
 * @LastEditTime: 2022-05-24 08:32:02
 * @  * Copyright: 2022 ���ȵ��� All Rights Reserved.
 * @  *  *
 * @  * This software is licensed under terms that can be found in the LICENSE file
 * @  * in the root directory of this software component.
 * @  * If no LICENSE file comes with this software, it is provided GPL3.0.
 * @  *  *
 * @Description:
 * @�޸���־:
 * copyright @2022(c) GPL-3.0  *
*/
#ifndef __XL1509_H__
#define __XL1509_H__
#include "main.h"
//#include "led.h"

#define XL1509_ON() HAL_GPIO_WritePin(C_GPIO_Port,C_Pin,GPIO_PIN_RESET)
#define XL1509_OFF() HAL_GPIO_WritePin(C_GPIO_Port,C_Pin,GPIO_PIN_SET)

//��ʼ��
void XL1509_Init(void);
//���õ�ѹֵ
void XL1509_SetVoltage(uint16_t vol);
//DAC�ĺ���
uint16_t function(uint16_t vol);
//���ص�ѹֵ
uint16_t getVoltage(void);
//���ؿ���״̬
uint8_t getOnOff(void);
//С����
float xiaoqingqing_function(float vol);

#endif
