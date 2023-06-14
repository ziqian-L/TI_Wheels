/**
 * @file oled.c
 * @author ziqian移植至YEWANhub
 * @brief 实现OLED的驱动和一些基础图形的绘制
 * @version V1.0.0
 * @date 2023-06-13
 * @encoding GB2312
 * @details 在绘制旋转图形时，角度参数是角度制
 */
/******************************************************************************
	说明: 仅使用软件SPI，无DMA
	----------------------------------------------------------------
	GND    电源地
	VCC  接5V或3.3v电源
	D0   接PB13（SCL）
	D1   接PB15（SDA）
	RES  接PD9
	DC   接PD11
	CS   接PD13
	----------------------------------------------------------------
******************************************************************************/
#include "spi.h"

/**
 * @fn SPI_GPIO_Init
 * @brief 初始化SPI1
 * @return None
 */
void SPI_GPIO_Init(void)
{
    //GPIOB、GPIOD时钟
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;

    //输出、挽推、100MHz、浮空
    GPIOB->MODER    |= (GPIO_MODER_MODER13_0 | GPIO_MODER_MODER15_0);
    GPIOB->OTYPER   |= 0x00;
    GPIOB->OSPEEDR  |= (GPIO_OSPEEDER_OSPEEDR13 | GPIO_OSPEEDER_OSPEEDR15);
    GPIOB->PUPDR    |= 0x00;
	GPIO_SetBits(GPIOB,SPI_SCLK_Pin);
	GPIO_SetBits(GPIOB,SPI_SDIN_Pin);

	//输出、挽推、100MHz、浮空
    GPIOD->MODER    |= (GPIO_MODER_MODER9_0 | GPIO_MODER_MODER11_0 | GPIO_MODER_MODER13_0);
    GPIOD->OTYPER   |= 0x00;
    GPIOD->OSPEEDR  |= (GPIO_OSPEEDER_OSPEEDR9 | GPIO_OSPEEDER_OSPEEDR11 | GPIO_OSPEEDER_OSPEEDR13);
    GPIOD->PUPDR    |= 0x00;
	GPIO_SetBits(GPIOD,SPI_RST_Pin);
	GPIO_SetBits(GPIOD,SPI_DC_Pin);
	GPIO_SetBits(GPIOD,SPI_CS_Pin);
}

/**
 * @brief 主机向从机（SSD1306）写入一个字节
 * @param dat 要写入的数据/命令
 * @param cmd 数据/命令标志 0,表示命令;1,表示数据
 * @return None
 */
void SPI_WR_Byte(u8 dat, u8 cmd)
{
	if(cmd) SPI_DC_Set();
	else SPI_DC_Clr();
	SPI_CS_Clr();

	u8 i;
	for(i=0;i<8;i++)
	{
		SPI_SCLK_Clr();
		if(dat&0x80) SPI_SDIN_Set();
		else SPI_SDIN_Clr();
		SPI_SCLK_Set();
		dat<<=1;   
	}

	SPI_CS_Set();
	SPI_DC_Set();
}


/*********************************
2023.3.30
OLED移植于库函数，适配寄存器代码
*********************************/

void GPIO_ResetBits(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin)
{
    GPIOx->BSRRH |= (uint32_t)1<<GPIO_Pin;
}

void GPIO_SetBits(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin)
{
    GPIOx->BSRRL |= (uint32_t)1<<GPIO_Pin;
}
