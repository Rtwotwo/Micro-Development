/**
 * @file key.c
 * @brief
 * @author HeYingmeng (XiaoMengmeng@lanmeng.com)
 * @version 1.0
 * @date 2022-05-12
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
#include <string.h>
#include "key.h"
#include "stdio.h"

static my_key_param_t key_param[KEY_NUMBER] = {0};  // ���������ṹ������

bool read_key1(void)  // ��sw1�ܽŵ�ƽ
{
	return HAL_GPIO_ReadPin(SW1_GPIO_Port, SW1_Pin)? true : false;  // �˴�����Լ��Ķ��ܽź���
}

bool read_key2(void)  // ��sw2�ܽŵ�ƽ
{
	return HAL_GPIO_ReadPin(SW2_GPIO_Port, SW2_Pin)? true : false;  // �˴�����Լ��Ķ��ܽź���
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

void my_key_init(void)  // ������ʼ��
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

void my_key_scan(void)  // ����ɨ��
{
	for (uint8_t i = 0; i < KEY_NUMBER; i++)  // ��ѯ����
	{
		if (key_param[i].read_key == NULL) continue;

		key_param[i].key_is_press = !key_param[i].read_key();  // ��ȡ����״̬

		// ɨ�谴��״̬
		switch (key_param[i].state)  // ����״̬������
		{
			case KEY_STATE_IDLE:  // ��������״̬
				if (key_param[i].key_is_press)  // ������������
				{
					key_param[i].count = 0;  // ��������
					key_param[i].state = KEY_STATE_FIRST_PRESS;  // ��ת��������һ�ΰ���״̬
				}
				else
				{
					key_param[i].count = 0;  // ��������
					key_param[i].state = KEY_STATE_IDLE;  // ��ת����������״̬
					key_param[i].event = KEY_EVENT_IDLE;  // ���������¼�
				}
				break;

			case KEY_STATE_FIRST_PRESS:  // ������һ�ΰ���״̬
				if (key_param[i].key_is_press)  // ������������
				{
					if (++key_param[i].count >= KEY_SHORT_PRESS_COUNT_NUMBER)  // ����һ�ΰ��µ�ʱ���������KEY_SHORT_PRESS_COUNT_NUMBER
					{
						key_param[i].count = 0;  // ��������
						key_param[i].state = KEY_STATE_FIRST_PRESS_VALID;  // ��ת��������һ�ΰ�����Ч״̬
					}
				}
				else
				{
					// ����ʱ�䲻������������������
					key_param[i].count = 0;  // ��������
					key_param[i].state = KEY_STATE_IDLE;  // ��ת����������״̬
					key_param[i].event = KEY_EVENT_IDLE;  // ���������¼�
				}
				break;

			case KEY_STATE_FIRST_PRESS_VALID:	// ������һ�ΰ�����Ч״̬
				if (key_param[i].key_is_press)  // ������������
				{
					if (++key_param[i].count >= KEY_LONG_PRESS_COUNT_NUMBER - KEY_SHORT_PRESS_COUNT_NUMBER)  // ����һ�ΰ��µ�ʱ���������KEY_LONG_PRESS_COUNT_NUMBER
					{
						key_param[i].count = 0;  // ��������
						key_param[i].state = KEY_STATE_LONG_PRESS;  // ��ת����������״̬
						key_param[i].event = KEY_EVENT_LONG_PRESS;  // ���������¼�
					}
				}
				else
				{
					key_param[i].count = 0;  // ��������
					key_param[i].state = KEY_STATE_FIRST_RELEASE;  // ��ת��������һ���ͷ�״̬
				}
				break;

			case KEY_STATE_FIRST_RELEASE:  // ������һ���ͷ�״̬
				if (!key_param[i].key_is_press)  // �������ͷ�
				{
					if (++key_param[i].count >= KEY_DOUBLE_CLICK_INTERVAL_COUNT_NUMBER)  // ������KEY_DOUBLE_CLICK_INTERVAL_COUNT_NUMBER����ʱ�䣬����û�еڶ��α�����
					{
						key_param[i].count = 0;  // ��������
						key_param[i].state = KEY_STATE_IDLE;  // ��ת����������״̬
						key_param[i].event = KEY_EVENT_SINGLE_CLICK;  // ���������¼�
					}
				}
				else if (key_param[i].count >= KEY_SHORT_PRESS_COUNT_NUMBER && key_param[i].count < KEY_DOUBLE_CLICK_INTERVAL_COUNT_NUMBER)  // ���ڼ��ʱ���ڣ���ȡ�˵ڶ��ΰ�������
				{
					key_param[i].count = 0;  // ��������
					key_param[i].state = KEY_STATE_SECOND_PRESS;  // ��ת�������ڶ��ΰ���״̬
				}
				else if (key_param[i].count < KEY_SHORT_PRESS_COUNT_NUMBER) // ����KEY_SHORT_PRESS_COUNT_NUMBER����ʱ���ڣ���ȡ�˵ڶ��ΰ������£���ô�˴ζ������ԣ�����������
				{
					key_param[i].count = 0;  // ��������
					key_param[i].state = KEY_STATE_IDLE;  // ��ת����������״̬
					key_param[i].event = KEY_EVENT_SINGLE_CLICK;  // ���������¼�
				}
				break;

			case KEY_STATE_SECOND_PRESS:  // �����ڶ��ΰ���״̬
				if (key_param[i].key_is_press)  // ������������
				{
					if (++key_param[i].count >= KEY_SHORT_PRESS_COUNT_NUMBER)  // ����һ�ΰ��µ�ʱ���������KEY_SHORT_PRESS_COUNT_NUMBER
					{
						key_param[i].count = 0;  // ��������
						key_param[i].state = KEY_STATE_SECOND_PRESS_VALID;  // ��ת�������ڶ��ΰ�����Ч״̬
					}
				}
				else
				{
					// ����ʱ�䲻������������������
					key_param[i].count = 0;  // ��������
					key_param[i].state = KEY_STATE_IDLE;  // ��ת����������״̬
					key_param[i].event = KEY_EVENT_SINGLE_CLICK;  // ���������¼�
				}
				break;

			case KEY_STATE_SECOND_PRESS_VALID:  // �����ڶ��ΰ�����Ч״̬
				if (key_param[i].key_is_press)  // ������������
				{
					if (++key_param[i].count >= KEY_LONG_PRESS_COUNT_NUMBER - KEY_SHORT_PRESS_COUNT_NUMBER)  // ���ڶ��ΰ��µ�ʱ���������KEY_LONG_PRESS_COUNT_NUMBER
					{
						key_param[i].count = 0;  // ��������
						key_param[i].state = KEY_STATE_SHORT_LONG_PRESS;  // ��ת�������̳���״̬
						key_param[i].event = KEY_EVENT_SHORT_LONG_PRESS;  // �����̳����¼�
					}
				}
				else
				{
					key_param[i].count = 0;  // ��������
					key_param[i].state = KEY_STATE_IDLE;  // ��ת����������״̬
					key_param[i].event = KEY_EVENT_DOUBLE_CLICK;  // ����˫���¼�
				}
				break;

			case KEY_STATE_SECOND_RELEASE:  // �����ڶ����ͷ�״̬
				// Do nothing!
				break;

			case KEY_STATE_LONG_PRESS:
				if (key_param[i].key_is_press)  // ������������
				{
					if (++key_param[i].count >= KEY_LONG_LONG_PRESS_COUNT_NUMBER - KEY_LONG_PRESS_COUNT_NUMBER)  // �������µ�ʱ���������KEY_LONG_LONG_PRESS_COUNT_NUMBER
					{
						key_param[i].count = 0;  // ��������
						key_param[i].state = KEY_STATE_LONG_LONG_PRESS;  // ��ת������������״̬
						key_param[i].event = KEY_EVENT_LONG_LONG_PRESS;  // �����������¼�
					}
				}
				else
				{
					key_param[i].count = 0;  // ��������
					key_param[i].state = KEY_STATE_LONG_RELEASE;  // ��ת�����������ͷ�״̬
					key_param[i].event = KEY_EVENT_LONG_RELEASE;  // ���������ͷ��¼�
				}
				break;

			case KEY_STATE_LONG_RELEASE:
				key_param[i].count = 0;  // ��������
				key_param[i].state = KEY_STATE_IDLE;  // ��ת����������״̬
				key_param[i].event = KEY_EVENT_IDLE;  // ���������¼�
				break;

			case KEY_STATE_SHORT_LONG_PRESS:
				if (key_param[i].key_is_press)  // ������������
				{
					// �̳����󣬲����������ȴ������ͷ�
					// Do nothing!
				}
				else
				{
					key_param[i].count = 0;  // ��������
					key_param[i].state = KEY_STATE_SHORT_LONG_RELEASE;  // ��ת�������̳����ͷ�״̬
					key_param[i].event = KEY_EVENT_SHORT_LONG_RELEASE;  // �����̳����ͷ��¼�
				}
				break;

			case KEY_STATE_SHORT_LONG_RELEASE:
				key_param[i].count = 0;  // ��������
				key_param[i].state = KEY_STATE_IDLE;  // ��ת����������״̬
				key_param[i].event = KEY_EVENT_IDLE;  // ���������¼�
				break;

			case KEY_STATE_LONG_LONG_PRESS:
				if (key_param[i].key_is_press)  // ������������
				{
					// �������󣬲����������ȴ������ͷ�
					// Do nothing!
				}
				else
				{
					key_param[i].count = 0;  // ��������
					key_param[i].state = KEY_STATE_LONG_LONG_RELEASE;  // ��ת�������������ͷ�״̬
					key_param[i].event = KEY_EVENT_LONG_LONG_RELEASE;  // �����������ͷ��¼�
				}
				break;

			case KEY_STATE_LONG_LONG_RELEASE:
				key_param[i].count = 0;  // ��������
				key_param[i].state = KEY_STATE_IDLE;  // ��ת����������״̬
				key_param[i].event = KEY_EVENT_IDLE;  // ���������¼�
				break;
		}

		// ɨ�谴���¼�
		switch (key_param[i].event)  // ����״̬������
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

		key_param[i].event = KEY_EVENT_IDLE;  // ��������¼�
	}
}

