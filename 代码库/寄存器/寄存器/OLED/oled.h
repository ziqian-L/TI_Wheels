#ifndef __OLED_H
#define __OLED_H 

#include "sys.h"
#include "stdlib.h"	


//#define Front	/*�����߷�----->��Խ����STM32F4XX������OLD-TFTҺ�����ӿ�*/
#define Front


/**============================================
	*============================================
	*             Front�ӷ�OLED�˿�							
	*--------------------------------------------
	*	 OLED�ӿ�	|  ��Ƭ���ӿ�		|	TFTҺ�����ӿ�
	*--------------------------------------------
	*		GND			|			GND				|				30
	*--------------------------------------------
	*		VCC			|			PB15			|				28
	*--------------------------------------------
	*	CLK(D0)			|			PB2				|				26
	*--------------------------------------------
	*		SDA			|			PC13			|				24
	*--------------------------------------------
	*		RES			|			PG12			|				22
	*--------------------------------------------
	*		DC			|			PD5				|				20
	*--------------------------------------------
	*		CS			|			PD14			|				18
	*--------------------------------------------
	*============================================
	*============================================
	*             Back�ӷ�OLED�˿�							
	*--------------------------------------------
	*	 OLED�ӿ�	|  ��Ƭ���ӿ�		|	TFTҺ�����ӿ�
	*--------------------------------------------
	*		GND			|			GND				|				1
	*--------------------------------------------
	*		VCC			|			PD10			|				3
	*--------------------------------------------
	*	CLK(D0)			|			PD8				|				5
	*--------------------------------------------
	*		SDA			|			PE14			|				7
	*--------------------------------------------
	*		RES			|			PE12			|				9
	*--------------------------------------------
	*		DC			|			PE10			|				11
	*--------------------------------------------
	*		CS			|			PE8				|				13
	*--------------------------------------------
	*============================================
	*============================================	
	**********************************************
	*/


/***********************************************************************
															Front���߷�
***********************************************************************/
#ifdef  Front 

#define LED_ON GPIO_ResetBits(GPIOC,12)
#define LED_OFF GPIO_SetBits(GPIOC,12)//PB15 vcc

//-----------------OLED�˿ڶ���---------------- 

#define OLED_SCL_Clr() GPIO_ResetBits(GPIOB,2)	//SCL
#define OLED_SCL_Set() GPIO_SetBits(GPIOB,2)

#define OLED_SDA_Clr() GPIO_ResetBits(GPIOC,13)//SDA
#define OLED_SDA_Set() GPIO_SetBits(GPIOC,13)

#define OLED_RES_Clr() GPIO_ResetBits(GPIOG,12)//RES
#define OLED_RES_Set() GPIO_SetBits(GPIOG,12)

#define OLED_DC_Clr()  GPIO_ResetBits(GPIOD,5)	//DC
#define OLED_DC_Set()  GPIO_SetBits(GPIOD,5)
 		     
#define OLED_CS_Clr()  GPIO_ResetBits(GPIOD,14)//CS
#define OLED_CS_Set()  GPIO_SetBits(GPIOD,14)
																												
#endif
/*------------------------------ endif ------------------------------------*/



/****************************************************************************
															Back���߷�
*****************************************************************************/
#ifdef Back

#define LED_ON GPIO_SetBits(GPIOD,10)
#define LED_OFF GPIO_ResetBits(GPIOD,10)//PD10 vcc

//-----------------OLED�˿ڶ���---------------- 

#define OLED_SCL_Clr() GPIO_ResetBits(GPIOD,8)	//SCL
#define OLED_SCL_Set() GPIO_SetBits(GPIOD,8)

#define OLED_SDA_Clr() GPIO_ResetBits(GPIOE,14)//SDA
#define OLED_SDA_Set() GPIO_SetBits(GPIOE,14)

#define OLED_RES_Clr() GPIO_ResetBits(GPIOE,12)//RES
#define OLED_RES_Set() GPIO_SetBits(GPIOE,12)

#define OLED_DC_Clr()  GPIO_ResetBits(GPIOE,10)	//DC
#define OLED_DC_Set()  GPIO_SetBits(GPIOE,10)
 		     
#define OLED_CS_Clr()  GPIO_ResetBits(GPIOE,8)//CS
#define OLED_CS_Set()  GPIO_SetBits(GPIOE,8)
																												

#endif
/*------------------------------- endif ------------------------------------*/


#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����

void OLED_ClearPoint(u8 x,u8 y);
void OLED_ColorTurn(u8 i);
void OLED_DisplayTurn(u8 i);
void OLED_WR_Byte(u8 dat,u8 mode);
void OLED_DisPlay_On(void);
void OLED_DisPlay_Off(void);
void OLED_Refresh(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_DrawLine(u8 x1,u8 y1,u8 x2,u8 y2,u8 mode);
void OLED_DrawCircle(u8 x,u8 y,u8 r);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size1,u8 mode);
void OLED_ShowChar6x8(u8 x,u8 y,u8 chr,u8 mode);
void OLED_ShowString(u8 x,u8 y,s8 *chr,u8 size1,u8 mode);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size1,u8 mode);
void OLED_ShowSNum(u8 x,u8 y,int num,u8 len,u8 size1,u8 mode);//�����ʾ��������
void OLED_ShowFNum(u8 x,u8 y,float num,u8 len,u8 size1,u8 mode);//��ʾ��һλС��������
void OLED_ShowChinese(u8 x,u8 y,u8 num,u8 size1,u8 mode);
void OLED_ScrollDisplay(u8 num,u8 space,u8 mode);
void OLED_ShowPicture(u8 x,u8 y,u8 sizex,u8 sizey,u8 BMP[],u8 mode);
void OLED_Init(void);

/*********************************
2023.3.30
OLED��ֲ�ڿ⺯��������Ĵ�������
*********************************/
void GPIO_ResetBits(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin);
void GPIO_SetBits(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin);
#endif

