#include "pwm.h"
#include "timer.h"
#include "led.h"

/******************************************PWM��ʼ��******************************************/

/******
 * TIM1_CH1 PWM�����ʼ��
 * PA8
 * ʱ����Ԫ��
 * ����Ƚϣ�
******/
void TIM1_PWM_Init(uint16_t psc,uint16_t arr,uint8_t x)
{
    //GPIOʱ�ӡ�TIM1ʱ��
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
    //GPIO��ʼ��
    GPIOA->MODER    |= GPIO_MODER_MODER8_1;
    GPIOA->OTYPER   |= 0x00;
    GPIOA->OSPEEDR  |= GPIO_OSPEEDER_OSPEEDR8;
    GPIOA->PUPDR    |= GPIO_PUPDR_PUPDR8_1;

	//GPIO����
//	GPIOA->AFR[1] |= 1<<((8-8)*4);
	GPIO_AF_Set(GPIOA,8,1);
	//ʱ����Ԫ��ʼ��
//	TIM1->CR1 |= 0<<4;
//	TIM1->CR1 |= 0<<8;
//	TIM1->PSC = psc;
//	TIM1->ARR = arr;
    TIM1_Init(psc,arr);
	//����Ƚϳ�ʼ��
//	TIM1->CCMR1 |= (TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1);
//	TIM1->CCER  |= (TIM_CCER_CC1E | TIM_CCER_CC1P);
	TIM1_Output_Compare_x_Init(x);
	//ʹ��Ԥװ�ؼĴ���
//	TIM1->CCMR1 |= 1<<3;
	TIM_Preload_EN(TIM1,1);
	//ʹ���Զ���װ�ص�Ԥװ�ؼĴ�������λ
//	TIM1->CR1 |= 1<<7;
	TIM_ARR_Preload_EN(TIM1);
	//ʹ��ɲ���������Ĵ���
    TIM1->BDTR |= TIM_BDTR_MOE;
	//ʹ�ܶ�ʱ��
//	TIM1->CR1   |= 1<<0;
	TIM_Cmd(TIM1);
}

/******************************************�޸�Ԥװ��ֵ******************************************/

/******
 * �����ڣ�
 *      4 ͨ����TIM1��TIM8
 *      4 ͨ����TIM2��TIM3��TIM4��TIM5
 *      2 ͨ����TIM9��TIM12
 *      1 ͨ����TIM10��TIM11��TIM13��TIM14
 * ͨ������4
 * Ԥװ��ֵ��Χ��0~65535
******/
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



