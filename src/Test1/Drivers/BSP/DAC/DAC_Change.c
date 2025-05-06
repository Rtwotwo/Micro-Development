#include "stm32f4xx.h"                  // Device header
#include "math.h"
#include "dac.h"
#include ".\CD4053\CD4053.h" 
#include "tim.h"
#define SAMPLE_SIZE 80

uint16_t Normal_Wave[SAMPLE_SIZE];
uint16_t Top_Distort[SAMPLE_SIZE];
uint16_t Bottom_Distort[SAMPLE_SIZE];
uint16_t Bi_Distort[SAMPLE_SIZE];

void Generate_Normal_Wave(void) {
    for (int i = 0; i < SAMPLE_SIZE; i++) {
        Normal_Wave[i] = (uint16_t)(1750 + 1750 * sin(2 * 3.1415926f * i / SAMPLE_SIZE));
				//增加上偏移，防止底部失真
				Normal_Wave[i] += (uint16_t)150;
    }
}

void Generate_Top_Distort(void) {
    uint16_t value=700;
    uint16_t k=0;
    for (int i = 0; i < SAMPLE_SIZE; i++) {
        k=(uint16_t)(1750+value/2+ (1750+value/2)* sin(2 * 3.1415926f * i / SAMPLE_SIZE));
        Top_Distort[i]=((k>=3500)?3500:k);
				Normal_Wave[i] += (uint16_t)150;
    }
}

void Generate_Bottom_Distort(void) {
    uint16_t value=700;
    int16_t k=0;
    for (int i = 0; i < SAMPLE_SIZE; i++) {
        k=(1750-value/2+ (1750+value/2)* sin(2 * 3.1415926f * i / SAMPLE_SIZE));
        Bottom_Distort[i]=((k<=0)?0:k);
    }
}

void Generate_Bi_Distort(void) {
    uint16_t value=700;
    int16_t k=0;
    for (int i = 0; i < SAMPLE_SIZE; i++) {
        k=(1750+ (1750+value)* sin(2 * 3.1415926f * i / SAMPLE_SIZE));
        if(k>=3500)
        {
            Bi_Distort[i]=3500;            
        }
        else if(k<=0)
        {
            Bi_Distort[i]=0;            
        }
        else
        {
            Bi_Distort[i]=k;
        }
    }
}

void Wave_data_Init(void)
{
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

