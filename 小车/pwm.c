#include "pwm.h"

/***
 * ���ֽ�A,���ֽ�B
 * ���A��B��PWM���
 * PA1(TIM5_CH2)�����A
 * PA2(TIM5_CH3)�����B
 * 
 * TIM5ͨ��2��ͨ��3��PWM�����ʼ��
 * psc:Ԥ��Ƶϵ��
 * arr:�Զ���װ��ֵ
***/
void TIM5_PWM_Init(uint32_t psc,uint32_t arr)
{
    //GPIOAʱ�ӡ�TIM5ʱ��
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;

    //GPIO��ʼ��
    //������������ơ�100MHz������
    GPIOA->MODER    |= GPIO_MODER_MODER1_1 | GPIO_MODER_MODER2_1;
    GPIOA->OTYPER   |= 0x00;
    GPIOA->OSPEEDR  |= GPIO_OSPEEDER_OSPEEDR1 | GPIO_OSPEEDER_OSPEEDR2;
    GPIOA->PUPDR    |= GPIO_PUPDR_PUPDR1_1 | GPIO_PUPDR_PUPDR2_1;

    //GPIO����
    //PA1��PA2����ΪTIM5
    GPIO_AF_Set(GPIOA,1,2);
    GPIO_AF_Set(GPIOA,2,2);

    //ʱ����Ԫ��ʼ��
    TIM5->CR1 |= 0<<4;
    TIM5->CR1 |= 0<<8;
    TIM5->PSC  = psc;
    TIM5->ARR  = arr;
    
    //CH2��CH3����Ƚϳ�ʼ��
    TIM5->CCMR1 |= (TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1);
    TIM5->CCMR2 |= (TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1);
    TIM5->CCER  |= (TIM_CCER_CC2E | TIM_CCER_CC2P | TIM_CCER_CC3E | TIM_CCER_CC3P);

    //ʹ��Ԥװ��ֵ�Ĵ���
    TIM5->CCMR1 |= TIM_CCMR1_OC2PE;
    TIM5->CCMR2 |= TIM_CCMR2_OC3PE;
    //ʹ���Զ���װ�ص�Ԥװ�ؼĴ�������λ
    TIM5->CR1 |= 1<<7;
    //ʹ�ܶ�ʱ��
    TIM_Cmd(TIM5,ENABLE);
}

/**
 * �޸�TIM5_CH2�Զ���װ��ֵ�����޸�PWMռ�ձ�
**/
void TIM5_CH2_SetComparex(uint32_t Comparex)
{
    TIM5->CCR2 = Comparex;
}

/**
 * �޸�TIM5_CH3�Զ���װ��ֵ�����޸�PWMռ�ձ�
**/
void TIM5_CH3_SetComparex(uint32_t Comparex)
{
    TIM5->CCR3 = Comparex;
}
