#ifndef XL1509_DAC_CONTROL_H
#define XL1509_DAC_CONTROL_H
uint16_t Adc_GetValue(void);
uint16_t adc_get_result_average(void);
void Get_actual_voltage(void);
void bubbleSort(uint16_t arr[], uint8_t n);

//下面这里才是接口
uint16_t Get_Target_Voltage_DACNUM(void);
void Set_Enable_DAC_Output(void);
void Stop_DAC_Output(void);
#endif
