#include "stm32f4xx.h"
#include "adc.h"



float ADC_getvoltage(void)
{
	int value = 0;
  float voltage = 0.0;
	
	HAL_ADC_Start(&hadc1);
	
	value = HAL_ADC_GetValue(&hadc1);
	voltage = (value/4095.0)*3.3*1.6056;
	
	return voltage;
}
