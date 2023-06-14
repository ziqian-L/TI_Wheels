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
#ifndef __SPI_H
#define __SPI_H

#include "sys.h"

/* ---------------- SPI端口定义 ---------------- */

#define SPI_SCLK_Clr()	GPIO_ResetBits(GPIOB,GPIO_Pin_13)	// CLK清除
#define SPI_SCLK_Set()	GPIO_SetBits(GPIOB,GPIO_Pin_13)		// CLK置位
#define SPI_SCLK_Pin	GPIO_Pin_13							// CLK引脚

#define SPI_SDIN_Clr()	GPIO_ResetBits(GPIOB,GPIO_Pin_15)	// DIN清除
#define SPI_SDIN_Set()	GPIO_SetBits(GPIOB,GPIO_Pin_15)		// DIN置位
#define SPI_SDIN_Pin	GPIO_Pin_15							// DIN引脚

#define SPI_RST_Clr()	GPIO_ResetBits(GPIOD,GPIO_Pin_9)	// RES清除
#define SPI_RST_Set()	GPIO_SetBits(GPIOD,GPIO_Pin_9)		// RES置位
#define SPI_RST_Pin		GPIO_Pin_9							// RES引脚

#define SPI_DC_Clr()	GPIO_ResetBits(GPIOD,GPIO_Pin_11)	// DC清除
#define SPI_DC_Set()	GPIO_SetBits(GPIOD,GPIO_Pin_11)		// DC置位
#define SPI_DC_Pin		GPIO_Pin_11							// DC引脚

#define SPI_CS_Clr()	GPIO_ResetBits(GPIOD,GPIO_Pin_13)	// CS清除
#define SPI_CS_Set()	GPIO_SetBits(GPIOD,GPIO_Pin_13)		// CS置位
#define SPI_CS_Pin		GPIO_Pin_13							// CS引脚

/* ---------------- SPI端口定义 ---------------- */


/* ---------------- SPI实现方式定义 ---------------- */

void SPI_GPIO_Init(void);
void SPI_WR_Byte(u8 dat, u8 cmd);

#endif
