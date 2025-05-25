/**
 * @file key.c
 * @brief
 * @author HeYingmeng (XiaoMengmeng@lanmeng.com)
 * @version 1.0
 * @date 2022-05-12
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
#include <string.h>
#include "key.h"
#include "stdio.h"

static my_key_param_t key_param[KEY_NUMBER] = {0};  // 按键参数结构体数组

bool read_key1(void)  // 读sw1管脚电平
{
	return HAL_GPIO_ReadPin(SW1_GPIO_Port, SW1_Pin)? true : false;  // 此处添加自己的读管脚函数
}

bool read_key2(void)  // 读sw2管脚电平
{
	return HAL_GPIO_ReadPin(SW2_GPIO_Port, SW2_Pin)? true : false;  // 此处添加自己的读管脚函数
}

__weak void key0_single_click_callback(void)
{
	//printf("key0_single_click_callback\r\n");
}

__weak void key0_double_click_callback(void)
{
	//printf("key0_double_click_callback\r\n");
}

__weak void key0_long_press_callback(void)
{
	//printf("key0_long_press_callback\r\n");
}

__weak void key0_long_release_callback(void)
{
	//printf("key0_long_release_callback\r\n");
}

__weak void key0_short_long_press_callback(void)
{
	//printf("key0_short_long_press_callback\r\n");
}

__weak void key0_short_long_release_callback(void)
{
	//printf("key0_short_long_release_callback\r\n");
}

__weak void key0_long_long_press_callback(void)
{
	//printf("key0_long_long_press_callback\r\n");
}

__weak void key0_long_long_release_callback(void)
{
	//printf("key0_long_long_release_callback\r\n");
}

__weak void key1_single_click_callback(void)
{
	//printf("key1_single_click_callback\r\n");
}

__weak void key1_double_click_callback(void)
{
	//printf( "key1_double_click_callback\r\n");
}

__weak void key1_long_press_callback(void)
{
	//printf("key1_long_press_callback\r\n");
}

__weak void key1_long_release_callback(void)
{
	//printf("key1_long_release_callback\r\n");
}

__weak void key1_short_long_press_callback(void)
{
	//printf("key1_short_long_press_callback\r\n");
}

__weak void key1_short_long_release_callback(void)
{
	//printf("key1_short_long_release_callback\r\n");
}

__weak void key1_long_long_press_callback(void)
{
	//printf("key1_long_long_press_callback\r\n");
}

__weak void key1_long_long_release_callback(void)
{
	//printf("key1_long_long_release_callback\r\n");
}

void my_key_init(void)  // 按键初始化
{
	key_param[0].read_key = read_key1;
	key_param[0].key_is_press = false;
	key_param[0].count = 0;
	key_param[0].state = KEY_STATE_IDLE;
	key_param[0].event = KEY_EVENT_IDLE;
	key_param[0].single_click_callback = key0_single_click_callback;
	key_param[0].double_click_callback = key0_double_click_callback;
	key_param[0].long_press_callback = key0_long_press_callback;
	key_param[0].long_release_callback = key0_long_release_callback;
	key_param[0].short_long_press_callback = key0_short_long_press_callback;
	key_param[0].short_long_release_callback = key0_short_long_release_callback;
	key_param[0].long_long_press_callback = key0_long_long_press_callback;
	key_param[0].long_long_release_callback = key0_long_long_release_callback;

	key_param[1].read_key = read_key2;
	key_param[1].key_is_press = false;
	key_param[1].count = 0;
	key_param[1].state = KEY_STATE_IDLE;
	key_param[1].event = KEY_EVENT_IDLE;
	key_param[1].single_click_callback = key1_single_click_callback;
	key_param[1].double_click_callback = key1_double_click_callback;
	key_param[1].long_press_callback = key1_long_press_callback;
	key_param[1].long_release_callback = key1_long_release_callback;
	key_param[1].short_long_press_callback = key1_short_long_press_callback;
	key_param[1].short_long_release_callback = key1_short_long_release_callback;
	key_param[1].long_long_press_callback = key1_long_long_press_callback;
	key_param[1].long_long_release_callback = key1_long_long_release_callback;
}

void my_key_scan(void)  // 按键扫描
{
	for (uint8_t i = 0; i < KEY_NUMBER; i++)  // 轮询按键
	{
		if (key_param[i].read_key == NULL) continue;

		key_param[i].key_is_press = !key_param[i].read_key();  // 获取按键状态

		// 扫描按键状态
		switch (key_param[i].state)  // 进入状态机流程
		{
			case KEY_STATE_IDLE:  // 按键空闲状态
				if (key_param[i].key_is_press)  // 若按键被按下
				{
					key_param[i].count = 0;  // 计数清零
					key_param[i].state = KEY_STATE_FIRST_PRESS;  // 跳转到按键第一次按下状态
				}
				else
				{
					key_param[i].count = 0;  // 计数清零
					key_param[i].state = KEY_STATE_IDLE;  // 跳转到按键空闲状态
					key_param[i].event = KEY_EVENT_IDLE;  // 按键空闲事件
				}
				break;

			case KEY_STATE_FIRST_PRESS:  // 按键第一次按下状态
				if (key_param[i].key_is_press)  // 若按键被按下
				{
					if (++key_param[i].count >= KEY_SHORT_PRESS_COUNT_NUMBER)  // 若第一次按下的时间计数超过KEY_SHORT_PRESS_COUNT_NUMBER
					{
						key_param[i].count = 0;  // 计数清零
						key_param[i].state = KEY_STATE_FIRST_PRESS_VALID;  // 跳转到按键第一次按下有效状态
					}
				}
				else
				{
					// 按下时间不够，不处理，按键消抖
					key_param[i].count = 0;  // 计数清零
					key_param[i].state = KEY_STATE_IDLE;  // 跳转到按键空闲状态
					key_param[i].event = KEY_EVENT_IDLE;  // 按键空闲事件
				}
				break;

			case KEY_STATE_FIRST_PRESS_VALID:	// 按键第一次按下有效状态
				if (key_param[i].key_is_press)  // 若按键被按下
				{
					if (++key_param[i].count >= KEY_LONG_PRESS_COUNT_NUMBER - KEY_SHORT_PRESS_COUNT_NUMBER)  // 若第一次按下的时间计数超过KEY_LONG_PRESS_COUNT_NUMBER
					{
						key_param[i].count = 0;  // 计数清零
						key_param[i].state = KEY_STATE_LONG_PRESS;  // 跳转到按键长按状态
						key_param[i].event = KEY_EVENT_LONG_PRESS;  // 按键长按事件
					}
				}
				else
				{
					key_param[i].count = 0;  // 计数清零
					key_param[i].state = KEY_STATE_FIRST_RELEASE;  // 跳转到按键第一次释放状态
				}
				break;

			case KEY_STATE_FIRST_RELEASE:  // 按键第一次释放状态
				if (!key_param[i].key_is_press)  // 若按键释放
				{
					if (++key_param[i].count >= KEY_DOUBLE_CLICK_INTERVAL_COUNT_NUMBER)  // 若超过KEY_DOUBLE_CLICK_INTERVAL_COUNT_NUMBER计数时间，按键没有第二次被按下
					{
						key_param[i].count = 0;  // 计数清零
						key_param[i].state = KEY_STATE_IDLE;  // 跳转到按键空闲状态
						key_param[i].event = KEY_EVENT_SINGLE_CLICK;  // 按键单击事件
					}
				}
				else if (key_param[i].count >= KEY_SHORT_PRESS_COUNT_NUMBER && key_param[i].count < KEY_DOUBLE_CLICK_INTERVAL_COUNT_NUMBER)  // 若在间隔时间内，获取了第二次按键按下
				{
					key_param[i].count = 0;  // 计数清零
					key_param[i].state = KEY_STATE_SECOND_PRESS;  // 跳转到按键第二次按下状态
				}
				else if (key_param[i].count < KEY_SHORT_PRESS_COUNT_NUMBER) // 若在KEY_SHORT_PRESS_COUNT_NUMBER计数时间内，获取了第二次按键按下，那么此次动作忽略，做消抖处理
				{
					key_param[i].count = 0;  // 计数清零
					key_param[i].state = KEY_STATE_IDLE;  // 跳转到按键空闲状态
					key_param[i].event = KEY_EVENT_SINGLE_CLICK;  // 按键单击事件
				}
				break;

			case KEY_STATE_SECOND_PRESS:  // 按键第二次按下状态
				if (key_param[i].key_is_press)  // 若按键被按下
				{
					if (++key_param[i].count >= KEY_SHORT_PRESS_COUNT_NUMBER)  // 若第一次按下的时间计数超过KEY_SHORT_PRESS_COUNT_NUMBER
					{
						key_param[i].count = 0;  // 计数清零
						key_param[i].state = KEY_STATE_SECOND_PRESS_VALID;  // 跳转到按键第二次按下有效状态
					}
				}
				else
				{
					// 按下时间不够，不处理，按键消抖
					key_param[i].count = 0;  // 计数清零
					key_param[i].state = KEY_STATE_IDLE;  // 跳转到按键空闲状态
					key_param[i].event = KEY_EVENT_SINGLE_CLICK;  // 按键单击事件
				}
				break;

			case KEY_STATE_SECOND_PRESS_VALID:  // 按键第二次按下有效状态
				if (key_param[i].key_is_press)  // 若按键被按下
				{
					if (++key_param[i].count >= KEY_LONG_PRESS_COUNT_NUMBER - KEY_SHORT_PRESS_COUNT_NUMBER)  // 若第二次按下的时间计数超过KEY_LONG_PRESS_COUNT_NUMBER
					{
						key_param[i].count = 0;  // 计数清零
						key_param[i].state = KEY_STATE_SHORT_LONG_PRESS;  // 跳转到按键短长按状态
						key_param[i].event = KEY_EVENT_SHORT_LONG_PRESS;  // 按键短长按事件
					}
				}
				else
				{
					key_param[i].count = 0;  // 计数清零
					key_param[i].state = KEY_STATE_IDLE;  // 跳转到按键空闲状态
					key_param[i].event = KEY_EVENT_DOUBLE_CLICK;  // 按键双击事件
				}
				break;

			case KEY_STATE_SECOND_RELEASE:  // 按键第二次释放状态
				// Do nothing!
				break;

			case KEY_STATE_LONG_PRESS:
				if (key_param[i].key_is_press)  // 若按键被按下
				{
					if (++key_param[i].count >= KEY_LONG_LONG_PRESS_COUNT_NUMBER - KEY_LONG_PRESS_COUNT_NUMBER)  // 若长按下的时间计数超过KEY_LONG_LONG_PRESS_COUNT_NUMBER
					{
						key_param[i].count = 0;  // 计数清零
						key_param[i].state = KEY_STATE_LONG_LONG_PRESS;  // 跳转到按键超长按状态
						key_param[i].event = KEY_EVENT_LONG_LONG_PRESS;  // 按键超长按事件
					}
				}
				else
				{
					key_param[i].count = 0;  // 计数清零
					key_param[i].state = KEY_STATE_LONG_RELEASE;  // 跳转到按键长按释放状态
					key_param[i].event = KEY_EVENT_LONG_RELEASE;  // 按键长按释放事件
				}
				break;

			case KEY_STATE_LONG_RELEASE:
				key_param[i].count = 0;  // 计数清零
				key_param[i].state = KEY_STATE_IDLE;  // 跳转到按键空闲状态
				key_param[i].event = KEY_EVENT_IDLE;  // 按键空闲事件
				break;

			case KEY_STATE_SHORT_LONG_PRESS:
				if (key_param[i].key_is_press)  // 若按键被按下
				{
					// 短长按后，不再做处理，等待按键释放
					// Do nothing!
				}
				else
				{
					key_param[i].count = 0;  // 计数清零
					key_param[i].state = KEY_STATE_SHORT_LONG_RELEASE;  // 跳转到按键短长按释放状态
					key_param[i].event = KEY_EVENT_SHORT_LONG_RELEASE;  // 按键短长按释放事件
				}
				break;

			case KEY_STATE_SHORT_LONG_RELEASE:
				key_param[i].count = 0;  // 计数清零
				key_param[i].state = KEY_STATE_IDLE;  // 跳转到按键空闲状态
				key_param[i].event = KEY_EVENT_IDLE;  // 按键空闲事件
				break;

			case KEY_STATE_LONG_LONG_PRESS:
				if (key_param[i].key_is_press)  // 若按键被按下
				{
					// 超长按后，不再做处理，等待按键释放
					// Do nothing!
				}
				else
				{
					key_param[i].count = 0;  // 计数清零
					key_param[i].state = KEY_STATE_LONG_LONG_RELEASE;  // 跳转到按键超长按释放状态
					key_param[i].event = KEY_EVENT_LONG_LONG_RELEASE;  // 按键超长按释放事件
				}
				break;

			case KEY_STATE_LONG_LONG_RELEASE:
				key_param[i].count = 0;  // 计数清零
				key_param[i].state = KEY_STATE_IDLE;  // 跳转到按键空闲状态
				key_param[i].event = KEY_EVENT_IDLE;  // 按键空闲事件
				break;
		}

		// 扫描按键事件
		switch (key_param[i].event)  // 进入状态机流程
		{
			case KEY_EVENT_IDLE:
				// Do nothing!
				break;

			case KEY_EVENT_SINGLE_CLICK:
				if (key_param[i].single_click_callback != NULL) key_param[i].single_click_callback();
				break;

			case KEY_EVENT_DOUBLE_CLICK:
				if (key_param[i].double_click_callback != NULL) key_param[i].double_click_callback();
				break;

			case KEY_EVENT_LONG_PRESS:
				if (key_param[i].long_press_callback != NULL) key_param[i].long_press_callback();
				break;

			case KEY_EVENT_LONG_RELEASE:
				if (key_param[i].long_release_callback != NULL) key_param[i].long_release_callback();
				break;

			case KEY_EVENT_SHORT_LONG_PRESS:
				if (key_param[i].short_long_press_callback != NULL) key_param[i].short_long_press_callback();
				break;

			case KEY_EVENT_SHORT_LONG_RELEASE:
				if (key_param[i].short_long_release_callback != NULL) key_param[i].short_long_release_callback();
				break;

			case KEY_EVENT_LONG_LONG_PRESS:
				if (key_param[i].long_long_press_callback != NULL) key_param[i].long_long_press_callback();
				break;

			case KEY_EVENT_LONG_LONG_RELEASE:
				if (key_param[i].long_long_release_callback != NULL) key_param[i].long_long_release_callback();
				break;
		}

		key_param[i].event = KEY_EVENT_IDLE;  // 清除按键事件
	}
}

