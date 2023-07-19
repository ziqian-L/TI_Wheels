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
 * ������
 *      ��������TIM3_CH1(PA6)��TIM4_CH2(PA7)
 *      PWM�����TIM8_CH1(PC6)
 *      ����ת��PG7��PG5
 * �ҵ����
 *      ��������TIM4_CH1(PB6)��TIM4_CH2(PB7)
 *      PWM�����TIM8_CH2(PC7)
 *      ����ת��PG8��PG6
 * MPU6050��PC1(SCL)��PC2(SDA)
 * ������PA2(USART2_TX)��PA3(USART2_RX)
*****/

#define DEBUG

int main(void)
{
    /*ϵͳ��ʼ��*/
	//�ж����ȼ�����2
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    delay_init(168);//��ʱ��ʼ��
    UART1_Init(115200);//����1��ʼ��
	RingBuff_Init(&Uart2_RingBuff);
	/*���������ʼ��*/
    LED_Init();//����LED
	KEY_Init();//���ذ���
	OLED_Init();//0.96��OLED-SSD1306
    /*���ģ���ʼ��*/	
	MPU_Init();
	DMP_Init();
	/*���Ƴ�ʼ��*/
	PID_Init();
	UART2_Init(115200);//����2
	Motor_drive_Init();//����ת
	TIM8_PWM_Init(168-1,7200-1);//PWM���
	TIM3_Encoder_Init();//����������
	TIM4_Encoder_Init();//�ҵ��������
	TIM9_Timed_Interrupt(168-1,5000);//��ʱ�ж�
    PID_Encoders_SetPoint(9,9);
	while(1)
	{
	}
}







