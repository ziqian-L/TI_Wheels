#ifndef __TIMER_H
#define __TIMER_H

#include "sys.h"

//������ģʽ
#define TIM_EncoderMode_TI1                ((uint16_t)0x0001)
#define TIM_EncoderMode_TI2                ((uint16_t)0x0002)
#define TIM_EncoderMode_TI12               ((uint16_t)0x0003)

//���벶����
#define  TIM_ICPolarity_Rising             ((uint16_t)0x0000)
#define  TIM_ICPolarity_Falling            ((uint16_t)0x0002)
#define  TIM_ICPolarity_BothEdge           ((uint16_t)0x000A)


int TIM_GetITUpdate_Status(TIM_TypeDef* TIMx);
void TIM_ClearITUpdate(TIM_TypeDef* TIMx);

void TIM_Cmd(TIM_TypeDef* TIMx);
void TIM_Preload_EN(TIM_TypeDef* TIMx,uint8_t CH);
void TIM_ARR_Preload_EN(TIM_TypeDef* TIMx);
void TIM_IT_Update(TIM_TypeDef* TIMx);

void TIM_SetCounter(TIM_TypeDef* TIMx,uint32_t Counter);

void TIM1_Init(uint16_t psc,uint16_t arr);
void TIM2_Init(uint32_t psc,uint32_t arr);
void TIM9_Init(uint16_t psc,uint16_t arr);
void TIM6_Init(uint16_t psc,uint16_t arr);

void TIM1_Output_Compare_x_Init(uint8_t CH);
void TIM2_Output_Compare_x_Init(uint8_t CH);
void TIM9_Output_Compare_x_Init(uint8_t CH);
void TIM10_Output_Compare_x_Init(void);


void TIM1_Input_Capture_x_Init(uint8_t CH);
void TIM2_Input_Capture_x_Init(uint8_t CH);
void TIM9_Input_Capture_x_Init(uint8_t CH);
void TIM10_Input_Capture_x_Init(void);

void TIM_EncoderConfig(TIM_TypeDef* TIMx,uint16_t EncoderMode,uint16_t TIM_IC1Polarity, uint16_t TIM_IC2Polarity);

















#endif

/*********************************************************************************************************************************
| ��ʱ������ | λ�� |     ������ģʽ      | ����DMA���� | ����/�Ƚ�ͨ�� | ������� |                ����Ӧ�ó���                 |
----------------------------------------------------------------------------------------------------------------------------------
| �߼���ʱ�� |  16  | ���ϡ����¡�����/�� |    ����     |       4       |    ��    |           ���ɱ�������Ļ������            |
|(TIM1��TIM8)|      |                     |             |               |          |                                             |
----------------------------------------------------------------------------------------------------------------------------------
| ͨ�ö�ʱ�� |  32  | ���ϡ����¡�����/�� |    ����     |       4       |    ��    | ͨ�á���ʱ������PWM��������벶������Ƚ� |
|(TIM2��TIM5)|      |                     |             |               |          |                                             |
----------------------------------------------------------------------------------------------------------------------------------
| ͨ�ö�ʱ�� |  16  | ���ϡ����¡�����/�� |    ����     |       4       |    ��    | ͨ�á���ʱ������PWM��������벶������Ƚ� |
|(TIM3��TIM4)|      |                     |             |               |          |                                             |
----------------------------------------------------------------------------------------------------------------------------------
| ͨ�ö�ʱ�� |  16  |        ����         |    û��     |       2       |    ��    | ͨ�á���ʱ������PWM��������벶������Ƚ� |
|(TIM9~TIM14)|      |                     |             |               |          |                                             |
----------------------------------------------------------------------------------------------------------------------------------
| ������ʱ�� |  16  | ���ϡ����¡�����/�� |    ����     |       0       |    ��    |              ��ҪӦ��������DAC              |
|(TIM6��TIM7)|      |                     |             |               |          |                                             |
*********************************************************************************************************************************/
/*******************************************
 * ϵͳʱ��168Mhz����Ƶ,AHBʱ�� 168Mhz;
 * APB1 4��Ƶ 42MHz
 * APB2 2��Ƶ 84Mhz
 * APBx�ķ�Ƶϵ��Ϊ1ʱ��ͨ�ü�ʱ����ʱ��ΪAPB1xʱ�ӵ�ֵ
 * APBx�ķ�Ƶϵ����Ϊ1ʱ��ͨ�ü�ʱ����ʱ��ΪAPBxʱ�ӵ�����
 * TIM 2,3,4,5,6,7,12,13,14��ʱ����Դ��APB1 * 2 = 84MHz
 * TIM 1,8,9,10,11��ʱ����Դ��APB2 * 2 = 168MHz
*******************************************/
/***************************************
TIM 2,3,4,5,6,7,12,13,14��ʱ����Դ��APB1
TIM2  ---> RCC->APB1ENR |= 1<<0;
TIM3  ---> RCC->APB1ENR |= 1<<1;
TIM4  ---> RCC->APB1ENR |= 1<<2;
TIM5  ---> RCC->APB1ENR |= 1<<3;
TIM6  ---> RCC->APB1ENR |= 1<<4;
TIM7  ---> RCC->APB1ENR |= 1<<5;
TIM12 ---> RCC->APB1ENR |= 1<<6;
TIM13 ---> RCC->APB1ENR |= 1<<7;
TIM14 ---> RCC->APB1ENR |= 1<<8;
TIM 1,8,9,10,11��ʱ����Դ��APB2
TIM1  ---> RCC->APB2ENR |= 1<<0;
TIM8  ---> RCC->APB2ENR |= 1<<1;
TIM9  ---> RCC->APB2ENR |= 1<<16;
TIM10 ---> RCC->APB2ENR |= 1<<17;
TIM11 ---> RCC->APB2ENR |= 1<<18;
***************************************/
