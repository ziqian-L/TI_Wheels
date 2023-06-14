#ifndef __ENCODER_H
#define __ENCODER_H

#include "sys.h"
void Encoder_A_TIM3_Init(void);
void Encoder_B_TIM4_Init(void);
int Read_Speed(int TIMx);

#endif
