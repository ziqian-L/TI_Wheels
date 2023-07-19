#include "motor.h"

/********************************************************************
 * ����L298N�����������Ҫʹ����·PWM���ĸ�IO������������ת�͵��ת��
 * һ��ʹ��TIM8_CH1(PC6)���PWM��ʹ��PG7��PG5��������ת
 * һ��ʹ��TIM8_CH2(PC7)���PWM��ʹ��PG8��PG6��������ת
********************************************************************/

#define PWM_MIN	-7200
#define PWM_MAX	7200

void Motor_drive_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	//GPIOGʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE);

	/******************��ʼ������תGPIO���******************/
	//PG5��PG6��PG7��PG8��������ơ�100MHz������
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType	= GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed	= GPIO_High_Speed;
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_UP;
	GPIO_Init(GPIOG,&GPIO_InitStruct);
}


/*****
 * ��ڲ�����
 *  PSC��Ԥ��Ƶϵ��
 *  ARR���Զ���װ��ֵ
*****/
void TIM8_PWM_Init(uint16_t PSC,uint16_t ARR)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	//GPIOC��TIM8ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);
	/******************��ʼ��PWM���******************/
	//PC6��PC7���á����ơ�100MHz������
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType	= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_DOWN;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	//GPIO����
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_TIM8);
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_TIM8);
	
	//��ʼ��TIM8ʱ����Ԫ
	//���ϼ��������ָ�ʱ�ӡ��Զ���װ��ֵ��Ԥ��Ƶϵ��
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_Period = ARR;
	TIM_TimeBaseStructure.TIM_Prescaler = PSC;
	TIM_TimeBaseInit(TIM8,&TIM_TimeBaseStructure);
	
	//��ʼ��TIM8_CH1��TIM8_CH2����Ƚϵ�PWMģʽ
	//PWM1ģʽ���ߵ�ƽ��Ч������Ƚ�ʹ�ܡ��Զ���װ��ֵΪ0
	TIM_OCInitStructure.TIM_OCMode	= TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC1Init(TIM8,&TIM_OCInitStructure);

	TIM_OCInitStructure.TIM_OCMode	= TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC2Init(TIM8,&TIM_OCInitStructure);
	
	TIM_OC1PreloadConfig(TIM8,TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM8,TIM_OCPreload_Enable);

    //ʹ���Զ���װ�ص�Ԥװ�ؼĴ�������λ
    TIM_ARRPreloadConfig(TIM8,ENABLE);
	
	//�߼���ʱ�������ʹ��
	TIM_CtrlPWMOutputs(TIM8,ENABLE);
	
	//ʹ�ܶ�ʱ��
	TIM_Cmd(TIM8,ENABLE);
}

/**********
 * �޷�����
**********/
void PWM_Limit(int32_t *motorA,int32_t *motorB)
{
	if(*motorA>PWM_MAX)*motorA=PWM_MAX;
	if(*motorA<PWM_MIN)*motorA=PWM_MIN;
	
	if(*motorB>PWM_MAX)*motorB=PWM_MAX;
	if(*motorB<PWM_MIN)*motorB=PWM_MIN;
}

/**********
 * ����ֵ����
**********/
int32_t my_abs(int32_t p)
{
	int q;
	q=p>0?p:(-p);
	return q;
}

/**********
 * ��ֵ����
**********/
void Load_PWM(int32_t motorA,int32_t motorB)
{
    //����ת
    if (motorA > 0) MotorA_IN1 = 0, MotorA_IN2 = 1;
    else            MotorA_IN1 = 1, MotorA_IN2 = 0;
    //��ֵ��PWMA���
    TIM_SetCompare1(TIM8,my_abs(motorA));

    //����ת
    if (motorB > 0) MotorB_IN3 = 0, MotorB_IN4 = 1;
    else            MotorB_IN3 = 1, MotorB_IN4 = 0;
    //��ֵ��PWMB���
    TIM_SetCompare2(TIM8,my_abs(motorB));
}

void Stop_PWM(void)
{
    MotorA_IN1 = 0, MotorA_IN2 = 0;
    MotorB_IN3 = 0, MotorB_IN4 = 0;
    TIM_SetCompare1(TIM8,0);
    TIM_SetCompare2(TIM8,0);
}




