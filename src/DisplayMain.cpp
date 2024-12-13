#include "DisplayMain.h"

DisplayMain::DisplayMain()
{
}

void DisplayMain::init()
{
    DisplayControl::init(3, &CalibriRegular8pt7b, TEXT_MAIN_COLOR, BACKGROUND_COLOR);// Default Font print line
	setFont(&CalibriBold12pt7b);// Initial Font for text
}

void DisplayMain::clearDisplay()
{
    DisplayGFX->fillScreen(BACKGROUND_COLOR);
	printLineReset();
}

void DisplayMain::updateMositureMeter(SensorData* sensorData, bool forceUpdate)
{
    if (!forceUpdate && (!sensorData->Connected || sensorData->CurrentValue == sensorData->PreviousValue))
    {
        return;
    }

    uint16_t x = sensorData->X;
    uint16_t y = sensorData->Y;
    uint16_t w = 60;//overall width
    uint16_t h = 256;//overall height
    uint16_t bl = 200;//bar graph height
    uint16_t bn = 10;//bar graph number of segments
    uint16_t bs = bl/bn;//bar graph segements height
    uint16_t bo = (256-bl)/2;//bar graph offset
    uint16_t bx = x+3;//bar graph x
    uint16_t by = y+bo;//bar graph y
    uint16_t bw = w-6;//bar graph width
    uint16_t bc = BLACK;//bar graph color

    if (forceUpdate)
    {
        DisplayGFX->fillRect(x, y, w, h, BACKGROUND_COLOR);
        DisplayGFX->drawRect(x+1, y+1, w-2, h-2, GRAY);
        drawString(sensorData->Label, x + (w/2), y + (bo/2), TEXT_CENTER_MIDDLE, CYAN);
    }

    for (uint8_t i = 0; i < bn; i++)
    {
        if (sensorData->Connected && sensorData->CurrentValue > ((bn-i)*bn)-4)
        {
            bc = colorLerp(WET_LIGHT_COLOR, WET_DARK_COLOR, (0.1 * i * 255));
        }
        else
        {
            bc = colorLerp(DRY_LIGHT_COLOR, DRY_DARK_COLOR, (0.1 * i * 255));
        }        
        DisplayGFX->fillRect(bx, by+(i*bs), bw, bs, bc);
        DisplayGFX->drawFastHLine(bx, by+(i*bs), bw, GRAY);
    }
    DisplayGFX->drawFastHLine(bx, by+bl, bw, GRAY);

    DisplayGFX->fillRect(bx, by+bl+1, bw, bo-3, BACKGROUND_COLOR);
    if (sensorData->Connected)
    {
        drawString(String(sensorData->CurrentValue), bx+(bw/2), by+bl+(bo/2), TEXT_CENTER_MIDDLE, CYAN);
    }
    else
    {
        drawString("---", bx+(bw/2), by+bl+(bo/2), TEXT_CENTER_MIDDLE, CYAN);
    }	
}

/// @brief Default size when set to 1 is 12x12
/// @param x 
/// @param y 
/// @param size 
void DisplayMain::drawWiFiSignal(int16_t x, int16_t y, int16_t size, uint16_t backgroundColor)
{
	size = max(size, (int16_t)1);
	uint16_t gray = colorLerp(FOREGROUND_COLOR, BACKGROUND_COLOR, 128);

	if (backgroundColor != UINT16_MAX)
	{
		DisplayGFX->fillRect(x, y, size*12, size*12, backgroundColor);
	}

	if (WiFi.status() == WL_CONNECTED)
	{
		int32_t strength = WiFi.RSSI();
		DisplayGFX->fillRect(x,        y+size*9, size*2, size*3,  strength > -80 ? FOREGROUND_COLOR : gray);
		DisplayGFX->fillRect(x+size*3, y+size*6, size*2, size*6,  strength > -70 ? FOREGROUND_COLOR : gray);
		DisplayGFX->fillRect(x+size*6, y+size*3, size*2, size*9,  strength > -60 ? FOREGROUND_COLOR : gray);
		DisplayGFX->fillRect(x+size*9, y,        size*2, size*12, strength > -50 ? FOREGROUND_COLOR : gray);
	}
	else
	{
		int16_t ly = y + size * 5;
		int16_t lw = size*10;
		for (int16_t t=0; t < size * 2; t++)
		{
			DisplayGFX->drawFastHLine(x+2, ly+t, lw, ERROR_COLOR);
		}
		int16_t r = 6*size;
		int16_t cx = x+r;
		int16_t cy = y+r;
		for (int16_t t=0; t < size * 2; t++)
		{
			DisplayGFX->drawCircle(cx, cy, r-t, ERROR_COLOR);
		}		
	}
}