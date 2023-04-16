#include "matrixkey.h"
#include "oled.h"
#include "nvic.h"
/******
 * ������̳�ʼ��(CH451L)
 * �������������DOUT : PA7�������жϡ����������
 * �������ݼ�����LOAD : PA5�������ؼ���12λ���
 * ��������������DIN  : PA3�����������CH451L���ڸ�λ����DCLK֮ǰ����͵�ƽ����(��->��->��)��ʹ��4�ߴ��нӿڡ�
 * ��������ʱ����DCLK : PA1����������ʱ����ÿ�������ؼ������ݸ�DIN���������ʱ����ÿ���½���������ݡ�
******/
void Matrix_Key_Init(void)
{
    //GPIOAʱ�ӳ�ʼ��
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    //PA7:��ͨ����ģʽ�����ơ�100MHz������
    GPIOA->MODER    |= 0x00;
    GPIOA->OTYPER   |= 0x00;
    GPIOA->OSPEEDR  |= GPIO_OSPEEDER_OSPEEDR7;
    GPIOA->PUPDR    |= GPIO_PUPDR_PUPDR7_0;
    //PA5��PA3��PA1:ͨ�����ģʽ�����ơ�100MHz������
    GPIOA->MODER    |= GPIO_OSPEEDER_OSPEEDR5_0|GPIO_OSPEEDER_OSPEEDR3_0|GPIO_OSPEEDER_OSPEEDR1_0;
    GPIOA->OTYPER   |= 0x00;
    GPIOA->OSPEEDR  |= GPIO_OSPEEDER_OSPEEDR5|GPIO_OSPEEDER_OSPEEDR3|GPIO_OSPEEDER_OSPEEDR1;
    GPIOA->PUPDR    |= GPIO_PUPDR_PUPDR5_0|GPIO_PUPDR_PUPDR3_0|GPIO_PUPDR_PUPDR1_0;
    //�ⲿ�ж�
    //GPIOA7 ���ӵ��ж��� 7
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI7_PA;
    //�ж��� 7 �½��ش���
    EXTI->IMR   |= (1<<7);
    EXTI->FTSR  |= (1<<7);
	//�����ж����ȼ�
	NVIC_Init(EXTI9_5_IRQn,0,0);
    //CH451L��ʼ��,DIN����͵�ƽ����,����DCLK��LOAD
    DIN  = 0;
    DIN  = 1;
    DCLK = 1;
    LOAD = 1;
    Matrix_Key_Writer(0x0402);
}

/******
 * �������д����(CH451L)
******/
void Matrix_Key_Writer(uint16_t command)
{
	uint8_t i;
    LOAD = 0;
    for(i=0;i<12;i++)
    {
        DIN  = (command&0x0001);
        DCLK = 0;
        DCLK = 1;
        command = command >> 1;
    }
    LOAD = 1;
}

/******
 * ������̼�ֵ�ж�(CH451L)
******/
void Matrix_Key_Data(uint8_t *key)
{
	switch(*key)
	{
        case 88:*key = 1;   break;
        case 89:*key = 2;   break;
        case 91:*key = 3;   break;
        case 90:*key = 4;   break;
        case 80:*key = 5;   break;
        case 81:*key = 6;   break;
        case 83:*key = 7;   break;
        case 82:*key = 8;   break;
        case 72:*key = 9;   break;
        case 73:*key = 10;  break;
        case 75:*key = 11;  break;
        case 74:*key = 12;  break;
        case 64:*key = 13;  break;
        case 65:*key = 14;  break;
        case 67:*key = 15;  break;
        case 66:*key = 16;  break;
		default:*key = 0;   break;
	}
}

/******
 * ��������ж�(CH451L)
******/
void EXTI9_5_IRQHandler(void)
{
    uint8_t i,KEY = 0;
    if (EXTI->PR & (EXTI->IMR))
    {
        Matrix_Key_Writer(0x700);
        for(i=0;i<7;i++)
        {
            KEY = KEY << 1;
            KEY |= DOUT_Read;
            DCLK = 0;
            DCLK = 1;
        }
        EXTI->PR |= 0<<7;
        Matrix_Key_Data(&KEY);
        OLED_Clear();
        OLED_ShowNum(0,0,KEY,2,16,1);
        OLED_Refresh();
    }
}
