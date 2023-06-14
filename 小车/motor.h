#ifndef __MOTOR_H
#define __MOTOR_H

#include "sys.h"

/****
 * AIN2！！PA3
 * AIN1！！PA5
****/
#define Ain2 PAout(3)
#define Ain1 PAout(5)
/****
 * BIN2！！PA4
 * BIN1！！PA6
****/
#define Bin2 PAout(4)
#define Bin1 PAout(6)

void Motor_Init(void);
void Limit(int32_t *motorA,int32_t *motorB);
int32_t my_abs(int32_t p);
void Load_PWM(int motorA,int motorB);

#endif
