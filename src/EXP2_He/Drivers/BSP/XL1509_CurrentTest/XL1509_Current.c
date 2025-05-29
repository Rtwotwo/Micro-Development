#include "stm32f4xx.h"                  // Device header
#include "adc.h"
#include "tim.h"
#include ".\uart_UI\uart_gui.h"
#include ".\XL1509_DAC\XL1509_DAC_Control.h"

#define Buffer_Size 20
#define R_TEST 0.1f
#define buffer_filiter_value 1
extern float Now_Current;//mA,若测试负载为100Ω,则电流小于75mA
extern float voltage_adc_dac;
extern uint8_t XL1509_State;

uint16_t ADC_buffer[20]={0};
uint8_t address_AD_buffer=0;

void Start_ADC_Current(void)
{
    address_AD_buffer=0;
    HAL_ADC_Stop_IT(&hadc1);
  ADC_ChannelConfTypeDef sConfig = {0};
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
  hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T2_TRGO;
  hadc1.Init.NbrOfConversion = 1;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  
  sConfig.Channel = ADC_CHANNEL_6;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  HAL_TIM_Base_Start(&htim2);
  HAL_ADC_Start_IT(&hadc1);  
}

void Stop_ADC_Current(void)
{
    HAL_ADC_Stop_IT(&hadc1);
    HAL_TIM_Base_Stop(&htim2);
    Now_Current=0.0f;
    address_AD_buffer=0;
}

void Compute_Now_Current(uint16_t Num)
{
    uint32_t sum=0;
    for(uint8_t i=Num;i<=Buffer_Size-1-Num;i++)
    {
        sum+=ADC_buffer[i];
    }
    sum/=(Buffer_Size-2*Num);
    Now_Current=((sum*voltage_adc_dac/4096.0f)*1000.0f/R_TEST);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    if(hadc->Instance==ADC1)
    {
        if(XL1509_State==0)
        {
            ADC_buffer[address_AD_buffer]=(uint16_t)HAL_ADC_GetValue(&hadc1);
            if(address_AD_buffer>=19)
            {
                address_AD_buffer=0;
                Compute_Now_Current(buffer_filiter_value);
                Correct_UI_current(Now_Current);
            }
            else if(address_AD_buffer<19)
            {
                address_AD_buffer++;
            }
        }
    }
}
