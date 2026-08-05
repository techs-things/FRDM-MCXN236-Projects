#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include <stdint.h>
#include <stdbool.h>

void Ultrasonic_Init(void);
void Ultrasonic_Trigger(void);
bool Ultrasonic_ReadEcho(void);

uint32_t Ultrasonic_GetDistanceCM(void);
uint32_t Ultrasonic_GetAverageDistance(void);
#endif
