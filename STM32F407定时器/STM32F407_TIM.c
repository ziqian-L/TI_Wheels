/**********************************************
 * 函数名：TIM1_PWM_Init
 * 函数功能：TIM1四个通道的PWM输出
 * 入口参数：arr--自动重装载值;psc--定时器分频;ch--通道选择,0--全部开启,1--开CH1,2--开CH2,3--开CH3,4--开CH4
 * 返回值：无
 * 说明：
 * STM32F407中TIM1的引脚为：
 * ETR  PA12            PE7(外部时钟源模式)
 * CH1  PA8             PE9
 * CH2  PA9             PE11
 * CH3  PA10            PE13
 * CH4  PA11            PE14
 * BKIN PA6     PB12    PE15
 * CH1N PA7     PB13    PE8(刹车功能输入)
 * CH2N PB0     PB14    PE10
 * CH3N PB1     PB15    PE12
 * 函数里选用的是PA8、PA9、PA10、PA11
 **********************************************/
void TIM1_PWM_Init(uint16_t arr, uint16_t psc, uint8_t ch)
{
    // 结构体定义
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    // 时钟使能
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);  // TIM1时钟使能
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); // 使能GPIOA时钟

    // GPIO复用
    if (ch == 0)
    {
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_TIM1);  // PA8 复用为 TIM1_CH1
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_TIM1);  // PA9 复用为 TIM1_CH2
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_TIM1); // PA10 复用为 TIM1_CH3
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_TIM1); // PA11 复用为 TIM1_CH4
    }
    if (ch == 1)
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_TIM1); // PA8 复用为 TIM1_CH1
    if (ch == 2)
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_TIM1); // PA9 复用为 TIM1_CH2
    if (ch == 3)
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_TIM1); // PA10 复用为 TIM1_CH3
    if (ch == 4)
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_TIM1); // PA11 复用为 TIM1_CH4

    // GPIO初始化
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;       // 复用功能
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // 速度100MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     // 推挽复用输出
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;     // 上拉
    GPIO_Init(GPIOA, &GPIO_InitStructure);             // 初始化

    // 定时器初始化
    TIM_TimeBaseStructure.TIM_Prescaler = psc;                  // 定时器分频
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数模式
    TIM_TimeBaseStructure.TIM_Period = arr;                     // 自动重装载值
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); // 初始化定时器14

    // 输出比较初始化
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;             // 选择定时器模式:TIM脉冲宽度调制模式2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // 比较输出使能
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;      // 输出极性:TIM输出比较极性低

    // 初始化TIM1的PWM模式
    if (ch == 0)
    {
        // 初始化TIM1 Channel1 PWM模式
        TIM_OC1Init(TIM1, &TIM_OCInitStructure);          // 根据指定的参数初始化外设TIM1OC1
        TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable); // 使能TIM1在CCR1上的预装载寄存器
        // 初始化TIM1 Channel2 PWM模式
        TIM_OC2Init(TIM1, &TIM_OCInitStructure);          // 根据指定的参数初始化外设TIM1OC2
        TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable); // 使能TIM1在CCR2上的预装载寄存器
        // 初始化TIM1 Channel3 PWM模式
        TIM_OC3Init(TIM1, &TIM_OCInitStructure);          // 根据指定的参数初始化外设TIM1OC3
        TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable); // 使能TIM14在CCR3上的预装载寄存器
        // 初始化TIM1 Channel4 PWM模式
        TIM_OC4Init(TIM1, &TIM_OCInitStructure);          // 根据指定的参数初始化外设TIM1OC4
        TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable); // 使能TIM1在CCR4上的预装载寄存
    }
    if (ch == 1) // 初始化TIM1 Channel1 PWM模式
    {
        TIM_OC1Init(TIM1, &TIM_OCInitStructure);          // 根据指定的参数初始化外设TIM1OC1
        TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable); // 使能TIM1在CCR1上的预装载寄存器
    }
    if (ch == 2) // 初始化TIM1 Channel2 PWM模式
    {
        TIM_OC2Init(TIM1, &TIM_OCInitStructure);          // 根据指定的参数初始化外设TIM1OC2
        TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable); // 使能TIM1在CCR2上的预装载寄存器
    }
    if (ch == 3) // 初始化TIM1 Channel3 PWM模式
    {
        TIM_OC3Init(TIM1, &TIM_OCInitStructure);          // 根据指定的参数初始化外设TIM1OC3
        TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable); // 使能TIM1在CCR3上的预装载寄存器
    }
    if (ch == 4) // 初始化TIM1 Channel4 PWM模式
    {
        TIM_OC4Init(TIM1, &TIM_OCInitStructure);          // 根据指定的参数初始化外设TIM1OC4
        TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable); // 使能TIM1在CCR4上的预装载寄存
    }

    TIM_ARRPreloadConfig(TIM1, DISABLE); // ARPE不使能

    TIM_SetCompare1(TIM1, 0); // CH1置零占空比
    TIM_SetCompare2(TIM1, 0); // CH1置零占空比
    TIM_SetCompare3(TIM1, 0); // CH3置零占空比
    TIM_SetCompare4(TIM1, 0); // CH4置零占空比

    TIM_CtrlPWMOutputs(TIM1, ENABLE); // 使能刹车和死区寄存器

    TIM_Cmd(TIM1, ENABLE); // 使能TIM1
}

/**********************************************
 * 函数名：TIM3_PWM_Init
 * 函数功能：TIM3四个通道的PWM输出
 * 入口参数：arr--自动重装载值;psc--定时器分频;ch--通道选择,0--全部开启,1--开CH1,2--开CH2,3--开CH3,4--开CH4
 * 返回值：无
 * 说明：
 * STM32F407中TIM3的引脚为：
 * ETR                  PD2(外部时钟源模式)
 * CH1  PA6     PB4     PC6
 * CH2  PA7     PB5     PC7
 * CH3  PB0             PC8
 * CH4  PB1             PC9
 * 函数里选用的是PC6，PC7，PC8，PC9
 **********************************************/
void TIM3_PWM_Init(uint16_t arr, uint16_t psc, uint8_t ch)
{
    // 结构体定义
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    // 时钟使能
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);  // TIM3时钟使能
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); // 使能GPIOC时钟

    // GPIO复用
    if (ch == 0)
    {
        GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3); // PC6 复用为 TIM3_CH1
        GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM3); // PC7 复用为 TIM3_CH2
        GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_TIM3); // PC8 复用为 TIM3_CH3
        GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_TIM3); // PC9 复用为 TIM3_CH4
    }
    if (ch == 1)
        GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3); // PC6 复用为 TIM3_CH1
    if (ch == 2)
        GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM3); // PC7 复用为 TIM3_CH2
    if (ch == 3)
        GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_TIM3); // PC8 复用为 TIM3_CH3
    if (ch == 4)
        GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_TIM3); // PC9 复用为 TIM3_CH4

    // GPIO初始化
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;       // 复用功能
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // 速度100MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     // 推挽复用输出
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;     // 上拉
    GPIO_Init(GPIOC, &GPIO_InitStructure);             // 初始化

    // 定时器初始化
    TIM_TimeBaseStructure.TIM_Prescaler = psc;                  // 定时器分频
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数模式
    TIM_TimeBaseStructure.TIM_Period = arr;                     // 自动重装载值
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); // 初始化定时器14

    // 输出比较初始化
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;             // 选择定时器模式:TIM脉冲宽度调制模式2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // 比较输出使能
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;      // 输出极性:TIM输出比较极性低

    // 初始化TIM3的PWM模式
    if (ch == 0)
    {
        // 初始化TIM3 Channel1 PWM模式
        TIM_OC1Init(TIM3, &TIM_OCInitStructure);          // 根据指定的参数初始化外设TIM3OC1
        TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable); // 使能TIM3在CCR1上的预装载寄存器
        // 初始化TIM3 Channel2 PWM模式
        TIM_OC2Init(TIM3, &TIM_OCInitStructure);          // 根据指定的参数初始化外设TIM3OC2
        TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable); // 使能TIM3在CCR2上的预装载寄存器
        // 初始化TIM3 Channel3 PWM模式
        TIM_OC3Init(TIM3, &TIM_OCInitStructure);          // 根据指定的参数初始化外设TIM3OC3
        TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable); // 使能TIM14在CCR3上的预装载寄存器
        // 初始化TIM3 Channel4 PWM模式
        TIM_OC4Init(TIM3, &TIM_OCInitStructure);          // 根据指定的参数初始化外设TIM3OC4
        TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable); // 使能TIM3在CCR4上的预装载寄存
    }
    if (ch == 1) // 初始化TIM3 Channel1 PWM模式
    {
        TIM_OC1Init(TIM3, &TIM_OCInitStructure);          // 根据指定的参数初始化外设TIM3OC1
        TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable); // 使能TIM3在CCR1上的预装载寄存器
    }
    if (ch == 2) // 初始化TIM3 Channel2 PWM模式
    {
        TIM_OC2Init(TIM3, &TIM_OCInitStructure);          // 根据指定的参数初始化外设TIM3OC2
        TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable); // 使能TIM3在CCR2上的预装载寄存器
    }
    if (ch == 3) // 初始化TIM3 Channel3 PWM模式
    {
        TIM_OC3Init(TIM3, &TIM_OCInitStructure);          // 根据指定的参数初始化外设TIM3OC3
        TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable); // 使能TIM3在CCR3上的预装载寄存器
    }
    if (ch == 4) // 初始化TIM3 Channel4 PWM模式
    {
        TIM_OC4Init(TIM3, &TIM_OCInitStructure);          // 根据指定的参数初始化外设TIM3OC4
        TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable); // 使能TIM3在CCR4上的预装载寄存
    }

    TIM_ARRPreloadConfig(TIM3, DISABLE); // ARPE不使能

    TIM_SetCompare1(TIM3, 0); // CH1置零占空比
    TIM_SetCompare2(TIM3, 0); // CH1置零占空比
    TIM_SetCompare3(TIM3, 0); // CH3置零占空比
    TIM_SetCompare4(TIM3, 0); // CH4置零占空比

    TIM_Cmd(TIM3, ENABLE); // 使能TIM3
}

/**********************************************
 * 函数名：TIM4_PWM_Init
 * 函数功能：TIM4四个通道的PWM输出
 * 入口参数：arr--自动重装载值;psc--定时器分频;ch--通道选择,0--全部开启,1--开CH1,2--开CH2,3--开CH3,4--开CH4
 * 返回值：无
 * 说明：
 * STM32F407中TIM4的引脚为：
 * ETR          PE0(外部时钟源模式)
 * CH1  PB6     PD12
 * CH2  PB7     PD13
 * CH3  PB8     PD14
 * CH4  PB9     PD15
 * 函数里选用的是PB6，PB7，PB8，PB9
 **********************************************/
void TIM4_PWM_Init(uint16_t arr, uint16_t psc, uint8_t ch)
{
    // 结构体定义
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    // 时钟使能
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);  // TIM4时钟使能
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); // 使能GPIOB时钟

    // GPIO复用
    if (ch == 0)
    {
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_TIM4); // PB6 复用为 TIM4_CH1
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_TIM4); // PB7 复用为 TIM4_CH2
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_TIM4); // PB8 复用为 TIM4_CH3
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_TIM4); // PB9 复用为 TIM4_CH4
    }
    if (ch == 1)
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_TIM4); // PB6 复用为 TIM4_CH1
    if (ch == 2)
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_TIM4); // PB7 复用为 TIM4_CH2
    if (ch == 3)
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_TIM4); // PB8 复用为 TIM4_CH3
    if (ch == 4)
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_TIM4); // PB9 复用为 TIM4_CH4

    // GPIO初始化
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;       // 复用功能
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // 速度100MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     // 推挽复用输出
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;     // 上拉
    GPIO_Init(GPIOB, &GPIO_InitStructure);             // 初始化

    // 定时器初始化
    TIM_TimeBaseStructure.TIM_Prescaler = psc;                  // 定时器分频
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数模式
    TIM_TimeBaseStructure.TIM_Period = arr;                     // 自动重装载值
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); // 初始化定时器14

    // 输出比较初始化
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;             // 选择定时器模式:TIM脉冲宽度调制模式2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // 比较输出使能
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;      // 输出极性:TIM输出比较极性低

    // 初始化TIM4的PWM模式
    if (ch == 0)
    {
        // 初始化TIM4 Channel1 PWM模式
        TIM_OC1Init(TIM4, &TIM_OCInitStructure);          // 根据指定的参数初始化外设TIM4OC1
        TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable); // 使能TIM4在CCR1上的预装载寄存器
        // 初始化TIM4 Channel2 PWM模式
        TIM_OC2Init(TIM4, &TIM_OCInitStructure);          // 根据指定的参数初始化外设TIM4OC2
        TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable); // 使能TIM4在CCR2上的预装载寄存器
        // 初始化TIM4 Channel3 PWM模式
        TIM_OC3Init(TIM4, &TIM_OCInitStructure);          // 根据指定的参数初始化外设TIM4OC3
        TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable); // 使能TIM4在CCR3上的预装载寄存器
        // 初始化TIM4 Channel4 PWM模式
        TIM_OC4Init(TIM4, &TIM_OCInitStructure);          // 根据指定的参数初始化外设TIM4OC4
        TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable); // 使能TIM4在CCR4上的预装载寄存
    }
    if (ch == 1) // 初始化TIM4 Channel1 PWM模式
    {
        TIM_OC1Init(TIM4, &TIM_OCInitStructure);          // 根据指定的参数初始化外设TIM4OC1
        TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable); // 使能TIM4在CCR1上的预装载寄存器
    }
    if (ch == 2) // 初始化TIM4 Channel2 PWM模式
    {
        TIM_OC2Init(TIM4, &TIM_OCInitStructure);          // 根据指定的参数初始化外设TIM4OC2
        TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable); // 使能TIM4在CCR2上的预装载寄存器
    }
    if (ch == 3) // 初始化TIM4 Channel3 PWM模式
    {
        TIM_OC3Init(TIM4, &TIM_OCInitStructure);          // 根据指定的参数初始化外设TIM4OC3
        TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable); // 使能TIM4在CCR3上的预装载寄存器
    }
    if (ch == 4) // 初始化TIM4 Channel4 PWM模式
    {
        TIM_OC4Init(TIM4, &TIM_OCInitStructure);          // 根据指定的参数初始化外设TIM4OC4
        TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable); // 使能TIM4在CCR4上的预装载寄存
    }

    TIM_ARRPreloadConfig(TIM4, DISABLE); // ARPE不使能

    TIM_SetCompare1(TIM4, 0); // CH1置零占空比
    TIM_SetCompare2(TIM4, 0); // CH2置零占空比
    TIM_SetCompare3(TIM4, 0); // CH3置零占空比
    TIM_SetCompare4(TIM4, 0); // CH4置零占空比

    TIM_Cmd(TIM4, ENABLE); // 使能TIM4
}
