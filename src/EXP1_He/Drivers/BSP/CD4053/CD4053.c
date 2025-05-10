#include "stm32f4xx.h"                  // Device header
#include ".\CD4053\CD4053.h"
#include "main.h"

//�������Σ�1  ����ʧ�棺2  �ײ�ʧ�棺3  ˫��ʧ�棺4
uint8_t State_CD4053=1;

uint8_t Get_Now_Wave_State(void)
{
    return State_CD4053;
}
void Correct_Now_Wave_State(uint8_t Num)
{
    State_CD4053=Num;
}
void Correct_To_Normal_Waveform(void)
{
    HAL_GPIO_WritePin(A_Pin_N_GPIO_Port, A_Pin_N_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(B_Pin_N_GPIO_Port, B_Pin_N_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(C_Pin_N_GPIO_Port, C_Pin_N_Pin, GPIO_PIN_SET);
}

void Correct_To_Top_Distortion(void)
{
    HAL_GPIO_WritePin(A_Pin_N_GPIO_Port, A_Pin_N_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(B_Pin_N_GPIO_Port, B_Pin_N_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(C_Pin_N_GPIO_Port, C_Pin_N_Pin, GPIO_PIN_RESET);
}

void Correct_To_Bottom_Distortion(void)
{
    HAL_GPIO_WritePin(A_Pin_N_GPIO_Port, A_Pin_N_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(B_Pin_N_GPIO_Port, B_Pin_N_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(C_Pin_N_GPIO_Port, C_Pin_N_Pin, GPIO_PIN_RESET);
}

void Correct_To_Bi_Distortion(void)
{
    HAL_GPIO_WritePin(A_Pin_N_GPIO_Port, A_Pin_N_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(B_Pin_N_GPIO_Port, B_Pin_N_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(C_Pin_N_GPIO_Port, C_Pin_N_Pin, GPIO_PIN_SET);
}
//�������Σ�1  ����ʧ�棺2  �ײ�ʧ�棺3  ˫��ʧ�棺4
void CorrectWave(uint8_t Num)
{
    switch(Num)
    {
        case 1:{
            Correct_To_Normal_Waveform();
            break;
        }
        case 2:{
            Correct_To_Top_Distortion();
            break;
        }
        case 3:{
            Correct_To_Bottom_Distortion();
            break;
        }
        case 4:{
            Correct_To_Bi_Distortion();
            break;
        }
        default:break;
    }
}

