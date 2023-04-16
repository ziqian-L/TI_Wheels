#include "timer.h"

/******************************************状态读取******************************************/

/******
 * 读取更新中断标志
 * TIM10/11/13/14没有TIMx_DIER，不能使用该函数
******/
int TIM_GetITUpdate_Status(TIM_TypeDef* TIMx)
{
    if (((TIMx->SR & (uint16_t)0x01) != 0) && ((TIMx->DIER & (uint16_t)0x01) != 0))
        return 1;
    else 
        return 0;
}

/******
 * 清除更新中断标志
 * TIM10/11/13/14没有TIMx_DIER，不能使用该函数
******/
void TIM_ClearITUpdate(TIM_TypeDef* TIMx)
{
    TIMx->SR = (uint16_t)~0x01;
}

/******************************************使能函数******************************************/

/******
 * 启用 TIMx
******/
void TIM_Cmd(TIM_TypeDef* TIMx)
{
    TIMx->CR1 |= 1<<0;
}

/******
 * 使能 TIMx 的 x 通道上的预装载寄存器
******/
void TIM_Preload_EN(TIM_TypeDef* TIMx,uint8_t CH)
{
    if (CH == 1)
    {
        TIMx->CCMR1 |= TIM_CCMR1_OC1PE;
    }
    else if(CH == 2)
    {
        TIMx->CCMR1 |= TIM_CCMR1_OC2PE;
    }
    else if(CH == 3)
    {
        TIMx->CCMR2 |= TIM_CCMR2_OC3PE;
    }
    else if (CH == 4)
    {
        TIMx->CCMR2 |= TIM_CCMR2_OC4PE;
    }   
}

/******
 * 使能自动重载预装载，即使用 TIMx_ARR 寄存器进行缓冲
******/
void TIM_ARR_Preload_EN(TIM_TypeDef* TIMx)
{
    TIMx->CR1 |= TIM_CR1_ARPE;
}

/******
 * 启用更新中断
 * TIM10/11/13/14没有TIMx_DIER，不能使用该函数
******/
void TIM_IT_Update(TIM_TypeDef* TIMx)
{
    TIMx->DIER |= TIM_DIER_UIE;
}

/******************************************修改定时器计数值******************************************/

/******
 * 修改定时器计数值，即修改 TIMx->CNT
******/
void TIM_SetCounter(TIM_TypeDef* TIMx,uint32_t Counter)
{
    TIMx->CNT = Counter;
}

/******************************************时基单元初始化******************************************/

void TIM1_Init(uint16_t psc,uint16_t arr)
{
    //APB2时钟初始化
    RCC->APB2ENR |= 1<<0;
    //TIM1、TIM8定时器初始化
    //向上计数模式、时钟不分频、自动重装载值、预分频系数、重复计数值为0
    //TIM1、TIM8定时器初始化,5ms溢出一次(psc=168-1,arr=5000-1)
    TIM1->CR1 |= 0<<4;
    TIM1->CR1 |= 0<<8;
    TIM1->PSC = psc;
    TIM1->ARR = arr;
    TIM1->RCR |= 0; //一般用不上
}

void TIM2_Init(uint32_t psc,uint32_t arr)
{
    //APB1时钟初始化
    RCC->APB1ENR |= 1<<0;
    //向上计数模式、时钟不分频、自动重装载值、预分频系数
    //装载值被设定为0xffff及以下的值时候，TIM2和TIM5是16位计数器。装载值被设定为0xffff-0xffffffff之间的时候，TIM2和TIM5是32位计数器。
    //TIM2、TIM3、TIM4、TIM5定时器初始化
    TIM2->CR1 |= 0<<4;
    TIM2->CR1 |= 0<<8;
    TIM2->PSC = psc;
    TIM2->ARR = arr;
}

void TIM9_Init(uint16_t psc,uint16_t arr)
{
    //APB2时钟初始化
    RCC->APB2ENR |= 1<<16;
    //默认向上计数、时钟不分频、自动重装载值、预分频系数
    //TIM9~TIM14定时器初始化
    TIM9->CR1 |= 0<<8;
    TIM9->PSC = psc;
    TIM9->ARR = arr;
}

void TIM6_Init(uint16_t psc,uint16_t arr)
{
    //APB1时钟初始化
    RCC->APB1ENR |= 1<<5;
    //默认向上计数、自动重装载值、预分频系数
    //TIM6、TIM7定时器初始化
    TIM6->PSC = psc;
    TIM6->ARR = arr;
}

/******************************************输出比较初始化******************************************/

/******
 * 适用于： TIM1、TIM8
 * 通道数： 4
 * CCMR1/CCMR2: 输出比较模式选择
 * CCER:        输出比较使能、输出比较极性、互补输出比较使能、互补输出极性
 * CR2:         输出空闲状态、互补输出空闲状态
 * CCRx:        比较寄存器 x 的预装载值
******/
void TIM1_Output_Compare_x_Init(uint8_t CH)
{
    if (CH == 1)
    {
        //TIM1_CH1输出比较初始化
        //PWM1模式
        TIM1->CCMR1 |= (TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1);
        //使能输出比较、输出比较极性为高、不使能互补输出比较、互补输出极性为低
        TIM1->CCER  |= (TIM_CCER_CC1E | TIM_CCER_CC1P | 0x00 | 0x00);
        //输出空闲状态、互补输出空闲状态均设置为0
        TIM1->CR2   |= 0x00;
        //输出比较的预装载值
        TIM1->CCR1  |= 0x00;
    }
    else if (CH == 2)
    {
        //TIM1_CH2输出比较初始化
        //PWM1模式
        TIM1->CCMR1 |= (TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1);
        //使能输出比较、输出比较极性为高、不使能互补输出比较、互补输出极性为低
        TIM1->CCER  |= (TIM_CCER_CC2E | TIM_CCER_CC2P | 0x00 | 0x00);
        //输出空闲状态、互补输出空闲状态均设置为0
        TIM1->CR2   |= 0x00;
        //输出比较的预装载值
        TIM1->CCR2  |= 0x00;
    }
    else if (CH == 3)
    {
        //TIM1_CH3输出比较初始化
        //PWM1模式
        TIM1->CCMR2 |= (TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1);
        //使能输出比较、输出比较极性为高、不使能互补输出比较、互补输出极性为低
        TIM1->CCER  |= (TIM_CCER_CC3E | TIM_CCER_CC3P | 0x00 | 0x00);
        //输出空闲状态、互补输出空闲状态均设置为0
        TIM1->CR2   |= 0x00;
        //输出比较的预装载值
        TIM1->CCR3  |= 0x00;
    }
    else if (CH == 4)
    {
        //TIM1_CH4输出比较初始化
        //PWM1模式
        TIM1->CCMR2 |= (TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1);
        //使能输出比较、输出比较极性为高
        TIM1->CCER  |= (TIM_CCER_CC4E | TIM_CCER_CC4P);
        //输出空闲状态
        TIM1->CR2   |= 0x00;
        //输出比较的预装载值
        TIM1->CCR4  |= 0x00;
    }
}

/******
 * 适用于： TIM2、TIM3、TIM4、TIM5
 * 通道数： 4
 * CCMR1/CCMR2: 输出比较模式选择
 * CCER:        输出比较使能、输出比较极性
 * CCRx:        比较寄存器 x 的预装载值
******/
void TIM2_Output_Compare_x_Init(uint8_t CH)
{
    if (CH == 1)
    {
        //TIM2_CH1输出比较初始化
        //PWM1模式
        TIM2->CCMR1 |= (TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1);
        //使能输出比较、输出比较极性为高
        TIM2->CCER  |= (TIM_CCER_CC1E | TIM_CCER_CC1P);
        //输出比较的预装载值
        TIM2->CCR1  |= 0x00;
    }
    else if (CH == 2)
    {
        //TIM2_CH2输出比较初始化
        //PWM1模式
        TIM2->CCMR1 |= (TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1);
        //使能输出比较、输出比较极性为高
        TIM2->CCER  |= (TIM_CCER_CC2E | TIM_CCER_CC2P);
        //输出比较的预装载值
        TIM2->CCR2  |= 0x00;        
    }
    else if (CH == 3)
    {
        //TIM2_CH3输出比较初始化
        //PWM1模式
        TIM2->CCMR2 |= (TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1);
        //使能输出比较、输出比较极性为高
        TIM2->CCER  |= (TIM_CCER_CC3E | TIM_CCER_CC3P);
        //输出比较的预装载值
        TIM2->CCR3  |= 0x00;        
    }
     else if (CH == 4)
    {
        //TIM2_CH4输出比较初始化
        //PWM1模式
        TIM2->CCMR2 |= (TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1);
        //使能输出比较、输出比较极性为高
        TIM2->CCER  |= (TIM_CCER_CC4E | TIM_CCER_CC4P);
        //输出比较的预装载值
        TIM2->CCR4  |= 0x00;        
    } 
}

/******
 * 适用于： TIM9、TIM12
 * 通道数： 2
 * CCMR1:  输出比较模式选择
 * CCER:   输出比较使能、输出比较极性
 * CCRx:   比较寄存器 x 的预装载值
******/
void TIM9_Output_Compare_x_Init(uint8_t CH)
{
    if (CH == 1)
    {
        //TIM9_CH1输出比较初始化
        //PWM1模式
        TIM9->CCMR1 |= (TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1);
        //使能输出比较、输出比较极性为高
        TIM9->CCER  |= (TIM_CCER_CC1E | TIM_CCER_CC1P);
        //输出比较的预装载值
        TIM9->CCR1  |= 0x00;
    }
    else if (CH == 2)
    {
        //TIM9_CH2输出比较初始化
        //PWM1模式
        TIM9->CCMR1 |= (TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1);
        //使能输出比较、输出比较极性为高
        TIM9->CCER  |= (TIM_CCER_CC2E | TIM_CCER_CC2P);
        //输出比较的预装载值
        TIM9->CCR2  |= 0x00;  
    }
}

/******
 * 适用于： TIM10、TIM11、TIM13、TIM14
 * 通道数： 1
 * CCMR1:  输出比较模式选择
 * CCER:   输出比较使能、输出比较极性
 * CCRx:   比较寄存器 x 的预装载值
******/
void TIM10_Output_Compare_x_Init(void)
{
    //TIM9_CH1输出比较初始化
    //PWM1模式
    TIM10->CCMR1 |= (TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1);
    //使能输出比较、输出比较极性为高
    TIM10->CCER  |= (TIM_CCER_CC1E | TIM_CCER_CC1P);
    //输出比较的预装载值
    TIM10->CCR1  |= 0x00;
}

//没有测试该函数是否正确
/******************************************输入捕获初始化******************************************/

/******
 * 适用于： TIM1、TIM8
 * 通道数： 4
 * CCMR1/CCMR2: 映射关系、分频系数、滤波器
 * CCER:        使能输入捕获、边沿触发方式
******/
void TIM1_Input_Capture_x_Init(uint8_t CH)
{
    if (CH == 1)
    {
        //TIM1_CH1输入捕获初始化
        //IC1映射到TI1、不分频、不滤波
        TIM1->CCMR1 |= (TIM_CCMR1_CC1S_0 | 0x00 | 0x00);
        //使能输入捕获、非反相/上升沿触发( TIM_CCER_CC1P = 0x00,TIM_CCER_CC1NP = 0x00 )
        TIM1->CCER  |= (TIM_CCER_CC1E | 0x00 | 0x00);
    }
    else if (CH ==2)
    {
        //TIM1_CH2输入捕获初始化
        //IC2映射到TI2、不分频、不滤波
        TIM1->CCMR1 |= (TIM_CCMR1_CC2S_0 | 0x00 | 0x00);
        //使能输入捕获、非反相/上升沿触发( TIM_CCER_CC2P = 0x00,TIM_CCER_CC2NP = 0x00 )
        TIM1->CCER  |= (TIM_CCER_CC2E | 0x00 | 0x00);
    }
    else if (CH == 3)
    {
        //TIM1_CH3输入捕获初始化
        //IC3映射到TI3、不分频、不滤波
        TIM1->CCMR2 |= (TIM_CCMR2_CC3S_0 | 0x00 | 0x00);
        //使能输入捕获、非反相/上升沿触发( TIM_CCER_CC3P = 0x00,TIM_CCER_CC3NP = 0x00 )
        TIM1->CCER  |= (TIM_CCER_CC3E | 0x00 | 0x00);        
    }
    else if (CH == 4)
    {
        //TIM1_CH4输入捕获初始化
        //IC4映射到TI4、不分频、不滤波
        TIM1->CCMR2 |= (TIM_CCMR2_CC4S_0 | 0x00 | 0x00);
        //使能输入捕获、非反相/上升沿触发( TIM_CCER_CC4P = 0x00,TIM_CCER_CC4NP = 0x00 )
        TIM1->CCER  |= (TIM_CCER_CC4E | 0x00 | 0x00);        
    }
}

/******
 * 适用于： TIM2、TIM3、TIM4、TIM5
 * 通道数： 4
 * CCMR1/CCMR2: 映射关系、分频系数、滤波器
 * CCER:        使能输入捕获、边沿触发方式
******/
void TIM2_Input_Capture_x_Init(uint8_t CH)
{
    if (CH == 1)
    {
        //TIM2_CH1输入捕获初始化
        //IC1映射到TI1、不分频、不滤波
        TIM2->CCMR1 |= (TIM_CCMR1_CC1S_0 | 0x00 | 0x00);
        //使能输入捕获、非反相/上升沿触发( TIM_CCER_CC1P = 0x00,TIM_CCER_CC1NP = 0x00 )
        TIM2->CCER  |= (TIM_CCER_CC1E | 0x00 | 0x00);
    }
    else if (CH ==2)
    {
        //TIM2_CH2输入捕获初始化
        //IC2映射到TI2、不分频、不滤波
        TIM2->CCMR1 |= (TIM_CCMR1_CC2S_0 | 0x00 | 0x00);
        //使能输入捕获、非反相/上升沿触发( TIM_CCER_CC2P = 0x00,TIM_CCER_CC2NP = 0x00 )
        TIM2->CCER  |= (TIM_CCER_CC2E | 0x00 | 0x00);
    }
    else if (CH == 3)
    {
        //TIM2_CH3输入捕获初始化
        //IC3映射到TI3、不分频、不滤波
        TIM2->CCMR2 |= (TIM_CCMR2_CC3S_0 | 0x00 | 0x00);
        //使能输入捕获、非反相/上升沿触发( TIM_CCER_CC3P = 0x00,TIM_CCER_CC3NP = 0x00 )
        TIM2->CCER  |= (TIM_CCER_CC3E | 0x00 | 0x00);        
    }
    else if (CH == 4)
    {
        //TIM2_CH4输入捕获初始化
        //IC4映射到TI4、不分频、不滤波
        TIM2->CCMR2 |= (TIM_CCMR2_CC4S_0 | 0x00 | 0x00);
        //使能输入捕获、非反相/上升沿触发( TIM_CCER_CC4P = 0x00,TIM_CCER_CC4NP = 0x00 )
        TIM2->CCER  |= (TIM_CCER_CC4E | 0x00 | 0x00);        
    }
}

/******
 * 适用于： TIM9、TIM12
 * 通道数： 2
 * CCMR1:  映射关系、分频系数、滤波器
 * CCER:   使能输入捕获、边沿触发方式
******/
void TIM9_Input_Capture_x_Init(uint8_t CH)
{
    if (CH == 1)
    {
        //TIM9_CH1输入捕获初始化
        //IC1映射到TI1、不分频、不滤波
        TIM9->CCMR1 |= (TIM_CCMR1_CC1S_0 | 0x00 | 0x00);
        //使能输入捕获、非反相/上升沿触发( TIM_CCER_CC1P = 0x00,TIM_CCER_CC1NP = 0x00 )
        TIM9->CCER  |= (TIM_CCER_CC1E | 0x00 | 0x00);
    }
    else if (CH ==2)
    {
        //TIM9_CH2输入捕获初始化
        //IC2映射到TI2、不分频、不滤波
        TIM9->CCMR1 |= (TIM_CCMR1_CC2S_0 | 0x00 | 0x00);
        //使能输入捕获、非反相/上升沿触发( TIM_CCER_CC2P = 0x00,TIM_CCER_CC2NP = 0x00 )
        TIM9->CCER  |= (TIM_CCER_CC2E | 0x00 | 0x00);
    }
}

/******
 * 适用于： TIM10、TIM11、TIM13、TIM14
 * 通道数： 1
 * CCMR1:  映射关系、分频系数、滤波器
 * CCER:   使能输入捕获、边沿触发方式
******/
void TIM10_Input_Capture_x_Init(void)
{
    //TIM10_CH1输入捕获初始化
    //IC1映射到TI1、不分频、不滤波
    TIM10->CCMR1 |= (TIM_CCMR1_CC1S_0 | 0x00 | 0x00);
    //使能输入捕获、非反相/上升沿触发( TIM_CCER_CC1P = 0x00,TIM_CCER_CC1NP = 0x00 )
    TIM10->CCER  |= (TIM_CCER_CC1E | 0x00 | 0x00);
}

//没有测试该函数是否正确
/******************************************定时器编码器模式******************************************/

/******
 * 适用于： TIM1、TIM2、TIM3、TIM4、TIM5、TIM8、TIM9、TIM12
 * 通道：   CH1、CH2 
 * SMCR:    编码器模式
 * CCMR1:   输入通道，同时也可以设置滤波
 * CCER:    触发方式
******/
void TIM_EncoderConfig(TIM_TypeDef* TIMx,uint16_t EncoderMode,uint16_t TIM_IC1Polarity, uint16_t TIM_IC2Polarity)
{
    //设置编码器模式
    TIMx->SMCR  |= EncoderMode;
    //设置输入通道
    TIMx->CCMR1 |= ( TIM_CCMR1_CC1S_0 | TIM_CCMR1_CC2S_0 );
    //触发方式
    TIMx->CCER  |= (TIM_IC1Polarity | (TIM_IC2Polarity << 4));
}

