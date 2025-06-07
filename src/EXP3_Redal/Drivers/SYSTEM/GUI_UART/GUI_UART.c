#include "stm32f4xx.h"                  // Device header
#include "usart.h"

uint8_t Rx_buffer[9]={0};

extern uint8_t threshold_state;

uint8_t Commond_Data_Prefix[4]={0xA5,0x5A,0x05,0x82};
uint8_t Commond_Buzzer_Prefix[4]={0xA5,0x5A,0x03,0x80};

void GUI_Correct_Fire_Threshold(uint8_t rate)    //1-10级，大于阈值就响并且起火
{
    uint8_t i=0x00;
    uint8_t j=0x00;
    uint8_t Rate_high=(uint8_t)(rate>>8);
    uint8_t Rate_low=(uint8_t)(rate);
    HAL_UART_Transmit(&huart6, Commond_Data_Prefix, 4, 0xffff);
    HAL_UART_Transmit(&huart6, &i, 1, 0xffff);
    HAL_UART_Transmit(&huart6, &j, 1, 0xffff);
    HAL_UART_Transmit(&huart6, &Rate_high, 1, 0xffff);
    HAL_UART_Transmit(&huart6, &Rate_low, 1, 0xffff); 
}

void GUI_Correct_Fire_Sign(uint8_t flag)    //flag=0,对应不起火，flag=1,对应起火，
{
    uint8_t i=0x00;
    uint8_t j=0x06;
    uint8_t flag_high=(uint8_t)(flag>>8);
    uint8_t flag_low=(uint8_t)(flag);
    HAL_UART_Transmit(&huart6, Commond_Data_Prefix, 4, 0xffff);
    HAL_UART_Transmit(&huart6, &i, 1, 0xffff);
    HAL_UART_Transmit(&huart6, &j, 1, 0xffff);
    HAL_UART_Transmit(&huart6, &flag_high, 1, 0xffff);
    HAL_UART_Transmit(&huart6, &flag_low, 1, 0xffff); 
}

void GUI_Correct_Resistance_Value(uint8_t resistance)    //数据格式：两位整数，两位小数
{
    uint8_t i=0x00;
    uint8_t j=0x04;
    uint8_t resistance_high=(uint8_t)(resistance>>8);
    uint8_t resistance_low=(uint8_t)(resistance);
    HAL_UART_Transmit(&huart6, Commond_Data_Prefix, 4, 0xffff);
    HAL_UART_Transmit(&huart6, &i, 1, 0xffff);
    HAL_UART_Transmit(&huart6, &j, 1, 0xffff);
    HAL_UART_Transmit(&huart6, &resistance_high, 1, 0xffff);
    HAL_UART_Transmit(&huart6, &resistance_low, 1, 0xffff); 
}

void GUI_Buzzer_10ms_per(uint8_t Num)
{
    uint8_t i=0x02;
    HAL_UART_Transmit(&huart6, Commond_Buzzer_Prefix, 4, 0xffff);
    HAL_UART_Transmit(&huart6, &i, 1, 0xffff);
    HAL_UART_Transmit(&huart6, &Num, 1, 0xffff);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance==USART6)
    {
        if(Rx_buffer[0]==0xA5&&Rx_buffer[1]==0x5A&&Rx_buffer[3]==0x83)
        {
            if(Rx_buffer[5]==0x1A)
            {
                if(Rx_buffer[8]==0x01)
                {
                    threshold_state=1;
                }
                else if(Rx_buffer[8]==0x00)
                {
                    threshold_state=2;
                }
            }
        }
        else
        {
            for(uint8_t i=0;i<=8;i++)
            {
                Rx_buffer[i]=0;
            }
        }
        HAL_UART_Receive_IT(&huart6, Rx_buffer, 9);
    }
}



