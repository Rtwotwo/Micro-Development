#include "stm32f4xx.h"                  // Device header

#include "usart.h"
extern float Output_Voltage;//在主函数里定义
extern uint8_t XL1509_State;//值为1对应关，值为0对应开,在主函数
extern uint8_t XL1509_State_Change;//在主函数
const uint8_t UI_Button_On[8]={0xA5,0x5A,0x05,0x82,0x00,0x0D,0x00,0x00};
const uint8_t UI_Button_Off[8]={0xA5,0x5A,0x05,0x82,0x00,0x0D,0x00,0x01};

#define BUFFER_SIZE 20
uint8_t uart_buffer[BUFFER_SIZE]={0};

extern uint8_t voltage_State;//0表示不变，1表示步进增大，2表示步进变小，在主函数里
const uint8_t Tran_Voltage_Prefix[6]={0xA5,0x5A,0x05,0x82,0x00,0x00};
const uint8_t Tran_Current_Prefix[6]={0xA5,0x5A,0x05,0x82,0x00,0x04};


void Correct_UI_Button(uint8_t state)
{
    if(state==1)//state==1表示关闭
    {
        HAL_UART_Transmit(&huart6, UI_Button_Off, 8, 0xffff);
    }
    else if(state==0)//state==0表示打开
    {
        
        HAL_UART_Transmit(&huart6, UI_Button_On, 8, 0xffff);
    }
}

void Correct_UI_voltage(uint16_t Vol_16)
{
    uint8_t i=Vol_16>>8;
    uint8_t j=(uint8_t)Vol_16;
    HAL_UART_Transmit(&huart6, Tran_Voltage_Prefix, 6, 0xffff);
    HAL_UART_Transmit(&huart6, &i, 1, 0xffff);
    HAL_UART_Transmit(&huart6, &j, 1, 0xffff);
}

void Correct_UI_current(float current)
{
    
    uint16_t Cur_16=(uint16_t)(current*10);
    if(!XL1509_State)
    {
        if(Output_Voltage<6.00f&&Output_Voltage>=5.00f)
        {
            Cur_16+=250;
        }
        if(Output_Voltage<7.00f&&Output_Voltage>=6.00f)
        {
            Cur_16+=300;
        }
        if(Output_Voltage>=7.00f)
        {
            Cur_16+=225;
        }
        if(Output_Voltage<5.00f&&Output_Voltage>=4.00f)
        {
            Cur_16+=190;
        }
        if(Output_Voltage<4.00f&&Output_Voltage>=3.00f)
        {
            Cur_16+=100;
        }
        if(Output_Voltage>=2.5f)
        {
            Cur_16+=250;
        }
        
    }
    uint8_t i=Cur_16>>8;
    uint8_t j=(uint8_t)Cur_16;
    HAL_UART_Transmit(&huart6, Tran_Current_Prefix, 6, 0xffff);
    HAL_UART_Transmit(&huart6, &i, 1, 0xffff);
    HAL_UART_Transmit(&huart6, &j, 1, 0xffff);
}

void UI_UART_Init(void)//将按钮关闭，且电压和电流恢复初始值，并打开中断
{
    Correct_UI_Button(1);
    Correct_UI_voltage(500);
    Correct_UI_current(0.00f);
    HAL_UART_Receive_IT(&huart6, uart_buffer, 9);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance==USART6)
    {
        if(uart_buffer[0]==0xA5&&uart_buffer[1]==0x5A&&uart_buffer[3]==0x83)
        {
            if(uart_buffer[5]==0x0E&&uart_buffer[8]==0x01)//判断是不是电源开关按下
            {
                XL1509_State_Change=1;
            }
            if(uart_buffer[5]==0x0F)
            {
                if(uart_buffer[8]==0x01)
                {
                    voltage_State=2;
                }
                else if(uart_buffer[8]==0x00)
                {
                    voltage_State=1;
                }
            }
        }
        else
        {
            for(uint8_t i=0;i<=8;i++)
            {
                uart_buffer[i]=0;
            }
        }
        HAL_UART_Receive_IT(&huart6, uart_buffer, 9);
    }
}

