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
/***中断线***/
#define EXTI_Line0  0x00
#define EXTI_Line1  0x01
#define EXTI_Line2  0x02
#define EXTI_Line3  0x03
#define EXTI_Line4  0x04
#define EXTI_Line5  0x05
#define EXTI_Line6  0x06
#define EXTI_Line7  0x07
#define EXTI_Line8  0x08
#define EXTI_Line9  0x09
#define EXTI_Line10 0x0A
#define EXTI_Line11 0x0B
#define EXTI_Line12 0x0C
#define EXTI_Line13 0x0D
#define EXTI_Line14 0x0E
#define EXTI_Line15 0x0F
/***EXTI_GPIOx***/
#define EXTI_GPIOA 0x00
#define EXTI_GPIOB 0x01
#define EXTI_GPIOC 0x02
#define EXTI_GPIOD 0x03
#define EXTI_GPIOE 0x04
#define EXTI_GPIOF 0x05
#define EXTI_GPIOG 0x06
#define EXTI_GPIOH 0x07
#define EXTI_GPIOI 0x08

void NVIC_PriorityGroup(uint32_t Group);
void NVIC_Init(uint8_t NVIC_IRQChannel,uint8_t NVIC_IRQPreemptivePriority,uint8_t NVIC_IRQResponsePriority);

void SYSCFG_EXTI_GPIO(uint8_t EXTI_GPIOx,uint8_t EXTI_Line);
void EXTI_GPIO_Init(uint32_t EXTI_Line,uint8_t Rising_Falling);

void TIM2_Exti_Init(uint32_t psc,uint32_t arr);

#endif
