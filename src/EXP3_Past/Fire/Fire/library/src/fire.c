/**
 * ���ȵ��� ��Ȩ����  *
 * @FilePath: fire.c
 * @Author: ������
 * @Email: XiaoMengmeng@lanmeng.com
 * @Version: 1.0
 * @LastEditors:
 * @LastEditTime: 2022-06-09 10:24:47
 * @Language: C/C++
 * @Coding: GB2312
 * @  * Copyright: 2022 ���ȵ��� All Rights Reserved.
 * @  *  *
 * @  * This software is licensed under terms that can be found in the LICENSE file
 * @  * in the root directory of this software component.
 * @  * If no LICENSE file comes with this software, it is provided GPL3.0.
 * @  *  *
 * @Description:���洫��������
 * copyright @2022(c) GPL-3.0  *
 * !���������ע�⣬��Щʱ����Ҫ�Լ��ṩ������
 * ?(��Ȼ˵������ѹ���ò�����ô���ӵĵ�·��
 * �ҳ��ϣ��Լ�����һ�������źţ����·��ĺ�**
 * �²۽���
*/

#include "fire.h"
#include "usart.h"
#include "adc.h"
#include "stdio.h"
#include "tim.h"
#include "uartscreen.h"

#define DEFAULT_BUZZER_THRESHOLD 4

uint8_t buzzer_threshold = DEFAULT_BUZZER_THRESHOLD;									// >=, ��
const uint8_t buzzer[6] = { 0xA5, 0x5A, 0x03, 0x80, 0x02, 0x0FF };
uint8_t is_sounded = 0;
uint16_t adc_value = 0x0000;


//����ֱ�Ϊ1-10M��ʱ�ĵ�ѹֵ
//����ֵΪʵ����ֵ����Ҫ����ʵ��������е���
//���ð��ҳ�����Щ��ֵ����Ϲ��ģ�ԭ������ȫ���ˣ�
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
 * @brief ��ʼ�����洫����������PWM,DMA+ADC��
 */
void fire_Init(void)
{
    // ADC_DMA����
    HAL_ADC_Start_IT(&hadc1);
    // ����100HZ ռ�ձ�50% �Զ�����
    HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
	__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,5000);
    //����һ�д���ûʲô��
    HAL_TIM_Base_Start_IT(&htim3);
}

/**
 * @brief ��ȡͨ��ch��ת��ֵ��ȡtimes��,Ȼ��ƽ��
 * @param times ��ȡ����
 * @return uint16_t ͨ��ch��times��ת�����ƽ��ֵ
 */
uint16_t Get_Adc_Average(uint16_t times)
{
	uint32_t temp_val=0;
	uint8_t t;
	for(t=0;t<times;t++)
	{
        HAL_ADC_Start(&hadc1);
        HAL_ADC_PollForConversion(&hadc1, 5);    //�ȴ�ת����ɣ��ڶ���������ʾ��ʱʱ�䣬��λms
        if(HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc1), HAL_ADC_STATE_REG_EOC))
        {
            temp_val += HAL_ADC_GetValue(&hadc1);
        }
    }
	return temp_val/times;
}
/**
 * @brief ����1���͵�ǰADC��ֵ��������
 */
void debug_fire_value(void)
{
    printf("��ǰADC�ɼ�ֵΪ:%d\r\n", adc_value);
}

/**
 * @brief �������ֵ���͵�����������
 * @param reg �������Ĵ�����ֵ
 */
void transmit_Fire(uint16_t reg)
{
    uint16_t level = getFireLevel() + 1;

    uint8_t fire[2];
    fire[0] = level / 256;
    fire[1] = level % 256;
    VGUS_WriteData(2, reg, fire);

		// ������
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
 * @brief ��ȡ����ȼ�
 * @return uint8_t ����ȼ� 0Ϊ1M 9Ϊ10M
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
 * @brief ������洮�������ܵĺ�������ʾ�����С�Լ�ͼ�꣩
 */
void fireScreenFunction(void)
{
		transmit_Alarm(0x030a);
    transmit_Fire(0x030b);
    // setIconImage(0x0318,(uint16_t)getFireLevel());
		
}
