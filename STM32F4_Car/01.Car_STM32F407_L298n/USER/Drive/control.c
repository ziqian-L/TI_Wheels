#include "control.h"
#include "usart.h"
#include "motor.h"
#include "encoder.h"
#include "pid.h"
#include "oled.h"
#include "mpu6050.h"
#include "inv_mpu.h"

/********************************************************************
 * 控制均在中断里完成，使用TIM9作为定时中断
********************************************************************/
//PID控制
PID_TypeDef PID_Left,PID_Right;

//通过编码器获取的速度
int16_t Left_Wheel_speed,Right_Wheel_speed;
int32_t Left_Wheel_PWM,Right_Wheel_PWM;

//MPU6050获取的值
float Pitch,Roll,Yaw;		//角度

/*****
 * 入口参数：
 *  PSC：预分频系数
 *  ARR：自动重装载值
*****/
void TIM9_Timed_Interrupt(uint32_t PSC,uint32_t ARR)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    NVIC_InitTypeDef NVIC_InitStructure;

    //TIM9时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9,ENABLE);

    //初始化TIM9时基单元
	//向上计数、不分割时钟、自动重装载值、预分频系数
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_Period = ARR;
	TIM_TimeBaseInitStruct.TIM_Prescaler = PSC;
	TIM_TimeBaseInit(TIM9,&TIM_TimeBaseInitStruct);

    //中断优先级配置
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_BRK_TIM9_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    //使能定时器中断
    TIM_ITConfig(TIM9,TIM_IT_Update,ENABLE);
	
	//使能定时器9
	TIM_Cmd(TIM9,ENABLE);
}



/*****
 * TIM9定时中断服务函数
*****/
void TIM1_BRK_TIM9_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM9,TIM_IT_Update) != RESET)
    {	
		/*********************编码器控速*********************/

		/*1.定时读取编码器、MPU6050的值*/
        Left_Wheel_speed = Read_Speed(3);
        Right_Wheel_speed = Read_Speed(4);
        mpu_dmp_get_data(&Pitch,&Roll,&Yaw);
        /*2.PID控制*/
		Left_Wheel_PWM = Positional_PID_Contorl(&PID_Left,Left_Wheel_speed);
        Right_Wheel_PWM = Positional_PID_Contorl(&PID_Right,Right_Wheel_speed);
        /*3.PWM输出*/
        PWM_Limit(&Left_Wheel_PWM,&Right_Wheel_PWM);
        Load_PWM(Left_Wheel_PWM,Right_Wheel_PWM);
    }
    TIM_ClearFlag(TIM9,TIM_IT_Update);
}






