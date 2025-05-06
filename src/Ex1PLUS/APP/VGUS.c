#include "main.h"
#include <stdio.h>
#include "VGUS.h"
#include "dac.h"
/* ================= USART通信缓冲区定义 ================= */
/* 发送缓冲区 */
uint8_t transmitBuffer[USART_TRANSMIT_BUFFER_SIZE];

/* 接收缓冲区 */
uint8_t ReceiveByte[1];
uint8_t usartReceiveBuffer[USART_RECEIVE_BUFFER_SIZE];
static uint8_t VUGS_recvIndex = 0;	/* 接收计数 */
static uint8_t VUGS_recvLength = 0;	/* 指令长度 */
static uint8_t recvState = 0;	/* 接收状态机 */

void VGUS_WriteRegister(uint8_t cmd_length, uint8_t *cmd)
{
    transmitBuffer[0] = FRAME_HEADER_FIRST;
    transmitBuffer[1] = FRAME_HEADER_SECOND;
    transmitBuffer[2] = cmd_length + 1;
    transmitBuffer[3] = COMMAND_REGISTER_WRITE;

    for (uint8_t i = 0; i < cmd_length; i++) {
        transmitBuffer[4 + i] = cmd[i];
    }
    HAL_UART_Transmit_IT(&huart6, transmitBuffer, cmd_length + 4);
}

void VGUS_ReadRegister(uint8_t dat_length, uint16_t reg, uint8_t *dat)
{
    transmitBuffer[0] = FRAME_HEADER_FIRST;
    transmitBuffer[1] = FRAME_HEADER_SECOND;
    transmitBuffer[2] = dat_length + 3;
    transmitBuffer[3] = COMMAND_REGISTER_READ;
    transmitBuffer[4] = (reg >> 8) & 0xFF;
    transmitBuffer[5] = reg & 0xFF;
    for (uint8_t i = 0; i < dat_length; i++) {
        transmitBuffer[6 + i] = dat[i];
    }

    HAL_UART_Transmit(&huart6, transmitBuffer, dat_length + 6, HAL_MAX_DELAY);
}


void VGUS_RingBeep(uint8_t time)
{
    uint8_t beep_cmd[2] = {0x02, time};
    VGUS_WriteRegister(sizeof(beep_cmd), beep_cmd);
}

void VGUS_SetBackgroundLight(uint8_t light)
{
    uint8_t light_cmd[2] = {0x01, (light > 0x40) ? 0x40 : light};
    VGUS_WriteRegister(sizeof(light_cmd), light_cmd);
}
void VGUS_ChangePicture(uint16_t pic)
{
    uint8_t pic_cmd[3] = {
        0x03,
        (pic >> 8) & 0xFF,
        pic & 0xFF
    };
    VGUS_WriteRegister(sizeof(pic_cmd), pic_cmd);
}
void VGUS_WriteCarveCH0Data(uint16_t value)
{
    uint8_t transmitbuf[7] = {
        FRAME_HEADER_FIRST,
        FRAME_HEADER_SECOND,
        0x04,
        COMMOND_CURVE_WRITE,
        0x01,
        (value >> 8) & 0xFF,
        value & 0xFF
    };//构筑指令
    HAL_UART_Transmit(&huart6, transmitbuf, sizeof(transmitbuf), HAL_MAX_DELAY);
}
void VGUS_UART_RxCpltCallback(void)
{
    switch (recvState) {
        case 0:  // 等待帧头第1字节
            if (ReceiveByte[0] == 0xA5) {
                usartReceiveBuffer[0] = ReceiveByte[0];
                VUGS_recvIndex = 1;
                recvState = 1;
            }
            break;

        case 1:  // 等待帧头第2字节
            if (ReceiveByte[0] == 0x5A) {
                usartReceiveBuffer[VUGS_recvIndex++] = ReceiveByte[0];
                recvState = 2;
            } else {
                VUGS_recvIndex = 0;
                recvState = 0;
            }
            break;

        case 2:  // 收到长度字节
            usartReceiveBuffer[VUGS_recvIndex++] = ReceiveByte[0];
            VUGS_recvLength = ReceiveByte[0];  // 指令长度字段
            recvState = 3;
            break;

        case 3:  // 接收后续数据
            usartReceiveBuffer[VUGS_recvIndex++] = ReceiveByte[0];

            if (VUGS_recvIndex >= (VUGS_recvLength + 3)) { // 2帧头+1长度+N字节数据						
								/* 串口回显 */           
                printf("[USART6]: ");
								for (uint16_t i = 0; i < VUGS_recvLength + 3; i++)	// 串口1回显指令
								{
										printf("%02X ", usartReceiveBuffer[i]);
								}
								printf("\n");

								HAL_UART_Transmit(&huart6, usartReceiveBuffer, VUGS_recvLength + 3, HAL_MAX_DELAY);// 串口6回显指令
								
								/* 处理接收到的数据帧 */
                VGUS_HandleReceivedFrame(&usartReceiveBuffer[3], VUGS_recvLength); 
								
								/* 接收到完整的一帧,复位 */
                VUGS_recvIndex = 0;
							  VUGS_recvLength = 0;
								recvState = 0;
            }
            break;
    }
}

	
static void VGUS_HandleReceivedFrame(uint8_t *dat, uint8_t length)
{
	/**
 * @brief 处理接收到的数据帧
 * @param data 数据指针
 * @param length 数据长度
 */
    if (length < 3) return;

    if (dat[0] == COMMOND_MEMORY_READ) {
        uint16_t reg = (dat[1] << 8) | dat[2];	//读取寄存器
        uint8_t value = dat[3];

        switch (reg) {
            case 0x04DA:	//无明显失真
                if (value == 0x01) {
										printf("Output a normal sine wave");
                    HAL_GPIO_WritePin(AIN_GPIO_Port, AIN_Pin, GPIO_PIN_RESET);
                    HAL_GPIO_WritePin(BIN_GPIO_Port, BIN_Pin, GPIO_PIN_SET);
                    HAL_GPIO_WritePin(CIN_GPIO_Port, CIN_Pin, GPIO_PIN_RESET);
										DAC_Switch_Waveform(0);
                }
                break;
            case 0x04DB:	//顶部失真
                if (value == 0x01) {
										printf("Output top distortion sine wave");
                    HAL_GPIO_WritePin(AIN_GPIO_Port, AIN_Pin, GPIO_PIN_RESET);
                    HAL_GPIO_WritePin(BIN_GPIO_Port, BIN_Pin, GPIO_PIN_RESET);
                    HAL_GPIO_WritePin(CIN_GPIO_Port, CIN_Pin, GPIO_PIN_SET);
										DAC_Switch_Waveform(1);
                }
                break;
            case 0x04DC:	//底部失真
                if (value == 0x01) {
										printf("Output bottom distortion sine wave");
                    HAL_GPIO_WritePin(AIN_GPIO_Port, AIN_Pin, GPIO_PIN_RESET);
                    HAL_GPIO_WritePin(BIN_GPIO_Port, BIN_Pin, GPIO_PIN_RESET);
                    HAL_GPIO_WritePin(CIN_GPIO_Port, CIN_Pin, GPIO_PIN_RESET);
										DAC_Switch_Waveform(2);
                }
                break;
            case 0x04DD:	//双向失真
                if (value == 0x01) {
										printf("Output bidirectional distortion sine wave");
                    HAL_GPIO_WritePin(AIN_GPIO_Port, AIN_Pin, GPIO_PIN_SET);
                    HAL_GPIO_WritePin(BIN_GPIO_Port, BIN_Pin, GPIO_PIN_SET);
                    HAL_GPIO_WritePin(CIN_GPIO_Port, CIN_Pin, GPIO_PIN_RESET);
										DAC_Switch_Waveform(3);
                }
                break;
            default:
                break;
        }
    }
}
