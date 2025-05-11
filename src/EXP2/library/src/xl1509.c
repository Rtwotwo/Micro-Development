/**
 * 蓝萌电子 版权所有  *
 * @FilePath: xl1509.c
 * @Author: 沈玲玲
 * @Email: XiaoMengmeng@lanmeng.com
 * @Version: 1.0
 * @LastEditors:
 * @LastEditTime: 2022-05-24 08:40:22
 * @  * Copyright: 2022 蓝萌电子 All Rights Reserved.
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
uint16_t now_voltage = 50; //第一次电压为5V

/*************************************************
 *                 Main    Code
 * ***********************************************/

/**
 * @brief 初始化
 */
void XL1509_Init(void)
{
    HAL_DAC_Start(&hdac, DAC1_CHANNEL_1);
}

/**
 * @brief 设置电压 单位：0.1V
 */
void XL1509_SetVoltage(uint16_t vol)
{
    //限幅
//    if(vol <20)
//    {
//        vol = 20;
//    }
//    else if(vol >75)
//    {
//        vol = 75;
//    }
	now_voltage = vol;
    //改变DAC
    HAL_DAC_SetValue(&hdac, DAC1_CHANNEL_1, DAC_ALIGN_12B_R, function(vol));
}
/**
 * @brief 计算DAC值的函数
 * @param vol 输入电压值，单位：0.1V
 * @return uint16_t 计算的DAC值
 */
uint16_t function(uint16_t vol)
{
    double temp;
    temp = vol / 10.0;
    temp += xiaoqingqing_function(vol);
	//temp += (float)Register_Current * (returnCurrent()/1000.0)*1.65;//不管了，老子调不动了
    //temp = (8.856 - temp) * 4096.0 / (3.1*2.5) + 0.5;  //+0.5为四舍五入
	temp = (8.54 - temp) * 4096.0 / (3.0*2.5) + 0.5;  //+0.5为四舍五入, 3k, Vref = 2.5V
	//temp = (8.75 - temp) * 4096.0 / (3.1*3.3) + 0.5;  //+0.5为四舍五入
	if(temp<2017.96)
	{
		temp += (temp-2017.96)/100.0;  //神仙操作
	}
	else
	{
		temp += (temp-2017.96)/60.0;  //神仙操作2
	}
    //小晴晴算法 强行解决负载问题
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
 * @brief Get the On Off object  0为关 1为开
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
 * @brief 小晴晴算法，通过电流来修正0.1Ω电阻的误差
 * @return float
 */
float xiaoqingqing_function(float vol)
{
    float tmp;
    //tmp = Register_Current * (returnCurrent() / 1000.0); //电流A *
	//tmp *= fabs(vol - 5.0 / 4) + 1;
    //tmp *= 1.65; //实验参数，不可复制
	if(returnCurrent()>300)
	{
		//tmp = vol*1.043;
        tmp = vol * 0.004;

    }
    else
	{
		tmp=0;
	}
    return tmp; //返回：电压值
}
