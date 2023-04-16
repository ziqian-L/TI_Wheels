#include "pwm.h"
#include "timer.h"
#include "led.h"

/******************************************PWM初始化******************************************/

/******
 * TIM1_CH1 PWM输出初始化
 * PA8
 * 时基单元：
 * 输出比较：
******/
void TIM1_PWM_Init(uint16_t psc,uint16_t arr,uint8_t x)
{
    //GPIO时钟、TIM1时钟
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
    //GPIO初始化
    GPIOA->MODER    |= GPIO_MODER_MODER8_1;
    GPIOA->OTYPER   |= 0x00;
    GPIOA->OSPEEDR  |= GPIO_OSPEEDER_OSPEEDR8;
    GPIOA->PUPDR    |= GPIO_PUPDR_PUPDR8_1;

	//GPIO复用
//	GPIOA->AFR[1] |= 1<<((8-8)*4);
	GPIO_AF_Set(GPIOA,8,1);
	//时基单元初始化
//	TIM1->CR1 |= 0<<4;
//	TIM1->CR1 |= 0<<8;
//	TIM1->PSC = psc;
//	TIM1->ARR = arr;
    TIM1_Init(psc,arr);
	//输出比较初始化
//	TIM1->CCMR1 |= (TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1);
//	TIM1->CCER  |= (TIM_CCER_CC1E | TIM_CCER_CC1P);
	TIM1_Output_Compare_x_Init(x);
	//使能预装载寄存器
//	TIM1->CCMR1 |= 1<<3;
	TIM_Preload_EN(TIM1,1);
	//使能自动重装载的预装载寄存器允许位
//	TIM1->CR1 |= 1<<7;
	TIM_ARR_Preload_EN(TIM1);
	//使能刹车和死区寄存器
    TIM1->BDTR |= TIM_BDTR_MOE;
	//使能定时器
//	TIM1->CR1   |= 1<<0;
	TIM_Cmd(TIM1);
}

/******************************************修改预装载值******************************************/

/******
 * 适用于：
 *      4 通道的TIM1、TIM8
 *      4 通道的TIM2、TIM3、TIM4、TIM5
 *      2 通道的TIM9、TIM12
 *      1 通道的TIM10、TIM11、TIM13、TIM14
 * 通道数：4
 * 预装载值范围：0~65535
******/
void TIM_SetComparex(TIM_TypeDef* TIMx, uint32_t Compare,uint8_t CH)
{
    if (CH == 1)
    {
        TIMx->CCR1 = Compare;
    }
    else if (CH == 2)
    {
        TIMx->CCR2 = Compare;
    }
    else if (CH == 3)
    {
        TIMx->CCR3 = Compare;
    }
    else if (CH == 4)
    {
        TIMx->CCR4 = Compare;
    }
}



