#include "stm32f4xx.h"                  // Device header
void delay_us(uint32_t nus)
{
    uint32_t ticks;
    uint32_t told, tnow, tcnt = 0;
    uint32_t reload = SysTick->LOAD;        /* LOAD的值 */
    ticks = nus * 168;                 /* 需要的节拍数 */
    told = SysTick->VAL;                    /* 刚进入时的计数器值 */
    while (1)
    {
        tnow = SysTick->VAL;
        if (tnow != told)
        {
            if (tnow < told)
            {
                tcnt += told - tnow;        /* 这里注意一下SYSTICK是一个递减的计数器就可以了 */
            }
            else
            {
                tcnt += reload - tnow + told;
            }
            told = tnow;
            if (tcnt >= ticks) 
            {
                break;                      /* 时间超过/等于要延迟的时间,则退出 */
            }
        }
    }
}

void delay_ms(uint16_t nms)
{
    delay_us((uint32_t)(nms * 1000));                   /* 普通方式延时 */
}

void delay_s(uint16_t ns)
{
    delay_ms((uint32_t)(ns * 1000));                   /* 普通方式延时 */
}

///**
// * @brief       HAL库内部函数用到的延时
// * @note        HAL库的延时默认用Systick，如果我们没有开Systick的中断会导致调用这个延时后无法退出
// * @param       Delay : 要延时的毫秒数
// * @retval      None
// */
//void HAL_Delay(uint32_t Delay)
//{
//     delay_ms(Delay);
//}
