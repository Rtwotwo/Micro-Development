/**
 * 蓝萌电子 版权所有  *
 * @FilePath: uartscreen.c
 * @Author: 沈玲玲
 * @Email: XiaoMengmeng@lanmeng.com
 * @Version: 1.0
 * @LastEditors:
 * @LastEditTime: 2022-05-24 17:25:58
 * @  * Copyright: 2022 蓝萌电子 All Rights Reserved.
 * @  *  *
 * @  * This software is licensed under terms that can be found in the LICENSE file
 * @  * in the root directory of this software component.
 * @  * If no LICENSE file comes with this software, it is provided GPL3.0.
 * @  *  *
 * @Description:串口屏
 * copyright @2022(c) GPL-3.0  *
*/

#include "usart.h"
#include "uartscreen.h"

uint8_t transmitBuffer[32];
extern uint8_t errorFlag;

/**
 * @brief 发送命令
 * @param cmd_length
 * @param cmd
 */
void VGUS_WriteCommand(uint8_t cmd_length, uint8_t *cmd)
{
    uint8_t i;
    transmitBuffer[0] = FIR_DAT;
    transmitBuffer[1] = SEC_DAT;
    transmitBuffer[2] = cmd_length + 1;
    transmitBuffer[3] = WRITE_CMD_REG;
    for (i = 0; i < cmd_length;i++)
    {
        transmitBuffer[4 + i] = cmd[i];
    }
    HAL_UART_Transmit_IT(&huart6, transmitBuffer, cmd_length + 4);
}
/**
 * @brief 发送数据
 * @param dat_length
 * @param reg
 * @param dat
 */
void VGUS_WriteData(uint8_t dat_length,uint16_t reg,uint8_t *dat)
{
    uint8_t i;
    transmitBuffer[0] = FIR_DAT;
    transmitBuffer[1] = SEC_DAT;
    transmitBuffer[2] = dat_length + 3;
    transmitBuffer[3] = WRITE_DAT_REG;
    transmitBuffer[4] = reg / 256;
    transmitBuffer[5] = reg % 256;
    for (i = 0; i < dat_length;i++)
    {
        transmitBuffer[6 + i] = dat[i];
    }
    HAL_UART_Transmit(&huart6, transmitBuffer, dat_length + 6,0xFFFF);
}
/**
 * @brief
 * 以下为常用命令
 **/


/**
 * @brief 响蜂鸣器，单位10ms
 * @param time
 */
void ringBeep(uint8_t time)
{
    uint8_t beep[2];
    beep[0] = 0x02; //寄存器为0x02
    beep[1] = time;
    VGUS_WriteCommand(2,beep);
}
/**
 * @brief Set the Background Light object 设置背景亮度
 * @param light (0x00-0x40)
 */
void setBackgroundLight(uint8_t light)
{
    uint8_t beep[2];
    beep[0] = 0x01; //寄存器为0x01
    //限幅
    if(light>0x40)
    {
        light = 0x40;
    }
    beep[1] = light;
    VGUS_WriteCommand(2,beep);
}
/**
 * @brief 改换背景图片
 * @param pic
 */
void changePicture(uint16_t pic)
{
    uint8_t picture[3];
    picture[0] = 0x03;//寄存器
    picture[1] = pic / 256;
    picture[2] = pic % 256;
    VGUS_WriteCommand(3,picture);
}
/**
 * @brief 串口屏设置图标
 * @param reg 图标所在寄存器地址
 * @param value 图标的编号
 */
void setIconImage(uint16_t reg,uint16_t value)
{
    uint8_t icon[2];
    icon[0] = value / 256;
    icon[1] = value % 256;
    VGUS_WriteData(2, reg, icon);
}
