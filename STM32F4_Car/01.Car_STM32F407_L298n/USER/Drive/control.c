#include "control.h"
#include "pid.h"

/********************************************************************
 * 控制均在中断里完成，使用TIM2作为定时中断
********************************************************************/

/*****
 * TIM2定时中断服务函数
*****/
void TIM2_IRQHandler(void)
{}

void Control_Init(void)
{
	//5ms一次定时中断
	TIM2_Timed_Interrupt(84-1,5000);
}

/*****
 * 入口参数：
 *  PSC：预分频系数
 *  ARR：自动重装载值
*****/
void TIM2_Timed_Interrupt(uint32_t PSC,uint32_t ARR)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    NVIC_InitTypeDef NVIC_InitStructure;

    //TIM2时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);

    //初始化TIM2时基单元
	//向上计数、不分割时钟、自动重装载值、预分频系数
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_Period = ARR;
	TIM_TimeBaseInitStruct.TIM_Prescaler = PSC;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);

    //中断优先级配置
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    //使能定时器中断
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
}


