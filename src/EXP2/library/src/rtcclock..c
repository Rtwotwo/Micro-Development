/**
 * @file rtcclock..c
 * @brief
 * @author HeYingmeng (XiaoMengmeng@lanmeng.com)
 * @version 1.0
 * @date 2022-05-10
 *
 *   * @copyright Copyright (c) 2022 ���ȵ���
 *   * All rights reserved.
 *
 *   * This software is licensed under terms that can be found in the LICENSE file
 *   * in the root directory of this software component.
 *   * If no LICENSE file comes with this software, it is provided GPL3.0.
 *
 * @par �޸���־:
 */
#include "rtc.h"
#include "rtcclock.h"

RTC_DateTypeDef GetDate;  //��ȡ���ڽṹ��
RTC_TimeTypeDef GetTime;   //��ȡʱ��ṹ��
RTC_DateTypeDef SetDate;  //�������ڽṹ��
RTC_TimeTypeDef SetTime;   //����ʱ��ṹ��
/**
 * @brief
 * �����RTC����Ϊ2015-06-01����һ18:56:00
 * ���ڷ�������ָ�A5 5A 0A 80 1F 5A 15 06 01 00 18 56 00
 * VGUS�����Զ��������ڣ���дʱ��ʱ���ڿ���д����ֵ��
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
