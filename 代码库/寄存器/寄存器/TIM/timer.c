#include "timer.h"

/******************************************״̬��ȡ******************************************/

/******
 * ��ȡ�����жϱ�־
 * TIM10/11/13/14û��TIMx_DIER������ʹ�øú���
******/
int TIM_GetITUpdate_Status(TIM_TypeDef* TIMx)
{
    if (((TIMx->SR & (uint16_t)0x01) != 0) && ((TIMx->DIER & (uint16_t)0x01) != 0))
        return 1;
    else 
        return 0;
}

/******
 * ��������жϱ�־
 * TIM10/11/13/14û��TIMx_DIER������ʹ�øú���
******/
void TIM_ClearITUpdate(TIM_TypeDef* TIMx)
{
    TIMx->SR = (uint16_t)~0x01;
}

/******************************************ʹ�ܺ���******************************************/

/******
 * ���� TIMx
******/
void TIM_Cmd(TIM_TypeDef* TIMx)
{
    TIMx->CR1 |= 1<<0;
}

/******
 * ʹ�� TIMx �� x ͨ���ϵ�Ԥװ�ؼĴ���
******/
void TIM_Preload_EN(TIM_TypeDef* TIMx,uint8_t CH)
{
    if (CH == 1)
    {
        TIMx->CCMR1 |= TIM_CCMR1_OC1PE;
    }
    else if(CH == 2)
    {
        TIMx->CCMR1 |= TIM_CCMR1_OC2PE;
    }
    else if(CH == 3)
    {
        TIMx->CCMR2 |= TIM_CCMR2_OC3PE;
    }
    else if (CH == 4)
    {
        TIMx->CCMR2 |= TIM_CCMR2_OC4PE;
    }   
}

/******
 * ʹ���Զ�����Ԥװ�أ���ʹ�� TIMx_ARR �Ĵ������л���
******/
void TIM_ARR_Preload_EN(TIM_TypeDef* TIMx)
{
    TIMx->CR1 |= TIM_CR1_ARPE;
}

/******
 * ���ø����ж�
 * TIM10/11/13/14û��TIMx_DIER������ʹ�øú���
******/
void TIM_IT_Update(TIM_TypeDef* TIMx)
{
    TIMx->DIER |= TIM_DIER_UIE;
}

/******************************************�޸Ķ�ʱ������ֵ******************************************/

/******
 * �޸Ķ�ʱ������ֵ�����޸� TIMx->CNT
******/
void TIM_SetCounter(TIM_TypeDef* TIMx,uint32_t Counter)
{
    TIMx->CNT = Counter;
}

/******************************************ʱ����Ԫ��ʼ��******************************************/

void TIM1_Init(uint16_t psc,uint16_t arr)
{
    //APB2ʱ�ӳ�ʼ��
    RCC->APB2ENR |= 1<<0;
    //TIM1��TIM8��ʱ����ʼ��
    //���ϼ���ģʽ��ʱ�Ӳ���Ƶ���Զ���װ��ֵ��Ԥ��Ƶϵ�����ظ�����ֵΪ0
    //TIM1��TIM8��ʱ����ʼ��,5ms���һ��(psc=168-1,arr=5000-1)
    TIM1->CR1 |= 0<<4;
    TIM1->CR1 |= 0<<8;
    TIM1->PSC = psc;
    TIM1->ARR = arr;
    TIM1->RCR |= 0; //һ���ò���
}

void TIM2_Init(uint32_t psc,uint32_t arr)
{
    //APB1ʱ�ӳ�ʼ��
    RCC->APB1ENR |= 1<<0;
    //���ϼ���ģʽ��ʱ�Ӳ���Ƶ���Զ���װ��ֵ��Ԥ��Ƶϵ��
    //װ��ֵ���趨Ϊ0xffff�����µ�ֵʱ��TIM2��TIM5��16λ��������װ��ֵ���趨Ϊ0xffff-0xffffffff֮���ʱ��TIM2��TIM5��32λ��������
    //TIM2��TIM3��TIM4��TIM5��ʱ����ʼ��
    TIM2->CR1 |= 0<<4;
    TIM2->CR1 |= 0<<8;
    TIM2->PSC = psc;
    TIM2->ARR = arr;
}

void TIM9_Init(uint16_t psc,uint16_t arr)
{
    //APB2ʱ�ӳ�ʼ��
    RCC->APB2ENR |= 1<<16;
    //Ĭ�����ϼ�����ʱ�Ӳ���Ƶ���Զ���װ��ֵ��Ԥ��Ƶϵ��
    //TIM9~TIM14��ʱ����ʼ��
    TIM9->CR1 |= 0<<8;
    TIM9->PSC = psc;
    TIM9->ARR = arr;
}

void TIM6_Init(uint16_t psc,uint16_t arr)
{
    //APB1ʱ�ӳ�ʼ��
    RCC->APB1ENR |= 1<<5;
    //Ĭ�����ϼ������Զ���װ��ֵ��Ԥ��Ƶϵ��
    //TIM6��TIM7��ʱ����ʼ��
    TIM6->PSC = psc;
    TIM6->ARR = arr;
}

/******************************************����Ƚϳ�ʼ��******************************************/

/******
 * �����ڣ� TIM1��TIM8
 * ͨ������ 4
 * CCMR1/CCMR2: ����Ƚ�ģʽѡ��
 * CCER:        ����Ƚ�ʹ�ܡ�����Ƚϼ��ԡ���������Ƚ�ʹ�ܡ������������
 * CR2:         �������״̬�������������״̬
 * CCRx:        �ȽϼĴ��� x ��Ԥװ��ֵ
******/
void TIM1_Output_Compare_x_Init(uint8_t CH)
{
    if (CH == 1)
    {
        //TIM1_CH1����Ƚϳ�ʼ��
        //PWM1ģʽ
        TIM1->CCMR1 |= (TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1);
        //ʹ������Ƚϡ�����Ƚϼ���Ϊ�ߡ���ʹ�ܻ�������Ƚϡ������������Ϊ��
        TIM1->CCER  |= (TIM_CCER_CC1E | TIM_CCER_CC1P | 0x00 | 0x00);
        //�������״̬�������������״̬������Ϊ0
        TIM1->CR2   |= 0x00;
        //����Ƚϵ�Ԥװ��ֵ
        TIM1->CCR1  |= 0x00;
    }
    else if (CH == 2)
    {
        //TIM1_CH2����Ƚϳ�ʼ��
        //PWM1ģʽ
        TIM1->CCMR1 |= (TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1);
        //ʹ������Ƚϡ�����Ƚϼ���Ϊ�ߡ���ʹ�ܻ�������Ƚϡ������������Ϊ��
        TIM1->CCER  |= (TIM_CCER_CC2E | TIM_CCER_CC2P | 0x00 | 0x00);
        //�������״̬�������������״̬������Ϊ0
        TIM1->CR2   |= 0x00;
        //����Ƚϵ�Ԥװ��ֵ
        TIM1->CCR2  |= 0x00;
    }
    else if (CH == 3)
    {
        //TIM1_CH3����Ƚϳ�ʼ��
        //PWM1ģʽ
        TIM1->CCMR2 |= (TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1);
        //ʹ������Ƚϡ�����Ƚϼ���Ϊ�ߡ���ʹ�ܻ�������Ƚϡ������������Ϊ��
        TIM1->CCER  |= (TIM_CCER_CC3E | TIM_CCER_CC3P | 0x00 | 0x00);
        //�������״̬�������������״̬������Ϊ0
        TIM1->CR2   |= 0x00;
        //����Ƚϵ�Ԥװ��ֵ
        TIM1->CCR3  |= 0x00;
    }
    else if (CH == 4)
    {
        //TIM1_CH4����Ƚϳ�ʼ��
        //PWM1ģʽ
        TIM1->CCMR2 |= (TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1);
        //ʹ������Ƚϡ�����Ƚϼ���Ϊ��
        TIM1->CCER  |= (TIM_CCER_CC4E | TIM_CCER_CC4P);
        //�������״̬
        TIM1->CR2   |= 0x00;
        //����Ƚϵ�Ԥװ��ֵ
        TIM1->CCR4  |= 0x00;
    }
}

/******
 * �����ڣ� TIM2��TIM3��TIM4��TIM5
 * ͨ������ 4
 * CCMR1/CCMR2: ����Ƚ�ģʽѡ��
 * CCER:        ����Ƚ�ʹ�ܡ�����Ƚϼ���
 * CCRx:        �ȽϼĴ��� x ��Ԥװ��ֵ
******/
void TIM2_Output_Compare_x_Init(uint8_t CH)
{
    if (CH == 1)
    {
        //TIM2_CH1����Ƚϳ�ʼ��
        //PWM1ģʽ
        TIM2->CCMR1 |= (TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1);
        //ʹ������Ƚϡ�����Ƚϼ���Ϊ��
        TIM2->CCER  |= (TIM_CCER_CC1E | TIM_CCER_CC1P);
        //����Ƚϵ�Ԥװ��ֵ
        TIM2->CCR1  |= 0x00;
    }
    else if (CH == 2)
    {
        //TIM2_CH2����Ƚϳ�ʼ��
        //PWM1ģʽ
        TIM2->CCMR1 |= (TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1);
        //ʹ������Ƚϡ�����Ƚϼ���Ϊ��
        TIM2->CCER  |= (TIM_CCER_CC2E | TIM_CCER_CC2P);
        //����Ƚϵ�Ԥװ��ֵ
        TIM2->CCR2  |= 0x00;        
    }
    else if (CH == 3)
    {
        //TIM2_CH3����Ƚϳ�ʼ��
        //PWM1ģʽ
        TIM2->CCMR2 |= (TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1);
        //ʹ������Ƚϡ�����Ƚϼ���Ϊ��
        TIM2->CCER  |= (TIM_CCER_CC3E | TIM_CCER_CC3P);
        //����Ƚϵ�Ԥװ��ֵ
        TIM2->CCR3  |= 0x00;        
    }
     else if (CH == 4)
    {
        //TIM2_CH4����Ƚϳ�ʼ��
        //PWM1ģʽ
        TIM2->CCMR2 |= (TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1);
        //ʹ������Ƚϡ�����Ƚϼ���Ϊ��
        TIM2->CCER  |= (TIM_CCER_CC4E | TIM_CCER_CC4P);
        //����Ƚϵ�Ԥװ��ֵ
        TIM2->CCR4  |= 0x00;        
    } 
}

/******
 * �����ڣ� TIM9��TIM12
 * ͨ������ 2
 * CCMR1:  ����Ƚ�ģʽѡ��
 * CCER:   ����Ƚ�ʹ�ܡ�����Ƚϼ���
 * CCRx:   �ȽϼĴ��� x ��Ԥװ��ֵ
******/
void TIM9_Output_Compare_x_Init(uint8_t CH)
{
    if (CH == 1)
    {
        //TIM9_CH1����Ƚϳ�ʼ��
        //PWM1ģʽ
        TIM9->CCMR1 |= (TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1);
        //ʹ������Ƚϡ�����Ƚϼ���Ϊ��
        TIM9->CCER  |= (TIM_CCER_CC1E | TIM_CCER_CC1P);
        //����Ƚϵ�Ԥװ��ֵ
        TIM9->CCR1  |= 0x00;
    }
    else if (CH == 2)
    {
        //TIM9_CH2����Ƚϳ�ʼ��
        //PWM1ģʽ
        TIM9->CCMR1 |= (TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1);
        //ʹ������Ƚϡ�����Ƚϼ���Ϊ��
        TIM9->CCER  |= (TIM_CCER_CC2E | TIM_CCER_CC2P);
        //����Ƚϵ�Ԥװ��ֵ
        TIM9->CCR2  |= 0x00;  
    }
}

/******
 * �����ڣ� TIM10��TIM11��TIM13��TIM14
 * ͨ������ 1
 * CCMR1:  ����Ƚ�ģʽѡ��
 * CCER:   ����Ƚ�ʹ�ܡ�����Ƚϼ���
 * CCRx:   �ȽϼĴ��� x ��Ԥװ��ֵ
******/
void TIM10_Output_Compare_x_Init(void)
{
    //TIM9_CH1����Ƚϳ�ʼ��
    //PWM1ģʽ
    TIM10->CCMR1 |= (TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1);
    //ʹ������Ƚϡ�����Ƚϼ���Ϊ��
    TIM10->CCER  |= (TIM_CCER_CC1E | TIM_CCER_CC1P);
    //����Ƚϵ�Ԥװ��ֵ
    TIM10->CCR1  |= 0x00;
}

//û�в��Ըú����Ƿ���ȷ
/******************************************���벶���ʼ��******************************************/

/******
 * �����ڣ� TIM1��TIM8
 * ͨ������ 4
 * CCMR1/CCMR2: ӳ���ϵ����Ƶϵ�����˲���
 * CCER:        ʹ�����벶�񡢱��ش�����ʽ
******/
void TIM1_Input_Capture_x_Init(uint8_t CH)
{
    if (CH == 1)
    {
        //TIM1_CH1���벶���ʼ��
        //IC1ӳ�䵽TI1������Ƶ�����˲�
        TIM1->CCMR1 |= (TIM_CCMR1_CC1S_0 | 0x00 | 0x00);
        //ʹ�����벶�񡢷Ƿ���/�����ش���( TIM_CCER_CC1P = 0x00,TIM_CCER_CC1NP = 0x00 )
        TIM1->CCER  |= (TIM_CCER_CC1E | 0x00 | 0x00);
    }
    else if (CH ==2)
    {
        //TIM1_CH2���벶���ʼ��
        //IC2ӳ�䵽TI2������Ƶ�����˲�
        TIM1->CCMR1 |= (TIM_CCMR1_CC2S_0 | 0x00 | 0x00);
        //ʹ�����벶�񡢷Ƿ���/�����ش���( TIM_CCER_CC2P = 0x00,TIM_CCER_CC2NP = 0x00 )
        TIM1->CCER  |= (TIM_CCER_CC2E | 0x00 | 0x00);
    }
    else if (CH == 3)
    {
        //TIM1_CH3���벶���ʼ��
        //IC3ӳ�䵽TI3������Ƶ�����˲�
        TIM1->CCMR2 |= (TIM_CCMR2_CC3S_0 | 0x00 | 0x00);
        //ʹ�����벶�񡢷Ƿ���/�����ش���( TIM_CCER_CC3P = 0x00,TIM_CCER_CC3NP = 0x00 )
        TIM1->CCER  |= (TIM_CCER_CC3E | 0x00 | 0x00);        
    }
    else if (CH == 4)
    {
        //TIM1_CH4���벶���ʼ��
        //IC4ӳ�䵽TI4������Ƶ�����˲�
        TIM1->CCMR2 |= (TIM_CCMR2_CC4S_0 | 0x00 | 0x00);
        //ʹ�����벶�񡢷Ƿ���/�����ش���( TIM_CCER_CC4P = 0x00,TIM_CCER_CC4NP = 0x00 )
        TIM1->CCER  |= (TIM_CCER_CC4E | 0x00 | 0x00);        
    }
}

/******
 * �����ڣ� TIM2��TIM3��TIM4��TIM5
 * ͨ������ 4
 * CCMR1/CCMR2: ӳ���ϵ����Ƶϵ�����˲���
 * CCER:        ʹ�����벶�񡢱��ش�����ʽ
******/
void TIM2_Input_Capture_x_Init(uint8_t CH)
{
    if (CH == 1)
    {
        //TIM2_CH1���벶���ʼ��
        //IC1ӳ�䵽TI1������Ƶ�����˲�
        TIM2->CCMR1 |= (TIM_CCMR1_CC1S_0 | 0x00 | 0x00);
        //ʹ�����벶�񡢷Ƿ���/�����ش���( TIM_CCER_CC1P = 0x00,TIM_CCER_CC1NP = 0x00 )
        TIM2->CCER  |= (TIM_CCER_CC1E | 0x00 | 0x00);
    }
    else if (CH ==2)
    {
        //TIM2_CH2���벶���ʼ��
        //IC2ӳ�䵽TI2������Ƶ�����˲�
        TIM2->CCMR1 |= (TIM_CCMR1_CC2S_0 | 0x00 | 0x00);
        //ʹ�����벶�񡢷Ƿ���/�����ش���( TIM_CCER_CC2P = 0x00,TIM_CCER_CC2NP = 0x00 )
        TIM2->CCER  |= (TIM_CCER_CC2E | 0x00 | 0x00);
    }
    else if (CH == 3)
    {
        //TIM2_CH3���벶���ʼ��
        //IC3ӳ�䵽TI3������Ƶ�����˲�
        TIM2->CCMR2 |= (TIM_CCMR2_CC3S_0 | 0x00 | 0x00);
        //ʹ�����벶�񡢷Ƿ���/�����ش���( TIM_CCER_CC3P = 0x00,TIM_CCER_CC3NP = 0x00 )
        TIM2->CCER  |= (TIM_CCER_CC3E | 0x00 | 0x00);        
    }
    else if (CH == 4)
    {
        //TIM2_CH4���벶���ʼ��
        //IC4ӳ�䵽TI4������Ƶ�����˲�
        TIM2->CCMR2 |= (TIM_CCMR2_CC4S_0 | 0x00 | 0x00);
        //ʹ�����벶�񡢷Ƿ���/�����ش���( TIM_CCER_CC4P = 0x00,TIM_CCER_CC4NP = 0x00 )
        TIM2->CCER  |= (TIM_CCER_CC4E | 0x00 | 0x00);        
    }
}

/******
 * �����ڣ� TIM9��TIM12
 * ͨ������ 2
 * CCMR1:  ӳ���ϵ����Ƶϵ�����˲���
 * CCER:   ʹ�����벶�񡢱��ش�����ʽ
******/
void TIM9_Input_Capture_x_Init(uint8_t CH)
{
    if (CH == 1)
    {
        //TIM9_CH1���벶���ʼ��
        //IC1ӳ�䵽TI1������Ƶ�����˲�
        TIM9->CCMR1 |= (TIM_CCMR1_CC1S_0 | 0x00 | 0x00);
        //ʹ�����벶�񡢷Ƿ���/�����ش���( TIM_CCER_CC1P = 0x00,TIM_CCER_CC1NP = 0x00 )
        TIM9->CCER  |= (TIM_CCER_CC1E | 0x00 | 0x00);
    }
    else if (CH ==2)
    {
        //TIM9_CH2���벶���ʼ��
        //IC2ӳ�䵽TI2������Ƶ�����˲�
        TIM9->CCMR1 |= (TIM_CCMR1_CC2S_0 | 0x00 | 0x00);
        //ʹ�����벶�񡢷Ƿ���/�����ش���( TIM_CCER_CC2P = 0x00,TIM_CCER_CC2NP = 0x00 )
        TIM9->CCER  |= (TIM_CCER_CC2E | 0x00 | 0x00);
    }
}

/******
 * �����ڣ� TIM10��TIM11��TIM13��TIM14
 * ͨ������ 1
 * CCMR1:  ӳ���ϵ����Ƶϵ�����˲���
 * CCER:   ʹ�����벶�񡢱��ش�����ʽ
******/
void TIM10_Input_Capture_x_Init(void)
{
    //TIM10_CH1���벶���ʼ��
    //IC1ӳ�䵽TI1������Ƶ�����˲�
    TIM10->CCMR1 |= (TIM_CCMR1_CC1S_0 | 0x00 | 0x00);
    //ʹ�����벶�񡢷Ƿ���/�����ش���( TIM_CCER_CC1P = 0x00,TIM_CCER_CC1NP = 0x00 )
    TIM10->CCER  |= (TIM_CCER_CC1E | 0x00 | 0x00);
}

//û�в��Ըú����Ƿ���ȷ
/******************************************��ʱ��������ģʽ******************************************/

/******
 * �����ڣ� TIM1��TIM2��TIM3��TIM4��TIM5��TIM8��TIM9��TIM12
 * ͨ����   CH1��CH2 
 * SMCR:    ������ģʽ
 * CCMR1:   ����ͨ����ͬʱҲ���������˲�
 * CCER:    ������ʽ
******/
void TIM_EncoderConfig(TIM_TypeDef* TIMx,uint16_t EncoderMode,uint16_t TIM_IC1Polarity, uint16_t TIM_IC2Polarity)
{
    //���ñ�����ģʽ
    TIMx->SMCR  |= EncoderMode;
    //��������ͨ��
    TIMx->CCMR1 |= ( TIM_CCMR1_CC1S_0 | TIM_CCMR1_CC2S_0 );
    //������ʽ
    TIMx->CCER  |= (TIM_IC1Polarity | (TIM_IC2Polarity << 4));
}

