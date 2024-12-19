#include "PixelControl.h"

PixelControl::PixelControl()
{
}

void PixelControl::init()
{
    pixel.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
    pixel.show();            // Turn OFF all pixels ASAP
    pixel.setBrightness(128);// Set BRIGHTNESS to about 1/5 (max = 255)
    blinkRed(1, 250);
    blinkGreen(1, 250);
    blinkBlue(1, 250);
}

void PixelControl::blink(uint16_t times, uint16_t interval, uint32_t color)
{
    for (uint8_t i = 0; i < times; i++)
    {
        pixel.setPixelColor(0, color);  // turn the LED on (HIGH is the voltage level)
        pixel.show();
        delay(interval);                // wait for XmS
        pixel.clear();                  // turn the LED off by making the voltage LOW
        pixel.show();
        delay(interval);                // wait for XmS
    }
}

void PixelControl::blinkRed(uint16_t times, uint16_t interval)
{
    blink(times, interval, pixel.Color(255, 0, 0));
}

void PixelControl::blinkGreen(uint16_t times, uint16_t interval)
{
    blink(times, interval, pixel.Color(0, 255, 0));
}

void PixelControl::blinkBlue(uint16_t times, uint16_t interval)
{
    blink(times, interval, pixel.Color(0, 0, 255));
}
