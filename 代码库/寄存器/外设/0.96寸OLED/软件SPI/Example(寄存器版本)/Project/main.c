#include "sys.h"
#include "usart.h" 
#include "delay.h"
#include "oled.h"
#include "screen.h"

int main(void)
{
    // ����ʱ��,ϵͳʱ��168Mhz����Ƶ,AHBʱ�� 168Mhz;APB1 4��Ƶ;APB2 2��Ƶ
    Stm32_Clock_Init(336, 8, 2, 7);

	delay_init(168);
	OLED_Init();
    while(1)
    {
		Demo_01();
    }
}
