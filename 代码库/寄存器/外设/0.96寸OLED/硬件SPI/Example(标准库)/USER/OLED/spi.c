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
	说明: 仅使用硬件SPI，无DMA
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
	SPI_InitTypeDef SPI_InitStructure;
 	
    //GPIOB、GPIOD时钟
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
    RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;

    //复用、挽推、100MHz、浮空
    GPIOB->MODER    |= (GPIO_MODER_MODER13_1 | GPIO_MODER_MODER15_1);
    GPIOB->OTYPER   |= 0x00;
    GPIOB->OSPEEDR  |= (GPIO_OSPEEDER_OSPEEDR13 | GPIO_OSPEEDER_OSPEEDR15);
    GPIOB->PUPDR    |= 0x00;
	GPIO_SetBits(GPIOB,SPI_SCLK_Pin|SPI_SDIN_Pin);

	//输出、挽推、100MHz、浮空
    GPIOD->MODER    |= (GPIO_MODER_MODER9_0 | GPIO_MODER_MODER11_0 | GPIO_MODER_MODER13_0);
    GPIOD->OTYPER   |= 0x00;
    GPIOD->OSPEEDR  |= (GPIO_OSPEEDER_OSPEEDR9 | GPIO_OSPEEDER_OSPEEDR11 | GPIO_OSPEEDER_OSPEEDR13);
    GPIOD->PUPDR    |= 0x00;
	GPIO_SetBits(GPIOD,SPI_RST_Pin|SPI_DC_Pin|SPI_CS_Pin);

    //端口复用
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource13,GPIO_AF_SPI2);
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource15,GPIO_AF_SPI2);

	SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;  // SPI单线只发送
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;  // SPI主机模式
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;  // SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;  // 串行同步时钟空闲状态为低电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;  // 在第一个跳变沿采集数据
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;  // 软件控制片选（CS）
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;  // 2分频
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;  // MSB先发送
	SPI_InitStructure.SPI_CRCPolynomial = 7;  // CRC计算的多项式
    SPI_Init(SPI2, &SPI_InitStructure);

	SPI_Cmd(SPI2, ENABLE);

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

	u8 retry = 0;
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE)==RESET)
	{
		retry++;
		if(retry>200) return;
	}
	SPI_I2S_SendData(SPI2, dat);
	retry = 0;
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE)==RESET)
	{
		retry++;
		if(retry>200) return;
	}

	SPI_CS_Set();
	SPI_DC_Set();
}

