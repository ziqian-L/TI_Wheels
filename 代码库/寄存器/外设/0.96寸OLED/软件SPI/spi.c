/**
 * @file oled.c
 * @author ziqian��ֲ��YEWANhub
 * @brief ʵ��OLED��������һЩ����ͼ�εĻ���
 * @version V1.0.0
 * @date 2023-06-13
 * @encoding GB2312
 * @details �ڻ�����תͼ��ʱ���ǶȲ����ǽǶ���
 */
/******************************************************************************
	˵��: ��ʹ�����SPI����DMA
	----------------------------------------------------------------
	GND    ��Դ��
	VCC  ��5V��3.3v��Դ
	D0   ��PB13��SCL��
	D1   ��PB15��SDA��
	RES  ��PD9
	DC   ��PD11
	CS   ��PD13
	----------------------------------------------------------------
******************************************************************************/
#include "spi.h"

/**
 * @fn SPI_GPIO_Init
 * @brief ��ʼ��SPI1
 * @return None
 */
void SPI_GPIO_Init(void)
{
    //GPIOB��GPIODʱ��
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;

    //��������ơ�100MHz������
    GPIOB->MODER    |= (GPIO_MODER_MODER13_0 | GPIO_MODER_MODER15_0);
    GPIOB->OTYPER   |= 0x00;
    GPIOB->OSPEEDR  |= (GPIO_OSPEEDER_OSPEEDR13 | GPIO_OSPEEDER_OSPEEDR15);
    GPIOB->PUPDR    |= 0x00;
	GPIO_SetBits(GPIOB,SPI_SCLK_Pin);
	GPIO_SetBits(GPIOB,SPI_SDIN_Pin);

	//��������ơ�100MHz������
    GPIOD->MODER    |= (GPIO_MODER_MODER9_0 | GPIO_MODER_MODER11_0 | GPIO_MODER_MODER13_0);
    GPIOD->OTYPER   |= 0x00;
    GPIOD->OSPEEDR  |= (GPIO_OSPEEDER_OSPEEDR9 | GPIO_OSPEEDER_OSPEEDR11 | GPIO_OSPEEDER_OSPEEDR13);
    GPIOD->PUPDR    |= 0x00;
	GPIO_SetBits(GPIOD,SPI_RST_Pin);
	GPIO_SetBits(GPIOD,SPI_DC_Pin);
	GPIO_SetBits(GPIOD,SPI_CS_Pin);
}

/**
 * @brief ������ӻ���SSD1306��д��һ���ֽ�
 * @param dat Ҫд�������/����
 * @param cmd ����/�����־ 0,��ʾ����;1,��ʾ����
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
OLED��ֲ�ڿ⺯��������Ĵ�������
*********************************/

void GPIO_ResetBits(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin)
{
    GPIOx->BSRRH |= (uint32_t)1<<GPIO_Pin;
}

void GPIO_SetBits(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin)
{
    GPIOx->BSRRL |= (uint32_t)1<<GPIO_Pin;
}
