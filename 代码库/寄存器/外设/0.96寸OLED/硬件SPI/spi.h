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
#ifndef __SPI_H
#define __SPI_H

#include "sys.h"

/* ---------------- SPI�˿ڶ��� ---------------- */

#define SPI_SCLK_Clr()	GPIO_ResetBits(GPIOB,GPIO_Pin_13)	// CLK���
#define SPI_SCLK_Set()	GPIO_SetBits(GPIOB,GPIO_Pin_13)		// CLK��λ
#define SPI_SCLK_Pin	GPIO_Pin_13							// CLK����

#define SPI_SDIN_Clr()	GPIO_ResetBits(GPIOB,GPIO_Pin_15)	// DIN���
#define SPI_SDIN_Set()	GPIO_SetBits(GPIOB,GPIO_Pin_15)		// DIN��λ
#define SPI_SDIN_Pin	GPIO_Pin_15							// DIN����

#define SPI_RST_Clr()	GPIO_ResetBits(GPIOD,GPIO_Pin_9)	// RES���
#define SPI_RST_Set()	GPIO_SetBits(GPIOD,GPIO_Pin_9)		// RES��λ
#define SPI_RST_Pin		GPIO_Pin_9							// RES����

#define SPI_DC_Clr()	GPIO_ResetBits(GPIOD,GPIO_Pin_11)	// DC���
#define SPI_DC_Set()	GPIO_SetBits(GPIOD,GPIO_Pin_11)		// DC��λ
#define SPI_DC_Pin		GPIO_Pin_11							// DC����

#define SPI_CS_Clr()	GPIO_ResetBits(GPIOD,GPIO_Pin_13)	// CS���
#define SPI_CS_Set()	GPIO_SetBits(GPIOD,GPIO_Pin_13)		// CS��λ
#define SPI_CS_Pin		GPIO_Pin_13							// CS����

/* ---------------- SPI�˿ڶ��� ---------------- */


/* ---------------- SPIʵ�ַ�ʽ���� ---------------- */

void SPI_GPIO_Init(void);
void SPI_WR_Byte(u8 dat, u8 cmd);

#endif
