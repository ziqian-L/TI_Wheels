#include "encoder.h"

/***
 * ���A��������ʼ��
 * TIM3_CH3����PB0
 * TIM3_CH4����PB1
***/
void Encoder_A_TIM3_Init(void)
{
    //GPIOBʱ�ӡ�TIM3ʱ��
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

    //GPIO��ʼ��
    //���á����ơ�100MHz������
    GPIOB->MODER    |= (GPIO_MODER_MODER0_1 | GPIO_MODER_MODER1_1);
    GPIOB->OTYPER   |= 0x00;
    GPIOB->OSPEEDR  |= (GPIO_OSPEEDER_OSPEEDR0 | GPIO_OSPEEDER_OSPEEDR1);
    GPIOB->PUPDR    |= 0x00;

    //GPIO����
    GPIO_AF_Set(GPIOB,0,2);
    GPIO_AF_Set(GPIOB,1,2);

    //ʱ����Ԫ��ʼ��
    //����������ʱ�Ӳ���Ƶ��Ԥ��Ƶϵ��Ϊ1���Զ���װ��ֵΪ65536
    TIM3->CR1 |= 0x00;
    TIM3->PSC  = 0;
    TIM3->ARR  = 65535;

    //���ñ�����ģʽ
    //������ģʽ3(0x03)�����벶��ͨ��CH3��CH4���Ƿ���/�����ش���
    TIM3->SMCR  |= (TIM_SMCR_SMS_1 | TIM_SMCR_SMS_0);
    TIM3->CCMR2 |= (TIM_CCMR2_CC3S_0 | TIM_CCMR2_CC4S_0);
    TIM3->CCER  |= 0x00;

    //TIM3_CH3���벶���ʼ��
    //IC3ӳ�䵽TI3������Ƶ���˲�(fSAMPLING=fCK_INT��N=8)
    TIM3->CCMR2 |= (TIM_CCMR2_CC3S_0 | 0x00 | (TIM_CCMR2_IC3F_1 | TIM_CCMR2_IC3F_0));
    //ʹ�����벶�񡢷Ƿ���/�����ش���( TIM_CCER_CC3P = 0x00,TIM_CCER_CC3NP = 0x00 )
    TIM3->CCER  |= (TIM_CCER_CC3E | 0x00 | 0x00);
    //TIM3_CH4���벶���ʼ��
    //IC4ӳ�䵽TI4������Ƶ���˲�(fSAMPLING=fCK_INT��N=8)
    TIM3->CCMR2 |= (TIM_CCMR2_CC4S_0 | 0x00 | (TIM_CCMR2_IC4F_1 | TIM_CCMR2_IC4F_0));
    //ʹ�����벶�񡢷Ƿ���/�����ش���( TIM_CCER_CC4P = 0x00,TIM_CCER_CC4NP = 0x00 )
    TIM3->CCER  |= (TIM_CCER_CC4E | 0x00 | 0x00);

    //��������жϱ�־λ
    TIM3->SR = (uint16_t)~TIM_SR_UIF;
    //ʹ�ܸ����ж�
    TIM3->DIER |= TIM_DIER_UIE;
    //������ֵ����
    TIM3->CNT = 0;
    //ʹ�ܶ�ʱ��
    TIM_Cmd(TIM3,ENABLE);
}

/***
 * ���B��������ʼ��
 * TIM4_CH1����PB6
 * TIM4_CH2����PB7
***/
void Encoder_B_TIM4_Init(void)
{
    //GPIOBʱ�ӡ�TIM4ʱ��
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;

    //GPIO��ʼ��
    //���á����ơ�100MHz������
    GPIOB->MODER    |= (GPIO_MODER_MODER6_1 | GPIO_MODER_MODER7_1);
    GPIOB->OTYPER   |= 0x00;
    GPIOB->OSPEEDR  |= (GPIO_OSPEEDER_OSPEEDR6 | GPIO_OSPEEDER_OSPEEDR7);
    GPIOB->PUPDR    |= 0x00;
    //GPIO����
    GPIO_AF_Set(GPIOB,6,2);
    GPIO_AF_Set(GPIOB,7,2);

    //ʱ����Ԫ��ʼ��
    //����������ʱ�Ӳ���Ƶ��Ԥ��Ƶϵ��Ϊ1���Զ���װ��ֵΪ65535
    TIM4->CR1 |= 0x00;
    TIM4->PSC  = 0;
    TIM4->ARR  = 65535;

    //���ñ�����ģʽ
    //������ģʽ3(0x03)�����벶��ͨ��CH1��CH2���Ƿ���/�����ش���
    TIM4->SMCR  |= (TIM_SMCR_SMS_1 | TIM_SMCR_SMS_0);
    TIM4->CCMR1 |= (TIM_CCMR1_CC1S_0 | TIM_CCMR1_CC2S_0);
    TIM4->CCER  |= 0x00;
    
    //TIM4_CH1���벶���ʼ��
    //IC1ӳ�䵽TI1������Ƶ���˲�(fSAMPLING=fCK_INT��N=8)
    TIM4->CCMR1 |= (TIM_CCMR1_CC1S_0 | 0x00 | (TIM_CCMR1_IC1F_1 | TIM_CCMR1_IC1F_0));
    //ʹ�����벶�񡢷Ƿ���/�����ش���( TIM_CCER_CC1P = 0x00,TIM_CCER_CC1NP = 0x00 )
    TIM4->CCER  |= (TIM_CCER_CC1E | 0x00 | 0x00);
    //TIM4_CH2���벶���ʼ��
    //IC2ӳ�䵽TI2������Ƶ���˲�(fSAMPLING=fCK_INT��N=8)
    TIM4->CCMR1 |= (TIM_CCMR1_CC2S_0 | 0x00 | (TIM_CCMR1_IC2F_1 | TIM_CCMR1_IC2F_0));
    //ʹ�����벶�񡢷Ƿ���/�����ش���( TIM_CCER_CC2P = 0x00,TIM_CCER_CC2NP = 0x00 )
    TIM4->CCER  |= (TIM_CCER_CC2E | 0x00 | 0x00);

    //��������жϱ�־λ
    TIM4->SR = (uint16_t)~TIM_SR_UIF;
    //ʹ�ܸ����ж�
    TIM4->DIER |= TIM_DIER_UIE;
    //������ֵ����
    TIM4->CNT = 0;
    //ʹ�ܶ�ʱ��
    TIM_Cmd(TIM4,ENABLE);
}

/**********************
������
�ٶȶ�ȡ����
��ڲ�������ʱ��
**********************/
int Read_Speed(int TIMx)
{
	int value_1;
	switch(TIMx)
	{
        //1.�ɼ��������ļ���ֵ�����档2.����ʱ���ļ���ֵ���㡣
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
