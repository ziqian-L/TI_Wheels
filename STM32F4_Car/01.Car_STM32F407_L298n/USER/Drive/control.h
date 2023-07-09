#ifndef __CONTROL_H
#define __CONTROL_H

#include "sys.h"

void Control_Init(void);
void TIM2_Timed_Interrupt(uint32_t PSC,uint32_t ARR);

#endif
