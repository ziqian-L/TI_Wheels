#ifndef __MOTOR_PWM_H
#define __MOTOR_PWM_H

#include "stm32f4xx_conf.h"
#include "sys.h"

/***********************************
 * �߼���ʱ��TIM1��TIM8��PWM�����ʼ��
 * ����TIM1��TIM8��
 * λ����16 --- ������ģʽ�����ϡ����¡�����/�� --- DMA�����Բ���DAM���� --- ����/�Ƚ�ͨ����4 --- �����������
 * Ӧ�ó�������ʱ������PWM��������벶������Ƚϡ�
 * ���⣺���ɱ�������Ļ������
 ***********************************/
void TIM1_PWM_Init(uint16_t arr, uint16_t psc, uint8_t ch);
void TIM8_PWM_Init(uint16_t arr, uint16_t psc, uint8_t ch);

/***********************************
 * ͨ�ö�ʱ��TIM3��TIM4��PWM�����ʼ��
 * ����TIM3��TIM4��
 * λ����16 --- ������ģʽ�����ϡ����¡�����/�� --- DMA�����Բ���DAM���� --- ����/�Ƚ�ͨ����4 --- �����������
 * Ӧ�ó�����ͨ�á���ʱ������PWM��������벶������Ƚ�
 ***********************************/
void TIM3_PWM_Init(uint16_t arr, uint16_t psc, uint8_t ch);
void TIM4_PWM_Init(uint16_t arr, uint16_t psc, uint8_t ch);

/***********************************
 * ͨ�ö�ʱ��TIM2��TIM5��PWM�����ʼ��
 * ����TIM2��TIM5��
 * λ����32 --- ������ģʽ�����ϡ����¡�����/�� --- DMA�����Բ���DAM���� --- ����/�Ƚ�ͨ����4 --- �����������
 * Ӧ�ó�����ͨ�á���ʱ������PWM��������벶������Ƚ�
 ***********************************/
void TIM2_PWM_Init(uint32_t arr, uint32_t psc, uint8_t ch);
void TIM5_PWM_Init(uint32_t arr, uint32_t psc, uint8_t ch);

#endif
