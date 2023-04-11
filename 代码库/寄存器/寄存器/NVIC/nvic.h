#ifndef __NVIC_H
#define __NVIC_H

#include "sys.h"

/***复位控制寄存器(AIRCR)的访问钥匙***/
#define AIRCR_VECTKEY_MASK ((uint32_t)0x05FA0000)
/***中断优先级分组***/
#define NVIC_PriorityGroup_0    ((uint32_t)0x700) //0位抢占优先级，4位响应优先级
#define NVIC_PriorityGroup_1    ((uint32_t)0x600) //1位抢占优先级，3位响应优先级
#define NVIC_PriorityGroup_2    ((uint32_t)0x500) //2位抢占优先级，2位响应优先级
#define NVIC_PriorityGroup_3    ((uint32_t)0x400) //3位抢占优先级，1位响应优先级
#define NVIC_PriorityGroup_4    ((uint32_t)0x300) //4位抢占优先级，0位响应优先级

void NVIC_PriorityGroup(uint32_t Group);
void NVIC_Init(uint8_t NVIC_IRQChannel,uint8_t NVIC_IRQPreemptivePriority,uint8_t NVIC_IRQResponsePriority);

void TIM2_Exti_Init(uint32_t psc,uint32_t arr);

#endif
