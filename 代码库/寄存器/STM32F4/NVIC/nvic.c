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

/******************************************外部中断******************************************/
/******
 * GPIOx 的外部中断线连接
 * 可以有两种写法：库函数写法和寄存器写法
 * 库函数写法简单易用，不过需要大量宏定义来辅助简化程序
 * 寄存器写法虽然不通用，但是简化程序
 * 强烈建议使用寄存器写法。
******/
void SYSCFG_EXTI_GPIO(uint8_t EXTI_GPIOx,uint8_t EXTI_Line)
{
    //使能时钟
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    SYSCFG->EXTICR[EXTI_Line >> 0x02] |= (((uint32_t)EXTI_GPIOx) << (0x04 * (EXTI_Line & (uint8_t)0x03)));
    /**
    寄存器写法实例
    例: GPIOA 连接到中断线 6
    SYSCFG->EXTICR[1] &= ~SYSCFG_EXTICR2_EXTI6;     //对应位置清零(可省略)
    SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI6_PA;   //GPIOA 连接到中断线 6
    **/
}

/******
 * 初始化EXTI中断配置
 * 只针对GPIOA~I;不包括PVD,RTC,USB_OTG,USB_HS,以太网唤醒等
 * EXTI_Line:     需要使能的中断线 0~15
 * Rising_Falling:触发模式,1,下升沿;2,上降沿;3，任意电平触发
******/
void EXTI_GPIO_Init(uint32_t EXTI_Line,uint8_t Rising_Falling)
{
    EXTI->IMR   |= (1<<EXTI_Line);
    if(Rising_Falling&0x01)EXTI->RTSR  |= (1<<EXTI_Line);
    if(Rising_Falling&0x02)EXTI->FTSR  |= (1<<EXTI_Line);
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
