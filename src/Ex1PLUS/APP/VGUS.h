#ifndef __VGUS_H
#define __VGUS_H

#include "stm32f4xx_hal.h"

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart6;
extern uint8_t ReceiveByte[];

/* 定义串口指令相关常量 */
#define FRAME_HEADER_FIRST     0xA5    // 指令帧头1
#define FRAME_HEADER_SECOND    0x5A    // 指令帧头2
#define COMMAND_REGISTER_WRITE 0x80   // 寄存器写指令
#define COMMAND_REGISTER_READ  0x81   // 寄存器读指令
#define COMMOND_MEMORY_WRITE   0x82   // 变量存储器写指令
#define COMMOND_MEMORY_READ    0x83   // 变量存储器读指令
#define COMMOND_CURVE_WRITE    0x84   // 写曲线缓冲区指令

/* 发送缓冲区大小 */
#define USART_TRANSMIT_BUFFER_SIZE  32	
/* 接收缓冲区大小 */
#define USART_RECEIVE_BUFFER_SIZE   32

/**
 * @brief 向 VGUS 寄存器读
 * @param cmd_length 指令数据长度
 * @param cmd 指令数据指针
 */
void VGUS_WriteRegister(uint8_t cmd_length, uint8_t *cmd);

/**
 * @brief 向 VGUS 寄存器写
 * @param dat_length 数据长度
 * @param reg 寄存器地址
 * @param dat 数据指针
 */
void VGUS_ReadRegister(uint8_t dat_length, uint16_t reg, uint8_t *dat);

/**
 * @brief 控制蜂鸣器响声
 * @param time 响鸣时间，单位为10ms
 */
void VGUS_RingBeep(uint8_t time);

/**
 * @brief 设置背景灯亮度
 * @param light 背景灯亮度（0x00~0x40）
 */
void VGUS_SetBackgroundLight(uint8_t light);

/**
 * @brief 切换画面
 * @param pic 图片编号
 */
void VGUS_ChangePicture(uint16_t pic);

/**
 * @brief 向曲线缓冲区通道0写入数据
 * @param value 写入的数据值
 */
void VGUS_WriteCarveCH0Data(uint16_t value);

/**
 * @brief UART通信回调函数，用于识别接收指令
 */
void VGUS_UART_RxCpltCallback(void);

/**
 * @brief 用于处理接收到的 VGUS 主动发出的信息
 * @param *dat 有效指令数据指针
 * @param length 有效指令长度
 */
static void VGUS_HandleReceivedFrame(uint8_t *dat, uint8_t length);
#endif /* __VGUS_H */
