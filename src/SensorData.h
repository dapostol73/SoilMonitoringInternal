#ifndef _SENSOR_DATA_H_
#define _SENSOR_DATA_H_

#include <Arduino.h>

class SensorData
{
    private:
        int16_t clamp(int16_t value, int16_t minimum, int16_t maximum);

    public:
        char Label[4] = "SX";
        bool Connected = false;
        uint8_t Index = 0;
        uint8_t Pin = A0;
        uint16_t CurrentValue = 0;
        uint16_t PreviousValue = 0;
        uint16_t MinValue = 1250;
        uint16_t MaxValue = 2950;
        uint16_t X = 0;
        uint16_t Y = 0;

        SensorData();
        void init(uint8_t pin, uint8_t index, uint16_t x, uint16_t y);
        void readValue();
};

#endif