/**
 * @file key.h
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
#ifndef __KEY_H_
#define __KEY_H_

#include "main.h"

#include <stdbool.h>
#include <stdint.h>

// ��10ms�Ķ�ʱ���н��м���
#define KEY_SHORT_PRESS_COUNT_NUMBER				(5)	 // �̰�ʱ��
#define KEY_DOUBLE_CLICK_INTERVAL_COUNT_NUMBER		(15)  // ˫�����
#define KEY_LONG_PRESS_COUNT_NUMBER					(150)  // ����ʱ��
#define KEY_LONG_LONG_PRESS_COUNT_NUMBER			(400)  // ������ʱ��

typedef bool (* my_read_key_t)(void);
typedef void (* my_key_callback_t)(void);

#define KEY_NUMBER									(2)  // ��������

// ����״̬
typedef enum
{
	KEY_STATE_IDLE,					// ��������״̬
	KEY_STATE_FIRST_PRESS,			// ������һ�ΰ���״̬
	KEY_STATE_FIRST_PRESS_VALID,	// ������һ�ΰ�����Ч״̬
	KEY_STATE_FIRST_RELEASE,		// ������һ���ͷ�״̬
	KEY_STATE_SECOND_PRESS,			// �����ڶ��ΰ���״̬
	KEY_STATE_SECOND_PRESS_VALID,	// �����ڶ��ΰ�����Ч״̬
	KEY_STATE_SECOND_RELEASE,		// �����ڶ����ͷ�״̬
	KEY_STATE_LONG_PRESS,			// ��������״̬
	KEY_STATE_LONG_RELEASE,			// ���������ͷ�״̬
	KEY_STATE_SHORT_LONG_PRESS,		// �����̳���״̬
	KEY_STATE_SHORT_LONG_RELEASE,	// �����̳����ͷ�״̬
	KEY_STATE_LONG_LONG_PRESS,		// ����������״̬
	KEY_STATE_LONG_LONG_RELEASE,	// �����������ͷ�״̬
} my_key_state_t;

// �����¼�
typedef enum
{
	KEY_EVENT_IDLE,					// ���������¼�
	KEY_EVENT_SINGLE_CLICK,			// ���������¼�
	KEY_EVENT_DOUBLE_CLICK,			// ����˫���¼�
	KEY_EVENT_LONG_PRESS,			// ���������¼�
	KEY_EVENT_LONG_RELEASE,			// ���������ͷ��¼�
	KEY_EVENT_SHORT_LONG_PRESS,		// �����̳����¼�
	KEY_EVENT_SHORT_LONG_RELEASE,	// �����̳����ͷ��¼�
	KEY_EVENT_LONG_LONG_PRESS,		// �����������¼�
	KEY_EVENT_LONG_LONG_RELEASE,	// �����������ͷ��¼�
} my_key_event_t;

// ��������
typedef struct
{
	my_read_key_t 		read_key;  // �������ܽ�״̬
	bool 				key_is_press;  // �����Ƿ񱻰���
	uint32_t 			count;  // ��������ʱ�䶨ʱ������ֵ
	my_key_state_t 		state;  // ����״̬
	my_key_event_t 		event;  // �����¼�
	my_key_callback_t 	single_click_callback;  // ���������ص�����
	my_key_callback_t 	double_click_callback;  // ����˫���ص�����
	my_key_callback_t 	long_press_callback;  // ���������ص�����
	my_key_callback_t 	long_release_callback;  // ���������ͷŻص�����
	my_key_callback_t 	short_long_press_callback;  // �����̳����ص�����
	my_key_callback_t 	short_long_release_callback;  // �����̳����ͷŻص�����
	my_key_callback_t 	long_long_press_callback;  // �����������ص�����
	my_key_callback_t 	long_long_release_callback;  // �����������ͷŻص�����
} my_key_param_t;


void my_key_init(void);  // ������ʼ���������ڶ�ʱ����ʼ��֮ǰ������
void my_key_scan(void);  // ����ɨ��



#endif  // __MY_KEY_H__

