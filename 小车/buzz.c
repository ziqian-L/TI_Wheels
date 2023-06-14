#include "buzz.h"

/**
 * ��������PE8
 * Ĭ������
**/
void Buzz_Init(void)
{
    //GPIOʱ��
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
    //GPIO��ʼ��
    //ͨ����������ơ�100MHz������
    GPIOE->MODER    |= GPIO_MODER_MODER8_0;
    GPIOE->OTYPER   |= 0x00;
    GPIOE->OSPEEDR  |= GPIO_OSPEEDER_OSPEEDR8;
    GPIOE->PUPDR    |= GPIO_PUPDR_PUPDR8_1;
}


void Buzz(uint8_t flag)
{
    if (flag == 0)
        Buzz_PE8 = 0;
    else if (flag == 1)
        Buzz_PE8 = 1;
    else
        Buzz_PE8 = 0;
}
