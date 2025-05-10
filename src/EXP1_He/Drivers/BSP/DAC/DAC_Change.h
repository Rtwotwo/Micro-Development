#ifndef __DAC_CHANGE_H
#define __DAC_CHANGE_H

void Wave_data_Init(void);
void Start_DAC_DMA(uint8_t Num);
void Stop_DAC_DMA(void);
void Correct_DAC_Wave(uint8_t Num);
#endif

