#ifndef SENSOR_DATA_H
#define SENSOR_DATA_H

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    uint8_t hour;
    uint8_t minute;
    uint8_t second;

    float temperature;
    float humidity;

    uint32_t distance;

    bool objectDetected;

    bool motorRunning;

    uint8_t servoAngle;

} SensorData_t;

extern SensorData_t SensorData;

#endif
