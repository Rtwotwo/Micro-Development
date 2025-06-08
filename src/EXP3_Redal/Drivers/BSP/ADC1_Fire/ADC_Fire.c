#include "stm32f4xx.h"                  // Device header
#include "adc.h"
#include "tim.h"
#include "./delay/delay.h"
#include "./GUI_UART/GUI_UART.h"

#define Buffer_Size 100
#define VREFINT_ADDRESS 0x1FFF7A2A
#define BUFFER_FILITER_VALUE 5
//float voltage_adc=3.300f;   //老师给的标准板是2.5V，这里是自己的板是3.3V
float voltage_adc=2.500f;   //老师给的标准板是2.5V，这里是自己的板是3.3V

uint8_t fire_threshold=4;
uint8_t threshold_state=0;      //0为不变,1为增加，2为减少

uint16_t Fire_value_int=0;
float Fire_value_float=0.0f;
uint8_t ADC_GetReady=0;           //0为没准备好，1准备好了
uint8_t adc_count=0;

uint16_t ADC_buffer[Buffer_Size]={0};
uint8_t address_AD_buffer=0;

uint8_t Fire_State=0;   //0表示不起火，1表示起火
uint8_t Resistance_Value=10;
uint8_t Resistance_Value_buffer[2]={10,10};
uint8_t Resistance_Value_buffer_Address=0;

uint16_t ADC_compare_value[10]={2589,1368,885,608,446,332,250,192,144,62};

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

uint16_t Adc_GetValue(void)
{
    HAL_ADC_Start(&hadc1); 
    HAL_ADC_PollForConversion(&hadc1, 5); 
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

void Get_actual_voltage(void)
{
    uint16_t i=0;
    i=adc_get_result_average();
    voltage_adc=(3.3f*(*(__IO uint16_t*)VREFINT_ADDRESS))/i;
}

void Update_Fire_State(void)
{
    for(uint8_t i=0;i<=9;i++)
    {
        if(Fire_value_int>=ADC_compare_value[i])
        {
            Resistance_Value_buffer[Resistance_Value_buffer_Address]=(i+1);
            
            Resistance_Value_buffer_Address=Resistance_Value_buffer_Address?0:1;
            break;
        }
    }
    
    if(Resistance_Value_buffer[0]==Resistance_Value_buffer[1])
    {
        Resistance_Value=Resistance_Value_buffer[0];
        ADC_GetReady=1;
    }
    
    if(Resistance_Value<=fire_threshold)
    {
        Fire_State=1;
    }
    else
    {
        Fire_State=0;
    }
}

void ADC_Fire_Start_init(void)
{
    address_AD_buffer=0;
    HAL_ADC_Stop_IT(&hadc1);
    ADC_ChannelConfTypeDef sConfig = {0};
    hadc1.Init.ContinuousConvMode = DISABLE;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
    hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;
    hadc1.Init.NbrOfConversion = 1;
    if (HAL_ADC_Init(&hadc1) != HAL_OK)
    {
        Error_Handler();
    }

    sConfig.Channel = ADC_CHANNEL_6;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_144CYCLES;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }
    HAL_TIM_Base_Start(&htim3);
    HAL_ADC_Start_IT(&hadc1);  
}

void Stop_ADC_Fire(void)
{
    HAL_ADC_Stop_IT(&hadc1);
    HAL_TIM_Base_Stop(&htim3);
    address_AD_buffer=0;
    ADC_GetReady=0;
}

void Compute_Fire_Value(uint16_t Num)
{
    uint32_t sum=0;
    for(uint8_t i=Num;i<=Buffer_Size-1-Num;i++)
    {
        sum+=ADC_buffer[i];
    }
    sum/=(Buffer_Size-2*Num);
    Fire_value_int=sum;
    Fire_value_float=(sum*voltage_adc/4096.0f);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    if(hadc->Instance==ADC1)
    {
        ADC_buffer[address_AD_buffer]=(uint16_t)HAL_ADC_GetValue(&hadc1);
        if(address_AD_buffer>=(Buffer_Size-1))
        {
            address_AD_buffer=0;
            bubbleSort(ADC_buffer, Buffer_Size);
            Compute_Fire_Value(BUFFER_FILITER_VALUE);
            Update_Fire_State();
            adc_count=1;
        }
        else if(address_AD_buffer<(Buffer_Size-1))
        {
            address_AD_buffer++;
        }
    }
}





