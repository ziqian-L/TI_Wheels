#include "iic.h"
#include "delay.h"

/******
IICЭ�鴫����̣���ʼ�ź�->7λ�豸��ַ+R/W->R/W�ļĴ���->R/W->ֹͣ�ź�
д�����ݣ���ʼ�ź�->7λ�豸��ַ+0->д��ļĴ���->д��->ֹͣ�ź�
��ȡ���ݣ���ʼ�ź�->7λ�豸��ַ+0->��ȡ����->���ͷ�Ӧ���ź�(1)->ֹͣ�ź�
******/

/******************************************���IIC��ʼ��******************************************/
/******
 * IIC��ʼ��
 * PB6��SCL
 * PB7��SDA
******/
void IIC_Init(void)
{
    //ʹ��ʱ��
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    //GPIO��ʼ��
    GPIOB->MODER    |= (GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0);
    GPIOB->OTYPER   |= 0x00;
    GPIOB->OSPEEDR  |= (GPIO_OSPEEDER_OSPEEDR6_1 | GPIO_OSPEEDER_OSPEEDR7_1);
    GPIOB->PUPDR    |= (GPIO_PUPDR_PUPDR6_0 | GPIO_PUPDR_PUPDR7_0);
    //������������
    IIC_SCL = 1;
	IIC_SDA = 1;
}

/******************************************���IICЭ��******************************************/
/******
 * ������ʼ�ź�
******/
void IIC_Start(void)
{
	SDA_OUT;     //sda�����
	IIC_SDA=1;	
	delay_us(4);	
	IIC_SCL=1;
	delay_us(4);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
	delay_us(4);
}

/******
 * д��һ���ֽ�
******/
void IIC_Write_Byte(uint8_t IIC_data)
{
    uint8_t i;
    //ȷ��SDAΪ���
    SDA_OUT;
    //SCLΪ��ʱ������SDA�ı仯
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
    //׼���������
    IIC_SCL = 0;
}
/******
 * �ȴ�Ӧ���ź�
******/
uint8_t IIC_Wait_Ack(void)
{
    uint8_t ErrTime = 0;
    //ȷ��SDAΪ���룬SCL��SDA����
    SDA_IN;
    IIC_SDA = 1;delay_us(6);
    IIC_SCL = 1;delay_us(6);
    //�͵�ƽ��ʾӦ�𣬸ߵ�ƽ��ʾ��Ӧ��
    while (READ_SDA)
    {
        ErrTime++;
        if (ErrTime>250)
        {
            IIC_Stop();
            return 1;
        }
    }
    //׼���������
    IIC_SCL = 0;
    return 0;
}

/******
 * ��ȡһ���ֽ�
 * ack=1ʱ������Ack��ack=0������NAck
******/
uint8_t IIC_Read_Byte(unsigned char ack)
{
    unsigned char i,receive = 0;
    //ȷ��SDAΪ����
    SDA_IN;
    //��SCLΪ��ʱ����ȡSDA
    for (i = 0; i < 8; i++)
    {
        IIC_SCL = 0;
        delay_us(10);
        IIC_SCL=1;
        receive<<=1;
        if(READ_SDA)receive++;
		delay_us(5);         
    }
    //ack=1ʱ������Ack��ack=0������NAck
    if (ack)
        IIC_Ack();
    else
        IIC_NAck();
    return receive;
}

/******
 * ����Ӧ���ź�
******/
void IIC_Ack(void)
{
    //��9��ʱ������֮ǰ�ĵ͵�ƽ�ڼ佫SDA������
    //����ȷ���ڸ�ʱ�ӵĸߵ�ƽ�ڼ�Ϊ�ȶ��ĵ͵�ƽ
    IIC_SCL = 0;
    //ȷ��SDAΪ���
    SDA_OUT;
    IIC_SDA = 0;
    delay_us(10);
    IIC_SCL = 1;
    delay_us(10);
    //׼���������
    IIC_SCL = 0;
}

/******
 * ������Ӧ���ź�
******/
void IIC_NAck(void)
{
    //��9��ʱ������֮ǰ�ĵ͵�ƽ�ڼ佫SDA������
    //����ȷ���ڸ�ʱ�ӵĸߵ�ƽ�ڼ�Ϊ�ȶ��ĸߵ�ƽ
    IIC_SCL = 0;
    //ȷ��SDAΪ���
    SDA_OUT;
    IIC_SDA = 1;
    delay_us(2);
    IIC_SCL = 1;
    delay_us(2);
    //׼���������
    IIC_SCL = 0;
}

/******
 * ����ֹͣ�ź�
******/
void IIC_Stop(void)
{
    //ȷ��SDAΪ�����SCL��SDA����
    SDA_OUT;
    IIC_SCL = 0;
    IIC_SDA = 0;
    delay_us(4);
    //SCLΪ��ʱ��SDA�ɵ����䵽��
    IIC_SCL = 1;
    IIC_SDA = 1;
    delay_us(4);
}

