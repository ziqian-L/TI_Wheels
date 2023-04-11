#include "encoder.h"
#include "timer.h"
#include "led.h"
#include "nvic.h"

void TIM3_Encoder_Init(void)
{
    //电机编码器    PA6、PA7    TIM3

    //开启GPIOA、TIM3时钟
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    //初始化GPIOA6、GPIOA7
    //复用功能模式、输出挽推、100MHz、无上拉或下拉
    GPIOA->MODER    |= (GPIO_MODER_MODER6_1|GPIO_MODER_MODER7_1);
    GPIOA->OTYPER   |= 0x00;
    GPIOA->OSPEEDR  |= (GPIO_OSPEEDER_OSPEEDR6 | GPIO_OSPEEDER_OSPEEDR7);
    GPIOA->PUPDR    |= 0x00;
    //GPIOA6、GPIOA7复用为TIM3
    GPIO_AF_Set(GPIOA,6,2);
    GPIO_AF_Set(GPIOA,7,2);
    //初始化TIM3时基单元
    TIM3->CR1 |= 0x00;
    TIM3->CR1 |= 0x00;
    TIM3->PSC = 65535;
    TIM3->ARR = 0;
    //配置编码器模式、滤波设置(没有测试该函数是否正确)
    TIM_EncoderConfig(TIM3,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
    TIM3->CCMR1 |= (uint16_t)((0x000F << 4) | (0x000F << 12));
    //使能溢出更新中断
    TIM_IT_Update(TIM3);
    //清零定时器
    TIM_SetCounter(TIM3,0);
    //开启定时器
    TIM_Cmd(TIM3);
}

//没有测试该函数是否正确
void TIM3_Input_Capture_Init(void)
{
    //开启GPIOA、TIM3时钟
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    //初始化GPIOA6、GPIOA7
    //复用功能模式、输出挽推、100MHz、无上拉或下拉
    GPIOA->MODER    |= GPIO_MODER_MODER6_1;
    GPIOA->OTYPER   |= 0x00;
    GPIOA->OSPEEDR  |= GPIO_OSPEEDER_OSPEEDR6;
    GPIOA->PUPDR    |= 0x00;
    //GPIOA6、GPIOA7复用为TIM3
    GPIO_AF_Set(GPIOA,6,2);
    //初始化TIM3时基单元
    TIM3->CR1 |= 0x00;
    TIM3->CR1 |= 0x00;
    TIM3->PSC = 84-1;
    TIM3->ARR = 65535;
    //TIM3_CH1输入捕获初始化
    //IC1映射到TI1、不分频、不滤波、使能输入捕获、非反相/上升沿触发( TIM_CCER_CC1P = 0x00,TIM_CCER_CC1NP = 0x00 )
    TIM3->CCMR1 |= (TIM_CCMR1_CC1S_0 | 0x00 | 0x00);
    TIM3->CCER  |= (TIM_CCER_CC1E | 0x00 | 0x00);
    //软件控制产生更新事件
    TIM3->EGR |= 1<<0;
    //允许捕获1中断
    TIM3->DIER |= 1<<1;
    //允许更新中断
    TIM_IT_Update(TIM3);
    //设置中断优先级
    NVIC_Init(TIM3_IRQn,1,1);
    //使能TIM3
    TIM_Cmd(TIM3);
}

void TIM3_IRQHandler(void)
{
    if((TIM3->SR & (uint16_t)0x10) != 0)
    {
        LED0 = !LED0;
    }
    TIM3->SR = (uint16_t)~0x10;
}
