/**
 * 蓝萌电子 版权所有  *
 * @FilePath: current.h
 * @Author: 沈玲玲
 * @Email: XiaoMengmeng@lanmeng.com
 * @Version: 1.0
 * @LastEditors:
 * @LastEditTime: 2022-05-24 11:17:51
 * @  * Copyright: 2022 蓝萌电子 All Rights Reserved.
 * @  *  *
 * @  * This software is licensed under terms that can be found in the LICENSE file
 * @  * in the root directory of this software component.
 * @  * If no LICENSE file comes with this software, it is provided GPL3.0.
 * @  *  *
 * @Description:
 * copyright @2022(c) GPL-3.0  *
*/
#ifndef __CURRENT_H_
#define __CURRENT_H_

#include "main.h"

#define Register_Current 0.1

void startGetCurrent(void);
float returnCurrent(void);
void smooth_filter(void);
#endif
