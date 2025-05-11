/**
 * 蓝萌电子 版权所有  *
 * @FilePath: powerUARTScreen.c
 * @Author: 沈玲玲
 * @Email: XiaoMengmeng@lanmeng.com
 * @Version: 1.0
 * @LastEditors:
 * @LastEditTime: 2022-05-24 09:10:39
 * @  * Copyright: 2022 蓝萌电子 All Rights Reserved.
 * @  *  *
 * @  * This software is licensed under terms that can be found in the LICENSE file
 * @  * in the root directory of this software component.
 * @  * If no LICENSE file comes with this software, it is provided GPL3.0.
 * @  *  *
 * @Description:
 * copyright @2022(c) GPL-3.0  *
*/
#include "powerUARTScreen.h"
#include "main.h"
#include "uartscreen.h"
#include "xl1509.h"
#include "current.h"
//#include "usart.h"  //串口通信协议，自主发送电流（小晴晴）
//协议：printf("电流:%d",getCurrent());
//雨思琪（蓝晴儿）

/**
 * 蓝萌电子 版权所有  *
 * @brief: 发送电压:串口屏
 * @param[in] reg 电压值：寄存器
 * @return
 * copyright @2022(c) GPL-3.0  *
*/
void transmitVoltage(uint16_t reg)
{
    uint16_t vol1;
    vol1 = getVoltage();
    uint8_t temp[2];
    temp[0] = vol1 / 256;
    temp[1] = vol1 % 256;
    VGUS_WriteData(2,reg,temp);
//void VGUS_WriteData(uint8_t dat_length,uint16_t reg,uint8_t *dat)
}
/**
 * 蓝萌电子 版权所有  *
 * @brief: 发送开启 / 关闭
 * @return
 * copyright @2022(c) GPL-3.0  *
*/
void transmitONOFF(uint16_t reg)
{
    uint8_t str[] = "OFF";
    uint8_t str2[] = "ON";
    uint8_t i;
    uint8_t buf[4];
    for (i = 0; i < 4;i++)
    {
        if(getOnOff()==0)//开启
        {
            buf[i] = str[i];
        }
        else if(getOnOff()==1)//关闭
        {
            buf[i] = str2[i];
        }
    }
    VGUS_WriteData(4,reg,buf);
}
/**
 * @brief 发送电流
 * @param reg
 */
void transmitCurrentByMCU(uint16_t reg)
{
    int cur = returnCurrent();
    uint8_t temp[2];
    temp[0] = cur / 256;
    temp[1] = cur % 256;
    VGUS_WriteData(2,reg,temp);
}
