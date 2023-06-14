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
	˵��: ��ʹ��Ӳ��SPI����DMA
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
	SPI_InitTypeDef SPI_InitStructure;
 	
    //GPIOB��GPIODʱ��
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
    RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;

    //���á����ơ�100MHz������
    GPIOB->MODER    |= (GPIO_MODER_MODER13_1 | GPIO_MODER_MODER15_1);
    GPIOB->OTYPER   |= 0x00;
    GPIOB->OSPEEDR  |= (GPIO_OSPEEDER_OSPEEDR13 | GPIO_OSPEEDER_OSPEEDR15);
    GPIOB->PUPDR    |= 0x00;
	GPIO_SetBits(GPIOB,SPI_SCLK_Pin|SPI_SDIN_Pin);

	//��������ơ�100MHz������
    GPIOD->MODER    |= (GPIO_MODER_MODER9_0 | GPIO_MODER_MODER11_0 | GPIO_MODER_MODER13_0);
    GPIOD->OTYPER   |= 0x00;
    GPIOD->OSPEEDR  |= (GPIO_OSPEEDER_OSPEEDR9 | GPIO_OSPEEDER_OSPEEDR11 | GPIO_OSPEEDER_OSPEEDR13);
    GPIOD->PUPDR    |= 0x00;
	GPIO_SetBits(GPIOD,SPI_RST_Pin|SPI_DC_Pin|SPI_CS_Pin);

    //�˿ڸ���
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource13,GPIO_AF_SPI2);
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource15,GPIO_AF_SPI2);

	SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;  // SPI����ֻ����
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;  // SPI����ģʽ
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;  // SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;  // ����ͬ��ʱ�ӿ���״̬Ϊ�͵�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;  // �ڵ�һ�������زɼ�����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;  // �������Ƭѡ��CS��
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;  // 2��Ƶ
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;  // MSB�ȷ���
	SPI_InitStructure.SPI_CRCPolynomial = 7;  // CRC����Ķ���ʽ
    SPI_Init(SPI2, &SPI_InitStructure);

	SPI_Cmd(SPI2, ENABLE);

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

