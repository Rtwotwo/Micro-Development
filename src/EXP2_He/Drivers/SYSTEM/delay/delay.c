#include "stm32f4xx.h"                  // Device header
void delay_us(uint32_t nus)
{
    uint32_t ticks;
    uint32_t told, tnow, tcnt = 0;
    uint32_t reload = SysTick->LOAD;        /* LOAD��ֵ */
    ticks = nus * 168;                 /* ��Ҫ�Ľ����� */
    told = SysTick->VAL;                    /* �ս���ʱ�ļ�����ֵ */
    while (1)
    {
        tnow = SysTick->VAL;
        if (tnow != told)
        {
            if (tnow < told)
            {
                tcnt += told - tnow;        /* ����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ����� */
            }
            else
            {
                tcnt += reload - tnow + told;
            }
            told = tnow;
            if (tcnt >= ticks) 
            {
                break;                      /* ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳� */
            }
        }
    }
}

void delay_ms(uint16_t nms)
{
    delay_us((uint32_t)(nms * 1000));                   /* ��ͨ��ʽ��ʱ */
}

void delay_s(uint16_t ns)
{
    delay_ms((uint32_t)(ns * 1000));                   /* ��ͨ��ʽ��ʱ */
}

///**
// * @brief       HAL���ڲ������õ�����ʱ
// * @note        HAL�����ʱĬ����Systick���������û�п�Systick���жϻᵼ�µ��������ʱ���޷��˳�
// * @param       Delay : Ҫ��ʱ�ĺ�����
// * @retval      None
// */
//void HAL_Delay(uint32_t Delay)
//{
//     delay_ms(Delay);
//}
