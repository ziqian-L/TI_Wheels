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
	˵��: 
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

#define SPI_SCLK_Clr()	GPIO_ResetBits(GPIOB,13)	// CLK���
#define SPI_SCLK_Set()	GPIO_SetBits(GPIOB,13)		// CLK��λ
#define SPI_SCLK_Pin	13							// CLK����

#define SPI_SDIN_Clr()	GPIO_ResetBits(GPIOB,15)	// DIN���
#define SPI_SDIN_Set()	GPIO_SetBits(GPIOB,15)		// DIN��λ
#define SPI_SDIN_Pin	15							// DIN����

#define SPI_RST_Clr()	GPIO_ResetBits(GPIOD,9)	// RES���
#define SPI_RST_Set()	GPIO_SetBits(GPIOD,9)		// RES��λ
#define SPI_RST_Pin		9							// RES����

#define SPI_DC_Clr()	GPIO_ResetBits(GPIOD,11)	// DC���
#define SPI_DC_Set()	GPIO_SetBits(GPIOD,11)		// DC��λ
#define SPI_DC_Pin		11							// DC����

#define SPI_CS_Clr()	GPIO_ResetBits(GPIOD,13)	// CS���
#define SPI_CS_Set()	GPIO_SetBits(GPIOD,13)		// CS��λ
#define SPI_CS_Pin		13							// CS����

/* ---------------- SPI�˿ڶ��� ---------------- */


/* ---------------- SPI��DMAʵ�ַ�ʽ���� ---------------- */

void SPI_GPIO_Init(void);
void SPI_WR_Byte(u8 dat, u8 cmd);

/*********************************
2023.3.30
OLED��ֲ�ڿ⺯��������Ĵ�������
*********************************/
void GPIO_ResetBits(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin);
void GPIO_SetBits(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin);

#endif
