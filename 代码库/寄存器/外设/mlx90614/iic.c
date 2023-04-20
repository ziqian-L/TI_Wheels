#include "iic.h"
#include "delay.h"

/******
IIC协议传输过程：起始信号->7位设备地址+R/W->R/W的寄存器->R/W->停止信号
写入数据：起始信号->7位设备地址+0->写入的寄存器->写入->停止信号
读取数据：起始信号->7位设备地址+0->读取数据->发送非应答信号(1)->停止信号
******/

/******************************************软件IIC初始化******************************************/
/******
 * IIC初始化
 * PB6：SCL
 * PB7：SDA
******/
void IIC_Init(void)
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

/******************************************软件IIC协议******************************************/
/******
 * 产生起始信号
******/
void IIC_Start(void)
{
	SDA_OUT;     //sda线输出
	IIC_SDA=1;	
	delay_us(4);	
	IIC_SCL=1;
	delay_us(4);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
	delay_us(4);
}

/******
 * 写入一个字节
******/
void IIC_Write_Byte(uint8_t IIC_data)
{
    uint8_t i;
    //确保SDA为输出
    SDA_OUT;
    //SCL为低时，允许SDA的变化
    for (i = 0; i < 8; i++)
    {
        IIC_SCL = 0;
        delay_us(10);
        IIC_SDA = (IIC_data&0x80)>>7;
        IIC_data<<=1;
        delay_us(10);
		IIC_SCL=1;
		delay_us(10); 
    }
    //准备输出数据
    IIC_SCL = 0;
}
/******
 * 等待应答信号
******/
uint8_t IIC_Wait_Ack(void)
{
    uint8_t ErrTime = 0;
    //确保SDA为输入，SCL、SDA拉高
    SDA_IN;
    IIC_SDA = 1;delay_us(6);
    IIC_SCL = 1;delay_us(6);
    //低电平表示应答，高电平表示不应答
    while (READ_SDA)
    {
        ErrTime++;
        if (ErrTime>250)
        {
            IIC_Stop();
            return 1;
        }
    }
    //准备输出数据
    IIC_SCL = 0;
    return 0;
}

/******
 * 读取一个字节
 * ack=1时，发送Ack，ack=0，发送NAck
******/
uint8_t IIC_Read_Byte(unsigned char ack)
{
    unsigned char i,receive = 0;
    //确保SDA为输入
    SDA_IN;
    //当SCL为高时，读取SDA
    for (i = 0; i < 8; i++)
    {
        IIC_SCL = 0;
        delay_us(10);
        IIC_SCL=1;
        receive<<=1;
        if(READ_SDA)receive++;
		delay_us(5);         
    }
    //ack=1时，发送Ack，ack=0，发送NAck
    if (ack)
        IIC_Ack();
    else
        IIC_NAck();
    return receive;
}

/******
 * 产生应答信号
******/
void IIC_Ack(void)
{
    //第9个时钟脉冲之前的低电平期间将SDA线拉低
    //并且确保在该时钟的高电平期间为稳定的低电平
    IIC_SCL = 0;
    //确保SDA为输出
    SDA_OUT;
    IIC_SDA = 0;
    delay_us(10);
    IIC_SCL = 1;
    delay_us(10);
    //准备输出数据
    IIC_SCL = 0;
}

/******
 * 产生不应答信号
******/
void IIC_NAck(void)
{
    //第9个时钟脉冲之前的低电平期间将SDA线拉高
    //并且确保在该时钟的高电平期间为稳定的高电平
    IIC_SCL = 0;
    //确保SDA为输出
    SDA_OUT;
    IIC_SDA = 1;
    delay_us(2);
    IIC_SCL = 1;
    delay_us(2);
    //准备输出数据
    IIC_SCL = 0;
}

/******
 * 产生停止信号
******/
void IIC_Stop(void)
{
    //确保SDA为输出，SCL、SDA拉低
    SDA_OUT;
    IIC_SCL = 0;
    IIC_SDA = 0;
    delay_us(4);
    //SCL为高时，SDA由低跳变到高
    IIC_SCL = 1;
    IIC_SDA = 1;
    delay_us(4);
}

