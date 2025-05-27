/**
 * @file uartscreen.h
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
 * @par 修改日志:串口屏专属头文件
 */
#ifndef __UARTSCREEN_H_
#define __UARTSCREEN_H_

#include "main.h"
//指令集
#define WRITE_CMD_REG 0x80
#define READ_CMD_REG 0x81
#define WRITE_DAT_REG 0x82
#define READ_DAT_REG 0x83
#define EXTEND_REG 0x85

//格式 FIR_DAT + SEC_DAT + 后面的总长度 + 指令 + 数据
#define FIR_DAT 0xA5
#define SEC_DAT 0x5A

void VGUS_WriteCommand(uint8_t cmd_length, uint8_t *cmd);
void VGUS_WriteData(uint8_t dat_length, uint16_t reg, uint8_t *dat);

/**
 * @brief 常用命令
 */
void ringBeep(uint8_t time);
void setBackgroundLight(uint8_t light);
void changePicture(uint16_t pic);
void transmit_RTC_Value(uint16_t reg);

void setIconImage(uint16_t reg,uint16_t value);

#endif
