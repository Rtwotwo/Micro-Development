/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    dac.c
  * @brief   This file provides code for the configuration
  *          of the DAC instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "dac.h"

/* USER CODE BEGIN 0 */
#include <math.h>

// 准备一个正弦波表
#define SINE_SAMPLE_COUNT 100

// 四种波形数组
uint16_t StandardSine[SINE_SAMPLE_COUNT];
uint16_t TopClippedSine[SINE_SAMPLE_COUNT];
uint16_t BottomClippedSine[SINE_SAMPLE_COUNT];
uint16_t BothClippedSine[SINE_SAMPLE_COUNT];

// 当前正在输出的波形指针
uint16_t *CurrentWave = StandardSine;

/* USER CODE END 0 */

DAC_HandleTypeDef hdac;
DMA_HandleTypeDef hdma_dac1;

/* DAC init function */
void MX_DAC_Init(void)
{

  /* USER CODE BEGIN DAC_Init 0 */

  /* USER CODE END DAC_Init 0 */

  DAC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN DAC_Init 1 */

  /* USER CODE END DAC_Init 1 */

  /** DAC Initialization
  */
  hdac.Instance = DAC;
  if (HAL_DAC_Init(&hdac) != HAL_OK)
  {
    Error_Handler();
  }

  /** DAC channel OUT1 config
  */
  sConfig.DAC_Trigger = DAC_TRIGGER_T6_TRGO;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DAC_Init 2 */

  /* USER CODE END DAC_Init 2 */

}

void HAL_DAC_MspInit(DAC_HandleTypeDef* dacHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(dacHandle->Instance==DAC)
  {
  /* USER CODE BEGIN DAC_MspInit 0 */

  /* USER CODE END DAC_MspInit 0 */
    /* DAC clock enable */
    __HAL_RCC_DAC_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**DAC GPIO Configuration
    PA4     ------> DAC_OUT1
    */
    GPIO_InitStruct.Pin = GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* DAC DMA Init */
    /* DAC1 Init */
    hdma_dac1.Instance = DMA1_Stream5;
    hdma_dac1.Init.Channel = DMA_CHANNEL_7;
    hdma_dac1.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_dac1.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_dac1.Init.MemInc = DMA_MINC_ENABLE;
    hdma_dac1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_dac1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_dac1.Init.Mode = DMA_CIRCULAR;
    hdma_dac1.Init.Priority = DMA_PRIORITY_HIGH;
    hdma_dac1.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_dac1) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(dacHandle,DMA_Handle1,hdma_dac1);

  /* USER CODE BEGIN DAC_MspInit 1 */

  /* USER CODE END DAC_MspInit 1 */
  }
}

void HAL_DAC_MspDeInit(DAC_HandleTypeDef* dacHandle)
{

  if(dacHandle->Instance==DAC)
  {
  /* USER CODE BEGIN DAC_MspDeInit 0 */

  /* USER CODE END DAC_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_DAC_CLK_DISABLE();

    /**DAC GPIO Configuration
    PA4     ------> DAC_OUT1
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_4);

    /* DAC DMA DeInit */
    HAL_DMA_DeInit(dacHandle->DMA_Handle1);
  /* USER CODE BEGIN DAC_MspDeInit 1 */

  /* USER CODE END DAC_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
void GenerateWaves(void)
{
    // 计算削波阈值(基于振幅的90%)
		float DAC_CENTER_VALUE = 2047.5;	// DAC中心值(0-4095)
		float AMPLITUDE = 1638.0;		// 振幅大小(最大为DAC_CENTER_VALUE)
    const float clipThreshold = AMPLITUDE * 0.9f;
    const float maxValue = DAC_CENTER_VALUE + AMPLITUDE;
    const float minValue = DAC_CENTER_VALUE - AMPLITUDE;

    for (int i = 0; i < SINE_SAMPLE_COUNT; i++)
    {
        // 生成正弦波角度
        float angle = (2.0f * 3.1415926f * i) / SINE_SAMPLE_COUNT;
        
        // 生成标准正弦波
        float sin_val = sinf(angle); // -1 ~ 1
        uint16_t dac_val = (uint16_t)(DAC_CENTER_VALUE + sin_val * AMPLITUDE);
        
        // 确保不超出范围
        dac_val = (dac_val > 4095) ? 4095 : dac_val;
        dac_val = (dac_val < 0) ? 0 : dac_val;
        
        StandardSine[i] = dac_val;

        // 顶部失真（削去超过90%的部分）
        TopClippedSine[i] = (dac_val > (DAC_CENTER_VALUE + clipThreshold)) 
                          ? (uint16_t)(DAC_CENTER_VALUE + clipThreshold) 
                          : dac_val;

        // 底部失真（抬高低于-90%的部分）
        BottomClippedSine[i] = (dac_val < (DAC_CENTER_VALUE - clipThreshold)) 
                             ? (uint16_t)(DAC_CENTER_VALUE - clipThreshold) 
                             : dac_val;

        // 双向失真（同时削去顶部和底部）
        if (dac_val > (DAC_CENTER_VALUE + clipThreshold))
            BothClippedSine[i] = (uint16_t)(DAC_CENTER_VALUE + clipThreshold);
        else if (dac_val < (DAC_CENTER_VALUE - clipThreshold))
            BothClippedSine[i] = (uint16_t)(DAC_CENTER_VALUE - clipThreshold);
        else
            BothClippedSine[i] = dac_val;
    }
}
void DAC_Start_Output(void)
{
  HAL_DAC_Start_DMA(&hdac,
                    DAC_CHANNEL_1,
                    (uint32_t *)CurrentWave,  // 当前波形数组
                    SINE_SAMPLE_COUNT,
                    DAC_ALIGN_12B_R);
}
void DAC_Stop_Output(void)
{
  HAL_DAC_Stop_DMA(&hdac, DAC_CHANNEL_1);
}
void DAC_Switch_Waveform(uint8_t mode)
{
    DAC_Stop_Output(); // 先停止

    switch (mode)
    {
    case 0:
        CurrentWave = StandardSine;
        break;
    case 1:
        CurrentWave = TopClippedSine;
        break;
    case 2:
        CurrentWave = BottomClippedSine;
        break;
    case 3:
        CurrentWave = BothClippedSine;
        break;
    default:
        CurrentWave = StandardSine;
        break;
    }
    DAC_Start_Output(); // 再重新开始
}

/* USER CODE END 1 */
