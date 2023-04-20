#ifndef __MLX90614_H
#define __MLX90614_H

#include "sys.h"

void Temperature_measure_Init(void);
float Read_Temperature(uint8_t address);

#endif
