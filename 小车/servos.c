#include "servos.h"

/*舵机PWM最小、舵机PWM最大、舵机角度为 0 PWM、舵机角度为 180 PWM、舵机最大角度*/
#define Servo_PWM_Limit_Min 341
#define Servo_PWM_Limit_Max 2675
#define Servo_PWM_Angle_Min 550
#define Servo_PWM_Angle_Max 2480
#define Servo_Angle 180

/******************************************舵机相关******************************************/

/**********
 * 舵机初始化
 * TIM2_CH1
 * PA15
**********/
void Servos_Init(void)
{
    //开启时钟
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    //复用、挽推、100MHz、下拉
    GPIOA->MODER    |= GPIO_MODER_MODER15_1;
    GPIOA->OTYPER   |= 0x00;
    GPIOA->OSPEEDR  |= GPIO_OSPEEDER_OSPEEDR15;
    GPIOA->PUPDR    |= GPIO_PUPDR_PUPDR15_1;
    //GPIO复用
    GPIO_AF_Set(GPIOA,15,1);
    //初始化时基单元(20ms)
    TIM2->CR1 |= 0<<4;
    TIM2->CR1 |= 0<<8;
    TIM2->PSC = 84-1;
    TIM2->ARR = 20000-1;
    //PWM1模式,使能输出比较、输出比较极性为低,输出比较的预装载值,使能预装载寄存器,使能自动重装载的预装载寄存器允许位
    TIM2->CCMR1 |= (TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1);
    TIM2->CCER  |= (TIM_CCER_CC1E | 0x00);
    TIM2->CCR2  |= 0x00;
    TIM2->CCMR1 |= TIM_CCMR1_OC1PE;
    TIM2->CR1 |= TIM_CR1_ARPE;
    //使能定时器
    TIM2->CR1 |= 1<<0;
}

/**********
 * 舵机角度修改()
 * 舵机角度：         -90     -45      0       45      90       
 * 自动重装载值：     530   1013.75  1497.5  1981.25  2465
**********/
void servos_angle(TIM_TypeDef* TIMx,uint8_t CH,float angle)
{
    float angle_compare = 0;
    angle_compare = (float)(Servo_PWM_Angle_Max - Servo_PWM_Angle_Min)*(float)(angle/Servo_Angle);
    angle_compare += Servo_PWM_Angle_Min;
    TIM_SetComparex(TIMx,angle_compare,CH);
}

/******************************************修改预装载值******************************************/

/**********
 * 适用于：
 *      4 通道的TIM1、TIM8
 *      4 通道的TIM2、TIM3、TIM4、TIM5
 *      2 通道的TIM9、TIM12
 *      1 通道的TIM10、TIM11、TIM13、TIM14
 * 通道数：4
 * 预装载值范围：0~65535、TIM2,TIM5可达 0 ~ 2^32
**********/
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
