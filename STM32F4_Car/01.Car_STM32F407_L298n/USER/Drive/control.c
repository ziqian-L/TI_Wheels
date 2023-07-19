#include "control.h"
#include "usart.h"
#include "motor.h"
#include "encoder.h"
#include "pid.h"
#include "oled.h"
#include "mpu6050.h"
#include "inv_mpu.h"

/********************************************************************
 * ���ƾ����ж�����ɣ�ʹ��TIM9��Ϊ��ʱ�ж�
********************************************************************/
//PID����
PID_TypeDef PID_Left,PID_Right;

//ͨ����������ȡ���ٶ�
int16_t Left_Wheel_speed,Right_Wheel_speed;
int32_t Left_Wheel_PWM,Right_Wheel_PWM;

//MPU6050��ȡ��ֵ
float Pitch,Roll,Yaw;		//�Ƕ�

/*****
 * ��ڲ�����
 *  PSC��Ԥ��Ƶϵ��
 *  ARR���Զ���װ��ֵ
*****/
void TIM9_Timed_Interrupt(uint32_t PSC,uint32_t ARR)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    NVIC_InitTypeDef NVIC_InitStructure;

    //TIM9ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9,ENABLE);

    //��ʼ��TIM9ʱ����Ԫ
	//���ϼ��������ָ�ʱ�ӡ��Զ���װ��ֵ��Ԥ��Ƶϵ��
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_Period = ARR;
	TIM_TimeBaseInitStruct.TIM_Prescaler = PSC;
	TIM_TimeBaseInit(TIM9,&TIM_TimeBaseInitStruct);

    //�ж����ȼ�����
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_BRK_TIM9_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    //ʹ�ܶ�ʱ���ж�
    TIM_ITConfig(TIM9,TIM_IT_Update,ENABLE);
	
	//ʹ�ܶ�ʱ��9
	TIM_Cmd(TIM9,ENABLE);
}



/*****
 * TIM9��ʱ�жϷ�����
*****/
void TIM1_BRK_TIM9_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM9,TIM_IT_Update) != RESET)
    {	
		/*********************����������*********************/

		/*1.��ʱ��ȡ��������MPU6050��ֵ*/
        Left_Wheel_speed = Read_Speed(3);
        Right_Wheel_speed = Read_Speed(4);
        mpu_dmp_get_data(&Pitch,&Roll,&Yaw);
        /*2.PID����*/
		Left_Wheel_PWM = Positional_PID_Contorl(&PID_Left,Left_Wheel_speed);
        Right_Wheel_PWM = Positional_PID_Contorl(&PID_Right,Right_Wheel_speed);
        /*3.PWM���*/
        PWM_Limit(&Left_Wheel_PWM,&Right_Wheel_PWM);
        Load_PWM(Left_Wheel_PWM,Right_Wheel_PWM);
    }
    TIM_ClearFlag(TIM9,TIM_IT_Update);
}






