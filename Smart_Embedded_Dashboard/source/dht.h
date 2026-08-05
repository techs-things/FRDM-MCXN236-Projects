#ifndef DHT22_H
#define DHT22_H

#include <stdint.h>
#include <stdbool.h>

void DHT_Init(void);

bool DHT_Read(void);

float DHT_GetTemperature(void);

float DHT_GetHumidity(void);

bool DHT_ReadReliable(void);
#endif
