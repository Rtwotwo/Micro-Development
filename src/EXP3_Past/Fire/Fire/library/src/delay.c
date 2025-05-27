/**
 * 蓝萌电子 版权所有  *
 * @FilePath: delay.c
 * @Author: 沈玲玲
 * @Email: XiaoMengmeng@lanmeng.com
 * @Version: 1.0
 * @LastEditors:
 * @LastEditTime: 2022-06-09 10:22:39
 * @Language: C/C++
 * @Coding: GB2312
 * @  * Copyright: 2022 蓝萌电子 All Rights Reserved.
 * @  *  *
 * @  * This software is licensed under terms that can be found in the LICENSE file
 * @  * in the root directory of this software component.
 * @  * If no LICENSE file comes with this software, it is provided GPL3.0.
 * @  *  *
 * @Description:
 * copyright @2022(c) GPL-3.0  *
*/

#include "delay.h"

uint32_t fac_us;

void HAL_Delay_us_init(uint32_t SYS_CLK)//单位:MHZ
{
    fac_us = SYS_CLK;
}

void HAL_Delay_us(uint32_t nus)
{

    uint32_t ticks;
    uint32_t told,tnow,tcnt=0;
    uint32_t reload=SysTick->LOAD;
    ticks=nus*fac_us;
    told=SysTick->VAL;
    while(1)
    {
        tnow=SysTick->VAL;
        if(tnow!=told)
        {
            if(tnow<told)tcnt+=told-tnow;
            else tcnt+=reload-tnow+told;
            told=tnow;
            if(tcnt>=ticks)break;
        }
    };
}

void HAL_Delay_ms(uint32_t nms)
{
    HAL_Delay_us(1000 * nms);
}
