# 本项目旨在搭建小车开发框架，节省开发时间

### 基于STM32F407ZGT6开发板的小车开发模板

使用软件：Keil uVision5

使用器件：稳压模块、L289n电机驱动、带霍尔编码器的直流电机、电池、一块STM32F407ZGT6开发板

#### 工程文件结构：

```
└─01.Car_STM32F407_L298n
    ├─CODE_ARM
    ├─FWLIB_ST
    │  ├─inc
    │  └─src
    ├─OBJ
    ├─Project
    ├─SYSTEM
    └─USER
        ├─Control
        └─Drive
```

- **CODE_ARM**：由ARM公司提供的CMSIS
- **FWLIB_ST**：由ST公司开发的标准外设库
  - inc：标准外设库头文件
  - src：标准外设库C文件
- **OBJ**：编译产生的文件，包括产生的`.hex`文件
- **Project**：
- **SYSTEM**：存放启动文件、标准外设库的配置文件、寄存器定义文件
  - startup_stm32f40_41xxx.s —— 启动文件
  - system_stm32f4xx.c —— STM32F4xx系统时钟配置文件
  - system_stm32f4xx.h —— STM32F4xx系统时钟配置文件头文件
    - stm32f4xx_conf.h —— 库配置文件
    - stm32f4xx_it.c —— 中断处理文件
    - stm32f4xx_it.h —— 中断处理文件头文件
  - stm32f4xx —— 包含所有外设寄存器的定义（使用寄存器开发不可或缺）
- **USER**
  - ==Drive==：存放驱动文件，定时器，ADC，串口等
  - ==Hardware==：各种外设和模块的驱动程序

#### 使用的单片机资源

