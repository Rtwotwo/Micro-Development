#include "stm32f4xx.h"                  // Device header
#include "tim.h"

void PWM_Start_Output(void)
{
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
}

void PWM_Stop_Output(void)
{
    HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);
}

void Correct_PWM_Output(uint8_t state)  //state=0¹Ø±Õ£¬state=1´ò¿ª
{
    if(state==1)
    {
       PWM_Start_Output();
    }
    else if(state==0)
    {
       PWM_Stop_Output();
    } 
}

