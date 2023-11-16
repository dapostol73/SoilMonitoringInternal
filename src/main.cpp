
/*********************************************************************
  This is an example for our Monochrome OLEDs based on SH110X drivers

  This example is for a 128x64 size display using I2C to communicate
  3 pins are required to interface (2 I2C and one reset)

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada  for Adafruit Industries.
  BSD license, check license.txt for more information
  All text above, and the splash screen must be included in any redistribution

  i2c SH1106 modified by Rupert Hirst  12/09/21
*********************************************************************/

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <WiFiEspAT.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

/* Uncomment the initialize the I2C address , uncomment only one, If you get a totally blank screen try the other*/
#define i2c_Address 0x3c //initialize with the I2C addr 0x3C Typically eBay OLED's
//#define i2c_Address 0x3d //initialize with the I2C addr 0x3D Typically Adafruit OLED's

#define SCREEN_WIDTH 64 // OLED display width, in pixels
#define SCREEN_HEIGHT 128 // OLED display height, in pixels
#define OLED_RESET -1   //   QT-PY / XIAO
Adafruit_SH1107 display = Adafruit_SH1107(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define SOILMOISTURE_PIN1 0
#define SOILMOISTURE_MIN1 510
#define SOILMOISTURE_MAX1 210

int sensorVal1;

void testdrawchar(void)
{
	display.setTextSize(1);
	display.setTextColor(SH110X_WHITE);
	display.setCursor(0, 0);

	for (uint8_t i = 0; i < 168; i++) {
	if (i == '\n') continue;
		display.write(i);
	if ((i > 0) && (i % 21 == 0))
		display.println();
	}
	display.display();
	delay(1);
}

void drawPercentageBar(int value, int x, int y = 0) {
	const int height = 5;
	const int width = 16;
	const int spacing = 1;
	const int offset = 3;
	const int bars = 10;
	y += offset;
	for (int i=0; i < bars; i++) {
		if ((bars-i)*10-5 > value) {
			display.drawRoundRect(x, y, width, height, 1, SH110X_WHITE);
		}
		else {
			display.fillRoundRect(x, y, width, height, 1, SH110X_WHITE);
		}
		y += height + spacing;
	}
}

void setup() {
	Serial.begin(9600);

	// Show image buffer on the display hardware.
	// Since the buffer is intialized with an Adafruit splashscreen
	// internally, this will display the splashscreen.

	delay(250); // wait for the OLED to power up
	display.begin(i2c_Address, true); // Address 0x3C default
	display.setRotation(1);
	//display.setContrast (0); // dim display

	display.display();
	delay(2000);
	// Clear the buffer.
	display.clearDisplay();

	/*
	// draw the first ~12 characters in the font
	testdrawchar();
	display.display();
	delay(2000);
	display.clearDisplay();

	// text display tests
	display.setTextSize(1);
	display.setTextColor(SH110X_WHITE);
	display.setCursor(0, 0);
	display.println("Failure is always an option");
	display.setTextColor(SH110X_BLACK, SH110X_WHITE); // 'inverted' text
	display.println(3.141592);
	display.setTextSize(2);
	display.setTextColor(SH110X_WHITE);
	display.print("0x"); display.println(0xDEADBEEF, HEX);
	display.display();
	delay(2000);
	display.clearDisplay();
	*/
}


void loop() {
	sensorVal1 = analogRead(SOILMOISTURE_PIN1);
	sensorVal1 = map(sensorVal1, SOILMOISTURE_MIN1, SOILMOISTURE_MAX1, 0, 100);
	sensorVal1 = max(min(sensorVal1, 100), 0);
	drawPercentageBar(sensorVal1, 0);
	display.setTextSize(4);
	display.setTextColor(SH110X_WHITE);
	display.setCursor(24, 16);
	display.print(sensorVal1);
	display.display();
	delay(500);
	display.clearDisplay();
	//Serial.println(sensorVal1);
}