#ifndef _DISPLAY_MAIN_H_
#define _DISPLAY_MAIN_H_

#include <Arduino.h>
#include <WiFi.h>

#include "DisplayControl.h"
#include "DisplayFonts.h"

#define BACKGROUND_COLOR 0x0000
#define FOREGROUND_COLOR 0xEF5D
#define OVERLAY_COLOR 0x1082
#define TEXT_TITLE_COLOR 0xDF81
#define TEXT_MAIN_COLOR 0xFD20
#define TEXT_ALT_COLOR 0x07FF

#define SUCCESS_COLOR 0x07E0
#define ERROR_COLOR 0xF800

#define DRY_LIGHT_COLOR 0xCCEC
#define DRY_DARK_COLOR 0x28E0
#define WET_COLOR 0x05FF

class DisplayMain : public DisplayControl
{
    private:
        void drawWiFiSignal(int16_t x, int16_t y, int16_t size, uint16_t backgroundColor = UINT16_MAX);
        void drawMositureMeter(const char* label, uint16_t x, uint16_t y);
    public:
        DisplayMain();
        void init();
        void clearDisplay();
        void drawMositureMeters(uint16_t x, uint16_t y);
        void updateMositureMeters(uint16_t x, uint16_t y, uint8_t value);
        //void drawSensorData(int16_t x, int16_t y, SensorData *internalSensorData, SensorData *externalSensorData);
};

#endif