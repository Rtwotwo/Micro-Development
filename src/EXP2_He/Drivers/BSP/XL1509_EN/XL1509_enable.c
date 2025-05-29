#include "stm32f4xx.h"                  // Device header
#include "main.h"

void Set_XL1509_ENABLE(void)
{
    HAL_GPIO_WritePin(XL1509_EN_GPIO_Port, XL1509_EN_Pin, GPIO_PIN_RESET);
}

void Set_XL1509_DISABLE(void)
{
    HAL_GPIO_WritePin(XL1509_EN_GPIO_Port, XL1509_EN_Pin, GPIO_PIN_SET);
}

void Set_XL1509_ENPIN(uint8_t state)//跟前面一样，1为关，0为开
{
    if(state==1)
    {
        Set_XL1509_DISABLE();
    }
    else if(state==0)
    {
        Set_XL1509_ENABLE();
    }
}

