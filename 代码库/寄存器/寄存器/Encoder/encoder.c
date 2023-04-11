#include "encoder.h"
#include "timer.h"
#include "led.h"
#include "nvic.h"

void TIM3_Encoder_Init(void)
{
    //���������    PA6��PA7    TIM3

    //����GPIOA��TIM3ʱ��
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    //��ʼ��GPIOA6��GPIOA7
    //���ù���ģʽ��������ơ�100MHz��������������
    GPIOA->MODER    |= (GPIO_MODER_MODER6_1|GPIO_MODER_MODER7_1);
    GPIOA->OTYPER   |= 0x00;
    GPIOA->OSPEEDR  |= (GPIO_OSPEEDER_OSPEEDR6 | GPIO_OSPEEDER_OSPEEDR7);
    GPIOA->PUPDR    |= 0x00;
    //GPIOA6��GPIOA7����ΪTIM3
    GPIO_AF_Set(GPIOA,6,2);
    GPIO_AF_Set(GPIOA,7,2);
    //��ʼ��TIM3ʱ����Ԫ
    TIM3->CR1 |= 0x00;
    TIM3->CR1 |= 0x00;
    TIM3->PSC = 65535;
    TIM3->ARR = 0;
    //���ñ�����ģʽ���˲�����(û�в��Ըú����Ƿ���ȷ)
    TIM_EncoderConfig(TIM3,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
    TIM3->CCMR1 |= (uint16_t)((0x000F << 4) | (0x000F << 12));
    //ʹ����������ж�
    TIM_IT_Update(TIM3);
    //���㶨ʱ��
    TIM_SetCounter(TIM3,0);
    //������ʱ��
    TIM_Cmd(TIM3);
}

//û�в��Ըú����Ƿ���ȷ
void TIM3_Input_Capture_Init(void)
{
    //����GPIOA��TIM3ʱ��
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    //��ʼ��GPIOA6��GPIOA7
    //���ù���ģʽ��������ơ�100MHz��������������
    GPIOA->MODER    |= GPIO_MODER_MODER6_1;
    GPIOA->OTYPER   |= 0x00;
    GPIOA->OSPEEDR  |= GPIO_OSPEEDER_OSPEEDR6;
    GPIOA->PUPDR    |= 0x00;
    //GPIOA6��GPIOA7����ΪTIM3
    GPIO_AF_Set(GPIOA,6,2);
    //��ʼ��TIM3ʱ����Ԫ
    TIM3->CR1 |= 0x00;
    TIM3->CR1 |= 0x00;
    TIM3->PSC = 84-1;
    TIM3->ARR = 65535;
    //TIM3_CH1���벶���ʼ��
    //IC1ӳ�䵽TI1������Ƶ�����˲���ʹ�����벶�񡢷Ƿ���/�����ش���( TIM_CCER_CC1P = 0x00,TIM_CCER_CC1NP = 0x00 )
    TIM3->CCMR1 |= (TIM_CCMR1_CC1S_0 | 0x00 | 0x00);
    TIM3->CCER  |= (TIM_CCER_CC1E | 0x00 | 0x00);
    //������Ʋ��������¼�
    TIM3->EGR |= 1<<0;
    //������1�ж�
    TIM3->DIER |= 1<<1;
    //��������ж�
    TIM_IT_Update(TIM3);
    //�����ж����ȼ�
    NVIC_Init(TIM3_IRQn,1,1);
    //ʹ��TIM3
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
