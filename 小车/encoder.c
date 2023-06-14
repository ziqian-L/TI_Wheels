#include "encoder.h"

/***
 * 电机A编码器初始化
 * TIM3_CH3——PB0
 * TIM3_CH4——PB1
***/
void Encoder_A_TIM3_Init(void)
{
    //GPIOB时钟、TIM3时钟
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

    //GPIO初始化
    //复用、挽推、100MHz、浮空
    GPIOB->MODER    |= (GPIO_MODER_MODER0_1 | GPIO_MODER_MODER1_1);
    GPIOB->OTYPER   |= 0x00;
    GPIOB->OSPEEDR  |= (GPIO_OSPEEDER_OSPEEDR0 | GPIO_OSPEEDER_OSPEEDR1);
    GPIOB->PUPDR    |= 0x00;

    //GPIO复用
    GPIO_AF_Set(GPIOB,0,2);
    GPIO_AF_Set(GPIOB,1,2);

    //时基单元初始化
    //递增计数、时钟不分频，预分频系数为1、自动重装载值为65536
    TIM3->CR1 |= 0x00;
    TIM3->PSC  = 0;
    TIM3->ARR  = 65535;

    //配置编码器模式
    //编码器模式3(0x03)，输入捕获通道CH3、CH4，非反相/上升沿触发
    TIM3->SMCR  |= (TIM_SMCR_SMS_1 | TIM_SMCR_SMS_0);
    TIM3->CCMR2 |= (TIM_CCMR2_CC3S_0 | TIM_CCMR2_CC4S_0);
    TIM3->CCER  |= 0x00;

    //TIM3_CH3输入捕获初始化
    //IC3映射到TI3、不分频、滤波(fSAMPLING=fCK_INT，N=8)
    TIM3->CCMR2 |= (TIM_CCMR2_CC3S_0 | 0x00 | (TIM_CCMR2_IC3F_1 | TIM_CCMR2_IC3F_0));
    //使能输入捕获、非反相/上升沿触发( TIM_CCER_CC3P = 0x00,TIM_CCER_CC3NP = 0x00 )
    TIM3->CCER  |= (TIM_CCER_CC3E | 0x00 | 0x00);
    //TIM3_CH4输入捕获初始化
    //IC4映射到TI4、不分频、滤波(fSAMPLING=fCK_INT，N=8)
    TIM3->CCMR2 |= (TIM_CCMR2_CC4S_0 | 0x00 | (TIM_CCMR2_IC4F_1 | TIM_CCMR2_IC4F_0));
    //使能输入捕获、非反相/上升沿触发( TIM_CCER_CC4P = 0x00,TIM_CCER_CC4NP = 0x00 )
    TIM3->CCER  |= (TIM_CCER_CC4E | 0x00 | 0x00);

    //清除更新中断标志位
    TIM3->SR = (uint16_t)~TIM_SR_UIF;
    //使能更新中断
    TIM3->DIER |= TIM_DIER_UIE;
    //计数器值清零
    TIM3->CNT = 0;
    //使能定时器
    TIM_Cmd(TIM3,ENABLE);
}

/***
 * 电机B编码器初始化
 * TIM4_CH1——PB6
 * TIM4_CH2——PB7
***/
void Encoder_B_TIM4_Init(void)
{
    //GPIOB时钟、TIM4时钟
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;

    //GPIO初始化
    //复用、挽推、100MHz、浮空
    GPIOB->MODER    |= (GPIO_MODER_MODER6_1 | GPIO_MODER_MODER7_1);
    GPIOB->OTYPER   |= 0x00;
    GPIOB->OSPEEDR  |= (GPIO_OSPEEDER_OSPEEDR6 | GPIO_OSPEEDER_OSPEEDR7);
    GPIOB->PUPDR    |= 0x00;
    //GPIO复用
    GPIO_AF_Set(GPIOB,6,2);
    GPIO_AF_Set(GPIOB,7,2);

    //时基单元初始化
    //递增计数、时钟不分频、预分频系数为1、自动重装载值为65535
    TIM4->CR1 |= 0x00;
    TIM4->PSC  = 0;
    TIM4->ARR  = 65535;

    //配置编码器模式
    //编码器模式3(0x03)，输入捕获通道CH1、CH2，非反相/上升沿触发
    TIM4->SMCR  |= (TIM_SMCR_SMS_1 | TIM_SMCR_SMS_0);
    TIM4->CCMR1 |= (TIM_CCMR1_CC1S_0 | TIM_CCMR1_CC2S_0);
    TIM4->CCER  |= 0x00;
    
    //TIM4_CH1输入捕获初始化
    //IC1映射到TI1、不分频、滤波(fSAMPLING=fCK_INT，N=8)
    TIM4->CCMR1 |= (TIM_CCMR1_CC1S_0 | 0x00 | (TIM_CCMR1_IC1F_1 | TIM_CCMR1_IC1F_0));
    //使能输入捕获、非反相/上升沿触发( TIM_CCER_CC1P = 0x00,TIM_CCER_CC1NP = 0x00 )
    TIM4->CCER  |= (TIM_CCER_CC1E | 0x00 | 0x00);
    //TIM4_CH2输入捕获初始化
    //IC2映射到TI2、不分频、滤波(fSAMPLING=fCK_INT，N=8)
    TIM4->CCMR1 |= (TIM_CCMR1_CC2S_0 | 0x00 | (TIM_CCMR1_IC2F_1 | TIM_CCMR1_IC2F_0));
    //使能输入捕获、非反相/上升沿触发( TIM_CCER_CC2P = 0x00,TIM_CCER_CC2NP = 0x00 )
    TIM4->CCER  |= (TIM_CCER_CC2E | 0x00 | 0x00);

    //清除更新中断标志位
    TIM4->SR = (uint16_t)~TIM_SR_UIF;
    //使能更新中断
    TIM4->DIER |= TIM_DIER_UIE;
    //计数器值清零
    TIM4->CNT = 0;
    //使能定时器
    TIM_Cmd(TIM4,ENABLE);
}

/**********************
编码器
速度读取函数
入口参数：定时器
**********************/
int Read_Speed(int TIMx)
{
	int value_1;
	switch(TIMx)
	{
        //1.采集编码器的计数值并保存。2.将定时器的计数值清零。
		case 3:value_1=(short)TIM_GetCounter(TIM3);TIM_SetCounter(TIM3,0);break;
		case 4:value_1=(short)TIM_GetCounter(TIM4);TIM_SetCounter(TIM4,0);break;
		default:value_1=0;
	}
	return value_1;
}

void TIM3_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM3,TIM_IT_Update)!=0)
    {   
        TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
    }
}

void TIM4_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM4,TIM_IT_Update)!=0)
    {
        TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
    }
}
