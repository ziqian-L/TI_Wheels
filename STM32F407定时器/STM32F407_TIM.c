/**********************************************
 * ��������TIM3_PWM_Init
 * �������ܣ�TIM3�ĸ�ͨ����PWM���
 * ��ڲ�����arr--�Զ���װ��ֵ;psc--��ʱ����Ƶ;ch--ͨ��ѡ��,0--ȫ������,1--��CH1,2--��CH2,3--��CH3,4--��CH4
 * ����ֵ����
 * ˵����
 * STM32F407��TIM3������Ϊ��
 * ETR                  PD2(�ⲿʱ��Դģʽ)
 * CH1  PA6     PB4     PC6
 * CH2  PA7     PB5     PC7
 * CH3  PB0             PC8
 * CH4  PB1             PC9
 * ������ѡ�õ���PC6��PC7��PC8��PC9
 **********************************************/
void TIM3_PWM_Init(uint16_t arr, uint16_t psc, uint8_t ch)
{
    //�ṹ�嶨��
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    //ʱ��ʹ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);  // TIM3ʱ��ʹ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); //ʹ��GPIOCʱ��

    //GPIO����
    if (ch == 0)
    {
        GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3); // PC6 ����Ϊ TIM3_CH1
        GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM3); // PC7 ����Ϊ TIM3_CH2
        GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_TIM3); // PC8 ����Ϊ TIM3_CH3
        GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_TIM3); // PC9 ����Ϊ TIM3_CH4
    }
    if (ch == 1)
        GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3); // PC6 ����Ϊ TIM3_CH1
    if (ch == 2)
        GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM3); // PC7 ����Ϊ TIM3_CH2
    if (ch == 3)
        GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_TIM3); // PC8 ����Ϊ TIM3_CH3
    if (ch == 4)
        GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_TIM3); // PC9 ����Ϊ TIM3_CH4

    //GPIO��ʼ��
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;       //���ù���
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //�ٶ�100MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     //���츴�����
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;     //����
    GPIO_Init(GPIOC, &GPIO_InitStructure);             //��ʼ��

    //��ʱ����ʼ��
    TIM_TimeBaseStructure.TIM_Prescaler = psc;                  //��ʱ����Ƶ
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ���ģʽ
    TIM_TimeBaseStructure.TIM_Period = arr;                     //�Զ���װ��ֵ
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //��ʼ����ʱ��14

    //����Ƚϳ�ʼ��
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;             //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;      //�������:TIM����Ƚϼ��Ե�

    //��ʼ��TIM3��PWMģʽ
    if (ch == 0)
    {
        //��ʼ��TIM3 Channel1 PWMģʽ
        TIM_OC1Init(TIM3, &TIM_OCInitStructure);          //����Tָ���Ĳ�����ʼ������TIM3OC1
        TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable); //ʹ��TIM3��CCR1�ϵ�Ԥװ�ؼĴ���
        //��ʼ��TIM3 Channel2 PWMģʽ
        TIM_OC2Init(TIM3, &TIM_OCInitStructure);          //����Tָ���Ĳ�����ʼ������TIM3OC2
        TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable); //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���
        //��ʼ��TIM3 Channel3 PWMģʽ
        TIM_OC3Init(TIM3, &TIM_OCInitStructure);          //����Tָ���Ĳ�����ʼ������TIM3OC3
        TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable); //ʹ��TIM14��CCR3�ϵ�Ԥװ�ؼĴ���
        //��ʼ��TIM3 Channel4 PWMģʽ
        TIM_OC4Init(TIM3, &TIM_OCInitStructure);          //����Tָ���Ĳ�����ʼ������TIM3OC4
        TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable); //ʹ��TIM3��CCR4�ϵ�Ԥװ�ؼĴ�
    }
    if (ch == 1) //��ʼ��TIM3 Channel1 PWMģʽ
    {
        TIM_OC1Init(TIM3, &TIM_OCInitStructure);          //����Tָ���Ĳ�����ʼ������TIM3OC1
        TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable); //ʹ��TIM3��CCR1�ϵ�Ԥװ�ؼĴ���
    }
    if (ch == 2) //��ʼ��TIM3 Channel2 PWMģʽ
    {
        TIM_OC2Init(TIM3, &TIM_OCInitStructure);          //����Tָ���Ĳ�����ʼ������TIM3OC2
        TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable); //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���
    }
    if (ch == 3) //��ʼ��TIM3 Channel3 PWMģʽ
    {
        TIM_OC3Init(TIM3, &TIM_OCInitStructure);          //����Tָ���Ĳ�����ʼ������TIM3OC3
        TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable); //ʹ��TIM3��CCR3�ϵ�Ԥװ�ؼĴ���
    }
    if (ch == 4) //��ʼ��TIM3 Channel4 PWMģʽ
    {
        TIM_OC4Init(TIM3, &TIM_OCInitStructure);          //����Tָ���Ĳ�����ʼ������TIM3OC4
        TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable); //ʹ��TIM3��CCR4�ϵ�Ԥװ�ؼĴ�
    }

    TIM_ARRPreloadConfig(TIM3, DISABLE); // ARPE��ʹ��
    TIM_Cmd(TIM3, ENABLE);               //ʹ��TIM3

    TIM_SetCompare1(TIM3, 0); // CH1����ռ�ձ�
    TIM_SetCompare2(TIM3, 0); // CH1����ռ�ձ�
    TIM_SetCompare3(TIM3, 0); // CH3����ռ�ձ�
    TIM_SetCompare4(TIM3, 0); // CH4����ռ�ձ�
}

/**********************************************
 * ��������TIM4_PWM_Init
 * �������ܣ�TIM4�ĸ�ͨ����PWM���
 * ��ڲ�����arr--�Զ���װ��ֵ;psc--��ʱ����Ƶ;ch--ͨ��ѡ��,0--ȫ������,1--��CH1,2--��CH2,3--��CH3,4--��CH4
 * ����ֵ����
 * ˵����
 * STM32F407��TIM4������Ϊ��
 * ETR          PE0(�ⲿʱ��Դģʽ)
 * CH1  PB6     PD12
 * CH2  PB7     PD13
 * CH3  PB8     PD14
 * CH4  PB9     PD15
 * ������ѡ�õ���PB6��PB7��PB8��PB9
 **********************************************/
void TIM4_PWM_Init(uint16_t arr, uint16_t psc, uint8_t ch)
{
    //�ṹ�嶨��
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    //ʱ��ʹ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);  // TIM4ʱ��ʹ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); //ʹ��GPIOBʱ��

    // GPIO����
    if (ch == 0)
    {
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_TIM4); // PB6 ����Ϊ TIM4_CH1
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_TIM4); // PB7 ����Ϊ TIM4_CH2
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_TIM4); // PB8 ����Ϊ TIM4_CH3
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_TIM4); // PB9 ����Ϊ TIM4_CH4
    }
    if (ch == 1)
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_TIM4); // PB6 ����Ϊ TIM4_CH1
    if (ch == 2)
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_TIM4); // PB7 ����Ϊ TIM4_CH2
    if (ch == 3)
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_TIM4); // PB8 ����Ϊ TIM4_CH3
    if (ch == 4)
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_TIM4); // PB9 ����Ϊ TIM4_CH4

    // GPIO��ʼ��
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;       //���ù���
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //�ٶ�100MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     //���츴�����
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;     //����
    GPIO_Init(GPIOB, &GPIO_InitStructure);             //��ʼ��

    //��ʱ����ʼ��
    TIM_TimeBaseStructure.TIM_Prescaler = psc;                  //��ʱ����Ƶ
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ���ģʽ
    TIM_TimeBaseStructure.TIM_Period = arr;                     //�Զ���װ��ֵ
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //��ʼ����ʱ��14

    //����Ƚϳ�ʼ��
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;             //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;      //�������:TIM����Ƚϼ��Ե�

    //��ʼ��TIM4��PWMģʽ
    if (ch == 0)
    {
        //��ʼ��TIM4 Channel1 PWMģʽ
        TIM_OC1Init(TIM4, &TIM_OCInitStructure);          //����Tָ���Ĳ�����ʼ������TIM4OC1
        TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable); //ʹ��TIM4��CCR1�ϵ�Ԥװ�ؼĴ���
        //��ʼ��TIM4 Channel2 PWMģʽ
        TIM_OC2Init(TIM4, &TIM_OCInitStructure);          //����Tָ���Ĳ�����ʼ������TIM4OC2
        TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable); //ʹ��TIM4��CCR2�ϵ�Ԥװ�ؼĴ���
        //��ʼ��TIM4 Channel3 PWMģʽ
        TIM_OC3Init(TIM4, &TIM_OCInitStructure);          //����Tָ���Ĳ�����ʼ������TIM4OC3
        TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable); //ʹ��TIM4��CCR3�ϵ�Ԥװ�ؼĴ���
        //��ʼ��TIM4 Channel4 PWMģʽ
        TIM_OC4Init(TIM4, &TIM_OCInitStructure);          //����Tָ���Ĳ�����ʼ������TIM4OC4
        TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable); //ʹ��TIM4��CCR4�ϵ�Ԥװ�ؼĴ�
    }
    if (ch == 1) //��ʼ��TIM4 Channel1 PWMģʽ
    {
        TIM_OC1Init(TIM4, &TIM_OCInitStructure);          //����Tָ���Ĳ�����ʼ������TIM4OC1
        TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable); //ʹ��TIM4��CCR1�ϵ�Ԥװ�ؼĴ���
    }
    if (ch == 2) //��ʼ��TIM4 Channel2 PWMģʽ
    {
        TIM_OC2Init(TIM4, &TIM_OCInitStructure);          //����Tָ���Ĳ�����ʼ������TIM4OC2
        TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable); //ʹ��TIM4��CCR2�ϵ�Ԥװ�ؼĴ���
    }
    if (ch == 3) //��ʼ��TIM4 Channel3 PWMģʽ
    {
        TIM_OC3Init(TIM4, &TIM_OCInitStructure);          //����Tָ���Ĳ�����ʼ������TIM4OC3
        TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable); //ʹ��TIM4��CCR3�ϵ�Ԥװ�ؼĴ���
    }
    if (ch == 4) //��ʼ��TIM4 Channel4 PWMģʽ
    {
        TIM_OC4Init(TIM4, &TIM_OCInitStructure);          //����Tָ���Ĳ�����ʼ������TIM4OC4
        TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable); //ʹ��TIM4��CCR4�ϵ�Ԥװ�ؼĴ�
    }

    TIM_ARRPreloadConfig(TIM4, DISABLE); // ARPE��ʹ��
    TIM_Cmd(TIM4, ENABLE);               //ʹ��TIM4

    TIM_SetCompare1(TIM4, 0); // CH1����ռ�ձ�
    TIM_SetCompare2(TIM4, 0); // CH2����ռ�ձ�
    TIM_SetCompare3(TIM4, 0); // CH3����ռ�ձ�
    TIM_SetCompare4(TIM4, 0); // CH4����ռ�ձ�
}
