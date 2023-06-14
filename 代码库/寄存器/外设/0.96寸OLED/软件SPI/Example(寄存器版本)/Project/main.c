#include "sys.h"
#include "usart.h" 
#include "delay.h"
#include "oled.h"
#include "screen.h"

int main(void)
{
    // 设置时钟,系统时钟168Mhz不分频,AHB时钟 168Mhz;APB1 4分频;APB2 2分频
    Stm32_Clock_Init(336, 8, 2, 7);

	delay_init(168);
	OLED_Init();
    while(1)
    {
		Demo_01();
    }
}
