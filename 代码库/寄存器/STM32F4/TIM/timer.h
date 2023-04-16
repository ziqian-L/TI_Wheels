#ifndef __TIMER_H
#define __TIMER_H

#include "sys.h"

//编码器模式
#define TIM_EncoderMode_TI1                ((uint16_t)0x0001)
#define TIM_EncoderMode_TI2                ((uint16_t)0x0002)
#define TIM_EncoderMode_TI12               ((uint16_t)0x0003)

//输入捕获极性
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
| 定时器种类 | 位数 |     计数器模式      | 产生DMA请求 | 捕获/比较通道 | 互补输出 |                特殊应用场景                 |
----------------------------------------------------------------------------------------------------------------------------------
| 高级定时器 |  16  | 向上、向下、向上/下 |    可以     |       4       |    有    |           带可编程死区的互补输出            |
|(TIM1、TIM8)|      |                     |             |               |          |                                             |
----------------------------------------------------------------------------------------------------------------------------------
| 通用定时器 |  32  | 向上、向下、向上/下 |    可以     |       4       |    无    | 通用。定时计数、PWM输出、输入捕获、输出比较 |
|(TIM2、TIM5)|      |                     |             |               |          |                                             |
----------------------------------------------------------------------------------------------------------------------------------
| 通用定时器 |  16  | 向上、向下、向上/下 |    可以     |       4       |    无    | 通用。定时计数、PWM输出、输入捕获、输出比较 |
|(TIM3、TIM4)|      |                     |             |               |          |                                             |
----------------------------------------------------------------------------------------------------------------------------------
| 通用定时器 |  16  |        向上         |    没有     |       2       |    无    | 通用。定时计数、PWM输出、输入捕获、输出比较 |
|(TIM9~TIM14)|      |                     |             |               |          |                                             |
----------------------------------------------------------------------------------------------------------------------------------
| 基本定时器 |  16  | 向上、向下、向上/下 |    可以     |       0       |    无    |              主要应用于驱动DAC              |
|(TIM6、TIM7)|      |                     |             |               |          |                                             |
*********************************************************************************************************************************/
/*******************************************
 * 系统时钟168Mhz不分频,AHB时钟 168Mhz;
 * APB1 4分频 42MHz
 * APB2 2分频 84Mhz
 * APBx的分频系数为1时，通用计时器的时钟为APB1x时钟的值
 * APBx的分频系数不为1时，通用计时器的时钟为APBx时钟的两倍
 * TIM 2,3,4,5,6,7,12,13,14的时钟来源于APB1 * 2 = 84MHz
 * TIM 1,8,9,10,11的时钟来源于APB2 * 2 = 168MHz
*******************************************/
/***************************************
TIM 2,3,4,5,6,7,12,13,14的时钟来源于APB1
TIM2  ---> RCC->APB1ENR |= 1<<0;
TIM3  ---> RCC->APB1ENR |= 1<<1;
TIM4  ---> RCC->APB1ENR |= 1<<2;
TIM5  ---> RCC->APB1ENR |= 1<<3;
TIM6  ---> RCC->APB1ENR |= 1<<4;
TIM7  ---> RCC->APB1ENR |= 1<<5;
TIM12 ---> RCC->APB1ENR |= 1<<6;
TIM13 ---> RCC->APB1ENR |= 1<<7;
TIM14 ---> RCC->APB1ENR |= 1<<8;
TIM 1,8,9,10,11的时钟来源于APB2
TIM1  ---> RCC->APB2ENR |= 1<<0;
TIM8  ---> RCC->APB2ENR |= 1<<1;
TIM9  ---> RCC->APB2ENR |= 1<<16;
TIM10 ---> RCC->APB2ENR |= 1<<17;
TIM11 ---> RCC->APB2ENR |= 1<<18;
***************************************/
