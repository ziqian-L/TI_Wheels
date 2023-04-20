#ifndef __IIC_H
#define __IIC_H

#include "sys.h"

#define SDA_IN  {GPIOB->MODER &= ~GPIO_MODER_MODER7;GPIOB->MODER |= 0x00;}
#define SDA_OUT {GPIOB->MODER &= ~GPIO_MODER_MODER7;GPIOB->MODER |= GPIO_MODER_MODER7_0;}

#define IIC_SCL    PBout(6) //SCL
#define IIC_SDA    PBout(7) //SDA	 
#define READ_SDA   PBin(7)  // ‰»ÎSDA 

void IIC_Init(void);

void IIC_Write_Byte(uint8_t IIC_data);
uint8_t IIC_Read_Byte(unsigned char ack);

void IIC_Start(void);
void IIC_Stop(void);

uint8_t IIC_Wait_Ack(void);
void IIC_Ack(void);
void IIC_NAck(void);

#endif

