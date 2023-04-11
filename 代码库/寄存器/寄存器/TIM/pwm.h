#ifndef __PWM_H
#define __PWM_H

#include "sys.h"

void TIM1_PWM_Init(uint16_t psc,uint16_t arr,uint8_t x);

void TIM_SetComparex(TIM_TypeDef* TIMx, uint32_t Compare1,uint8_t CH);

#endif
