#ifndef IR_H
#define IR_H

#include <stdbool.h>
#include <stdint.h>

/*---------------------------------------------------------
 * Driver API
 *--------------------------------------------------------*/

void IR_Init(void);

bool IR_ObjectDetected(void);

uint8_t IR_GetState(void);

#endif
