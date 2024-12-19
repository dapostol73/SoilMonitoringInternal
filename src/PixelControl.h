#ifndef _PIXEL_CONTROL_H_
#define _PIXEL_CONTROL_H_

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

class PixelControl
{
    private:
        uint16_t pixelCount = 1;
        uint16_t pixelPin = PIN_NEOPIXEL;
        Adafruit_NeoPixel pixel = Adafruit_NeoPixel(pixelCount, pixelPin, NEO_GRB + NEO_KHZ800);
    public:
        PixelControl();
        void init();
        void blink(uint16_t times, uint16_t interval, uint32_t color);
        void blinkRed(uint16_t times, uint16_t interval);
        void blinkGreen(uint16_t times, uint16_t interval);
        void blinkBlue(uint16_t times, uint16_t interval);
};

#endif