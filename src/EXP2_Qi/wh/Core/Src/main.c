/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"
#include "adc.h"
#include "dac.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include <stdio.h>
#include <string.h>
#include "ADC_myself.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#define FRAME_HEADER_1  0xA5
#define FRAME_HEADER_2  0x5A
#define CMD_KEY_VALUE   0x83
#define RX_BUFFER_SIZE  20

uint8_t rx_buffer[RX_BUFFER_SIZE];
uint8_t rx_index = 0;
uint8_t parsing_state = 0;
uint8_t expected_length = 0;

void ProcessKeyValue(uint16_t key_value);
void Send_ADC_To_Serial_LCD(void);
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

uint32_t dac_value; 
uint32_t dac_2v = 3950;// 2.414V GPIO输出
uint32_t dac_5v = 2265; //1.382V GPIO输出
uint32_t dac_7_5v = 880; //0.535V GPIO输出
#define DAC_STEP 55         // 每步进0.0322V (理论上0.1V Vout 步进)
uint32_t dac_4ou_value;
uint32_t dac_4ou_2v = 3900;// 2.414V GPIO输出
uint32_t dac_4ou_5v = 2100; //1.382V GPIO输出
uint32_t dac_4ou_7_5v = 80; //0.535V GPIO输出
#define DAC_STEP_4ou 60

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
uint16_t key_value = 0;
uint32_t last_tick = 0;  // 在 main 中定义变量做定时判断

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_DAC_Init();
  MX_USART6_UART_Init();
  MX_TIM6_Init();
  MX_ADC1_Init();
  MX_USART1_UART_Init();
	HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
	HAL_ADC_Start(&hadc1);
  /* USER CODE BEGIN 2 */
	//char message[20] = "";

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	HAL_TIM_Base_Start(&htim6);
	

  while (1)
  {
		
 // 非阻塞接收，确保不会卡住主循环
    if (HAL_UART_Receive(&huart6, &rx_buffer[rx_index], 1, 0) == HAL_OK) {
        switch (parsing_state) {
            case 0:
                if (rx_buffer[rx_index] == FRAME_HEADER_1)
                    parsing_state = 1;
                else
                    rx_index = 0;
                break;

            case 1:
                if (rx_buffer[rx_index] == FRAME_HEADER_2)
                    parsing_state = 2;
                else
                    parsing_state = rx_index = 0;
                break;

            case 2:
                expected_length = rx_buffer[rx_index];
                if (expected_length != 6 || rx_index + expected_length >= RX_BUFFER_SIZE)
                    parsing_state = rx_index = 0;
                else
                    parsing_state = 3;
                break;

            case 3:
                if (++rx_index >= 3 + expected_length) {
                    if (rx_buffer[3] == CMD_KEY_VALUE) {
                        key_value = (rx_buffer[7] << 8) | rx_buffer[8];
                        ProcessKeyValue(key_value);
                    }
                    parsing_state = rx_index = 0;
                }
                continue;
        }
        rx_index++;
    }
// 每500ms更新ADC电压值到串口屏（实时采集）
    if (HAL_GetTick() - last_tick >= 500) {
        Send_ADC_To_Serial_LCD();
        last_tick = HAL_GetTick();
    }
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 205;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
void ProcessKeyValue(uint16_t key_value) {
//	char message[20] = "";

  if (key_value == 0x0001) {
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
		}
	else if (key_value == 0x0002) {
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET);
	}
	else if (key_value == 0x0003) {
		
	//HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1,DAC_ALIGN_12B_R,dac_2v);
	//dac_value = 3950;
		HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1,DAC_ALIGN_12B_R,dac_4ou_2v);
		dac_4ou_value = 4000;
	}
	else if (key_value == 0x0004) {
		
	//HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1,DAC_ALIGN_12B_R,dac_5v);
	//dac_value = 2265;
		HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1,DAC_ALIGN_12B_R,dac_4ou_5v);
		dac_4ou_value = 2165;
	}
	else if (key_value == 0x0005) {
		
	//HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1,DAC_ALIGN_12B_R,dac_7_5v);
		HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1,DAC_ALIGN_12B_R,dac_4ou_7_5v);
	//dac_value = 880;
		dac_4ou_value = 80;
	}
	else if (key_value == 0x0006) {
	//if (dac_value < 800) {
  //    dac_value = 880;
  //    }
	//else dac_value -= DAC_STEP;
	//HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1,DAC_ALIGN_12B_R,dac_value);
		if(dac_4ou_value < 80){
			dac_4ou_value = 80;
		}
		else dac_4ou_value -= DAC_STEP_4ou;
		HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1,DAC_ALIGN_12B_R,dac_4ou_value);
   
	}
	else if (key_value == 0x0007) {
  //if (dac_value > 4000) {
  //     dac_value = 3950;
  //     }
	//else dac_value += DAC_STEP;
	//HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1,DAC_ALIGN_12B_R,dac_value);
		if(dac_4ou_value > 4000){
			dac_4ou_value = 4000;
		}
		else dac_4ou_value -= DAC_STEP_4ou;	
		HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1,DAC_ALIGN_12B_R,dac_4ou_value);
    
	}


}
void Send_ADC_To_Serial_LCD(void)
{
    float voltage = ADC_getvoltage();  // 正确调用
    uint16_t scaled_value = (uint16_t)(voltage * 1000)*5;  // 转成整数显示

    uint8_t frame[8];
    frame[0] = 0xA5;
    frame[1] = 0x5A;
    frame[2] = 0x05;
    frame[3] = 0x82;
    frame[4] = 0x00;
    frame[5] = 0x0A;
    frame[6] = (scaled_value >> 8) & 0xFF;
    frame[7] = scaled_value & 0xFF;

    HAL_UART_Transmit(&huart6, frame, 8, HAL_MAX_DELAY);
}


#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
