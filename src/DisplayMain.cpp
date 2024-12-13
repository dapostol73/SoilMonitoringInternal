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

void DisplayMain::drawMositureMeters(uint16_t x, uint16_t y)
{
	uint16_t w = 64;
	drawMositureMeter("S1", x + (0 * w), y);
	drawMositureMeter("S2", x + (1 * w), y);
	drawMositureMeter("S3", x + (2 * w), y);
	drawMositureMeter("S4", x + (3 * w), y);
	drawMositureMeter("S5", x + (4 * w), y);
	drawMositureMeter("S6", x + (5 * w), y);
	drawMositureMeter("S7", x + (6 * w), y);	
}

void DisplayMain::drawMositureMeter(const char* label, uint16_t x, uint16_t y)
{
    uint16_t w = 64;
	uint16_t h = 256;
	uint16_t bx = x+3;
	uint16_t bw = w-6;
	uint16_t bt = y+24;
	uint16_t bh = 256-48;
	uint16_t bl = 200;
	uint16_t bn = 10;
	uint16_t bs = bl/bn;
	uint16_t bo = (bh-bl)/2;
	uint16_t bc = DRY_LIGHT_COLOR;
    DisplayGFX->drawRect(x+1, y+1, w-2, h-2, GRAY);

	for (int i = 0; i < bn; i ++)
	{
		bc = colorLerp(DRY_LIGHT_COLOR, DRY_DARK_COLOR, (0.1 * i * 255));
		DisplayGFX->fillRect(bx, bt+bo+(i*bs), bw, bs, bc);
		DisplayGFX->drawFastHLine(bx, bt+bo+(i*bs), bw, GRAY);
	}
	DisplayGFX->drawFastHLine(bx, bt+bo+bl, bw, GRAY);
    

    //DisplayGFX->fillTriangle(x + 6, y + 218 + 8, x + 6 + 20, y + 218, x + 6, y + 218 - 8, RED);
	drawString(label, x + 32, y + 12, TEXT_CENTER_MIDDLE, CYAN);
	drawString("---", x + 32, y + 256 - 12, TEXT_CENTER_MIDDLE, CYAN);
}

void DisplayMain::updateMositureMeters(uint16_t x, uint16_t y, uint8_t value)
{

	drawString(String(value), x + 32, y + 256 - 12, TEXT_CENTER_MIDDLE, CYAN);
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