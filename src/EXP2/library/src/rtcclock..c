/**
 * @file rtcclock..c
 * @brief
 * @author HeYingmeng (XiaoMengmeng@lanmeng.com)
 * @version 1.0
 * @date 2022-05-10
 *
 *   * @copyright Copyright (c) 2022 蓝萌电子
 *   * All rights reserved.
 *
 *   * This software is licensed under terms that can be found in the LICENSE file
 *   * in the root directory of this software component.
 *   * If no LICENSE file comes with this software, it is provided GPL3.0.
 *
 * @par 修改日志:
 */
#include "rtc.h"
#include "rtcclock.h"

RTC_DateTypeDef GetDate;  //获取日期结构体
RTC_TimeTypeDef GetTime;   //获取时间结构体
RTC_DateTypeDef SetDate;  //设置日期结构体
RTC_TimeTypeDef SetTime;   //设置时间结构体
/**
 * @brief
 * 例如把RTC设置为2015-06-01星期一18:56:00
 * 串口发送以下指令：A5 5A 0A 80 1F 5A 15 06 01 00 18 56 00
 * VGUS屏会自动换算星期，改写时间时星期可以写任意值。
 */
void setDateTime(void)
{
    HAL_RTC_SetTime(&hrtc, &SetTime, RTC_FORMAT_BCD);
	HAL_RTC_SetDate(&hrtc, &SetDate, RTC_FORMAT_BCD);

}

void getDateTime(void)
{
    HAL_RTC_GetTime(&hrtc, &GetTime, RTC_FORMAT_BCD);
	HAL_RTC_GetDate(&hrtc, &GetDate, RTC_FORMAT_BCD);
}

void rtcClockInit(void)
{
    SetTime.Hours = 0x08;
    SetTime.Minutes = 0x30;
    SetTime.Seconds = 0x00;
    SetDate.Year = 0x22;
    SetDate.Month = 0x05;
    SetDate.Date = 0x14;
    SetDate.WeekDay = 0x06;
    setDateTime();
}
