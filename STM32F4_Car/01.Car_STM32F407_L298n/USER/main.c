#include "led.h"
#include "key.h"
#include "delay.h"
#include "usart.h"
#include "motor.h"
#include "encoder.h"
#include "control.h"
#include "pid.h"

int main(void)
{
	//设置系统中断优先级分组2
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    delay_init(168);//延时初始化
    uart_init(115200);//串口1初始化
	Motor_Init();//PWM输出初始化
	Encoder_Init();//编码器初始化
	Control_Init();//控制初始化
    while(1)
    {
    }
}
