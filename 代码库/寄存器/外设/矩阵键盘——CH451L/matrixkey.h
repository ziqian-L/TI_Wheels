#ifndef __MATRIXKEY_H
#define __MATRIXKEY_H

#include "sys.h"

#define DOUT_Read   PAin(7)
#define LOAD    PAout(5)
#define DIN     PAout(3)
#define DCLK    PAout(1)

void Matrix_Key_Init(void);
void Matrix_Key_Writer(uint16_t command);
void Matrix_Key_Data(uint8_t *key);

#endif
