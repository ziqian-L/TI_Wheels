#ifndef __MOTOR_PWM_H
#define __MOTOR_PWM_H

#include "stm32f4xx_conf.h"
#include "sys.h"

/***********************************
 * 高级定时器TIM1、TIM8的PWM输出初始化
 * 关于TIM1、TIM8：
 * 位数：16 --- 计数器模式：向上、向下、向上/下 --- DMA：可以产生DAM请求 --- 捕获/比较通道：4 --- 互补输出：有
 * 应用场景：定时计数，PWM输出，输入捕获，输出比较。
 * 特殊：带可编程死区的互补输出
 ***********************************/
void TIM1_PWM_Init(uint16_t arr, uint16_t psc, uint8_t ch);
void TIM8_PWM_Init(uint16_t arr, uint16_t psc, uint8_t ch);

/***********************************
 * 通用定时器TIM3、TIM4的PWM输出初始化
 * 关于TIM3、TIM4：
 * 位数：16 --- 计数器模式：向上、向下、向上/下 --- DMA：可以产生DAM请求 --- 捕获/比较通道：4 --- 互补输出：无
 * 应用场景：通用。定时计数，PWM输出，输入捕获，输出比较
 ***********************************/
void TIM3_PWM_Init(uint16_t arr, uint16_t psc, uint8_t ch);
void TIM4_PWM_Init(uint16_t arr, uint16_t psc, uint8_t ch);

/***********************************
 * 通用定时器TIM2、TIM5的PWM输出初始化
 * 关于TIM2、TIM5：
 * 位数：32 --- 计数器模式：向上、向下、向上/下 --- DMA：可以产生DAM请求 --- 捕获/比较通道：4 --- 互补输出：无
 * 应用场景：通用。定时计数，PWM输出，输入捕获，输出比较
 ***********************************/
void TIM2_PWM_Init(uint32_t arr, uint32_t psc, uint8_t ch);
void TIM5_PWM_Init(uint32_t arr, uint32_t psc, uint8_t ch);

#endif
