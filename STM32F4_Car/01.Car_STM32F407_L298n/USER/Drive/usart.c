#include "sys.h"
#include "usart.h"
#include "led.h"
#include "cJSON.h"
#include "string.h"
//////////////////////////////////////////////////////////////////////////////// 	 

RingBuff_t Uart1_RingBuff,Uart2_RingBuff,Uart3_RingBuff;//创建一个ringBuff的缓冲区

#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
	USART1->DR = (u8) ch;      
	return ch;
}
/*
加上该句重写_ttywrch()函数
可以避免出现
..\OBJ\STM32F4.axf: Error: L6915E: Library reports error: 
__use_no_semihosting was requested, but _ttywrch was referenced
Not enough information to list load addresses in the image map.
参考：https://blog.csdn.net/weixin_42911817/article/details/109539547
*/
 void _ttywrch(int ch)
    {
        ch = ch;
    }
#endif



/***************************************串口收发相关***************************************/
//初始化IO 串口1
//bound:波特率
void UART1_Init(uint32_t bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//使能USART1时钟

	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10复用为USART1

	//USART1端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA9，PA10

	//USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART1, &USART_InitStructure); //初始化串口1

	USART_Cmd(USART1, ENABLE);  //使能串口1

#if EN_USART1_RX	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、

#endif
	
}

/*****
 * 串口1中断服务函数
*****/
void USART1_IRQHandler(void)
{
	uint8_t rdata;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		rdata = USART_ReceiveData(USART1);
		if(Write_RingBuff(&Uart1_RingBuff,rdata) == RINGBUFF_ERR)
			LED1=0;
		else
			LED1=1;
	}
}

//初始化IO 串口2
//bound:波特率
void UART2_Init(uint32_t bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    //GPIOA、USART2时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);

    //PA2、PA3复用、挽推、100MHz、浮空
    GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_2|GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType   = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed   = GPIO_High_Speed;
    GPIO_InitStructure.GPIO_PuPd    = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA,&GPIO_InitStructure);

    //GPIO复用
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2);
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2);

    //初始化USART2
    //8字长、1停止位、无奇偶校验、收发模式、无硬件数据流
    USART_InitStructure.USART_BaudRate = bound;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART2,&USART_InitStructure);

    //配置USART2中断优先级
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    //开启数据接收寄存器非空中断
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

    //使能串口2
    USART_Cmd(USART2,ENABLE);
}

/*****
 * 串口2中断服务函数
*****/
void USART2_IRQHandler(void)
{
	uint8_t rdata;
    if (USART_GetITStatus(USART2,USART_IT_RXNE) != RESET)
    {
		rdata = USART_ReceiveData(USART2);
		if(Write_RingBuff(&Uart2_RingBuff, rdata) == RINGBUFF_ERR){//缓冲区满灯亮
			LED1=0;
		}else{
			LED1=1;
		}
    }
}

/***************************************环形缓冲区相关***************************************/
/*****
 * 环形缓冲区初始化
*****/
void RingBuff_Init(RingBuff_t *ringbuff)
{
	//初始化环形缓冲区相关参数
	ringbuff->Head = 0;
	ringbuff->Tail = 0;
	ringbuff->Length = 0;
}

/*****
 * 往环形缓冲区写入数据
 * 首先判断缓冲区是否为满，为满返回错误
 * 非满将值写入缓冲区
 * 将尾自增，尾对缓冲区大小取余，当尾=缓冲区大小，尾等于0，防止越界
*****/
uint8_t Write_RingBuff(RingBuff_t *ringbuff, uint8_t data)
{
  if(ringbuff->Length >= RINGBUFF_LEN) //判断缓冲区是否已满
  {
    return RINGBUFF_ERR;
  }
  ringbuff->Ring_data[ringbuff->Tail]=data;
  ringbuff->Tail = (ringbuff->Tail+1)%RINGBUFF_LEN;//防止越界非法访问
  ringbuff->Length++;
  return RINGBUFF_OK;
}

/*****
 * 从环形缓冲区读取数据
 * 首先判断缓冲区是非空，为空返回错误
 * 非满将数据赋值给传进来的指针
 * 将头自增，头对缓冲区大小取余，当头=缓冲区大小，头等于0，防止越界
*****/
uint8_t Read_RingBuff(RingBuff_t *ringbuff, uint8_t *rData)
{
  if(ringbuff->Length == 0)//判断非空
  {
    return RINGBUFF_ERR;
  }
  *rData = ringbuff->Ring_data[ringbuff->Head];//先进先出FIFO，从缓冲区头出
  ringbuff->Head = (ringbuff->Head+1)%RINGBUFF_LEN;//防止越界非法访问
  ringbuff->Length--;
  return RINGBUFF_OK;
}

/***************************************数据处理相关***************************************/

/* 接收json字符串 */
int8_t receiveJson(RingBuff_t *ringbuff, char *str)
{
	static uint8_t reading,j;
	uint8_t readbuff,ok = 0;
	if(Read_RingBuff(ringbuff,&readbuff) == RINGBUFF_OK){
		if(readbuff == '{'){
			reading = 1;
			str[j++] = readbuff;
			ok = 0;
		}else if(readbuff == '}' && reading == 1){
			reading = 0;
			str[j++] = readbuff;  
			str[j++] = '\0';
			ok = 1;
		}else if(reading == 1){
			str[j++] = readbuff;
		}else {
			j = 0;
			reading = 0;
			ok = 0;
		}
	}
	return ok;
}

int8_t read_json_pid(const char *str, const char *pid, double *p, double *i, double *d)
{
	int8_t ret = 0;
	cJSON *cjson,*cj_pid,*pid_item;
	const char *buffer = str;
	cjson = cJSON_Parse(buffer);
	cj_pid = cJSON_GetObjectItem(cjson, pid);
	if(strcmp(cj_pid->string, pid) == 0){
		ret = 0;
		pid_item = cJSON_GetArrayItem(cj_pid, 0);
		*p = pid_item->valuedouble;
		pid_item = cJSON_GetArrayItem(cj_pid, 1);
		*i = pid_item->valuedouble;
		pid_item = cJSON_GetArrayItem(cj_pid, 2);
		*d = pid_item->valuedouble;
	} else {
		ret = 1;
	}
	cJSON_Delete(cjson);
	return ret;
}










