#include "delay.h"
#include "sys.h"
#include "oled.h"
#include "screen.h"

int main(void)
{
    delay_init(168);
	OLED_Init();
	OLED_Clear();
    while(1)
    {
		Demo_01();
    }
}
