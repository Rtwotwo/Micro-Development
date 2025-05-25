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
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include ".\delay\delay.h"
#include ".\uart_UI\uart_gui.h"
#include ".\XL1509_EN\XL1509_enable.h"
#include ".\XL1509_DAC\XL1509_DAC_Control.h"
#include ".\XL1509_CurrentTest\XL1509_Current.h"
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
uint8_t XL1509_State=1;
uint8_t XL1509_State_Change=0;
uint8_t voltage_State=0;//0表示不变，1表示步进增大，2表示步进变小，在主函数里

float voltage_adc_dac=2.500f;//ADC和DAC的实际电平，需要校准
float Output_Voltage=5.00f;//此时的电源输出电平
uint16_t Output_Voltage_int=500;

float Set_dac_voltage=0;
uint16_t Set_dac_value=0;

float Now_Current=0.00f;
float Top_Value=8.568f;
float Top_Value1=8.60f;
float bujing=3.05f;
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
  MX_USART6_UART_Init();
  MX_ADC1_Init();
  MX_DAC_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
    Set_XL1509_ENPIN(XL1509_State);
    UI_UART_Init();
    Get_actual_voltage();//得到此时真正的模拟电源电压
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
      if(XL1509_State_Change==1)
      {
          XL1509_State_Change=0;
          XL1509_State=XL1509_State?0:1;
//          Correct_UI_Button(XL1509_State);
          if(XL1509_State==1)
          {
              Set_XL1509_ENPIN(XL1509_State);
              Stop_ADC_Current();
              Stop_DAC_Output();
              Correct_UI_current(0.0f);
              Correct_UI_voltage(Output_Voltage_int);
          }
          else if(XL1509_State==0)
          {
              Set_Enable_DAC_Output();
              Correct_UI_voltage(Output_Voltage_int);
              Set_XL1509_ENPIN(XL1509_State);
              Start_ADC_Current();
          }
          Correct_UI_Button(XL1509_State);
      }
      
      if(voltage_State==1)//电压步进增大0.1V
      {
          Output_Voltage+=0.1f;
          Output_Voltage_int+=10;
          if(Output_Voltage_int>=750)
          {
              Output_Voltage_int=750;
          }
          Output_Voltage=Output_Voltage_int/100.0f;
          
          if(Output_Voltage>=7.500f)
          {
              Output_Voltage=7.500f;
          }
          
          if(XL1509_State==0)
          {
            Set_Enable_DAC_Output();
          }
          Correct_UI_voltage(Output_Voltage_int);
          voltage_State=0;
      }
      else if(voltage_State==2)//电压步进减小0.1V
      {
          Output_Voltage-=0.1f;
          Output_Voltage_int-=10;
          if(Output_Voltage_int<=250)
          {
              Output_Voltage_int=250;
          }
          Output_Voltage=Output_Voltage_int/100.0f;
          
          if(Output_Voltage<=2.500f)
          {
              Output_Voltage=2.500f;
          }
          
          if(XL1509_State==0)
          {
            Set_Enable_DAC_Output();
          }
          Correct_UI_voltage(Output_Voltage_int);
          voltage_State=0;
      }
      
      if(XL1509_State==0)
      {
          
      }
      
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
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
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
