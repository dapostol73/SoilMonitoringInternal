#include "SensorData.h"

SensorData::SensorData()
{
}

void SensorData::init(uint8_t pin, uint8_t index, uint16_t x, uint16_t y)
{
    Pin = pin;
    sprintf(Label, "S%d" , index+1);
    Index = index;
    X = x;
    Y = y;
    pinMode(Pin, INPUT);
}

int16_t SensorData::clamp(int16_t value, int16_t minimum, int16_t maximum)
{
    return min(max(value, minimum), maximum);
}

void SensorData::readValue()
{
    int mois = analogRead(Pin);
    Connected = mois > 1000;
    PreviousValue = CurrentValue;
    CurrentValue = map(clamp(mois, MinValue, MaxValue), MinValue, MaxValue, 100, 0);

    #ifdef SERIAL_LOGGING
    char info[48] = "";
    if(mois >= 4000)
    {
        strcpy(info, "Sensor is not in the Soil or DISCONNECTED");
    }
    else if (mois >= 2400)
    {
        strcpy(info, "Soil is DRY");
    }
    else if(mois >= 1500)
    {
        strcpy(info, "Soil is HUMID"); 
    }
    else if(mois >= 50)
    {
        strcpy(info, "Sensor in WATER");
    }
    else
    {
        strcpy(info, "Sensor in DISCONNECTED?");
    }
    Serial.println(info);
    sprintf(info, "Value: %d", mois);
    Serial.println(info);
    #endif  
}
