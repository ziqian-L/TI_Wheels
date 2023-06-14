#include "servos.h"

/*���PWM��С�����PWM��󡢶���Ƕ�Ϊ 0 PWM������Ƕ�Ϊ 180 PWM��������Ƕ�*/
#define Servo_PWM_Limit_Min 341
#define Servo_PWM_Limit_Max 2675
#define Servo_PWM_Angle_Min 550
#define Servo_PWM_Angle_Max 2480
#define Servo_Angle 180

/******************************************������******************************************/

/**********
 * �����ʼ��
 * TIM2_CH1
 * PA15
**********/
void Servos_Init(void)
{
    //����ʱ��
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    //���á����ơ�100MHz������
    GPIOA->MODER    |= GPIO_MODER_MODER15_1;
    GPIOA->OTYPER   |= 0x00;
    GPIOA->OSPEEDR  |= GPIO_OSPEEDER_OSPEEDR15;
    GPIOA->PUPDR    |= GPIO_PUPDR_PUPDR15_1;
    //GPIO����
    GPIO_AF_Set(GPIOA,15,1);
    //��ʼ��ʱ����Ԫ(20ms)
    TIM2->CR1 |= 0<<4;
    TIM2->CR1 |= 0<<8;
    TIM2->PSC = 84-1;
    TIM2->ARR = 20000-1;
    //PWM1ģʽ,ʹ������Ƚϡ�����Ƚϼ���Ϊ��,����Ƚϵ�Ԥװ��ֵ,ʹ��Ԥװ�ؼĴ���,ʹ���Զ���װ�ص�Ԥװ�ؼĴ�������λ
    TIM2->CCMR1 |= (TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1);
    TIM2->CCER  |= (TIM_CCER_CC1E | 0x00);
    TIM2->CCR2  |= 0x00;
    TIM2->CCMR1 |= TIM_CCMR1_OC1PE;
    TIM2->CR1 |= TIM_CR1_ARPE;
    //ʹ�ܶ�ʱ��
    TIM2->CR1 |= 1<<0;
}

/**********
 * ����Ƕ��޸�()
 * ����Ƕȣ�         -90     -45      0       45      90       
 * �Զ���װ��ֵ��     530   1013.75  1497.5  1981.25  2465
**********/
void servos_angle(TIM_TypeDef* TIMx,uint8_t CH,float angle)
{
    float angle_compare = 0;
    angle_compare = (float)(Servo_PWM_Angle_Max - Servo_PWM_Angle_Min)*(float)(angle/Servo_Angle);
    angle_compare += Servo_PWM_Angle_Min;
    TIM_SetComparex(TIMx,angle_compare,CH);
}

/******************************************�޸�Ԥװ��ֵ******************************************/

/**********
 * �����ڣ�
 *      4 ͨ����TIM1��TIM8
 *      4 ͨ����TIM2��TIM3��TIM4��TIM5
 *      2 ͨ����TIM9��TIM12
 *      1 ͨ����TIM10��TIM11��TIM13��TIM14
 * ͨ������4
 * Ԥװ��ֵ��Χ��0~65535��TIM2,TIM5�ɴ� 0 ~ 2^32
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
