#ifndef _SENSOR_DATA_H_
#define _SENSOR_DATA_H_

#include <Arduino.h>

typedef struct SensorData
{
    char Label[4] = "SX";
    bool Connected = false;
	uint8_t Index = 0;
    uint8_t Pin = A0;
    uint16_t CurrentValue = 0;
    uint16_t PreviousValue = 0;
    uint16_t X = 0;
    uint16_t Y = 0;
} SensorData;

#endif