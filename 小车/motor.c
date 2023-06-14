#include "motor.h"
#include "pwm.h"

#define PWM_MIN 7000
#define PWM_MAX 7000

//���70�������棬���180�������棬ֱ��
//���ֽ�A,���ֽ�B
/**********
 * ����ת��
 * PWMAʹ��TIM5_CH2��
 * AIN2����PA3
 * AIN1����PA5
 * PWMBʹ��TIM5_CH3��
 * BIN2����PA4
 * BIN1����PA6
**********/
void Motor_Init(void)
{
    //GPIOAʱ��
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    
    //GPIO��ʼ��
    //ͨ����������ơ��ٶȡ�����
    GPIOA->MODER    |= (GPIO_MODER_MODER3_0 | GPIO_MODER_MODER4_0 | GPIO_MODER_MODER5_0 | GPIO_MODER_MODER6_0);
    GPIOA->OTYPER   |= 0x00;
    GPIOA->OSPEEDR  |= (GPIO_OSPEEDER_OSPEEDR3 | GPIO_OSPEEDER_OSPEEDR4 | GPIO_OSPEEDER_OSPEEDR5 | GPIO_OSPEEDER_OSPEEDR6);
    GPIOA->PUPDR    |= 0x00;
}

/**********
 * �޷�����
**********/
void Limit(int32_t *motorA,int32_t *motorB)
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
void Load_PWM(int motorA,int motorB)
{
    //����ת
    if (motorA > 0) Ain1 = 0, Ain2 = 1;
    else            Ain1 = 1, Ain2 = 0;
    //��ֵ��PWMA���
    TIM5_CH2_SetComparex(my_abs(motorA));

    //����ת
    if (motorB > 0) Bin1 = 0, Bin2 = 1;
    else            Bin1 = 1, Bin2 = 0;
    //��ֵ��PWMB���
    TIM5_CH3_SetComparex(my_abs(motorB));
}
