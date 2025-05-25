/**
 * ���ȵ��� ��Ȩ����  *
 * @FilePath: current.c
 * @Author: ������
 * @Email: XiaoMengmeng@lanmeng.com
 * @Version: 1.0
 * @LastEditors:
 * @LastEditTime: 2022-05-24 11:17:45
 * @  * Copyright: 2022 ���ȵ��� All Rights Reserved.
 * @  *  *
 * @  * This software is licensed under terms that can be found in the LICENSE file
 * @  * in the root directory of this software component.
 * @  * If no LICENSE file comes with this software, it is provided GPL3.0.
 * @  *  *
 * @Description:
 * copyright @2022(c) GPL-3.0  *
*/
#include "current.h"
#include "adc.h"

#define NUM 16
uint16_t pool[NUM];
uint16_t current = 0;
void startGetCurrent(void)
{
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, 1);
}
/**
 * @brief Get the Current object ���ص���ֵ����λ1mA
 * @return uint16_t
 */
float returnCurrent(void)
{
    float temp;
    temp = 2.5 * current * 1000 / 4096.0 / Register_Current - 20.0;
		// temp = 3.3 * current * 1000 / 4096.0 / Register_Current;
		if(temp < 5.0)
			temp = 0.0;
    return temp;
}
/**
 * @brief ADC�жϻص�����
 * @param hadc
 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)    //ADCת����ɻص�
{
    // HAL_ADC_Stop_IT(&hadc1);    //�ر�ADC
    // current=HAL_ADC_GetValue(&hadc1);//��ȡADCת����ֵ
}

/**����ƽ���˲����������ȶ��ض�ȡ��������ֹƯ��*/

/**
 * @brief ��������ָ�--�����±�
 * @param unsorted
 * @param low
 * @param high
 * @return int
 */
int partition(float unsorted[], int low, int high)
{
    int pivot = unsorted[low];
    while(low < high)
    {
        while((low < high) && (unsorted[high] >= pivot))
        --high;
        unsorted[low] = unsorted[high];
        while((low < high) && (unsorted[low] <= pivot))
        ++low;
        unsorted[high] = unsorted[low];
    }
    unsorted[low] = pivot;
    return low;
}
/**
 * @brief ���������㷨
 * @param unsorted
 * @param low
 * @param high
 */
void quickSort(float unsorted[], int low, int high)
{
    int loc = 0; //����λ���±�
    if(low < high)
    {
        loc = partition(unsorted, low, high);
        quickSort(unsorted, low, loc -1);
        quickSort(unsorted, loc + 1, high);
    }
}


/*����ƽ���˲���*/

//N>=3
void smooth_filter(void)
{
    static uint16_t i = 0, j;
    static float buffer[NUM];
    uint16_t seat;
    uint16_t N = NUM;
    float sum;
    startGetCurrent();
    pool[i++] = HAL_ADC_GetValue(&hadc1);//��ȡADCת����ֵ
    for(j = 0;j<N;j++)
		{
			buffer[j] = pool[j];
		}
		if(i == N)
		{
			i = 0;
		}
		quickSort(buffer,0, NUM-1);

		// ȥ�������С��ƽ��
		for(seat = 0;seat < N; seat ++)
		{
			sum += pool[seat];
		}
		sum = sum - buffer[0]- buffer[1]- buffer[2]- buffer[3] - buffer[NUM-1] - buffer[NUM-2] - buffer[NUM-3] - buffer[NUM-4];
		//return sum/(N-8);
		current = sum / (N - 8);
}
