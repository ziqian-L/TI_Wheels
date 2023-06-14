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
	说明: 
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

#define SPI_SCLK_Clr()	GPIO_ResetBits(GPIOB,13)	// CLK清除
#define SPI_SCLK_Set()	GPIO_SetBits(GPIOB,13)		// CLK置位
#define SPI_SCLK_Pin	13							// CLK引脚

#define SPI_SDIN_Clr()	GPIO_ResetBits(GPIOB,15)	// DIN清除
#define SPI_SDIN_Set()	GPIO_SetBits(GPIOB,15)		// DIN置位
#define SPI_SDIN_Pin	15							// DIN引脚

#define SPI_RST_Clr()	GPIO_ResetBits(GPIOD,9)	// RES清除
#define SPI_RST_Set()	GPIO_SetBits(GPIOD,9)		// RES置位
#define SPI_RST_Pin		9							// RES引脚

#define SPI_DC_Clr()	GPIO_ResetBits(GPIOD,11)	// DC清除
#define SPI_DC_Set()	GPIO_SetBits(GPIOD,11)		// DC置位
#define SPI_DC_Pin		11							// DC引脚

#define SPI_CS_Clr()	GPIO_ResetBits(GPIOD,13)	// CS清除
#define SPI_CS_Set()	GPIO_SetBits(GPIOD,13)		// CS置位
#define SPI_CS_Pin		13							// CS引脚

/* ---------------- SPI端口定义 ---------------- */


/* ---------------- SPI及DMA实现方式定义 ---------------- */

void SPI_GPIO_Init(void);
void SPI_WR_Byte(u8 dat, u8 cmd);

/*********************************
2023.3.30
OLED移植于库函数，适配寄存器代码
*********************************/
void GPIO_ResetBits(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin);
void GPIO_SetBits(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin);

#endif
