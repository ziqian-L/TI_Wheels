#include "motor.h"

/********************************************************************
 * 采用L298N驱动电机，需要使用两路PWM，四个IO口来控制正反转和电机转速
 * 一个使用TIM8_CH1(PC6)输出PWM，使用PG7、PG5控制正反转
 * 一个使用TIM8_CH2(PC7)输出PWM，使用PG8、PG6控制正反转
********************************************************************/

#define PWM_MIN	-7200
#define PWM_MAX	7200

void Motor_Init(void)
{
	Motor_drive_Init();
	TIM8_PWM_Init(168-1,7200-1);
}


void Motor_drive_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	//GPIOG时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE);

	/******************初始化正反转GPIO输出******************/
	//PG5、PG6、PG7、PG8输出、挽推、100MHz、下拉
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType	= GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed	= GPIO_High_Speed;
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_UP;
	GPIO_Init(GPIOG,&GPIO_InitStruct);
}


/*****
 * 入口参数：
 *  PSC：预分频系数
 *  ARR：自动重装载值
*****/
void TIM8_PWM_Init(uint16_t PSC,uint16_t ARR)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;

	//GPIOC时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);

	/******************初始化PWM输出******************/
	//PC6、PC7复用、挽推、100MHz、下拉
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType	= GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed	= GPIO_High_Speed;
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_UP;
	GPIO_Init(GPIOC,&GPIO_InitStruct);
	
	//GPIO复用
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_TIM8);
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_TIM8);
	
	//初始化TIM8时基单元
	//向上计数、不分割时钟、自动重装载值、预分频系数
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_Period = ARR;
	TIM_TimeBaseInitStruct.TIM_Prescaler = PSC;
	TIM_TimeBaseInit(TIM8,&TIM_TimeBaseInitStruct);
	
	//初始化TIM8_CH1、TIM8_CH2输出比较的PWM模式
	//PWM1模式、高电平有效、输出比较使能、自动重装载值为0
	TIM_OCInitStruct.TIM_OCMode	= TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse = 0;
	TIM_OC1Init(TIM8,&TIM_OCInitStruct);
	TIM_OC2Init(TIM8,&TIM_OCInitStruct);
	
	//使能定时器
	TIM_Cmd(TIM8,ENABLE);
}

/**********
 * 限幅函数
**********/
void PWM_Limit(int32_t *motorA,int32_t *motorB)
{
	if(*motorA>PWM_MAX)*motorA=PWM_MAX;
	if(*motorA<PWM_MIN)*motorA=PWM_MIN;
	
	if(*motorB>PWM_MAX)*motorB=PWM_MAX;
	if(*motorB<PWM_MIN)*motorB=PWM_MIN;
}

/**********
 * 绝对值函数
**********/
int32_t my_abs(int32_t p)
{
	int q;
	q=p>0?p:(-p);
	return q;
}

/**********
 * 赋值函数
**********/
void Load_PWM(int32_t motorA,int32_t motorB)
{
    //正反转
    if (motorA > 0) MotorA_IN1 = 1, MotorA_IN2 = 0;
    else            MotorA_IN1 = 0, MotorA_IN2 = 1;
    //赋值，PWMA输出
    TIM_SetCompare1(TIM8,my_abs(motorA));

    //正反转
    if (motorB > 0) MotorB_IN3 = 1, MotorB_IN4 = 0;
    else            MotorB_IN3 = 0, MotorB_IN4 = 1;
    //赋值，PWMB输出
    TIM_SetCompare2(TIM8,my_abs(motorB));
}

void Stop_PWM(void)
{
    MotorA_IN1 = 0, MotorA_IN2 = 0;
    MotorB_IN3 = 0, MotorB_IN4 = 0;
    TIM_SetCompare1(TIM8,0);
    TIM_SetCompare2(TIM8,0);
}




