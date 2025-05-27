/**
 * 蓝萌电子 版权所有  *
 * @FilePath: fire.c
 * @Author: 沈玲玲
 * @Email: XiaoMengmeng@lanmeng.com
 * @Version: 1.0
 * @LastEditors:
 * @LastEditTime: 2022-06-09 10:24:47
 * @Language: C/C++
 * @Coding: GB2312
 * @  * Copyright: 2022 蓝萌电子 All Rights Reserved.
 * @  *  *
 * @  * This software is licensed under terms that can be found in the LICENSE file
 * @  * in the root directory of this software component.
 * @  * If no LICENSE file comes with this software, it is provided GPL3.0.
 * @  *  *
 * @Description:火焰传感器代码
 * copyright @2022(c) GPL-3.0  *
 * !打电赛必须注意，有些时候需要自己提供激励的
 * ?(虽然说这玩意压根用不着这么复杂的电路）
 * 我承认，自己生成一个方波信号，这电路真的很**
 * 吐槽结束
*/

#include "fire.h"
#include "usart.h"
#include "adc.h"
#include "stdio.h"
#include "tim.h"
#include "uartscreen.h"

#define DEFAULT_BUZZER_THRESHOLD 4

uint8_t buzzer_threshold = DEFAULT_BUZZER_THRESHOLD;									// >=, 响
const uint8_t buzzer[6] = { 0xA5, 0x5A, 0x03, 0x80, 0x02, 0x0FF };
uint8_t is_sounded = 0;
uint16_t adc_value = 0x0000;


//电阻分别为1-10MΩ时的电压值
//此数值为实验数值，需要根据实际情况进行调整
//（好吧我承认这些数值是我瞎编的，原来的我全忘了）
const uint16_t fire_level[10] ={
    1711,1151,902,760,668,
    604,556,520,493,0
};


void buzzer_5s(void)
{
		HAL_UART_Transmit_IT(&huart6, buzzer, 0x06);
		HAL_Delay(2560);
		HAL_UART_Transmit_IT(&huart6, buzzer, 0x06);
}

/**
 * @brief 初始化火焰传感器（开启PWM,DMA+ADC）
 */
void fire_Init(void)
{
    // ADC_DMA开启
    HAL_ADC_Start_IT(&hadc1);
    // 方波100HZ 占空比50% 自动生成
    HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
	__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,5000);
    //以下一行代码没什么用
    HAL_TIM_Base_Start_IT(&htim3);
}

/**
 * @brief 获取通道ch的转换值，取times次,然后平均
 * @param times 获取次数
 * @return uint16_t 通道ch的times次转换结果平均值
 */
uint16_t Get_Adc_Average(uint16_t times)
{
	uint32_t temp_val=0;
	uint8_t t;
	for(t=0;t<times;t++)
	{
        HAL_ADC_Start(&hadc1);
        HAL_ADC_PollForConversion(&hadc1, 5);    //等待转换完成，第二个参数表示超时时间，单位ms
        if(HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc1), HAL_ADC_STATE_REG_EOC))
        {
            temp_val += HAL_ADC_GetValue(&hadc1);
        }
    }
	return temp_val/times;
}
/**
 * @brief 串口1发送当前ADC的值到电脑上
 */
void debug_fire_value(void)
{
    printf("当前ADC采集值为:%d\r\n", adc_value);
}

/**
 * @brief 将火焰的值发送到串口屏上面
 * @param reg 串口屏寄存器的值
 */
void transmit_Fire(uint16_t reg)
{
    uint16_t level = getFireLevel() + 1;

    uint8_t fire[2];
    fire[0] = level / 256;
    fire[1] = level % 256;
    VGUS_WriteData(2, reg, fire);

		// 蜂鸣器
		if(level < buzzer_threshold)
		{
				is_sounded = 0;
		}
		else if(!is_sounded)
		{
				buzzer_5s();
				is_sounded = 1;
		}		
}

void transmit_Alarm(uint16_t reg)
{
    uint8_t alarm[2];
    alarm[0] = 0;
    alarm[1] = buzzer_threshold;
    VGUS_WriteData(2, reg, alarm);
}

/**
 * @brief 获取火焰等级
 * @return uint8_t 火焰等级 0为1M 9为10M
 */
uint8_t getFireLevel(void)
{
    uint8_t level = 0;
    while(level<10)
    {
        if(adc_value >= fire_level[level])
        {
            return level;
        }
				level++;
    }
		return level;
}
/**
 * @brief 满足火焰串口屏功能的函数（显示火焰大小以及图标）
 */
void fireScreenFunction(void)
{
		transmit_Alarm(0x030a);
    transmit_Fire(0x030b);
    // setIconImage(0x0318,(uint16_t)getFireLevel());
		
}
