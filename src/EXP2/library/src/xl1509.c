/**
 * ���ȵ��� ��Ȩ����  *
 * @FilePath: xl1509.c
 * @Author: ������
 * @Email: XiaoMengmeng@lanmeng.com
 * @Version: 1.0
 * @LastEditors:
 * @LastEditTime: 2022-05-24 08:40:22
 * @  * Copyright: 2022 ���ȵ��� All Rights Reserved.
 * @  *  *
 * @  * This software is licensed under terms that can be found in the LICENSE file
 * @  * in the root directory of this software component.
 * @  * If no LICENSE file comes with this software, it is provided GPL3.0.
 * @  *  *
 * @Description:
 * copyright @2022(c) GPL-3.0  *
*/

#include "dac.h"
#include "main.h"
#include "xl1509.h"
#include "current.h"
#include "math.h"
/*************************************************
 *                 Reg    Define
 * ***********************************************/
uint16_t now_voltage = 50; //��һ�ε�ѹΪ5V

/*************************************************
 *                 Main    Code
 * ***********************************************/

/**
 * @brief ��ʼ��
 */
void XL1509_Init(void)
{
    HAL_DAC_Start(&hdac, DAC1_CHANNEL_1);
}

/**
 * @brief ���õ�ѹ ��λ��0.1V
 */
void XL1509_SetVoltage(uint16_t vol)
{
    //�޷�
//    if(vol <20)
//    {
//        vol = 20;
//    }
//    else if(vol >75)
//    {
//        vol = 75;
//    }
	now_voltage = vol;
    //�ı�DAC
    HAL_DAC_SetValue(&hdac, DAC1_CHANNEL_1, DAC_ALIGN_12B_R, function(vol));
}
/**
 * @brief ����DACֵ�ĺ���
 * @param vol �����ѹֵ����λ��0.1V
 * @return uint16_t �����DACֵ
 */
uint16_t function(uint16_t vol)
{
    double temp;
    temp = vol / 10.0;
    temp += xiaoqingqing_function(vol);
	//temp += (float)Register_Current * (returnCurrent()/1000.0)*1.65;//�����ˣ����ӵ�������
    //temp = (8.856 - temp) * 4096.0 / (3.1*2.5) + 0.5;  //+0.5Ϊ��������
	temp = (8.54 - temp) * 4096.0 / (3.0*2.5) + 0.5;  //+0.5Ϊ��������, 3k, Vref = 2.5V
	//temp = (8.75 - temp) * 4096.0 / (3.1*3.3) + 0.5;  //+0.5Ϊ��������
	if(temp<2017.96)
	{
		temp += (temp-2017.96)/100.0;  //���ɲ���
	}
	else
	{
		temp += (temp-2017.96)/60.0;  //���ɲ���2
	}
    //С�����㷨 ǿ�н����������
    //temp += xiaoqingqing_function();
    return (int)temp;
}
/**
 * @brief Get the Voltage object
 * @param vol
 * @return uint16_t
 */
uint16_t getVoltage(void)
{
    return now_voltage;
}
/**
 * @brief Get the On Off object  0Ϊ�� 1Ϊ��
 * @return uint8_t
 */
uint8_t getOnOff(void)
{
    if(HAL_GPIO_ReadPin(C_GPIO_Port,C_Pin) == GPIO_PIN_SET)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
/**
 * @brief С�����㷨��ͨ������������0.1����������
 * @return float
 */
float xiaoqingqing_function(float vol)
{
    float tmp;
    //tmp = Register_Current * (returnCurrent() / 1000.0); //����A *
	//tmp *= fabs(vol - 5.0 / 4) + 1;
    //tmp *= 1.65; //ʵ����������ɸ���
	if(returnCurrent()>300)
	{
		//tmp = vol*1.043;
        tmp = vol * 0.004;

    }
    else
	{
		tmp=0;
	}
    return tmp; //���أ���ѹֵ
}
