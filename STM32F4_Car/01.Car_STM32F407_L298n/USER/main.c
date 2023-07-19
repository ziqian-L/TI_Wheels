#include "delay.h"
#include "usart.h"
#include "motor.h"
#include "encoder.h"
#include "control.h"
#include "pid.h"
#include "led.h"
#include "key.h"
#include "oled.h"
#include "mpu6050.h"

/*****
 * 左电机：
 *      编码器：TIM3_CH1(PA6)、TIM4_CH2(PA7)
 *      PWM输出：TIM8_CH1(PC6)
 *      正反转：PG7、PG5
 * 右电机：
 *      编码器：TIM4_CH1(PB6)、TIM4_CH2(PB7)
 *      PWM输出：TIM8_CH2(PC7)
 *      正反转：PG8、PG6
 * MPU6050：PC1(SCL)、PC2(SDA)
 * 蓝牙：PA2(USART2_TX)、PA3(USART2_RX)
*****/

#define DEBUG

int main(void)
{
    /*系统初始化*/
	//中断优先级分组2
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    delay_init(168);//延时初始化
    UART1_Init(115200);//串口1初始化
	RingBuff_Init(&Uart2_RingBuff);
	/*板载外设初始化*/
    LED_Init();//板载LED
	KEY_Init();//板载按键
	OLED_Init();//0.96寸OLED-SSD1306
    /*外接模块初始化*/	
	MPU_Init();
	DMP_Init();
	/*控制初始化*/
	PID_Init();
	UART2_Init(115200);//串口2
	Motor_drive_Init();//正反转
	TIM8_PWM_Init(168-1,7200-1);//PWM输出
	TIM3_Encoder_Init();//左电机编码器
	TIM4_Encoder_Init();//右电机编码器
	TIM9_Timed_Interrupt(168-1,5000);//定时中断
    PID_Encoders_SetPoint(9,9);
	while(1)
	{
	}
}







