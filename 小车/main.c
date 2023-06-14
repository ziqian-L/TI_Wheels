#include "stm32f4xx.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "led.h"
#include "oled.h"
#include "key.h"
#include "pwm.h"
#include "motor.h"
#include "encoder.h"
#include "servos.h"
#include "buzz.h"


/***
 * ���ֽ�A,���ֽ�B
 * �����PA15
 * ���A����(Ain1)(Ain2)��PA5��PA3
 * ���B����(Bin1)(Bin2)��PA6��PA4
 * ���A��������PB0(TIM3_CH3)��PB1(TIM3_CH4)
 * ���B��������PB6(TIM4_CH1)��PB7(TIM4_CH2)
 * PWM��PA1(TIM5_CH2)��PA2(TIM5_CH3)
 * ��������PE8
 * ����(OpenMV)������1
 ***/
int main(void)
{
	uint8_t i;
    //�ж����ȼ�����
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    delay_init(168);
    uart_init(115200);//����
    LED_Init();
    KEY_Init();
	Buzz_Init();
	OLED_Init();
    Servos_Init();//���
    Motor_Init();//�������
    TIM5_PWM_Init(84,7000);//PWM
    Encoder_A_TIM3_Init();
    Encoder_B_TIM4_Init();
	OLED_ShowString(0,0,"OK!",16,1);
	OLED_Refresh();	
    while(1)
    {
    }
}

/**********
 * ������Դ���
		delay_ms(3000);
        servos_angle(TIM2,1,0);
		delay_ms(3000);
        servos_angle(TIM2,1,45);
		delay_ms(3000);
        servos_angle(TIM2,1,90);
		delay_ms(3000);
        servos_angle(TIM2,1,135);
		delay_ms(3000);
        servos_angle(TIM2,1,180);
		delay_ms(3000);
        for (i = 170; i >= 72; i--)
        {
            OLED_Clear();
            OLED_ShowNum(0,0,i,4,16,1);
            OLED_Refresh();
            servos_angle(TIM2,1,i);
			delay_ms(10);
			if(i==72)
				delay_ms(6000);
            if(i==170)
                delay_ms(6000);
        }
        servos_angle(TIM2,1,110);



        for (i = 170; i >= 72; i--)
        {
            OLED_Clear();
            OLED_ShowNum(0,0,i,4,16,1);
            OLED_Refresh();
            servos_angle(TIM2,1,i);
			delay_ms(10);
			if(i==72)
				delay_ms(1500);
            if(i==170)
                delay_ms(1500);
        }
        servos_angle(TIM2,1,110);
        delay_ms(1500);
**********/



