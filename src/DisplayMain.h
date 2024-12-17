#ifndef _DISPLAY_MAIN_H_
#define _DISPLAY_MAIN_H_

#include <Arduino.h>
#include <WiFi.h>

#include "DisplayControl.h"
#include "DisplayFonts.h"
#include "SensorData.h"

#define BACKGROUND_COLOR 0x0000
#define FOREGROUND_COLOR 0xEF5D
#define OVERLAY_COLOR 0x1082
#define TEXT_TITLE_COLOR 0xDF81
#define TEXT_MAIN_COLOR 0xFD20
#define TEXT_ALT_COLOR 0x07FF

#define SUCCESS_COLOR 0x07E0
#define ERROR_COLOR 0xF800

#define DRY_LIGHT_COLOR 0xCCEC
#define DRY_DARK_COLOR 	0x1060
#define WET_LIGHT_COLOR 0x865C
#define WET_DARK_COLOR 	0x0083

class DisplayMain : public DisplayControl
{
    private:
        
    public:
        DisplayMain();
        void init();
        void clearDisplay();
        void updateMositureMeter(SensorData* sensorData, bool forceUpdate = false);
        void drawWiFiSignal(int16_t x, int16_t y, int16_t size, uint16_t backgroundColor = UINT16_MAX);
        void printWiFiInfo();
};

#endif