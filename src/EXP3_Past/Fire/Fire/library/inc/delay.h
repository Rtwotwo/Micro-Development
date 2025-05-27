/**
 * @file delay.h
 * @brief
 * @author HeYingmeng (XiaoMengmeng@lanmeng.com)
 * @version 1.0
 * @date 2022-05-12
 *
 *   * @copyright Copyright (c) 2022 ????
 *   * All rights reserved.
 *
 *   * This software is licensed under terms that can be found in the LICENSE file
 *   * in the root directory of this software component.
 *   * If no LICENSE file comes with this software, it is provided GPL3.0.
 *
 * @par ????:
 */
#ifndef __DELAY_H_
#define __DELAY_H_

#include "main.h"

void HAL_Delay_us_init(uint32_t SYS_CLK);//µ¥Î»:MHZ
void HAL_Delay_us(uint32_t nms);
void HAL_Delay_ms(uint32_t nms);

#endif
