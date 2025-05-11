#include "stm32f4xx.h"                  // Device header
#include "math.h"
#include "dac.h"
#include "adc.h"
#include ".\CD4053\CD4053.h" 
#include "tim.h"
#include ".\delay\delay.h"

#define SAMPLE_SIZE 480

#define VREFINT_ADDRESS 0x1FFF7A2A
//uint16_t ADC_Buffer[20];
uint16_t DAC_Wave_Value=3500;
uint16_t value=700;

void Set_DAC_2_Value(uint8_t times);

uint16_t Normal_Wave[SAMPLE_SIZE];
uint16_t Top_Distort[SAMPLE_SIZE];
uint16_t Bottom_Distort[SAMPLE_SIZE];
uint16_t Bi_Distort[SAMPLE_SIZE];

void Generate_Normal_Wave(void) {
    float inc=2 * 3.1415926/ SAMPLE_SIZE;
    float outdata=0;
    for (int i = 0; i < SAMPLE_SIZE; i++) 
    {
        outdata=(DAC_Wave_Value+1)/2 + (DAC_Wave_Value+1)/2 * sin(inc*i);
        if(outdata>(DAC_Wave_Value+1))
        {
            outdata=DAC_Wave_Value+1;
        }
        if(outdata<0)
        {
            outdata=0;
        }
        Normal_Wave[i]=(uint16_t)outdata+150;
    }
    
//    float inc=2 * 3.1415926/ SAMPLE_SIZE;
//    float outdata=0;
//    for (int i = 0; i < SAMPLE_SIZE; i++) 
//    {
//        outdata=1750 + 1750 * sin(inc*i);
//        if(outdata>3500)
//        {
//            outdata=3500;
//        }
//        if(outdata<0)
//        {
//            outdata=0;
//        }
//        Normal_Wave[i]=(uint16_t)outdata;
//    }
}

void Generate_Top_Distort(void) {
    float inc=2 * 3.1415926/ SAMPLE_SIZE;
    float outdata=0;
    
    for (int i = 0; i < SAMPLE_SIZE; i++) 
    {
        outdata=(DAC_Wave_Value+1)/2+value/2+ ((DAC_Wave_Value+1)/2+value/2)* sin(inc*i);
        if(outdata>(DAC_Wave_Value+1))
        {
            outdata=DAC_Wave_Value+1;
        }
        if(outdata<0)
        {
            outdata=0;
        }
        Top_Distort[i]=(uint16_t)outdata+150;
    }
    
//    float inc=2 * 3.1415926/ SAMPLE_SIZE;
//    float outdata=0;
//    
//    uint16_t value=700;
//    
//    for (int i = 0; i < SAMPLE_SIZE; i++) 
//    {
//        outdata=1750+value/2+ (1750+value/2)* sin(inc*i);
//        if(outdata>3500)
//        {
//            outdata=3500;
//        }
//        if(outdata<0)
//        {
//            outdata=0;
//        }
//        Top_Distort[i]=(uint16_t)outdata;
//    }
}

void Generate_Bottom_Distort(void) {
    float inc=2 * 3.1415926/ SAMPLE_SIZE;
    float outdata=0;
    
    for (int i = 0; i < SAMPLE_SIZE; i++) 
    {
        outdata=(DAC_Wave_Value+1)/2-value/2+ ((DAC_Wave_Value+1)/2+value/2)* sin(inc*i);
        if(outdata>(DAC_Wave_Value+1))
        {
            outdata=DAC_Wave_Value+1;
        }
        if(outdata<0)
        {
            outdata=0;
        }
        Bottom_Distort[i]=(uint16_t)outdata+150;
    }
    
//    float inc=2 * 3.1415926/ SAMPLE_SIZE;
//    float outdata=0;
//    
//    uint16_t value=700;
//    
//    for (int i = 0; i < SAMPLE_SIZE; i++) 
//    {
//        outdata=1750-value/2+ (1750+value/2)* sin(inc*i);
//        if(outdata>3500)
//        {
//            outdata=3500;
//        }
//        if(outdata<0)
//        {
//            outdata=0;
//        }
//        Bottom_Distort[i]=(uint16_t)outdata;
//    }
}

void Generate_Bi_Distort(void) {
    float inc=2 * 3.1415926/ SAMPLE_SIZE;
    float outdata=0;
    
    for (int i = 0; i < SAMPLE_SIZE; i++) 
    {
        outdata=(DAC_Wave_Value+1)/2+ ((DAC_Wave_Value+1)/2+value)* sin(inc*i);
        if(outdata>(DAC_Wave_Value+1))
        {
            outdata=DAC_Wave_Value+1;
        }
        if(outdata<0)
        {
            outdata=0;
        }
        Bi_Distort[i]=(uint16_t)outdata+150;
    }
    
//    float inc=2 * 3.1415926/ SAMPLE_SIZE;
//    float outdata=0;
//    
//    uint16_t value=700;
//    
//    for (int i = 0; i < SAMPLE_SIZE; i++) 
//    {
//        outdata=1750+ (1750+value)* sin(inc*i);
//        if(outdata>3500)
//        {
//            outdata=3500;
//        }
//        if(outdata<0)
//        {
//            outdata=0;
//        }
//        Bi_Distort[i]=(uint16_t)outdata;
//    }
}

void Wave_data_Init(void)
{
    Set_DAC_2_Value(10);
    Generate_Normal_Wave();
    Generate_Top_Distort();
    Generate_Bottom_Distort();
    Generate_Bi_Distort();
}

void Start_DAC_DMA(uint8_t Num) 
{
    HAL_TIM_Base_Start(&htim4);
    switch(Num)
    {
        case 1:{
            HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, (uint32_t *)Normal_Wave, SAMPLE_SIZE, DAC_ALIGN_12B_R);
            break;
        }
        case 2:{
            HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, (uint32_t *)Top_Distort, SAMPLE_SIZE, DAC_ALIGN_12B_R);
            break;
        }
        case 3:{
            HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, (uint32_t *)Bottom_Distort, SAMPLE_SIZE, DAC_ALIGN_12B_R);
            break;
        }
        case 4:{
            HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, (uint32_t *)Bi_Distort, SAMPLE_SIZE, DAC_ALIGN_12B_R);
            break;
        }
        default:break;
    }
}

void Stop_DAC_DMA(void) {
    HAL_DAC_Stop_DMA(&hdac, DAC_CHANNEL_1);
}

void Correct_DAC_Wave(uint8_t Num)
{
    Stop_DAC_DMA();
    Start_DAC_DMA(Num);
}

uint16_t Adc_GetValue(void)
{
    HAL_ADC_Start(&hadc1); 
    HAL_ADC_PollForConversion(&hadc1, 10); 
    return (uint16_t)HAL_ADC_GetValue(&hadc1);
}

uint16_t adc_get_result_average(uint8_t times)
{
    uint32_t temp_val = 0;
    uint8_t t;
    for (t = 0; t < times; t++) 
    {
        temp_val += Adc_GetValue();
        delay_ms(5);
    }
    return temp_val / times; 
}

void Set_DAC_2_Value(uint8_t times)
{
    uint16_t i=0;
    float V_Now=0;
    i=adc_get_result_average(times);
    V_Now=(3.3f*(*(__IO uint16_t*)VREFINT_ADDRESS))/i;
    DAC_Wave_Value=2.0f/V_Now*4096.0f;
}

