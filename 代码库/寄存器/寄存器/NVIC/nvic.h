#ifndef __NVIC_H
#define __NVIC_H

#include "sys.h"

/***��λ���ƼĴ���(AIRCR)�ķ���Կ��***/
#define AIRCR_VECTKEY_MASK ((uint32_t)0x05FA0000)
/***�ж����ȼ�����***/
#define NVIC_PriorityGroup_0    ((uint32_t)0x700) //0λ��ռ���ȼ���4λ��Ӧ���ȼ�
#define NVIC_PriorityGroup_1    ((uint32_t)0x600) //1λ��ռ���ȼ���3λ��Ӧ���ȼ�
#define NVIC_PriorityGroup_2    ((uint32_t)0x500) //2λ��ռ���ȼ���2λ��Ӧ���ȼ�
#define NVIC_PriorityGroup_3    ((uint32_t)0x400) //3λ��ռ���ȼ���1λ��Ӧ���ȼ�
#define NVIC_PriorityGroup_4    ((uint32_t)0x300) //4λ��ռ���ȼ���0λ��Ӧ���ȼ�

void NVIC_PriorityGroup(uint32_t Group);
void NVIC_Init(uint8_t NVIC_IRQChannel,uint8_t NVIC_IRQPreemptivePriority,uint8_t NVIC_IRQResponsePriority);

void TIM2_Exti_Init(uint32_t psc,uint32_t arr);

#endif
