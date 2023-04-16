#include "nvic.h"
#include "timer.h"
#include "usart.h"

/******************************************��ʱ�ж�******************************************/

void TIM2_Exti_Init(uint32_t psc,uint32_t arr)
{
    //����TIM2ʱ��
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    //ʱ����Ԫ��ʼ��
    //���ϼ���ģʽ��ʱ�Ӳ���Ƶ
    TIM2_Init(psc,arr);
    //�ж����ȼ�����
    NVIC_Init(TIM2_IRQn,0,0);
    //ʹ�ܶ�ʱ���ж�
    TIM_IT_Update(TIM2);
	//ʹ�ܶ�ʱ��
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

/******************************************�ⲿ�ж�******************************************/
/******
 * GPIOx ���ⲿ�ж�������
 * ����������д�����⺯��д���ͼĴ���д��
 * �⺯��д�������ã�������Ҫ�����궨���������򻯳���
 * �Ĵ���д����Ȼ��ͨ�ã����Ǽ򻯳���
 * ǿ�ҽ���ʹ�üĴ���д����
******/
void SYSCFG_EXTI_GPIO(uint8_t EXTI_GPIOx,uint8_t EXTI_Line)
{
    //ʹ��ʱ��
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    SYSCFG->EXTICR[EXTI_Line >> 0x02] |= (((uint32_t)EXTI_GPIOx) << (0x04 * (EXTI_Line & (uint8_t)0x03)));
    /**
    �Ĵ���д��ʵ��
    ��: GPIOA ���ӵ��ж��� 6
    SYSCFG->EXTICR[1] &= ~SYSCFG_EXTICR2_EXTI6;     //��Ӧλ������(��ʡ��)
    SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI6_PA;   //GPIOA ���ӵ��ж��� 6
    **/
}

/******
 * ��ʼ��EXTI�ж�����
 * ֻ���GPIOA~I;������PVD,RTC,USB_OTG,USB_HS,��̫�����ѵ�
 * EXTI_Line:     ��Ҫʹ�ܵ��ж��� 0~15
 * Rising_Falling:����ģʽ,1,������;2,�Ͻ���;3�������ƽ����
******/
void EXTI_GPIO_Init(uint32_t EXTI_Line,uint8_t Rising_Falling)
{
    EXTI->IMR   |= (1<<EXTI_Line);
    if(Rising_Falling&0x01)EXTI->RTSR  |= (1<<EXTI_Line);
    if(Rising_Falling&0x02)EXTI->FTSR  |= (1<<EXTI_Line);
}

/******************************************�ж����ȼ�����******************************************/
/******
 * �ж����ȼ�����
 * ������
 *      NVIC_PriorityGroup_0:0λ��ռ���ȼ���4λ��Ӧ���ȼ�
 *      NVIC_PriorityGroup_1:1λ��ռ���ȼ���3λ��Ӧ���ȼ�
 *      NVIC_PriorityGroup_2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
 *      NVIC_PriorityGroup_3:3λ��ռ���ȼ���1λ��Ӧ���ȼ�
 *      NVIC_PriorityGroup_4:4λ��ռ���ȼ���0λ��Ӧ���ȼ�
******/
void NVIC_PriorityGroup(uint32_t Group)
{
    SCB->AIRCR = AIRCR_VECTKEY_MASK | Group;
}

/******************************************����ĳһ�жϵ��ж����ȼ�******************************************/
/******
 * �жϳ�ʼ��
 * NVIC_IRQChannel�������ж����ȼ����ж�
 * NVIC_IRQPreemptivePriority����ռ���ȼ�
 * NVIC_IRQResponsePriority����Ӧ���ȼ�
******/
void NVIC_Init(uint8_t NVIC_IRQChannel,uint8_t NVIC_IRQPreemptivePriority,uint8_t NVIC_IRQResponsePriority)
{
    uint8_t Preemptive = 0x00,Response = 0x00,Priority = 0x0F,NVICsetting = 0x00;
    //�õ���ռ���ȼ���λ��
    Preemptive = (0x07 - ((uint32_t)(SCB->AIRCR)>>0x08));
    //�õ���Ӧ���ȼ���λ��
    Response = 0x04 - Preemptive;
    //�õ��ж����ȼ�����
    Priority = Priority >> Preemptive;

    //������жϵ��ж����ȼ�
    NVICsetting = NVIC_IRQPreemptivePriority << Response;
    NVICsetting |= (uint8_t)(NVIC_IRQResponsePriority & Priority);
    NVICsetting = NVICsetting << 0x04;

    //��ֵ
    NVIC->IP[NVIC_IRQChannel] = NVICsetting;

    //ʹ��
    NVIC->ISER[NVIC_IRQChannel >> 0x05] = (uint32_t)0x01 << (NVIC_IRQChannel & (uint8_t)0x1F);
}
