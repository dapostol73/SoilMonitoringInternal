#ifndef _DISPLAY_CONTROL_H_
#define _DISPLAY_CONTROL_H_

#define DISABLE_COLOR_DEFINES
#include <Arduino.h>
#include <gfxfont.h>
#include <Arduino_GFX_Library.h>
#include <XPT2046_Bitbang.h>

#define TFT_CS 42 // 10 or 34 (FSPI CS0)
#define TFT_RST 41 // 6
#define TFT_DC 40 // 7
#define TFT_MOSI 35 // 11 or 35 (FSPI D) T_DIN
#define TFT_SCLK 36 // 12 or 36 (FSPI CLK) T_CLK
#define TFT_MISO 37 // 13 or 37 (FSPI Q) T_DO
#define TCH_MOSI 11 // 11 or 35 (FSPI D) T_DIN
#define TCH_SCLK 12 // 12 or 36 (FSPI CLK) T_CLK
#define TCH_MISO 13 // 13 or 37 (FSPI Q) T_DO
#define TCH_CS 10 // T_CS
#define TCH_IRQ 9 // T_IRQ
// Use pins in range 0-31

#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 320

#define BITS_PER_PIXEL 2 // 2^2 =  4 colors
#define BIT_MASK ((1 << BITS_PER_PIXEL) - 1)
#define PIXELS_PER_BYTE (8 / BITS_PER_PIXEL)
#define CUSTOM_BITMAP_DATA_START 6

// Using window system colors and converter
// https://learn.microsoft.com/en-us/dotnet/api/system.windows.media.colors?view=windowsdesktop-7.0&viewFallbackFrom=netcore-3.1
// https://learn.microsoft.com/en-us/dotnet/media/art-color-table.png?view=windowsdesktop-7.0
// http://www.rinkydinkelectronics.com/calc_rgb565.php

#define BLACK 0x0000
#define GRAY 0x8410
#define WHITE 0xFFFF
#define BLUE 0x001F
#define CYAN 0x07FF
#define GREEN 0x07E0
#define YELLOW 0xFFE0
#define ORANGE 0xFD20
#define RED 0xF800
#define PURPLE 0x780F

enum FrameState
{
	IN_TRANSITION,
	FIXED
};

enum TextAlignment
{
	TEXT_LEFT_TOP,
	TEXT_LEFT_MIDDLE,
	TEXT_LEFT_BOTTOM,
	TEXT_CENTER_TOP,
	TEXT_CENTER_MIDDLE,
	TEXT_CENTER_BOTTOM,
	TEXT_RIGHT_TOP,
	TEXT_RIGHT_MIDDLE,
	TEXT_RIGHT_BOTTOM
};

struct RGBColor
{
	uint8_t R = 0;
	uint8_t G = 0;
	uint8_t B = 0;
};

struct HSVColor
{
	float H = 0;
	float S = 0;
	float V = 0;
};

struct DisplayContolProgress
{
	int16_t x = 0;
	int16_t y = 0;
	int16_t width = 100;
	int16_t height = 30;
	int16_t padding = 0;
	int16_t border = 2;
	int16_t corner = 2;
	int16_t progress = 0; 
	String message = "";
	const GFXfont *gfxFont;
	uint16_t foregroundColor = WHITE;
	uint16_t backgroundColor = BLACK;
};

class DisplayControl
{
	private:
		uint8_t m_currentLine = 0;
		uint8_t m_lineHeight  = 10;
		uint8_t m_maxLines    = 0;

		DisplayContolProgress* m_progress;

		void setMaxLines();

    protected:
		/* More data bus class: https://github.com/moononournation/Arduino_GFX/wiki/Data-Bus-Class */
		Arduino_DataBus *bus = new Arduino_ESP32SPI(TFT_DC, TFT_CS, TFT_SCLK, TFT_MOSI, TFT_MISO);

		const GFXfont *m_gfxFont;
		const GFXfont *m_gfxFontDefault;
		const GFXfont *m_gfxFontTemp;

		int16_t clamp(int16_t value, int16_t minimum, int16_t maximum);
		
	public:
		/* More display class: https://github.com/moononournation/Arduino_GFX/wiki/Display-Class */
		Arduino_GFX *DisplayGFX = new Arduino_ST7796(bus, TFT_RST, 3 /* rotation */, false /* IPS */);
		XPT2046_Bitbang *DisplayTouch = new XPT2046_Bitbang(TCH_MOSI, TCH_MISO, TCH_SCLK, TCH_CS, SCREEN_WIDTH, SCREEN_HEIGHT);

		DisplayControl();
		/// @brief 
		/// @param rotation 0,1,2,3 = (0,90,180,270)
		void init(uint16_t rotation, const GFXfont *gfxFont, uint16_t foregroundColor = WHITE, uint16_t backgroundColor = BLACK);

		uint16_t colorLerp(uint16_t fg, uint16_t bg, uint8_t alpha);

		uint16_t color565(RGBColor color);

		uint16_t color565(HSVColor color);

		RGBColor colorRGB(uint16_t color);

		HSVColor colorHSV(uint16_t color);

		void setFont(const GFXfont *font);

		void setRotation(uint16_t rotation);

		void fillScreen(uint16_t color);

		void drawChar(int16_t x, int16_t y, char c, TextAlignment align = TEXT_LEFT_TOP, uint16_t foregroundColor = WHITE);

		void drawString(String str, int16_t x, int16_t y, TextAlignment align = TEXT_LEFT_TOP, uint16_t foregroundColor = WHITE, uint16_t backgroundColor = BLACK, boolean invert = false, boolean mode = false);
		
		void print(const char *st, int16_t x, int16_t y);
		
		void print(char *st, int16_t x, int16_t y);
		
		void print(String st, int16_t x, int16_t y);
		
		void print(String str, uint16_t foregroundColor = WHITE, uint16_t backgroundColor = BLACK, boolean invert = false);

		void printLine();

		void printLine(String str, uint16_t foregroundColor = WHITE, uint16_t backgroundColor = BLACK, boolean invert = false);

		void printLineReset();

		void drawBitmap(int16_t x, int16_t y, int16_t sx, int16_t sy, const uint16_t *data, bool center = true, int16_t scale = 1);

		void drawMaskBitmap(int16_t x, int16_t y, int16_t sx, int16_t sy, const uint8_t *data, uint16_t foregroundColor = WHITE, uint16_t backgroundColor = BLACK, bool center = true, int16_t scale = 1);

		void drawPaletteBitmap(int16_t x, int16_t y, uint16_t *palette, const unsigned char *data);

		void drawPolygon(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3, uint16_t color);

		void fillPolygon(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3, uint16_t color);

		void fillArc(int16_t x, int16_t y, int16_t start_angle, int16_t seg_count, int16_t r, int16_t w, uint16_t color);

		void drawFatLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t wd, uint16_t color);

		void drawFatCircle(int16_t x, int16_t y, int16_t r, int16_t wd, uint16_t color);

		void gradientRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t startColor = WHITE, uint16_t endColor = BLACK, bool horizontal = false);

		void setProgress(DisplayContolProgress *progress);

		void drawProgress(int8_t progress = 0, String message = "");

		void testDisplay();
};

#endif