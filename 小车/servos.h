#ifndef __SERVOS_H
#define __SERVOS_H

#include "sys.h"

void Servos_Init(void);
void servos_angle(TIM_TypeDef* TIMx,uint8_t CH,float angle);

void TIM_SetComparex(TIM_TypeDef* TIMx, uint32_t Compare,uint8_t CH);

#endif
