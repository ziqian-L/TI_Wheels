#include "sys.h"
#include "usart.h"
#include "led.h"
#include "cJSON.h"
#include "string.h"
//////////////////////////////////////////////////////////////////////////////// 	 

RingBuff_t Uart1_RingBuff,Uart2_RingBuff,Uart3_RingBuff;//����һ��ringBuff�Ļ�����

#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
	USART1->DR = (u8) ch;      
	return ch;
}
/*
���ϸþ���д_ttywrch()����
���Ա������
..\OBJ\STM32F4.axf: Error: L6915E: Library reports error: 
__use_no_semihosting was requested, but _ttywrch was referenced
Not enough information to list load addresses in the image map.
�ο���https://blog.csdn.net/weixin_42911817/article/details/109539547
*/
 void _ttywrch(int ch)
    {
        ch = ch;
    }
#endif



/***************************************�����շ����***************************************/
//��ʼ��IO ����1
//bound:������
void UART1_Init(uint32_t bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//ʹ��USART1ʱ��

	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9����ΪUSART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10����ΪUSART1

	//USART1�˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9��GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA9��PA10

	//USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART1, &USART_InitStructure); //��ʼ������1

	USART_Cmd(USART1, ENABLE);  //ʹ�ܴ���1

#if EN_USART1_RX	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//��������ж�

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����

#endif
	
}

/*****
 * ����1�жϷ�����
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

//��ʼ��IO ����2
//bound:������
void UART2_Init(uint32_t bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    //GPIOA��USART2ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);

    //PA2��PA3���á����ơ�100MHz������
    GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_2|GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType   = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed   = GPIO_High_Speed;
    GPIO_InitStructure.GPIO_PuPd    = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA,&GPIO_InitStructure);

    //GPIO����
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2);
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2);

    //��ʼ��USART2
    //8�ֳ���1ֹͣλ������żУ�顢�շ�ģʽ����Ӳ��������
    USART_InitStructure.USART_BaudRate = bound;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART2,&USART_InitStructure);

    //����USART2�ж����ȼ�
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    //�������ݽ��ռĴ����ǿ��ж�
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

    //ʹ�ܴ���2
    USART_Cmd(USART2,ENABLE);
}

/*****
 * ����2�жϷ�����
*****/
void USART2_IRQHandler(void)
{
	uint8_t rdata;
    if (USART_GetITStatus(USART2,USART_IT_RXNE) != RESET)
    {
		rdata = USART_ReceiveData(USART2);
		if(Write_RingBuff(&Uart2_RingBuff, rdata) == RINGBUFF_ERR){//������������
			LED1=0;
		}else{
			LED1=1;
		}
    }
}

/***************************************���λ��������***************************************/
/*****
 * ���λ�������ʼ��
*****/
void RingBuff_Init(RingBuff_t *ringbuff)
{
	//��ʼ�����λ�������ز���
	ringbuff->Head = 0;
	ringbuff->Tail = 0;
	ringbuff->Length = 0;
}

/*****
 * �����λ�����д������
 * �����жϻ������Ƿ�Ϊ����Ϊ�����ش���
 * ������ֵд�뻺����
 * ��β������β�Ի�������Сȡ�࣬��β=��������С��β����0����ֹԽ��
*****/
uint8_t Write_RingBuff(RingBuff_t *ringbuff, uint8_t data)
{
  if(ringbuff->Length >= RINGBUFF_LEN) //�жϻ������Ƿ�����
  {
    return RINGBUFF_ERR;
  }
  ringbuff->Ring_data[ringbuff->Tail]=data;
  ringbuff->Tail = (ringbuff->Tail+1)%RINGBUFF_LEN;//��ֹԽ��Ƿ�����
  ringbuff->Length++;
  return RINGBUFF_OK;
}

/*****
 * �ӻ��λ�������ȡ����
 * �����жϻ������Ƿǿգ�Ϊ�շ��ش���
 * ���������ݸ�ֵ����������ָ��
 * ��ͷ������ͷ�Ի�������Сȡ�࣬��ͷ=��������С��ͷ����0����ֹԽ��
*****/
uint8_t Read_RingBuff(RingBuff_t *ringbuff, uint8_t *rData)
{
  if(ringbuff->Length == 0)//�жϷǿ�
  {
    return RINGBUFF_ERR;
  }
  *rData = ringbuff->Ring_data[ringbuff->Head];//�Ƚ��ȳ�FIFO���ӻ�����ͷ��
  ringbuff->Head = (ringbuff->Head+1)%RINGBUFF_LEN;//��ֹԽ��Ƿ�����
  ringbuff->Length--;
  return RINGBUFF_OK;
}

/***************************************���ݴ������***************************************/

/* ����json�ַ��� */
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










