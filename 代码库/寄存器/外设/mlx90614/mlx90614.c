#include "mlx90614.h"
#include "iic.h"

void Temperature_measure_Init(void)
{
    //使能时钟
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    //GPIO初始化
    GPIOB->MODER    |= (GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0);
    GPIOB->OTYPER   |= 0x00;
    GPIOB->OSPEEDR  |= (GPIO_OSPEEDER_OSPEEDR6_1 | GPIO_OSPEEDER_OSPEEDR7_1);
    GPIOB->PUPDR    |= (GPIO_PUPDR_PUPDR6_0 | GPIO_PUPDR_PUPDR7_0);
    //拉高两根总线
    IIC_SCL = 1;
	IIC_SDA = 1;
}

float Read_Temperature(uint8_t address)
{
    uint8_t Temp_L = 0,Temp_H = 0,Temp_PE = 0;
    uint16_t Temp = 0;		//寄存器的值
	float Temperature = 0;	//返回的温度值
	//发送读取的寄存器的地址
    IIC_Start();
    IIC_Write_Byte(0x00);
    IIC_Wait_Ack();
    IIC_Write_Byte(address);
    IIC_Wait_Ack();
	/*------------------*/
	//读取寄存器值
    IIC_Start();
    IIC_Write_Byte(0x01);
	IIC_Wait_Ack();
	//返回 16 位的数据和 8 位 PE
    Temp_L = IIC_Read_Byte(1);
    Temp_H = IIC_Read_Byte(1);
    Temp_PE = IIC_Read_Byte(1);
    IIC_Stop();
    Temp = (Temp_H<<8) | Temp_L;
    Temperature = (((float)Temp * 2) - 27315)/100;
	return Temperature;
}
