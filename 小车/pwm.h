#ifndef __PWM_H
#define __PWM_H

#include "sys.h"

void TIM5_PWM_Init(uint32_t psc,uint32_t arr);
void TIM5_CH2_SetComparex(uint32_t Comparex);
void TIM5_CH3_SetComparex(uint32_t Comparex);

#endif
