#include "stm32f4xx.h"                  // Device header
#include "adc.h"
#include ".\delay\delay.h"
#include "dac.h"

extern float voltage_adc_dac;
extern float Output_Voltage;//在主函数里定义
extern float Top_Value;
extern float bujing;
extern float Top_Value1;

#define VREFINT_ADDRESS 0x1FFF7A2A
uint16_t Adc_GetValue(void);
uint16_t adc_get_result_average(void);

extern float Set_dac_voltage;
extern uint16_t Set_dac_value;

void bubbleSort(uint16_t arr[], uint8_t n);


uint16_t Get_Target_Voltage_DACNUM(void)//这里R1=1KΩ，R2=3.0KΩ，R3=1KΩ
{
//    float R_test=0.265f;
//    float R_L=98.332f;
    
    if(Output_Voltage<=4.20f)
    {
        Set_dac_voltage=(Top_Value1-Output_Voltage)/bujing;//这里改方程
    }
    else
    {
        Set_dac_voltage=(Top_Value-Output_Voltage)/bujing;
    }
    Set_dac_voltage=(Set_dac_voltage>voltage_adc_dac)?voltage_adc_dac:Set_dac_voltage;
    
    Set_dac_value=(uint16_t)(Set_dac_voltage/voltage_adc_dac*4096.0f);
    return Set_dac_value;
}

void Set_Enable_DAC_Output(void)
{
    HAL_DAC_Stop(&hdac, DAC_CHANNEL_1);
    HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, Get_Target_Voltage_DACNUM());
    HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
}

void Stop_DAC_Output(void)
{
    HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 1);//这里可以写0的，但是为了不触发其他条件，写1
    HAL_DAC_Stop(&hdac, DAC_CHANNEL_1); 
}
    
void Get_actual_voltage(void)
{
    uint16_t i=0;
    i=adc_get_result_average();
    voltage_adc_dac=(3.3f*(*(__IO uint16_t*)VREFINT_ADDRESS))/i;
}

uint16_t Adc_GetValue(void)
{
    HAL_ADC_Start(&hadc1); 
    HAL_ADC_PollForConversion(&hadc1, 10); 
    return (uint16_t)HAL_ADC_GetValue(&hadc1);
}

uint16_t adc_get_result_average(void)
{
    uint16_t buffer[10];
    uint32_t temp_val = 0;
    
    for (uint8_t t = 0; t < 10; t++) 
    {
        buffer[t]=Adc_GetValue();
        delay_ms(5);
    }
    bubbleSort(buffer, 10);
    for(uint8_t i=1;i<=8;i++)
    {
        temp_val+=buffer[i];
    }
    return temp_val / 8; 
}

void bubbleSort(uint16_t arr[], uint8_t n) 
{
    uint8_t i, j, swapped;
    uint16_t temp;
    
    for (i = 0; i < n-1; i++)
    {
        swapped = 0;
        for (j = 0; j < n-i-1; j++)
        {
            if (arr[j] > arr[j+1])
            {
                temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
                swapped = 1;  
            }
        }
        if (swapped == 0)
            break;
    }
}

