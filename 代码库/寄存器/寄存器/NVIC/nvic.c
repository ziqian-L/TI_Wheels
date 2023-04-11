#include "nvic.h"
#include "timer.h"
#include "usart.h"

/******************************************定时中断******************************************/

void TIM2_Exti_Init(uint32_t psc,uint32_t arr)
{
    //开启TIM2时钟
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    //时基单元初始化
    //向上计数模式、时钟不分频
    TIM2_Init(psc,arr);
    //中断优先级配置
    NVIC_Init(TIM2_IRQn,0,0);
    //使能定时器中断
    TIM_IT_Update(TIM2);
	//使能定时器
	TIM_Cmd(TIM2);
}

void TIM2_IRQHandler(void)
{
    if(TIM_GetITUpdate_Status(TIM2)==1)
    {
		static int a=0;
		a++;
        printf("%d \r\n",a);
    }
    TIM_ClearITUpdate(TIM2);
}




/******************************************中断优先级分组******************************************/
/******
 * 中断优先级分组
 * 参数：
 *      NVIC_PriorityGroup_0:0位抢占优先级，4位响应优先级
 *      NVIC_PriorityGroup_1:1位抢占优先级，3位响应优先级
 *      NVIC_PriorityGroup_2:2位抢占优先级，2位响应优先级
 *      NVIC_PriorityGroup_3:3位抢占优先级，1位响应优先级
 *      NVIC_PriorityGroup_4:4位抢占优先级，0位响应优先级
******/
void NVIC_PriorityGroup(uint32_t Group)
{
    SCB->AIRCR = AIRCR_VECTKEY_MASK | Group;
}

/******************************************设置某一中断的中断优先级******************************************/
/******
 * 中断初始化
 * NVIC_IRQChannel：设置中断优先级的中断
 * NVIC_IRQPreemptivePriority：抢占优先级
 * NVIC_IRQResponsePriority：响应优先级
******/
void NVIC_Init(uint8_t NVIC_IRQChannel,uint8_t NVIC_IRQPreemptivePriority,uint8_t NVIC_IRQResponsePriority)
{
    uint8_t Preemptive = 0x00,Response = 0x00,Priority = 0x0F,NVICsetting = 0x00;
    //得到抢占优先级的位数
    Preemptive = (0x07 - ((uint32_t)(SCB->AIRCR)>>0x08));
    //得到响应优先级的位数
    Response = 0x04 - Preemptive;
    //得到中断优先级分组
    Priority = Priority >> Preemptive;

    //计算该中断的中断优先级
    NVICsetting = NVIC_IRQPreemptivePriority << Response;
    NVICsetting |= (uint8_t)(NVIC_IRQResponsePriority & Priority);
    NVICsetting = NVICsetting << 0x04;

    //赋值
    NVIC->IP[NVIC_IRQChannel] = NVICsetting;

    //使能
    NVIC->ISER[NVIC_IRQChannel >> 0x05] = (uint32_t)0x01 << (NVIC_IRQChannel & (uint8_t)0x1F);
}
