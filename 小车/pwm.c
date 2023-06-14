#include "pwm.h"

/***
 * 左轮接A,右轮接B
 * 电机A、B的PWM输出
 * PA1(TIM5_CH2)：电机A
 * PA2(TIM5_CH3)：电机B
 * 
 * TIM5通道2、通道3，PWM输出初始化
 * psc:预分频系数
 * arr:自动重装载值
***/
void TIM5_PWM_Init(uint32_t psc,uint32_t arr)
{
    //GPIOA时钟、TIM5时钟
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;

    //GPIO初始化
    //复用输出、挽推、100MHz、下拉
    GPIOA->MODER    |= GPIO_MODER_MODER1_1 | GPIO_MODER_MODER2_1;
    GPIOA->OTYPER   |= 0x00;
    GPIOA->OSPEEDR  |= GPIO_OSPEEDER_OSPEEDR1 | GPIO_OSPEEDER_OSPEEDR2;
    GPIOA->PUPDR    |= GPIO_PUPDR_PUPDR1_1 | GPIO_PUPDR_PUPDR2_1;

    //GPIO复用
    //PA1、PA2复用为TIM5
    GPIO_AF_Set(GPIOA,1,2);
    GPIO_AF_Set(GPIOA,2,2);

    //时基单元初始化
    TIM5->CR1 |= 0<<4;
    TIM5->CR1 |= 0<<8;
    TIM5->PSC  = psc;
    TIM5->ARR  = arr;
    
    //CH2、CH3输出比较初始化
    TIM5->CCMR1 |= (TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1);
    TIM5->CCMR2 |= (TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1);
    TIM5->CCER  |= (TIM_CCER_CC2E | TIM_CCER_CC2P | TIM_CCER_CC3E | TIM_CCER_CC3P);

    //使能预装载值寄存器
    TIM5->CCMR1 |= TIM_CCMR1_OC2PE;
    TIM5->CCMR2 |= TIM_CCMR2_OC3PE;
    //使能自动重装载的预装载寄存器允许位
    TIM5->CR1 |= 1<<7;
    //使能定时器
    TIM_Cmd(TIM5,ENABLE);
}

/**
 * 修改TIM5_CH2自动重装载值，即修改PWM占空比
**/
void TIM5_CH2_SetComparex(uint32_t Comparex)
{
    TIM5->CCR2 = Comparex;
}

/**
 * 修改TIM5_CH3自动重装载值，即修改PWM占空比
**/
void TIM5_CH3_SetComparex(uint32_t Comparex)
{
    TIM5->CCR3 = Comparex;
}
