#include "DisplayMain.h"

DisplayMain::DisplayMain()
{
}

void DisplayMain::init()
{
    DisplayControl::init(3, &CalibriRegular8pt7b);// Default Font print line
	setFont(&CalibriBold16pt7b);// Initial Font for text
}

void DisplayMain::clearDisplay()
{
    DisplayGFX->fillScreen(BACKGROUND_COLOR);
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