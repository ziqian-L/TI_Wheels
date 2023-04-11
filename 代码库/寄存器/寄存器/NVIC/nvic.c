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
